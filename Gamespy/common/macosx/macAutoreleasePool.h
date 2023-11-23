///////////////////////////////////////////////////////////////////////////////
// File:	gsPlatform.h
// SDK:		GameSpy Common
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2009 GameSpy Industries, Inc.

#ifndef _MACAUTORELEASEPOOL_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct macAutoreleasePool* macAutoreleasePoolRef;

macAutoreleasePoolRef macAutoreleasePoolCreate();
void macAutoreleasePoolRelease(macAutoreleasePoolRef pool);
int GetPlatformPath(char **filepath);

#ifdef __cplusplus
}
#endif

#endif