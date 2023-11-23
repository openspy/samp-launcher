///////////////////////////////////////////////////////////////////////////////
// File:	gt2Auth.h
// SDK:		GameSpy Transport 2 SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2002-2009 GameSpy Industries, Inc.

#ifndef _GT2_AUTH_H_
#define _GT2_AUTH_H_

#define GTI2_CHALLENGE_LEN           32
#define GTI2_RESPONSE_LEN            32

#ifdef __cplusplus
extern "C" {
#endif

GT2Byte * gti2GetChallenge
(
	GT2Byte * buffer
);

GT2Byte * gti2GetResponse
(
	GT2Byte * buffer,
	const GT2Byte * challenge
);

GT2Bool gti2CheckResponse
(
	const GT2Byte * response1,
	const GT2Byte * response2
);

#ifdef __cplusplus
}
#endif

#endif
