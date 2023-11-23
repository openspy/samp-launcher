///////////////////////////////////////////////////////////////////////////////
// File:	gt2Utility.h
// SDK:		GameSpy Transport 2 SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2002-2009 GameSpy Industries, Inc.

#ifndef _GT2_UTILITY_H_
#define _GT2_UTILITY_H_

#include "gt2Main.h"

void gti2MessageCheck(const GT2Byte ** message, int * len);

#ifdef RECV_LOG
void gti2LogMessage
(
	unsigned int fromIP, unsigned short fromPort,
	unsigned int toIP, unsigned short toPort,
	const GT2Byte * message, int len
);
#endif


#endif
