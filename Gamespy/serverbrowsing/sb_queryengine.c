///////////////////////////////////////////////////////////////////////////////
// File:	sb_queryengine.c
// SDK:		GameSpy Server Browsing SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2009 GameSpy Industries, Inc.

#include "sb_serverbrowsing.h"
#include "sb_internal.h"

#define SAMP_EXPECTED_QUERIES 3

#ifdef GSI_MANIC_DEBUG
// Make sure the server isn't already in the fifo
void FIFODebugCheckAdd(SBServerFIFO *fifo, SBServer server)
{
	SBServer aServer = fifo->first;
	while(aServer != NULL)
	{
		GS_ASSERT(aServer != server);
		aServer = aServer->next;
	}
}


// Verify the contents of the fifo
void FIFODebugCheck(SBServerFIFO *fifo)
{
	int i=0;
	SBServer aServer;

	GS_ASSERT(fifo != NULL);
	aServer = fifo->first;
	for (i=0; i < fifo->count; i++)
	{
		GS_ASSERT(aServer != NULL);
		aServer = aServer->next;
	}
}
#else
#define FIFODebugCheckAdd(a,b)
#define FIFODebugCheck(a)
#endif

//FIFO Queue management functions
static void FIFOAddRear(SBServerFIFO *fifo, SBServer server)
{
	FIFODebugCheckAdd(fifo, server);

	if (fifo->last != NULL)
		fifo->last->next = server;
	fifo->last = server;
	server->next = NULL;
	if (fifo->first == NULL)
		fifo->first = server;
	fifo->count++;

	FIFODebugCheck(fifo);
}

static void FIFOAddFront(SBServerFIFO *fifo, SBServer server)
{
	FIFODebugCheckAdd(fifo, server);

	server->next = fifo->first;
	fifo->first = server;
	if (fifo->last == NULL)
		fifo->last = server;
	fifo->count++;

	FIFODebugCheck(fifo);
}

static SBServer FIFOGetFirst(SBServerFIFO *fifo)
{
	SBServer hold;
	hold = fifo->first;
	if (hold != NULL)
	{
		fifo->first = hold->next;
		if (fifo->first == NULL)
			fifo->last = NULL;
		fifo->count--;
	}

	FIFODebugCheck(fifo);
	return hold;
}

static SBBool FIFORemove(SBServerFIFO *fifo, SBServer server)
{
	SBServer hold, prev;
	prev = NULL;
	hold = fifo->first;
	while (hold != NULL)
	{
		if (hold == server) //found
		{
			if (prev != NULL) //there is a previous..
				prev->next = hold->next;
			if (fifo->first == hold)
				fifo->first = hold->next;
			if (fifo->last == hold)
				fifo->last = prev;
			fifo->count--;
		//	GS_ASSERT((fifo->count == 0 && fifo->first == NULL && fifo->last == NULL) || fifo->count > 0);
			return SBTrue;
		}
		prev = hold;
		hold = hold->next;
	}

	FIFODebugCheck(fifo);
	return SBFalse;
}

static void FIFOClear(SBServerFIFO *fifo)
{
	fifo->first = fifo->last = NULL;
	fifo->count = 0;

	FIFODebugCheck(fifo);
}

#ifdef SB_ICMP_SUPPORT
static unsigned short IPChecksum(const unsigned short *buf, int len)
{
	unsigned long cksum = 0;
	
	//Calculate the checksum
	while (len > 1)
	{
		cksum += *buf++;
		len -= sizeof(unsigned short);
	}
	
	//If we have one char left
	if (len) {
		cksum += *(unsigned char*)buf;
	}
	
	//Complete the calculations
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	
	//Return the value (inversed)
	return (unsigned short)(~cksum);
}
#endif

static void QEStartQuery(SBQueryEngine *engine, SBServer server)
{
	unsigned char queryBuffer[256];
	int queryLen;
	gsi_bool querySuccess = gsi_false;
	struct sockaddr_in saddr;

	saddr.sin_family = AF_INET;
	server->updatetime = current_time();
	server->num_queries = 0;

	if (engine->queryversion == QVERSION_QR2)
	{
			// send IP verify request now, but send qr2 query later when IP verify returns
			int pos = 0;
			queryBuffer[pos++] = 'S';
			queryBuffer[pos++] = 'A';
			queryBuffer[pos++] = 'M';
			queryBuffer[pos++] = 'P';

			memcpy(&queryBuffer[pos], &server->publicip, 4);
			pos += 4;

			memcpy(&queryBuffer[pos], &server->publicport, 2);
			pos += 2;
			queryLen = pos;
	} 
	if (server->publicip == engine->mypublicip && (server->flags & PRIVATE_IP_FLAG)) //try querying the private IP
	{
		saddr.sin_addr.s_addr = server->privateip;
		saddr.sin_port = server->privateport;
	} else
	{
		saddr.sin_addr.s_addr = server->publicip;
		saddr.sin_port = server->publicport;
	}

	queryBuffer[queryLen] = 'i';
	sendto(engine->querysock, (char*)queryBuffer, queryLen + 1, 0, (struct sockaddr*)&saddr, sizeof(saddr));

	queryBuffer[queryLen] = 'c';
	sendto(engine->querysock, (char*)queryBuffer, queryLen + 1, 0, (struct sockaddr*)&saddr, sizeof(saddr));

	queryBuffer[queryLen] = 'r';
	sendto(engine->querysock, (char*)queryBuffer, queryLen + 1, 0, (struct sockaddr*)&saddr, sizeof(saddr));

	querySuccess = gsi_true;


	//add it to the query list
	if (gsi_is_true(querySuccess))
		FIFOAddRear(&engine->querylist, server);
	else
		server->updatetime = 0;
}


void SBQueryEngineInit(SBQueryEngine *engine, int maxupdates, int queryversion, SBBool lanBrowse, SBEngineCallbackFn callback, void *instance)
{
	// 11-03-2004 : Added by Saad Nader
	// fix for LANs and unnecessary availability check
	///////////////////////////////////////////////////
	if(lanBrowse == SBFalse)
	{
		if(__GSIACResult != GSIACAvailable)
		return;
	}
		SocketStartUp();
	engine->queryversion = queryversion;
	engine->maxupdates = maxupdates;
	engine->numserverkeys = 0;
	engine->ListCallback = callback;
	engine->instance = instance;
	engine->mypublicip = 0;
	engine->querysock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#if defined(SB_ICMP_SUPPORT)
	#if defined(SN_SYSTEMS)
	{
		// reset SNSystems internal ICMP ping structures
		sndev_set_ping_reset_type optval;
		optval.timeout_ms = MAX_QUERY_MSEC; // this gets rounded up to 3 sec
		optval.reserved = 0;
		sndev_set_options(0, SN_DEV_SET_PING_RESET, &optval, sizeof(optval));
	}
	#else
		#if defined(_MACOSX) || defined(_IPHONE)	//SOCK_RAW fails under OS X; use SOCK_DGRAM instead
			engine->icmpsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
		#else
			engine->icmpsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
		#endif
	#endif
#endif
	FIFOClear(&engine->pendinglist);
	FIFOClear(&engine->querylist);
}

void SBQueryEngineSetPublicIP(SBQueryEngine *engine, goa_uint32 mypublicip)
{
	engine->mypublicip = mypublicip;
}

void SBEngineHaltUpdates(SBQueryEngine *engine)
{
	FIFOClear(&engine->pendinglist);
	FIFOClear(&engine->querylist);
}


void SBEngineCleanup(SBQueryEngine *engine)
{
	closesocket(engine->querysock);
#ifdef SB_ICMP_SUPPORT
	#if !defined(SN_SYSTEMS)
	closesocket(engine->icmpsock);
	#endif
#endif
	engine->querysock = INVALID_SOCKET;
	FIFOClear(&engine->pendinglist);
	FIFOClear(&engine->querylist);
}


//NOTE: the server must not be in the pending or update list currently!
void SBQueryEngineUpdateServer(SBQueryEngine *engine, SBServer server, int addfront, int querytype, SBBool usequerychallenge)
{
	// Assert state of FIFOs
	FIFODebugCheckAdd(&engine->pendinglist, server);
	FIFODebugCheckAdd(&engine->querylist, server);

	server->state &= (unsigned char)~(STATE_PENDINGBASICQUERY|STATE_PENDINGFULLQUERY|STATE_PENDINGICMPQUERY|STATE_PENDINGQUERYCHALLENGE|STATE_QUERYFAILED); //clear out these flags
	server->splitResponseBitmap = 0;
	server->querychallenge = 0;

#ifndef SB_ICMP_SUPPORT
	if (querytype == QTYPE_ICMP)
		return; // ICMP not supported
#endif

	if (querytype == QTYPE_BASIC)
		server->state |= STATE_PENDINGBASICQUERY;
	else if (querytype == QTYPE_FULL)
		server->state |= STATE_PENDINGFULLQUERY;
	else if (querytype == QTYPE_ICMP)
		server->state |= STATE_PENDINGICMPQUERY;
	else
		return; // hoterror: unsupported querytype!
		
	if (usequerychallenge && (querytype == QTYPE_FULL || querytype == QTYPE_BASIC))
		server->state |= STATE_PENDINGQUERYCHALLENGE;
	
	if (engine->querylist.count < engine->maxupdates) //add it now..
	{
		QEStartQuery(engine, server);
		return;
	}
	//else need to queue it
	
	if (addfront)
		FIFOAddFront(&engine->pendinglist, server);
	else
		FIFOAddRear(&engine->pendinglist, server);
}

SBServer SBQueryEngineUpdateServerByIP(SBQueryEngine *engine, const char *ip, unsigned short queryport, int addfront, int querytype, SBBool usequerychallenge)
{
	//need to create a new server
	SBServer server;
	goa_uint32 ipaddr;
	ipaddr = inet_addr(ip);
	server = SBAllocServer(NULL, ipaddr, htons(queryport));
	server->flags = UNSOLICITED_UDP_FLAG; //we assume we can talk directly to it
	SBQueryEngineUpdateServer(engine, server, addfront, querytype, usequerychallenge);
	return server;
}

static void parse_samp_info_packet(SBQueryEngine* engine, SBServer server, char* data, int len) {
	char* p = &data[11];
	gsi_u8 password = *(gsi_u8*)p; p++;

	char hostname[256];
	char gamemode[256];
	char language[256];

	
	gsi_u16 numplayers = *(gsi_u16*)p; p += sizeof(gsi_u16);
	gsi_u16 maxplayers = *(gsi_u16*)p; p += sizeof(gsi_u16);
	gsi_u32 host_len = *(gsi_u32*)p; p += sizeof(gsi_u32);
	strcpy_s(hostname, sizeof(hostname), p); p += host_len;
	hostname[host_len] = 0;

	host_len = *(gsi_u32*)p; p += sizeof(gsi_u32);
	strcpy_s(gamemode, sizeof(gamemode), p); p += host_len;
	gamemode[host_len] = 0;

	host_len = *(gsi_u32*)p; p += sizeof(gsi_u32);
	strcpy_s(language, sizeof(language), p); p += host_len;
	language[host_len] = 0;

	SBServerAddIntKeyValue(server, "numplayers", numplayers);
	SBServerAddIntKeyValue(server, "maxplayers", maxplayers);

	SBServerAddKeyValue(server, "hostname", hostname);
	SBServerAddKeyValue(server, "gamemode", gamemode);
	SBServerAddKeyValue(server, "gamevariant", language);
}


static void parse_samp_rules_packet(SBQueryEngine* engine, SBServer server, char* data, int len) {
	char* p = &data[11];

	char key[1024];
	char value[1024];


	gsi_u16 num_rules = *(gsi_u16*)p; p += sizeof(gsi_u16);

	for (int i = 0; i < num_rules; i++) {
		gsi_u8 key_len = *(gsi_u8*)p; p += sizeof(gsi_u8);

		key[0] = 0;
		strcpy_s(key, sizeof(key), p); p += key_len;
		key[key_len] = 0;

		key_len = *(gsi_u8*)p; p += sizeof(gsi_u8);
		value[0] = 0;
		strcpy_s(value, sizeof(value), p); p += key_len;
		value[key_len] = 0;

		SBServerAddKeyValue(server, key, value);
	}
}

static void parse_samp_client_packet(SBQueryEngine* engine, SBServer server, char* data, int len) {
	char* p = &data[11];


	char player_key_prefix[512];
	char name[256];


	gsi_u16 player_count = *(gsi_u16*)p; p += sizeof(gsi_u16);

	for (int i = 0; i < player_count; i++) {
		
		gsi_u8 key_len = *(gsi_u8*)p; p += sizeof(gsi_u8);

		if (key_len > 0) {
			strcpy_s(name, sizeof(name), p); p += key_len;
		}
		name[key_len] = 0;

		sprintf_s(player_key_prefix, sizeof(player_key_prefix), "name_%d", i);
		SBServerAddKeyValue(server, player_key_prefix, name);

		sprintf_s(player_key_prefix, sizeof(player_key_prefix), "score_%d", i);

		gsi_u32 score = *(gsi_u32*)p; p += sizeof(gsi_u32);
		SBServerAddIntKeyValue(server, player_key_prefix, score);
	}


}

static void ParseSingleQR2Reply(SBQueryEngine *engine, SBServer server, char *data, int len)
{
	int i;
	int dlen;

	if (data[0] != 'S' && data[1] != 'A' && data[2] != 'M' && data[3] != 'P')
		return;

	switch (data[10]) {
	case 'i':
		parse_samp_info_packet(engine, server, data, len);
		break;
	case 'r':
		parse_samp_rules_packet(engine, server, data, len);
		break;
	case 'p':
		OutputDebugStringA("ping data\n");
		break;
	case 'c':
		parse_samp_client_packet(engine, server, data, len);
		break;
	}

	server->num_queries++;

		

	if (server->num_queries == SAMP_EXPECTED_QUERIES) {
		server->flags |= UNSOLICITED_UDP_FLAG;

		if (server->state & STATE_PENDINGBASICQUERY)
			server->state |= STATE_BASICKEYS | STATE_VALIDPING;
		else
			server->state |= STATE_FULLKEYS | STATE_VALIDPING;

		server->updatetime = current_time() - server->updatetime;

		server->state &= (unsigned char)~(STATE_PENDINGBASICQUERY | STATE_PENDINGFULLQUERY);
		
		FIFORemove(&engine->querylist, server);
		engine->ListCallback(engine, qe_updatesuccess, server, engine->instance);
	}
}

static void ParseSingleGOAReply(SBQueryEngine *engine, SBServer server, char *data, int len)
{
	int isfinal;
	//need to check before parse as it will modify the string
	isfinal = (strstr(data,"\\final\\") != NULL);
	SBServerParseKeyVals(server, data);
	if (isfinal)
	{
		if (server->state & STATE_PENDINGBASICQUERY)
			server->state |= STATE_BASICKEYS|STATE_VALIDPING;
		else
			server->state |= STATE_FULLKEYS|STATE_VALIDPING;
		server->state &= (unsigned char)~(STATE_PENDINGBASICQUERY|STATE_PENDINGFULLQUERY);
		server->updatetime = current_time() - server->updatetime;
		FIFORemove(&engine->querylist, server);
		engine->ListCallback(engine, qe_updatesuccess, server, engine->instance);
	}
	
	GSI_UNUSED(len);
}

static SBBool ParseSingleICMPReply(SBQueryEngine *engine, SBServer server, char *data, int len)
{
#ifdef SB_ICMP_SUPPORT
	SBIPHeader *ipheader = (SBIPHeader *)data;
	SBICMPHeader *icmpheader;
	int ipheaderlen;
	goa_uint32 packetpublicip;
	unsigned short packetpublicport;
	//todo: byte alignment on PS2
	ipheaderlen = (gsi_u8)(ipheader->ip_hl_ver & 15);
	ipheaderlen *= 4;
	icmpheader = (SBICMPHeader *)(data + ipheaderlen);
	if (icmpheader->type != SB_ICMP_ECHO_REPLY)
		return SBFalse;
	if (icmpheader->un.idseq != server->updatetime)
		return SBFalse;
	if (len < ipheaderlen + (int)sizeof(SBICMPHeader) + 6)
		return SBFalse; //not enough data
	//check the server IP and port
	memcpy(&packetpublicip, data + ipheaderlen + sizeof(SBICMPHeader), 4);
	memcpy(&packetpublicport, data + ipheaderlen + sizeof(SBICMPHeader) + 4, 2);
	if (packetpublicport != server->publicport || packetpublicip != server->publicip)
		return SBFalse;
	//else its a valid echo
	server->updatetime = current_time() - server->updatetime;
	server->state |= STATE_VALIDPING;
	server->state &= (unsigned char)~(STATE_PENDINGICMPQUERY);	
	FIFORemove(&engine->querylist, server);
	engine->ListCallback(engine, qe_updatesuccess, server, engine->instance);
#else
	GSI_UNUSED(engine);
	GSI_UNUSED(server);
	GSI_UNUSED(data);
	GSI_UNUSED(len);
#endif
	return SBTrue;
	
}

#if defined(SN_SYSTEMS) && defined(SB_ICMP_SUPPORT)
static void ProcessIncomingICMPReplies(SBQueryEngine *engine)
{
	SBServer server;
	int result = 0;
	int found  = 0;
	int i      = 0;
	sndev_stat_ping_times_type optval;
	gsi_i32 optsize = sizeof(optval);

	// Get the ICMP replies from the SNSystems stack
	result = sndev_get_status(0, SN_DEV_STAT_PING_TIMES, (void*)&optval, &optsize);
	if (result != 0)
	{
		gsDebugFormat(GSIDebugCat_SB, GSIDebugType_Network, GSIDebugLevel_WarmError,
			"Failed on sndev_get_status (checking ICMP pings): %d\r\n", result);
		return;
	}
	if (optval.num_entries == 0)
		return; // no outstanding pings (according to sn_systems)

	// match servers to ping responses
	for (server = engine->querylist.first; server != NULL; server = server->next)
	{
		if ((server->state & STATE_PENDINGICMPQUERY) == 0 ||
			(server->flags & ICMP_IP_FLAG) == 0)
			continue; // server not flagged for ICMP

		// find this server
		for (i=0; i<optval.num_entries; i++)
		{
			if (server->icmpip == optval.times[i].ip_addr)
			{
				if (optval.times[i].status == SN_PING_TIMES_CODE_GOTREPLY)
				{
					server->updatetime = optval.times[i].time_ms;
					server->state |= STATE_VALIDPING;
					server->state &= (unsigned char)~(STATE_PENDINGICMPQUERY);
					FIFORemove(&engine->querylist, server);
					engine->ListCallback(engine, qe_updatesuccess, server, engine->instance);
				}
				//else
				//   let query engine timeout queries on its own (for simplicity)

				found++;
				if (found == optval.num_entries)
					return; // found them all
			}
		}
	}
}
#endif // SN_SYSTEMS && SB_ICMP_SUPPORT

static void ProcessIncomingReplies(SBQueryEngine *engine, SBBool icmpSocket)
{
	int i;
	char indata[MAX_RECVFROM_SIZE]; 
	struct sockaddr_in saddr;
	socklen_t saddrlen = sizeof(saddr);
	SBServer server;
	SOCKET recvSock = 0;

	if (icmpSocket)
	{
		#ifdef SB_ICMP_SUPPORT
			#if defined(SN_SYSTEMS) 
				ProcessIncomingICMPReplies(engine);
				return;
			#else
				recvSock = engine->icmpsock;
			#endif
		#endif
	}
	else
	{
		recvSock = engine->querysock;
	}
	
	// Process all information in the socket buffer 
	while(CanReceiveOnSocket(recvSock))
	{
		i = (int)recvfrom(recvSock, indata, sizeof(indata) - 1, 0, (struct sockaddr *)&saddr, &saddrlen);

		if (gsiSocketIsError(i))
			break;
		indata[i] = 0;
		//find the server in our query list
		for (server = engine->querylist.first ; server != NULL ; server = server->next)
		{
			if ((icmpSocket && (server->flags & ICMP_IP_FLAG) && server->icmpip == saddr.sin_addr.s_addr) || //if it's an ICMP query and it matches the ICMP address			
				(server->publicip == saddr.sin_addr.s_addr && (server->publicport == saddr.sin_port || icmpSocket)) || //if it matches public - port doesnt need to match for ICMP
				(server->publicip == engine->mypublicip && (server->flags & PRIVATE_IP_FLAG) && server->privateip == saddr.sin_addr.s_addr && server->privateport == saddr.sin_port)) //or has a private, and matches
			{
				if (icmpSocket)
				{
					if (ParseSingleICMPReply(engine, server, indata, i))
						break; //only break if it matches exactly, since we may have multiple outstanding pings to the same ICMPIP for different servers!
				} else
				{
					if (engine->queryversion == QVERSION_QR2)
						ParseSingleQR2Reply(engine, server, indata, i);
					else
						ParseSingleGOAReply(engine, server, indata, i);
					break;
				}
			}
		}		
	}


}

static void TimeoutOldQueries(SBQueryEngine *engine)
{
	gsi_time ctime = current_time();
	while (engine->querylist.first != NULL)
	{
		if (ctime > engine->querylist.first->updatetime + MAX_QUERY_MSEC)
		{
			engine->querylist.first->flags |= STATE_QUERYFAILED;
			engine->querylist.first->updatetime = MAX_QUERY_MSEC;
			engine->querylist.first->flags  &= (unsigned char)~(STATE_PENDINGBASICQUERY|STATE_PENDINGFULLQUERY|STATE_PENDINGICMPQUERY);
			engine->ListCallback(engine, qe_updatefailed, engine->querylist.first, engine->instance);			
			FIFOGetFirst(&engine->querylist);
		} else
			break; //since servers are added in FIFO order, nothing later can have already expired
	}
}

static void QueueNextQueries(SBQueryEngine *engine)
{
	while (engine->querylist.count < engine->maxupdates && engine->pendinglist.count > 0)
	{
		SBServer server = FIFOGetFirst(&engine->pendinglist);
		QEStartQuery(engine, server);
	}
}

void SBQueryEngineThink(SBQueryEngine *engine)
{
	if (engine->querylist.count == 0) //not querying anything - we can go away
		return;
	ProcessIncomingReplies(engine, SBFalse);
#ifdef SB_ICMP_SUPPORT
	ProcessIncomingReplies(engine, SBTrue);
#endif
	TimeoutOldQueries(engine);
	if (engine->pendinglist.count > 0)
		QueueNextQueries(engine);
	if (engine->querylist.count == 0) //we are now idle..
		engine->ListCallback(engine, qe_engineidle, NULL, engine->instance);
}

void SBQueryEngineAddQueryKey(SBQueryEngine *engine, unsigned char keyid)
{
	if (engine->numserverkeys < MAX_QUERY_KEYS)
		engine->serverkeys[engine->numserverkeys++] = keyid;
}


//remove a server from our update FIFOs
void SBQueryEngineRemoveServerFromFIFOs(SBQueryEngine *engine, SBServer server)
{
	SBBool ret;

	// remove the server from the current query list
	ret = FIFORemove(&engine->querylist, server);
	if(ret)
		return; // -- Caution: assumes that server will not be in pendinglist
	FIFORemove(&engine->pendinglist, server);
}
