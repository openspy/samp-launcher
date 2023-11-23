///////////////////////////////////////////////////////////////////////////////
// File:	gt2Buffer.h
// SDK:		GameSpy Transport 2 SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2002-2009 GameSpy Industries, Inc.

#ifndef _GT2_BUFFER_H_
#define _GT2_BUFFER_H_

#include "gt2Main.h"

GT2Bool gti2AllocateBuffer(GTI2Buffer * buffer, int size);

int gti2GetBufferFreeSpace(const GTI2Buffer * buffer);

void gti2BufferWriteByte(GTI2Buffer * buffer, GT2Byte b);
void gti2BufferWriteUShort(GTI2Buffer * buffer, unsigned short s);
void gti2BufferWriteData(GTI2Buffer * buffer, const GT2Byte * data, int len);

// shortens the buffer by "shortenBy" (length, not size)
void gti2BufferShorten(GTI2Buffer * buffer, int start, int shortenBy);

#endif