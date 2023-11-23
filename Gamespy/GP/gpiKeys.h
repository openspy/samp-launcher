///////////////////////////////////////////////////////////////////////////////
// File:	gpiKeys.h
// SDK:		GameSpy Presence and Messaging SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _GPIKEYS_H_
#define _GPIKEYS_H_

#include "gpi.h"
#define GPI_INITIAL_NUM_KEYS 1

typedef struct
{
	char *keyName;
	char *keyValue;
} GPIKey;

void gpiStatusInfoKeyFree(void *element);
GPResult gpiStatusInfoKeysInit(GPConnection * connection);
void gpiStatusInfoKeysDestroy(GPConnection * connection);
int GS_STATIC_CALLBACK gpiStatusInfoKeyCompFunc(const void *elem1, const void *elem2);
GPResult gpiStatusInfoAddKey(GPConnection *connection, DArray keys, const char *theKeyName, const char *theKeyValue);
GPResult gpiStatusInfoDelKey(GPConnection *connection, DArray keys, const char *keyName);
GPResult gpiStatusInfoSetKey(GPConnection *connection, DArray keys, const char *keyName, const char *newKeyValue);
GPResult gpiStatusInfoGetKey(GPConnection *connection, DArray keys, const char *keyName, char **keyValue);
GPResult gpiSaveKeysToBuffer(GPConnection *connection, char **buffer);
GPResult gpiStatusInfoCheckKey(GPConnection *connection, DArray keys, const char *keyName, char **keyValue);
#endif
