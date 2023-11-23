///////////////////////////////////////////////////////////////////////////////
// File:	peerRooms.h
// SDK:		GameSpy Peer SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _PEERROOMS_H_
#define _PEERROOMS_H_

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
PEERBool piRoomsInit(PEER peer);
void piRoomsCleanup(PEER peer);
void piStartedEnteringRoom(PEER peer, RoomType roomType, const char * room);
void piFinishedEnteringRoom(PEER peer, RoomType roomType, const char * name);
void piLeaveRoom(PEER peer, RoomType roomType, const char * reason);
PEERBool piRoomToType(PEER peer, const char * room, RoomType * roomType);
void piSetLocalFlags(PEER peer);

#ifdef __cplusplus
}
#endif

#endif
