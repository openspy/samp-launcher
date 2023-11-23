///////////////////////////////////////////////////////////////////////////////
// File:	peerQR.h
// SDK:		GameSpy Peer SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _PEERQR_H_
#define _PEERQR_H_

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
#define PI_QUERYPORT       6500

/**************
** FUNCTIONS **
**************/
PEERBool piStartReporting(PEER peer, SOCKET socket, unsigned short port);
void piStopReporting(PEER peer);
void piSendStateChanged(PEER peer);
void piQRThink(PEER peer);
PEERBool piStartAutoMatchReporting(PEER peer);
void piStopAutoMatchReporting(PEER peer);

#ifdef __cplusplus
}
#endif

#endif
