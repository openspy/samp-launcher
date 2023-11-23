///////////////////////////////////////////////////////////////////////////////
// File:	peerPing.h
// SDK:		GameSpy Peer SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _PEERPING_H_
#define _PEERPING_H_

/*************
** INCLUDES **
*************/
#include "peerMain.h"


#ifdef __cplusplus
extern "C" {
#endif


/**************
** FUNCTIONS **
**************/
PEERBool piPingInit(PEER peer);
void piPingCleanup(PEER peer);
void piPingThink(PEER peer);
PEERBool piPingInitPlayer(PEER peer, piPlayer * player);
void piPingPlayerJoinedRoom(PEER peer, piPlayer * player, RoomType roomType);
void piPingPlayerLeftRoom(PEER peer, piPlayer * player);
void piUpdateXping(PEER peer, const char * nick1, const char * nick2, int ping);
PEERBool piGetXping(PEER peer, const char * nick1, const char * nick2, int * ping);

#ifdef __cplusplus
}
#endif

#endif
