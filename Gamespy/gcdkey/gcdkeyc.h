///////////////////////////////////////////////////////////////////////////////
// File:	gcdkeyc.h
// SDK:		GameSpy CD Key SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#ifndef _GOACDKEYC_H_
#define _GOACDKEYC_H_

#ifdef __cplusplus
extern "C" {
#endif

#define RESPONSE_SIZE 73

//////////////////////////////////////////////////////////////
// CDResponseMethod
// Summary
//		Values are passed to the gcd_compute_response function done client side.
typedef enum 
{
	CDResponseMethod_NEWAUTH, // method = 0 for normal auth. Used for primary authentications. 
	CDResponseMethod_REAUTH   // method = 1 for ison proof. Used for re-authentications. 
} CDResponseMethod;
	
//////////////////////////////////////////////////////////////
// gcd_compute_response
// Summary
//		Calculates a response to a challenge string.
// Parameters
//		cdkey		: [in] The client's CD key.
//		challenge	: [in] The challenge string. Should be no more than 32 characters.
//		response	: [out] Receives the computed response string.
//		method		: [in] Enum listing the response method - set to either CDResponseMethod_NEWAUTH or 
//							CDResponseMethod_REAUTH.
// Remarks
//		When the client receives the challenge string it should calculate a response using 
//		the gcd_compute_response function in the Client API.<p>	
void gcd_compute_response(char *cdkey, char *challenge,/*out*/ char response[73], CDResponseMethod method);


#ifdef __cplusplus
}
#endif

#endif
