///////////////////////////////////////////////////////////////////////////////
// File:	ghttpProcess.h
// SDK:		GameSpy HTTP SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _GHTTPPROCESS_H_
#define _GHTTPPROCESS_H_

#include "ghttpMain.h"
#include "ghttpConnection.h"

#ifdef __cplusplus
extern "C" {
#endif

void ghiDoSocketInit	  (GHIConnection * connection);
void ghiDoHostLookup      (GHIConnection * connection);
void ghiDoLookupPending   (GHIConnection * connection);
void ghiDoConnecting      (GHIConnection * connection);
#ifdef GS_USE_REFLECTOR
void ghiDoReflectorHeader (GHIConnection * connection);
#endif
void ghiDoSecuringSession (GHIConnection * connection);
void ghiDoSendingRequest  (GHIConnection * connection);
void ghiDoPosting         (GHIConnection * connection);
void ghiDoWaiting         (GHIConnection * connection);
void ghiDoReceivingStatus (GHIConnection * connection);
void ghiDoReceivingHeaders(GHIConnection * connection);
void ghiDoReceivingFile   (GHIConnection * connection);

#ifdef __cplusplus
}
#endif

#endif
