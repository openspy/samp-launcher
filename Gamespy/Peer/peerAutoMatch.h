///////////////////////////////////////////////////////////////////////////////
// File:	peerAutoMatch.h
// SDK:		GameSpy Peer SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _PEERAUTOMATCH_H_
#define _PEERAUTOMATCH_H_

/*************
** INCLUDES **
*************/
#include "peerMain.h"

#ifdef __cplusplus
extern "C" {
#endif

/************
** DEFINES **
************/
#define PI_AUTOMATCH_RATING_KEY "gsi_am_rating"
#define PEERREADYWAIT 2250  // wait 2.25 seconds once in PEERReady state to ensure you will not revert
#define	AMHOSTFLAGWAIT 2000  // wait 2 seconds to receive host flag after joining a room
#define	AMRESTARTTIME 61000  // wait over a minute to restart, since oftentimes push updates fix a stale client
/**************
** FUNCTIONS **
**************/
void piSetAutoMatchStatus(PEER peer, PEERAutoMatchStatus status);
void piStopAutoMatch(PEER peer);
PEERBool piJoinAutoMatchRoom(PEER peer, SBServer server);
void piAutoMatchDelayThink(PEER peer);
void piAutoMatchReadyTimeThink(PEER peer);
void piAutoMatchCheckWaitingForHostFlag(PEER peer);
void piAutoMatchRestartThink(PEER peer);
unsigned int piGetAutoMatchDelay();

#ifdef __cplusplus
}
#endif

#endif
