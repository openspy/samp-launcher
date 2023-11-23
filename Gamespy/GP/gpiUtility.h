///////////////////////////////////////////////////////////////////////////////
// File:	gpiUtility.h
// SDK:		GameSpy Presence and Messaging SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _GPIUTILITY_H_
#define _GPIUTILITY_H_

//INCLUDES
//////////
#include "gpi.h"

//DEFINES
/////////

// Buffer read size.
////////////////////
#define GPI_READ_SIZE                  (16 * 1024)

//MACROS
////////
#define freeclear(mem)      { gsifree(mem); (mem) = NULL; }

#define Error(connection, result, string)       { gpiSetErrorString(connection, string);\
                                                       return (result);}

#define CallbackError(connection, result, code, string)  { gpiSetError(connection, code, string);\
                                                           gpiCallErrorCallback(connection, result, GP_NON_FATAL);\
                                                           return result;}

#define CallbackErrorNoReturn(connection, result, code, string) { gpiSetError(connection, code, string);\
																  gpiCallErrorCallback(connection, result, GP_NON_FATAL);}

#define CallbackFatalError(connection, result, code, string)  { gpiSetError(connection, code, string);\
                                                                 gpiCallErrorCallback(connection, result, GP_FATAL);\
                                                                 return result;}

#define CHECK_RESULT(result)                          { GPResult __result__ = (result);\
                                                        if(__result__ != GP_NO_ERROR){\
                                                          return __result__;}}

//FUNCTIONS
///////////
void
strzcpy(
  char * dest,
  const char * src,
  size_t len  // length of buffer, including space for '\0'
);

void
gpiDebug(
  GPConnection * connection,
  const char * fmt,
  ...
);

GPIBool
gpiValueForKeyWithIndex(
  const char * command,
  const char * key,
  int * index,
  char * value,
  int len
);

GPIBool
gpiValueForKey(
  const char * command,
  const char * key,
  char * value,
  int len
);

char *
gpiValueForKeyAlloc(
  const char * command,
  const char * key
);

GPResult
gpiCheckSocketConnect(
  GPConnection * connection,
  SOCKET sock,
  int * state
);

GPResult
gpiReadKeyAndValue(
  GPConnection * connection,
  const char * buffer,
  int * index,
  char key[512],
  char value[512]
);

GPIBool
gpiCheckForError(
  GPConnection * connection,
  const char * input,
  GPIBool callErrorCallback
);

void
gpiSetError(
  GPConnection * connection,
  GPErrorCode errorCode,
  const char * errorString
);

void
gpiSetErrorString(
  GPConnection * connection,
  const char * errorString
);

void
gpiEncodeString(
  const char * unencodedString,
  char * encodedString
);

#endif
