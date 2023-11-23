///////////////////////////////////////////////////////////////////////////////
// File:	peerHost.h
// SDK:		GameSpy Peer SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _PEERHOST_H_
#define _PEERHOST_H_

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
PEERBool piStartHosting(PEER peer, SOCKET socket, unsigned short port);
void piStopHosting(PEER peer, PEERBool stopReporting);

#ifdef __cplusplus
}
#endif

#endif
