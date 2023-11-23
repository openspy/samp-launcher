///////////////////////////////////////////////////////////////////////////////
// File:	gt2Filter.h
// SDK:		GameSpy Transport 2 SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2002-2009 GameSpy Industries, Inc.

#ifndef _GT2_FILTER_H_
#define _GT2_FILTER_H_

#include "gt2Main.h"

GT2Bool gti2AddSendFilter(GT2Connection connection, gt2SendFilterCallback callback);
void gti2RemoveSendFilter(GT2Connection connection, gt2SendFilterCallback callback);
GT2Bool gti2FilteredSend(GT2Connection connection, int filterID, const GT2Byte * message, int len, GT2Bool reliable);

GT2Bool gti2AddReceiveFilter(GT2Connection connection, gt2ReceiveFilterCallback callback);
void gti2RemoveReceiveFilter(GT2Connection connection, gt2ReceiveFilterCallback callback);
GT2Bool gti2FilteredReceive(GT2Connection connection, int filterID, GT2Byte * message, int len, GT2Bool reliable);

#endif
