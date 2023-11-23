///////////////////////////////////////////////////////////////////////////////
// File:	gsbMain.h
// SDK:		GameSpy Brigades SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2009 GameSpy Industries, Inc.

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef __GSBMAIN_H__
#define __GSBMAIN_H__

// Internal header for GameSpy Brigades SDK.
// SEE brigades.H FOR PUBLIC INTERFACE

#if defined(__cplusplus)
extern "C"
{
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// constants, defines, macros for easier error reporting
#define GSB_SERVICE_MAX_URL_BASE_LEN    128
#define GSB_SERVICE_MAX_URL_LEN         2 * GSB_SERVICE_MAX_URL_BASE_LEN
#define GSB_ERROR(s,c)                  GS_ERROR(GSResultSDK_Brigades,s,c)
#define GSB_SUCCESS(s,c)                GS_RESULT(0,GSResultSDK_Brigades,s,c)
#define GSB_DP(t,l,m, ...)              gsDebugFormat(GSIDebugCat_Brigades, t, l, m, __VA_ARGS__) 
#define GSB_DP_FILE_FUNCTION_LINE(t, l) GSB_DP(t, l, "%s(%d): In %s:\n", __FILE__, __LINE__, __FUNCTION__)
// m is concatenated to format string
#define GSB_DEBUG_LOG(t, l, m, ... ) GSB_DP(t, l, "%s(%d): In %s: "m"\n" , __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// The first parameter is the assertion and if it fails,
// The message (m) will be printed if GSI_COMMON_DEBUG is defined
// Release versions of the code bubble up an error code
#define GSB_ASSERT_CHECK_PARAM(b, m, ...) GS_ASSERT(b);                                                               \
                                          if(!(b))                                                                    \
                                          {                                                                           \
                                              GSB_DEBUG_LOG(GSIDebugType_State, GSIDebugLevel_HotError, m, __VA_ARGS__);     \
                                              return GSB_ERROR(GSResultSection_State, GSResultCode_InvalidParameters);\
										  }                                                                              
// An availability check needs to be done each service call, not 
// in helper functions because they wouldn't have a use for helper 
// functions without service calls
#define GSB_CHECKAVAIL() GS_ASSERT(GSIGetAvailable()==GSIACAvailable);                                 \
                         if(GSIGetAvailable()!=GSIACAvailable)                                         \
                         {                                                                             \
						     GSB_DEBUG_LOG(GSIDebugType_State, GSIDebugLevel_HotError,                        \
                                    "was called without doing availability check.");      \
                             return GSB_ERROR(GSResultSection_State, GSResultCode_NoAvailabilityCheck);\
                         }                                                                              

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Types

 typedef struct GSBInternalInstance
{	    
    GSLoginCertificate mCertificate;
	GSLoginPrivateData mPrivateData;
    int mGameId;
	char mBaseServiceURL[GSB_SERVICE_MAX_URL_BASE_LEN];
	char mServiceURL[GSB_SERVICE_MAX_URL_LEN];
} GSBInternalInstance;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus)
} // extern "C"
#endif

#endif //__GSBMAIN_H__
