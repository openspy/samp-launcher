///////////////////////////////////////////////////////////////////////////////
// File:	gpiUnique.h
// SDK:		GameSpy Presence and Messaging SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _GPIUNIQUE_H_
#define _GPIUNIQUE_H_

//INCLUDES
//////////
#include "gpi.h"

//FUNCTIONS
///////////
GPResult gpiRegisterUniqueNick(
  GPConnection * connection,
  const char uniquenick[GP_UNIQUENICK_LEN],
  const char cdkey[GP_CDKEY_LEN],
  GPEnum blocking,
  GPCallback callback,
  void * param
);

GPResult gpiProcessRegisterUniqueNick(
  GPConnection * connection,
  GPIOperation * operation,
  const char * input
);

// Separated registration of unique nick and cdkey
GPResult gpiRegisterCdKey(
  GPConnection * connection,
  const char cdkey[GP_CDKEY_LEN],
  int gameId,
  GPEnum blocking,
  GPCallback callback,
  void * param
);

GPResult gpiProcessRegisterCdKey(
  GPConnection * connection,
  GPIOperation * operation,
  const char * input
);

#endif
