///////////////////////////////////////////////////////////////////////////////
// File:	brigadesTest.c
// SDK:		GameSpy Brigades SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// These files need to be added to application includes
#include "../../common/gsCommon.h"
#include "../../common/gsCore.h"
#include "../../common/gsAvailable.h"
#include "../../webservices/AuthService.h"
#include "../brigades.h"
#include "../gsbMain.h"
#include "../../common/gsDebug.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#if defined(_WIN32) && !defined(_XBOX) && defined(_DEBUG)
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#if defined(_NITRO)
	#include "../../common/nitro/screen.h"
	#define printf Printf
	#define vprintf VPrintf
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Globals
//#define GAMEID          101
#define GAMEID          0
#define LEADER_LOGIN_NICK      _T("sctest01")
#define LEADER_LOGIN_PASSWORD  _T("gspy")
#define MEMBER_LOGIN_NICK      _T("brigadetest01")
#define MEMBER_LOGIN_PASSWORD  _T("GameSpy")
#define MAX_NUM_TESTS   100

#if defined(_WIN32) && defined(USE_CRTDBG) && !defined(_XBOX) && defined(_DEBUG)
_CrtMemState s1, s2, s3;
#endif

typedef struct TestResults
{
    gsi_bool	pass;
    char		errString[512];
    char		requestType[512];
} TestResults;


gsi_u32                 gNumOperations;             // state control
TestResults             gResults[MAX_NUM_TESTS];
int                     gIndex = 0;
gsi_u32                 gMatchIdForMatchHistory = 0; // used for retrieveing the match history   


typedef struct LoginData
{
    GSLoginCertificate *mLoginCert;
    GSLoginPrivateData *mLoginPrivDat;
} LoginData;

typedef struct playerInfo
{
    LoginData       *mLogin;
    GSBInstancePtr  mBrigadeInstance;
    gsi_bool        mLoggedIn;
    GSBBrigadeList  *mBrigadeList;
    gsi_u32         mBrigadeId;

}playerInfo;

playerInfo Leader = {NULL, NULL, gsi_false, NULL, 0};
playerInfo Member = {NULL, NULL, gsi_false, NULL, 0};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void printResults()
{
    int i;
    int passed = 0;
    int failed = 0;

    printf("\n=======================================================\n");
    printf("==================== FINAL RESULTS ====================\n");
    printf("=======================================================\n");

    for (i=0; i<gIndex; i++)
    {
        printf("#%3d - %s: ", i+1, gResults[i].requestType);
        if (gResults[i].pass)
        {
            printf("PASS\n");
            passed++;
        }
        else
        {
            printf("FAIL\t[%s]\n", gResults[i].errString);
            failed++;
        }
    }
    printf("TOTAL PASSED = %d\n", passed);
    printf("TOTAL FAILED = %d\n\n", failed);

    if (failed == 0)
    {
        printf("         ALL TEST PASSED - WOOHOO!!!!\n\n");
        printf("                          __\n");
        printf("                  _ww   _a+'D\n");
        printf("           y#,  _r^ # _*^  y`\n");
        printf("          q0 0 a'   W*`    F   ____\n");
        printf("       ;  #^ Mw`  __`. .  4-~~^^`\n");
        printf("      _  _P   ` /'^           `www=.\n");
        printf("    , $  +F    `                q\n");
        printf("    K ]                         ^K`\n");
        printf("  , #_                . ___ r    ],\n");
        printf("  _*.^            '.__dP^^~#,  ,_ *,\n");
        printf("  ^b    / _         ``     _F   ]  ]_\n");
        printf("   '___  '               ~~^    ]   [\n");
        printf("   :` ]b_    ~k_               ,`  yl\n");
        printf("     #P        `*a__       __a~   z~`\n");
        printf("     #L     _      ^------~^`   ,/\n");
        printf("      ~-vww*'v_               _/`\n");
        printf("              ^'q_         _x'\n");
        printf("              __#my..___p/`mma____\n");
        printf("           _awP',`,^'-_'^`._ L L  #\n");
        printf("         _#0w_^_^,^r___...._ t [],'w\n");
        printf("        e^   ]b_x^_~^` __,  .]Wy7` x`\n");
        printf("         '=w__^9*$P-*MF`      ^[_.=\n");
        printf("             ^'y   qw/'^_____^~9 t\n");
        printf("               ]_l  ,'^_`..===  x'\n");
        printf("                '>.ak__awwwwWW###r\n");
        printf("                  ##WWWWWWWWWWWWWW__\n");
        printf("                 _WWWWWWMM#WWWW_JP^'~-=w_\n");
        printf("       .____awwmp_wNw#[w/`     ^#,      ~b___.\n");
        printf("        ` ^^^~^'W___            ]Raaaamw~`^``^^~\n");
        printf("                  ^~'~---~~~~~~`\n");
    }
}

//////////////////////////////////////////////////////////////////////////
// Display functions
//////////////////////////////////////////////////////////////////////////
//
void displayBrigadeMember(GSBBrigadeMember *brigadeMember)
{
    wprintf(L"\tTitle               : %s \n",brigadeMember->mTitle);
    wprintf(L"\tIs Leader           : %s \n", (brigadeMember->mIsLeader==gsi_true)? L"True":L"False");
    wprintf(L"\tProfile Id          : %d \n", brigadeMember->mProfileId);
    wprintf(L"\tMember Id           : %d \n", brigadeMember->mBrigadeMemberId);
    wprintf(L"\tBrigade Id          : %d \n", brigadeMember->mBrigadeId);
    wprintf(L"\tRole Id             : %d \n",brigadeMember->mRoleId);
    wprintf(L"\tEmail Opt In        : %s \n", gsi_is_true(brigadeMember->mEmailOptIn)? L"True": L"False");
    wprintf(L"\tDescription         : %s\n", brigadeMember->mDescription);
    printf("\tDate joined         : %s", gsiSecondsToString(&brigadeMember->mDateAdded));
    wprintf(L"\tMembership Status   : ");
    switch (brigadeMember->mStatus)
    {
    case   GSBBrigadeMemberStatus_Active :
        wprintf (L"Active \n");
        break;
    case    GSBBrigadeMemberStatus_Inactive :   
        wprintf (L"Inactive \n");
        break;
    case    GSBBrigadeMemberStatus_Invited :
        wprintf (L"Invited \n");
        break;
    case    GSBBrigadeMemberStatus_RequestJoin :
        wprintf (L"Pending Join \n");
        break;
    case    GSBBrigadeMemberStatus_Leader :	
        wprintf (L"Leader \n");
        break;
    case    GSBBrigadeMemberStatus_Kicked :
        wprintf (L"Kicked Out \n");
        break;
    case    GSBBrigadeMemberStatus_Blocked :
        wprintf (L"Blocked \n");
        break;
    default :
        wprintf (L"%d(Unknown Status) \n", brigadeMember->mStatus);
        break;
    }
}
//////////////////////////////////////////////////////////////////////////

void displayBrigade(GSBBrigade *brigade)
{
    gsi_u32 i = 0;
    wprintf(L"\tBrigade Name        : %s \n",brigade->mName);
    wprintf(L"\tTag                 : %s \n",brigade->mTag);
    wprintf(L"\tBrigade Id          : %d \n", brigade->mBrigadeId);
    wprintf(L"\tGame Id             : %d \n",brigade->mGameId);
    
    wprintf(L"\tCreator Profile Id  : %d \n", brigade->mCreatorProfileId);
    printf("\tCreated Date        : %s\n", gsiSecondsToString(&brigade->mCreatedDate));

    wprintf(L"\tDisbanded           : %s \n", gsi_is_true(brigade->mDisbanded)? L"True": L"False");
    if (gsi_is_true(brigade->mDisbanded))
    {
        printf("\tDisband Date        : %s\n", gsiSecondsToString(&brigade->mDisbandDate));
    }

    wprintf(L"\tLeader Profile Id   : %d\n", brigade->mLeaderProfileId);
    wprintf(L"\tMOTD                : %s\n", brigade->mMessageOfTheDay);
    wprintf(L"\tBrigade URL         : %s\n",brigade->mUrl);
    wprintf(L"\tBrigade Tag         : %s\n",brigade->mTag);
    wprintf(L"\tRecruiting Type     : ");
    switch (brigade->mRecruitingType)
    {
    case    GSBRecruitingStatus_Open :
        wprintf (L"Open \n");
        break;
    case    GSBRecruitingStatus_Moderated :   
        wprintf (L"Moderated \n");
        break;
    case    GSBRecruitingStatus_InviteOnly :
        wprintf (L"Invite Only \n");
        break;
    case    GSBRecruitingStatus_NoNewMembers :
        wprintf (L"No New Members \n");
        break;
    default :
        wprintf (L"%d(Unknown Recruiting Type) \n", brigade->mRecruitingType);
        break;
    }
    if (brigade->mLogoList.mCount> 0 )
    {
        for (i = 0; i<(brigade->mLogoList.mCount); i++)
        {

            wprintf(L"\tLOGO[%d] \n", i+1);
            wprintf(L"\t\tDefault Logo  : ");
            brigade->mLogoList.mLogos->mDefaultLogo? wprintf(L"True \n"): wprintf(L"False \n");

            wprintf(L"\t\tLogo File Id  : %d\n",brigade->mLogoList.mLogos->mFileId);
            wprintf(L"\t\tLogo Path     : %s\n",brigade->mLogoList.mLogos->mPath);
            wprintf(L"\t\tLogo Size     : %d\n",brigade->mLogoList.mLogos->mSizeId);
            wprintf(L"\t\tLogo URL      : %s\n",brigade->mLogoList.mLogos->mUrl);

        }
    }
}
//////////////////////////////////////////////////////////////////////////
void displayEntitlement(GSBEntitlement *entitlement)
{
    wprintf(L"\tEntitlement Id   : %d \n", entitlement->mEntitlementId);
    wprintf(L"\tEntitlement Name : %s \n", entitlement->mEntitlementName);
}

//////////////////////////////////////////////////////////////////////////
void displayBrigadeHistory(GSBBrigadeHistoryEntry *historyEntry)
{
    wprintf(L"\tHistory Entry Type      : %s \n", historyEntry->mAccessLevel);
    wprintf(L"\tHistory Entry Id        : %d \n", historyEntry->mHistoryEntryId);
    wprintf(L"\tBrigade Id              : %d \n", historyEntry->mBrigadeId);
    printf("\tDate                    : %s \n", gsiSecondsToString(&historyEntry->mDateCreated));
    wprintf(L"\tAction                  : %s \n", historyEntry->mHistoryAction);
    wprintf(L"\tInstigating Profile Id  : %d \n", historyEntry->mInstigatingProfileId);
    wprintf(L"\tTarget Profile Id       : %d \n", historyEntry->mTargetProfileId);
    wprintf(L"\tSource Profile Nick     : %s \n", historyEntry->mSourceProfileNickname);
    wprintf(L"\tTarget Profile Nick     : %s \n", historyEntry->mTargetProfileNickname);
    wprintf(L"\tNotes                   : %s \n", historyEntry->mNotes);
    wprintf(L"\tReference Id            : %d \n", historyEntry->mReferenceId);
}
///////////////////////////////////////////////////////////////////////////////
void displayRole( GSBRole *role)
{
    wprintf(L"\tBrigade Id      : %d\n", role->mBrigadeId);
    wprintf(L"\tRole Name       : %s\n",role->mRoleName);
    wprintf(L"\tRole Id         : %d \n",role->mRoleId);
    wprintf(L"\tIs Default      : %s\n", role->mIsDefault? L"True":L"False");
    wprintf(L"\tIs Game Role    : %s\n",role->mIsGameRole?L"True":L"False");
}

///////////////////////////////////////////////////////////////////////////////
void displayPendingAction( GSBBrigadePendingActions *action)
{
    wprintf(L"\tBrigade Id          : %d\n", action->mBrigadeId);
    wprintf(L"\tRole Name           : %s\n", action->mBrigadeName);
    wprintf(L"\tRole Id             : %d \n",action->mDateAdded);
    printf("\tDate joined           : %s", gsiSecondsToString(&action->mDateAdded));
    wprintf(L"\tMembership Status   : ");
    switch (action->mStatus)
    {
    case   GSBBrigadeMemberStatus_Active :
        wprintf (L"Active \n");
        break;
    case    GSBBrigadeMemberStatus_Inactive :   
        wprintf (L"Inactive \n");
        break;
    case    GSBBrigadeMemberStatus_Invited :
        wprintf (L"Invited \n");
        break;
    case    GSBBrigadeMemberStatus_RequestJoin :
        wprintf (L"Pending Join \n");
        break;
    case    GSBBrigadeMemberStatus_Leader :	
        wprintf (L"Leader \n");
        break;
    case    GSBBrigadeMemberStatus_Kicked :
        wprintf (L"Kicked Out \n");
        break;
    case    GSBBrigadeMemberStatus_Blocked :
        wprintf (L"Blocked \n");
        break;
    default :
        wprintf (L"%d(Unknown Status) \n", action->mStatus);
        break;
    }
}
///////////////////////////////////////////////////////////////////////////////
// prints request result, returns gsi_false for errors
static gsi_bool handleRequestResult(GSResult operationResult, GSResultSet *resultSet, const char * requestType)
{
    gNumOperations--; // decrement operation count
	
	sprintf(gResults[gIndex].requestType, "%s", requestType);

	if (GS_FAILED(operationResult))
	{
		// the operation failed so we need to record it as a failure and just print an error code
		// Developers are free to print better errors by extrapolating from the result
		sprintf(gResults[gIndex].errString, "Error: Operation failed: error code: %08X", operationResult);
		gResults[gIndex].pass = gsi_false;
		gIndex++;
		return gsi_false;
	}
	else if (resultSet && resultSet->mNumResults > 0)
	{
		int i;
		// The operation succeeded but there was a problem performing the operation 
		// e.g. the name of a brigade
		int len; 
        
        if (resultSet->mNumResults > 0)
        {
            len = snprintf(gResults[gIndex].errString, sizeof(gResults[gIndex].errString), "Error: Operation succeeded, but problems occurred when performing it: error codes:");
		
		    for (i = 0; i < resultSet->mNumResults; i++)
		    {
			    len += snprintf(gResults[gIndex].errString + len, sizeof(gResults[gIndex].errString), "% 08X", resultSet->mResults[i]);
		    }
            wprintf(L"FAILURE:  %s\n", resultSet->mErrorMessage);
            gResults[gIndex].pass = gsi_false;
            gIndex++;
            return gsi_false;
        }
	}

	gResults[gIndex].pass = gsi_true;
	gIndex++;
	return gsi_true;
}

///////////////////////////////////////////////////////////////////////////////
static void processTasks()
{
    if (gNumOperations > 1)
        printf("WHY ARE TWO TASKS PROCESSING HERE!?");

    // Think to process tasks
    while(gNumOperations > 0)
    {
        msleep(5);
        gsCoreThink(0);
    }
}
void freePlayerInfo( playerInfo *player)
{
    gsifree(player->mLogin->mLoginCert);

    gsifree(player->mLogin->mLoginPrivDat);

    gsifree(player->mLogin);

    gsbFreeBrigadeList(player->mBrigadeList);

}
///////////////////////////////////////////////////////////////////////////////
// Standard cleanup function
static int cleanupAndQuit(int ret)
{

    // Cleanup SDK
    //////////////////////

    ///////////////////
    // Leader Shutdown
    if (Leader.mBrigadeInstance)
        gsbShutdown(Leader.mBrigadeInstance);
    Leader.mBrigadeInstance = NULL;

    // Free all allocated memory
    freePlayerInfo(&Leader);

    ///////////////////
    // Member shutdown
    if (Member.mBrigadeInstance)
        gsbShutdown(Member.mBrigadeInstance);
    Member.mBrigadeInstance = NULL ;
    freePlayerInfo(&Member);

    //////////////////////
    // Shutdown the core
    gsCoreShutdown();
    while(gsCoreIsShutdown() == GSCore_SHUTDOWN_PENDING)
    {
        gsCoreThink(0);
        msleep(10);
    }
#if defined(_WIN32) && defined(USE_CRTDBG)
    if ( _CrtMemDifference( &s3, &s1, &s2 ) )
    { 
        printf("s3 stats\n");
        _CrtMemDumpStatistics( &s3 );
    }
    printf ("All Stats \n");
    _CrtMemDumpAllObjectsSince( NULL );
    if (! _CrtDumpMemoryLeaks())
        printf("No Leaks\n");
#endif

#if defined(_WIN32) && !defined(_XBOX) && defined(_DEBUG)
    fflush(stderr);
    printf("Done - Press Enter\r\n"); 
    fflush(stdout);
    getc(stdin);
#else
    printf("Done.\r\n"); 
#endif
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// Run the required availability check (all apps must do this)
static gsi_bool isBackendAvailable()
{
	GSIACResult result;

	GSIStartAvailableCheck("gmtest"); // use "gmtest" here for sample
	do 
	{
		result = GSIAvailableCheckThink();
		msleep(10);
	} while(result == GSIACWaiting);

	// check result
	if (result == GSIACUnavailable)
	{
		printf("Availability check returned GSIACUnavailable -- Backend services have been disabled\r\n");
		return gsi_false;
	}
	else if (result == GSIACTemporarilyUnavailable)
	{
		printf("Availability check returned GSIACTemporarilyUnavailable -- Backend services are temporarily down\r\n");
		return gsi_false;
	}

	// GSIACAvailable
	return gsi_true;
}

///////////////////////////////////////////////////////////////////////////////
void authCallback(GHTTPResult result, WSLoginResponse * response, void * userData)
{
	gNumOperations--;

    // Check for HTTP errors
	if (result != GHTTPSuccess)
	{
		printf("HTTP error when logging in: %d\r\n", result);
		//gLoggedIn = gsi_false;
	}

	// Check server result....invalid password etc
	else if (response->mLoginResult != WSLogin_Success)
	{
		printf("Login failed, server reported: %d\r\n", response->mLoginResult);
        //gLoggedIn = gsi_false;
	}
	else // Success!
	{
        playerInfo  *player = (playerInfo *) userData;
		LoginData *data = player->mLogin;
		GS_ASSERT(data);
		printf("Logged in as %d (%s)\r\n", response->mCertificate.mProfileId, response->mCertificate.mUniqueNick);
		memcpy(data->mLoginCert, &response->mCertificate, sizeof(GSLoginCertificate));
		memcpy(data->mLoginPrivDat, &response->mPrivateData, sizeof(GSLoginPrivateData));
        player->mLoggedIn = gsi_true;
	}

	GSI_UNUSED(userData);
}

///////////////////////////////////////////////////////////////////////////////
// Brigades requires authentication against the GameSpy Auth Service
static gsi_bool authenticatePlayer( playerInfo *player, gsi_char *loginNick , gsi_char *loginPassword)
{

    int result;
    player->mLogin = (LoginData *)gsimalloc(sizeof(LoginData));
    player->mLogin->mLoginCert = (GSLoginCertificate *) gsimalloc(sizeof(GSLoginCertificate));
    player->mLogin->mLoginPrivDat = (GSLoginPrivateData *) gsimalloc(sizeof(GSLoginPrivateData));

    result = wsLoginUnique(WSLogin_PARTNERCODE_GAMESPY, WSLogin_NAMESPACE_SHARED_UNIQUE, 
        loginNick, loginPassword, "", authCallback, (void *)player);

    if (result != WSLogin_Success)
    {
        printf("Failed to start wsLoginUnique: %d\r\n", result);
        // release all the memory allocated 
        gsifree(player->mLogin->mLoginCert);
        gsifree(player->mLogin->mLoginPrivDat);
        gsifree(player->mLogin);
        player->mLogin = NULL;
        return gsi_false;
    }

    gNumOperations++;
    processTasks();
    return player->mLoggedIn;
}

///////////////////////////////////////////////////////////////////////////////
void sampleGetBrigadeByIdCallback(GSResult      operationResult, 
                                      GSResultSet   *resultSet, 
                                      GSBBrigade    *theBrigade, 
                                      void          *userData)
{    
    
    wprintf(L"=============[ Get Brigade By Id Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbGetBrigadeById"))
    {
        printf (" *** [GetBrigadeById FAILED!!! ] \n");
        userData = NULL;
        gsbFreeBrigade(theBrigade);
    }
    else
    {  
        printf (" *** [GetBrigadeById SUCCESS!!! ] \n");
        displayBrigade(theBrigade);
        if (userData != NULL)
        {        
            // we want to pass back the the brigade
            *(GSBBrigade **)userData = theBrigade;
        }
        else
        {       
            // If developer does not need to keep the brigade around 
            // it can be deleted here or later.
            gsbFreeBrigade(theBrigade);
        }
    }
    wprintf(L"=============[ Get Brigade By Id Response END ]================\n");

    GSI_UNUSED(userData);
}

///////////////////////////////////////////////////////////////////////////////
void sampleGetBrigadeById(GSBInstancePtr gInstance, gsi_u32 brigadeId, void *userData)
{

    gsbGetBrigadeById(gInstance, 
                      brigadeId, 
                      sampleGetBrigadeByIdCallback, 
                      userData);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleDisbandBrigadeCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Disband Brigade Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbDisbandBrigade"))
    {
        printf(" *** [ DisbandBrigade FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ DisbandBrigade SUCCESS!!! ] \n");
    }

    wprintf(L"=============[ Disband Brigade Response END ]================\n");

    GSI_UNUSED(userData);
}

///////////////////////////////////////////////////////////////////////////////
void sampleDisbandBrigade(GSBInstancePtr instance, gsi_u32 brigadeId)
{
    wprintf(L"============== Testing : Disband Brigade (brigade id = %d) ===========\n", brigadeId);
    gsbDisbandBrigade(instance, 
                      brigadeId, 
                      sampleDisbandBrigadeCallback, 
                      NULL);
    gNumOperations++;
    processTasks();

    wprintf(L"Testing End Disband Brigade\n"); 
}

///////////////////////////////////////////////////////////////////////////////
void sampleInviteBrigadeMemberCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Invite Brigade Member Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbInviteBrigadeMember"))
    {
        printf(" *** [ InviteBrigadeMember FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ InviteBrigadeMember SUCCESS!!! ] \n");
    }

    wprintf(L"=============[ Invite Brigade Member Response END ]================\n");
    GSI_UNUSED(userData);
}
///////////////////////////////////////////////////////////////////////////////
void sampleInviteToBrigade(GSBInstancePtr gInstance,
                               gsi_u32 brigadeId, 
                               gsi_u32 invitedProfileId)
{
    wprintf(L"============== Testing : Invite to Brigade ===========\n");
    wprintf(L"\tBrigade Id = %d\n", brigadeId);
    wprintf(L"\tProfile Id = %d\n", invitedProfileId);
    gsbInviteToBrigade(gInstance,
                       brigadeId, 
                       invitedProfileId, 
                       sampleInviteBrigadeMemberCallback, 
                       NULL);
	gNumOperations++;
	processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleAcceptJoinCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Accept Join Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbAcceptJoin"))
    {
        printf(" *** [ AcceptJoin FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ AcceptJoin SUCCESS!!! ] \n");
    }

    wprintf(L"=============[ Accept Join Response END ]================\n");
	GSI_UNUSED(userData);
}

///////////////////////////////////////////////////////////////////////////////
void sampleAcceptJoin(GSBInstancePtr gInstance,gsi_u32 brigadeId, gsi_u32 profileId)
{
    wprintf(L"============== Testing : Accept Join ===========\n");
    wprintf(L"\tBrigade Id = %d\n", brigadeId);
    wprintf(L"\tProfile Id = %d\n", profileId);
	gsbAnswerJoin(gInstance,brigadeId, profileId, TRUE, sampleAcceptJoinCallback, NULL);
	gNumOperations++;
	processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleBanMemberCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Ban Member Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbBanMember"))
    {
        printf(" *** [ BanMember FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ BanMember SUCCESS!!! ] \n");
    }

    wprintf(L"=============[ Ban Member Response END ]================\n");
	GSI_UNUSED(userData);
}

void sampleBanMember(GSBInstancePtr gInstance,gsi_u32 brigadeId, gsi_u32 profileId)
{
    wprintf(L"============== Testing : Ban Member ===========\n");
    wprintf(L"\tBrigade Id = %d\n", brigadeId);
    wprintf(L"\tProfile Id = %d\n", profileId);
    gsbBanMember(gInstance, 
                 brigadeId, 
                 profileId, 
                 sampleBanMemberCallback, 
                 NULL);
	gNumOperations++;
	processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleRescindInviteCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Rescind Invite Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbRescindInvite"))
    {
        printf(" *** [ RescindInvite FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ RescindInvite SUCCESS!!! ] \n");
    }

    wprintf(L"=============[ Rescind Invite Response END ]================\n");
	GSI_UNUSED(userData);
}

void sampleRescindInvite(GSBInstancePtr gInstance,gsi_u32 brigadeId, gsi_u32 profileId)
{
    wprintf(L"============== Testing : Rescind Invite ===========\n");
    wprintf(L"\tBrigade Id = %d\n", brigadeId);
    wprintf(L"\tProfile Id = %d\n", profileId);
    gsbRescindInvite(gInstance, 
                     brigadeId, 
                     profileId, 
                     sampleRescindInviteCallback, 
                     NULL);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleLeaveBrigadeCallback(GSResult operationResult, GSResultSet *resultSet, void *userData)
{
    wprintf(L"=============[ Leave Brigade Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbLeaveBrigade"))
    {
        *((GSResult *)userData) = !GS_SUCCESS;
        printf(" *** [ LeaveBrigade FAILED!!! ] \n");
    }
    else
    {
        *((GSResult *)userData) = GS_SUCCESS;
        printf(" *** [ LeaveBrigade SUCCESS!!! ] \n");
    }

    wprintf(L"=============[ Leave Brigade Response END ]================\n");
}
///////////////////////////////////////////////////////////////////////////////
void sampleLeaveBrigade(GSBInstancePtr gInstance,gsi_u32 brigadeId, GSResult *result)
{
    wprintf(L"============== Testing : Leave Brigade (Brigade Id = %d)===========\n", brigadeId);
    gsbLeaveBrigade(gInstance,
                    brigadeId, 
                    sampleLeaveBrigadeCallback, 
                    result);
    gNumOperations++;
    processTasks();

}
///////////////////////////////////////////////////////////////////////////////
void sampleAnswerInviteCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Answer Invite Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbAnswerInvite"))
    {
        printf(" *** [ AnswerInvite FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ AnswerInvite SUCCESS!!! ] \n");
    }
    wprintf(L"=============[ Answer Invite Response END ]================\n");

    GSI_UNUSED(userData);
}
///////////////////////////////////////////////////////////////////////////////
void sampleAnswerInvite(GSBInstancePtr instance, gsi_u32 brigadeId, gsi_bool answer)
{

    wprintf(L"============== Testing : Answer Invite (%s)===========\n", (answer==gsi_true)?L"Accept ":L"Decline ");
    gsbAnswerInvite(instance ,
                    brigadeId, 
                    answer, 
                    sampleAnswerInviteCallback, 
                    NULL);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleAnswerJoinCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Answer Join Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbAnswerJoin"))
    {
        printf(" *** [ AnswerJoin FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ AnswerJoin SUCCESS!!! ] \n");
    }
    wprintf(L"=============[ Answer Join Response END ]================\n");

    GSI_UNUSED(userData);
}
///////////////////////////////////////////////////////////////////////////////
void sampleAnswerJoin(GSBInstancePtr instance,
                          gsi_u32 brigadeId, 
                          gsi_u32 profileId, 
                          gsi_bool answer)
{
    wprintf(L"============== Testing : Answer Join Brigade %s===========\n", (answer==gsi_true)?L"Accept ":L"Reject ");

    gsbAnswerJoin(instance ,
                  brigadeId, 
                  profileId, 
                  answer, 
                  sampleAnswerJoinCallback, 
                  NULL);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleJoinBrigadeCallback(GSResult operationResult, 
                                   GSResultSet *resultSet, 
                                   void * userData)
{
    wprintf(L"=============[ Join Brigade Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbJoinBrigade"))
    {
        printf(" *** [ JoinBrigade FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ JoinBrigade SUCCESS!!! ] \n");
    }
    wprintf(L"=============[ Join Brigade Response END ]================\n");
    GSI_UNUSED(userData);
}
///////////////////////////////////////////////////////////////////////////////
void sampleJoinBrigade(GSBInstancePtr instance, gsi_u32 brigadeId)
{
    wprintf(L"============== Testing : JoinBrigade ===========\n");

    gsbJoinBrigade(instance, 
                   brigadeId, 
                   sampleJoinBrigadeCallback, 
                   NULL);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleRemoveBrigadeMemberCallback(GSResult operationResult, 
                                           GSResultSet *resultSet, 
                                           void * userData)
{
    wprintf(L"=============[ Remove Member Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbRemoveMember"))
    {
        printf(" *** [ RemoveMember FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ RemoveMember SUCCESS!!! ] \n");
    }
    wprintf(L"=============[ Remove Member Response END ]================\n");
    GSI_UNUSED(userData);
}
///////////////////////////////////////////////////////////////////////////////
void sampleRemoveBrigadeMember(GSBInstancePtr instance, 
                                   gsi_u32 profileIdToRemove, 
                                   gsi_u32 brigadeId)
{
    wprintf(L"============== Testing : Remove Member ===========\n");
    gsbRemoveBrigadeMember(instance,
                           brigadeId, 
                           profileIdToRemove, 
                           sampleRemoveBrigadeMemberCallback, 
                           NULL);
    gNumOperations++;
    processTasks();
}
///////////////////////////////////////////////////////////////////////////////
void samplePromoteToLeaderCallback( GSResult operationResult, 
                                        GSResultSet *resultSet, 
                                        void * userData)
{
    wprintf(L"=============[ Promote to Leader Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbPromoteToLeader"))
    {
        printf(" *** [ Promote To Leader FAILED!!! ] \n");
    }
    else
    {
        printf(" *** [ Promote to Leader  SUCCESS!!! ] \n");
    }
    wprintf(L"=============[ Promote to Leader Response END ]================\n");
    GSI_UNUSED(userData);
}
///////////////////////////////////////////////////////////////////////////////
void samplePromoteToLeader(GSBInstancePtr instance, gsi_u32 brigadeId, gsi_u32 memberId)
{
    wprintf(L"============== Testing : Promote to Leader ===========\n");
    gsbPromoteToLeader(instance, 
                       brigadeId, 
                       memberId,
                       samplePromoteToLeaderCallback, 
                       NULL);
    gNumOperations++;
    processTasks();
}
///////////////////////////////////////////////////////////////////////////////
void sampleGetRoleListByBrigadeIdCallback(GSResult operationResult, 
                                              GSResultSet *resultSet, 
                                              GSBRoleList *roleList, 
                                              void *userData)
{

    gsi_u32  i = 0;
    gsi_bool resultSuccess = gsi_true; 
    wprintf(L"=============[ Get Role List By Brigade Id Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbGetRoleList"))
    {
        printf(" *** [ GetRoleList FAILED!!! ] \n");
        resultSuccess = gsi_false;
    }
    else
    {
        printf(" *** [ GetRoleList SUCCESS!!! ] \n");
        if (roleList == NULL)
        {
            printf ("GetRoleList: RoleList is NULL !!! ");
        }
        else
        {
            printf("\tNumber of roles = %d \n", roleList->mCount);
            for (i = 0; i<roleList->mCount; i++)
            {  
                // Just for now. Use the last role in the list for testing
                wprintf(L"\t====  Role [%d] ====\n", i+1);
                displayRole(&roleList->mRoles[i]);
            }
        }
    }
    wprintf(L"=============[ Get Role List By Brigade Id Response END ]================\n");

    if (userData == NULL || resultSuccess == gsi_false)
    {
        // Role list must be freed by the developer when no longer needed
        gsbFreeRoleList(roleList);
    }
    else
    {
        *((GSBRoleList **)userData) = roleList;
    }
    GSI_UNUSED(userData);
}

void sampleGetRoleListByBrigadeId(GSBInstancePtr gInstance, gsi_u32 brigadeId, GSBRoleList **userRoleList)
{
    wprintf(L"============== Testing : Get Role List By Brigade Id %d ===========\n", brigadeId);
    gsbGetRoleList(gInstance, brigadeId, sampleGetRoleListByBrigadeIdCallback, userRoleList);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleGetMyPendingInvitesAndJoinsCallback(GSResult operationResult, 
                                                   GSResultSet *resultSet,
                                                   GSBBrigadePendingActionsList *actionsList, 
                                                   void *userData)
{
    gsi_u32 i = 0;
    wprintf(L"=============[ Get My Pending Invites And Joins Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbGetMyPendingInvitesAndJoins"))
    {
        printf(" *** [ GetRoleList FAILED!!! ] \n");
        //Developer must free the memory allocated by the SDK.
        gsbFreePendingActionsList(actionsList);
    }
    else
    {
        printf(" *** [ GetRoleList SUCCESS!!! ] \n");
        for (i = 0; i< actionsList->mCount; i++)
        {  
            wprintf(L"============  Pending Action [%d] ============\n", i+1);
            displayPendingAction(&actionsList->mPendingActions[i]);
        }
        if (userData == NULL)
        {
            //Developer must free the memory allocated by the SDK.
            gsbFreePendingActionsList(actionsList);
        }
        else
        {
            *(GSBBrigadePendingActionsList **) userData = actionsList;
        }

    }
    wprintf(L"=============[ Get Pending Invites And Joins Response END ]================\n");


    GSI_UNUSED(userData);
}

//////////////////////////////////////////////////////////////////////////
void sampleGetMyPendingInvitesAndJoins(GSBInstancePtr instance, GSBBrigadePendingActionsList **actionList)
{
    wprintf(L"\n======== Testing: Get My Pending Invites and Joins  ===========\n");
    gsbGetMyPendingInvitesAndJoins(instance, sampleGetMyPendingInvitesAndJoinsCallback, actionList);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleCreateRoleCallback(GSResult operationResult, 
                                  GSResultSet *resultSet, 
                                  gsi_u32 roleId, 
                                  void *userData)
{    
    wprintf(L"=============[ Create Role Response START ]================\n");
    if(!handleRequestResult(operationResult, resultSet, "gsbCreateRole"))
    {
        wprintf(L"*** [Create Role FAILED]\n");
    }
    else
    {
        printf("***[ Create Role SUCCESS!!! ]\n");
        printf("Role ID = %d\n", roleId);

        if (userData != NULL)
        {
            // For this sample we keep track of the new role id.
            // Developers might choose to use what they need.
            *((gsi_u32 *) userData) = roleId ; 
        }
    }
    wprintf(L"=============[ Create Role Response END ]================\n");

    GSI_UNUSED(userData);
}

void sampleCreateCustomRole(GSBInstancePtr gInstance, 
                            GSBRole *role, 
                            GSBEntitlementIdList *entitlementIdList)
{

    wprintf(L"\n======== Testing: Create Role  ===========\n");
    gsbCreateRole(gInstance, role, entitlementIdList, sampleCreateRoleCallback, &role->mRoleId);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleUpdateRoleCallback(GSResult operationResult, 
                              GSResultSet *resultSet, 
                              gsi_u32 roleId, 
                              void *userData)
{    
    wprintf(L"=============[ Update Role Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbUpdateRole"))
    {
        printf("***[ UpdateRole FAILED!!! ]\n");
    }
    else
    {

        printf("*** [ UpdateRole SUCCESS!!! ]\n");
        printf("Role ID = %d\n", roleId);
        if (userData != NULL)
        {
            // For this sample we keep track of the new role id.
            // Developers might choose to use what they need.
            *((gsi_u32 *) userData) = roleId ; 
        }

    }
    wprintf(L"=============[ Update Role Response END ]================\n");

    GSI_UNUSED(userData);
}

///////////////////////////////////////////////////////////////////////////////
void sampleUpdateCustomRole(GSBInstancePtr gInstance, 
                            GSBRole *role, 
                            GSBEntitlementIdList *entitlementIdList)
{
    wprintf(L"\n======== Testing: Update Role  ===========\n");

    gsbUpdateRole(gInstance, 
        role, 
        entitlementIdList, 
        sampleUpdateRoleCallback, 
        NULL);

    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleRemoveRoleCallback(GSResult operationResult, 
                              GSResultSet *resultSet, 
                              void * userData)
{
    wprintf(L"=============[ Remove Role  Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbRemoveRoleEntitlementCallback"))
    {
        printf(" *** [ GetRoleList FAILED!!! ] \n");

        wprintf (L"FAILED with 0x%x\n", operationResult);
    }
    else
    {
        printf("***[ RemoveRole SUCCESS!!! ]\n");
        wprintf(L"=============[ Remove Role Response END ]================\n");
    }
    GSI_UNUSED(userData);
}

///////////////////////////////////////////////////////////////////////////////
void sampleRemoveRole(GSBInstancePtr gInstance, 
                          gsi_u32 brigadeId, 
                          gsi_u32 roleId)
{
    wprintf(L"\n======== Testing: Remove Role  ===========\n");
    wprintf(L"\t Brigade Id = %d\n", brigadeId);
    wprintf(L"\t Role Id    = %d\n", roleId);
    gsbRemoveRole(gInstance, 
                  brigadeId, 
                  roleId, 
                  sampleRemoveRoleCallback, 
                  NULL);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleGetBrigadeMemberListCallback(GSResult operationResult, 
                                        GSResultSet *resultSet, 
                                        GSBBrigadeMemberList *memberList, 
                                        void *userData)
{    
    gsi_u32 i;
    wprintf(L"=============[ Get Brigade Member List Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbGetBrigadeMemberList"))
    {
        wprintf (L"*** [GetBrigadeMember FAILED with 0x%x]\n", operationResult);
        gsbFreeBrigadeMemberList(memberList);
    }
    else
    {
        wprintf(L"*** [ GetBrigadeMember SUCCESS!!! ]\n");
    	
        for (i=0;i<memberList->mCount;i++) 
        {
            wprintf(L"=============================[Brigade Member[%d] ]==========================\n", (i+1));
            displayBrigadeMember(&memberList->mBrigadeMembers[i]);	
	    }
        
        if (userData != NULL)
        {
            *(GSBBrigadeMemberList **) userData = memberList ;
        }
        else
        {
            // Developer must free the memory allocated in the calling function when done with it.
            gsbFreeBrigadeMemberList(memberList);
        }
    }
    wprintf(L"=============[ Get Brigade Member List Response END ]================\n");

    GSI_UNUSED(userData);
}

///////////////////////////////////////////////////////////////////////////////
void sampleGetBrigadeMemberList(GSBInstancePtr gInstance, 
                                gsi_u32 brigadeId, 
                                gsi_u32 status, 
                                GSBBrigadeMemberList **memberList)
{
    wprintf(L"\n======== Testing: Get Brigade member list (brigade Id = %d) ===========\n", brigadeId);

    gsbGetBrigadeMemberList(gInstance, 
                            brigadeId, 
                            status, 
                            sampleGetBrigadeMemberListCallback, 
                            memberList);
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleUpdateBrigadeMemberCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Update Brigade Member Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbUpdateBrigadeMember"))
    {
        printf("***[ UpdateBrigadeMember FAILED!!! ]\n");
    }
    else
    {
        printf("***[ UpdateBrigadeMember SUCCESS!!! ]\n");
    }
    wprintf(L"=============[ Update Brigade Member Response END ]================\n");
    
    GSI_UNUSED(userData);
}
///////////////////////////////////////////////////////////////////////////////
void sampleUpdateBrigadeMember(GSBInstancePtr gInstance, 
                               GSBBrigadeMember *member)
{

    gsbUpdateBrigadeMember(gInstance,
                           member,
                           sampleUpdateBrigadeMemberCallback, 
                           NULL);
	gNumOperations++;
	processTasks();
}


//////////////////////////////////////////////////////////////////////////
void sampleSearchBrigadesCallback(GSResult operationResult, 
                                  GSResultSet *resultSet, 
                                  GSBBrigadeList *brigadeList, 
                                  void * userData)
{
    gsi_u32 i = 0;
    wprintf(L"=============[ Search Brigade Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbSearchBrigades"))
    {
        printf("***[ Search Brigade FAILED!!! ]\n");
        gsbFreeBrigadeList(brigadeList);
    }
    else  if (GS_SUCCEEDED(operationResult))
    {
        printf("***[ Search Brigade SUCCESS!!! ]\n");

        wprintf(L"=====================[Brigade List ]==========================\n");
        if(brigadeList != NULL)
        {
            printf("\t Number of brigades = %d\n", brigadeList->mCount);
            for (i = 0; i < brigadeList->mCount; i++)
            {
                wprintf(L"=============================[Brigade %d ]==========================\n", (i+1));

                displayBrigade(&brigadeList->mBrigades[i]);
            }

            if (userData)
            {
                *(GSBBrigadeList **)userData = brigadeList;
            }
            else
            {
                //Developers must free the memory allocated by the sdk call.
                gsbFreeBrigadeList(brigadeList);
            }
        }
        else
            printf("\t brigadeList = NULL \n");
    }
    wprintf(L"=============[ Search Brigade Response END ]================\n");
    GSI_UNUSED(userData);
}
//////////////////////////////////////////////////////////////////////////
void sampleSearchBrigade(GSBInstancePtr gInstance, 
                             UCS2String     brigadeNameorTag,
                             gsi_u32        brigadeId, 
                             GSBRecruitingType brigadeStatus,
                             time_t         dateCreated,
                             UCS2String     extraParams[],
                             gsi_u32        numExtraParams,
                             gsi_u32        numOfSearchRecords)
{
    GSBSearchFilterList *filterList = (GSBSearchFilterList *) gsimalloc(sizeof(GSBSearchFilterList));
    int                 filterIndex = 0;
    
    wprintf(L"\n======== Testing: Search Brigades  ===========\n");

    // display the search filter
    printf("\tSearch Filter \n");
    if (brigadeNameorTag != NULL) wprintf(L"\t\tPlayerNameOrTag           : %s \n" ,brigadeNameorTag);
    if (brigadeId >0)             wprintf(L"\t\tBrigadeId                 : %d \n" ,brigadeId );
    if (brigadeStatus < GSBRecruitingStatus_Max)
    {
        wprintf(L"\t\tBrigadeStatus             : ");
        switch (brigadeStatus)
        {
        case    GSBRecruitingStatus_Open :
            wprintf (L"Open \n");
            break;
        case    GSBRecruitingStatus_Moderated :   
            wprintf (L"Moderated \n");
            break;
        case    GSBRecruitingStatus_InviteOnly :
            wprintf (L"Invite Only \n");
            break;
        case    GSBRecruitingStatus_NoNewMembers :
            wprintf (L"No New Members \n");
            break;
        default :
            wprintf (L"%d(Unknown Recruiting Type) \n", brigadeStatus);
            break;
        }
    }
    if (dateCreated > 0 ) printf("\t\tDate Created              : %s\n", gsiSecondsToString(&dateCreated));
    if (extraParams != NULL)
    {
        gsi_u32 i = 0;
        for (i= 0; i< numExtraParams; i++) wprintf(L"\t\tExtraParam[%d]   : %s \n" ,i, extraParams[i]);
    }
    if (numOfSearchRecords >0) wprintf(L"\t\tMaximum number of results : %d \n", numOfSearchRecords);

    // Now set the filter values
    memset(filterList, 0, sizeof(GSBSearchFilterList));
    if (brigadeNameorTag!= NULL) 
        filterList->mCount++;
    if (brigadeId >0) 
        filterList->mCount++;
    if (brigadeStatus <GSBRecruitingStatus_Max)
        filterList->mCount++;
    if (dateCreated > 0) 
        filterList->mCount++;
    if (extraParams != NULL)
        filterList->mCount= filterList->mCount+numExtraParams;
    if (numOfSearchRecords >0)
        filterList->mCount++;

    filterList->mFilters = (GSBSearchFilter *) gsimalloc (sizeof(GSBSearchFilter)*filterList->mCount);
    memset(filterList->mFilters, 0, (sizeof(GSBSearchFilter)*filterList->mCount));    

    if (brigadeNameorTag!= NULL)
    {
        filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_Name;
        filterList->mFilters[filterIndex].mType= GSBSearchFilterValueType_Unicode;
        filterList->mFilters[filterIndex].mValue.mValueStr = goawstrdup(brigadeNameorTag);
        filterIndex++;
    }
    if (brigadeId >0)
    {
        filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_BrigadeId;
        filterList->mFilters[filterIndex].mType = GSBSearchFilterValueType_Uint;
        filterList->mFilters[filterIndex].mValue.mValueUint = brigadeId;
        filterIndex++;

    }
    if (brigadeStatus < GSBRecruitingStatus_Max)
    {
        filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_BrigadeStatus;
        filterList->mFilters[filterIndex].mType = GSBSearchFilterValueType_Uint;
        filterList->mFilters[filterIndex].mValue.mValueUint = brigadeStatus;
        filterIndex++;
    }
    if (dateCreated > 0)
    {
        filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_DateCreated;
        filterList->mFilters[filterIndex].mType = GSBSearchFilterValueType_Time;
        filterList->mFilters[filterIndex].mValue.mValueTime = dateCreated;
        filterIndex++;
    }
    if (extraParams != NULL)
    {
        gsi_u32 i = 0;
        for (i= 0; i< numExtraParams; i++)
        {
            filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_ExtraParams;
            filterList->mFilters[filterIndex].mType = GSBSearchFilterValueType_Unicode;
            filterList->mFilters[filterIndex].mValue.mValueStr = goawstrdup(extraParams[i]);
            filterIndex++;
        }
    }
    if (numOfSearchRecords >0)
    {
        filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_MaxResultCount;
        filterList->mFilters[filterIndex].mType = GSBSearchFilterValueType_Uint;
        filterList->mFilters[filterIndex].mValue.mValueUint = numOfSearchRecords;
    }

    gsbSearchBrigades(gInstance, 
                      filterList, 
                      sampleSearchBrigadesCallback, 
                      NULL);
    // free the filter list here
    gsbFreeFilterList(filterList);
    gNumOperations++;
    processTasks();
}

//////////////////////////////////////////////////////////////////////////
void sampleSearchPlayersCallback(GSResult operationResult, 
                                 GSResultSet *resultSet, 
                                 GSBBrigadeMemberList *playerList, 
                                 void * userData)
{
    gsi_u32 i = 0;
    wprintf(L"=============[ Search Player Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbSearchPlayers"))
    {
        printf("***[ Search Players FAILED!!! ]\n");
        gsbFreeBrigadeMemberList(playerList);
    }
    else if (GS_SUCCEEDED(operationResult))
    {
        printf("***[ Search Players SUCCESS!!! ]\n");
        wprintf(L"=============================[Player List ]==========================\n");

        if (playerList != NULL)
        {
            printf("\tNumber of players found = %d\n", playerList->mCount);
       
            for (i = 0; i< playerList->mCount; i++)
            {
                wprintf(L"=============================[Player %d ]==========================\n", (i+1));

                displayBrigadeMember(&playerList->mBrigadeMembers[i]);
            }
            if (userData)
            {
                *(GSBBrigadeMemberList **)userData = playerList;
            }
            else
            {
                // Developers must free the memory allocated by the SDK.
                gsbFreeBrigadeMemberList(playerList);
            }
        }
        else
        {
            printf("\t Player List is NULL\n");
        }
    }
    wprintf(L"=============[ Search Player Response END ]================\n");
    GSI_UNUSED(userData);
}
//////////////////////////////////////////////////////////////////////////
void sampleSearchPlayers(GSBInstancePtr gInstance, 
                             UCS2String     brigadeNameorTag,
                             gsi_u32        brigadeId, 
                             UCS2String     extraParams[],
                             gsi_u32        numExtraParams,
                             gsi_u32        numOfSearchRecords)
{

    GSBSearchFilterList *filterList;
    int                 filterIndex = 0;

    wprintf(L"\n======== Testing: Search Players  ===========\n");

    // display the search filter
    printf("\tSearch Filter \n");
    if (brigadeNameorTag != NULL) wprintf(L"\t\tPlayerNameOrTag           : %s \n" ,brigadeNameorTag);
    if (brigadeId >0)             wprintf(L"\t\tBrigadeId                 : %d \n" ,brigadeId );
                                  
    if (extraParams != NULL)
    {
        gsi_u32 i = 0;
        for (i= 0; i< numExtraParams; i++) wprintf(L"\t\tExtraParam[%d]   : %s \n" ,i, extraParams[i]);
    }
    if (numOfSearchRecords >0) wprintf(L"\t\tMaximum number of results : %d \n", numOfSearchRecords);

    // Now set the filter values
    filterList = (GSBSearchFilterList *) gsimalloc(sizeof(GSBSearchFilterList));
    filterList->mCount = 0;

    if (brigadeNameorTag!= NULL) filterList->mCount++;
    if (brigadeId >0) filterList->mCount++;
    if (extraParams != NULL)filterList->mCount= filterList->mCount+numExtraParams;
    if (numOfSearchRecords >0)filterList->mCount++;
    filterList->mFilters = (GSBSearchFilter *) gsimalloc (sizeof(GSBSearchFilter) * filterList->mCount);


    if (brigadeNameorTag!= NULL)
    {
        filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_Name;
        filterList->mFilters[filterIndex].mType= GSBSearchFilterValueType_Unicode;
        filterList->mFilters[filterIndex++].mValue.mValueStr = goawstrdup(brigadeNameorTag);
    }
    if (brigadeId >0)
    {
        filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_BrigadeId;
        filterList->mFilters[filterIndex].mType = GSBSearchFilterValueType_Uint;
        filterList->mFilters[filterIndex++].mValue.mValueUint = brigadeId;
    }
    
    if (extraParams != NULL)
    {
        gsi_u32 i = 0;
        for (i= 0; i< numExtraParams; i++)
        {
            filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_ExtraParams;
            filterList->mFilters[filterIndex].mType = GSBSearchFilterValueType_Unicode;
            filterList->mFilters[filterIndex++].mValue.mValueStr = goawstrdup(extraParams[i]);
        }
    }
    if (numOfSearchRecords >0)
    {
        filterList->mFilters[filterIndex].mKey = GSBSearchFilterKey_MaxResultCount;
        filterList->mFilters[filterIndex].mType = GSBSearchFilterValueType_Uint;
        filterList->mFilters[filterIndex++].mValue.mValueUint = numOfSearchRecords;
    }
    gsbSearchPlayers(gInstance, 
                     filterList, 
                     sampleSearchPlayersCallback, 
                     NULL);
    gsbFreeFilterList(filterList);
    gNumOperations++;
    processTasks();

}
//////////////////////////////////////////////////////////////////////////
void sampleUploadCompleteCallback(GSResult operationResult, 
                                  GSResultSet *resultSet, 
                                  void *userData)
{
	if(!handleRequestResult(operationResult, resultSet, "gsbUploadLogo"))
    {
        printf("******[ UploadLogo FAILURE!!! ]\n");
    }
    else
    {
	    printf("******[ UploadLogo SUCCESS!!! ]\n");
    }
    GSI_UNUSED(userData);
     
}

//////////////////////////////////////////////////////////////////////////
void sampleUploadBrigadeLogo(GSBInstancePtr gInstance, 
                                 gsi_char *fileName, 
                                 gsi_u32 brigadeId)
{
    wprintf(L"\n============== Testing: Upload Brigade Logo ===========\n");
    wprintf(L"\t Brigade Id = %d\n", brigadeId);
    wprintf(L"\t File Name  = %s\n", fileName);
	gsbUploadLogo(gInstance, 
                  fileName, 
                  brigadeId,  
                  sampleUploadCompleteCallback, 
                  NULL);
	gNumOperations++;
	processTasks();

}
//////////////////////////////////////////////////////////////////////////

void sampleDownloadCompleteCallback(GSResult operationResult, 
                                    GSResultSet *resultSet, 
                                    void *userData)
{
	if(!handleRequestResult(operationResult, resultSet, "gsbDownloadLogo"))
    {
        printf("***[ DownloadLogo FAILED!!! ]\n");
    }
    else
    {
	    printf("***[ DownloadLogo SUCCESS!!! ]\n");
    }GSI_UNUSED(userData);
     
}
//////////////////////////////////////////////////////////////////////////
void sampleDownloadBrigadeLogo(GSBInstancePtr gInstance, 
                                   gsi_char *fileName,
                                   GSBBrigadeLogo *blogo)
{
    wprintf(L"\n============== Testing: Download Brigade Logo ===========\n");
    wprintf(L"\t File Name = %s\n", fileName);

	gsbDownloadLogo(gInstance, fileName, blogo ,  sampleDownloadCompleteCallback, NULL);
	gNumOperations++;
	processTasks();

}
///////////////////////////////////////////////////////////////////////////////
void sampleSaveBrigadeCallback(GSResult operationResult, 
                               GSResultSet *resultSet, 
                               GSBBrigade *brigade, 
                               void *userData)
{
	if(handleRequestResult(operationResult, resultSet, "gsbSaveBrigade"))
    {
        wprintf (L"*** [Update Brigade SUCCESS] \n");
    
	    if (brigade!= NULL)
	    {
		     displayBrigade(brigade);		
	    }
        if (userData != NULL)
        {
            // TODO explain how the user data is used
            *(gsi_u32 *)userData = brigade->mBrigadeId;
        }
    }
    else
    {
        wprintf (L"*** [ Update Brigade FAILED] \n");
    }

    //Developers must free the memory allocated.
    //IMPORTANT: 
    //This callback differs from the other callbacks
    //The brigade passed to the API is reused in the
    //callback. Therefore, free the brigade after the callback 
    //completes in one place only. 
    gsbFreeBrigade(brigade);
	GSI_UNUSED(userData);
}
///////////////////////////////////////////////////////////////////////////////
void sampleSaveBrigade(GSBInstancePtr gInstance, 
                       GSBBrigade *brigade, 
                       gsi_bool update, 
                       gsi_u32 *brigadeId)
{
    if (update)
    {
        wprintf(L"======[ Update Brigade ]===========\n");
    }
    else
    {
        wprintf(L"======[ Create Brigade ]===========\n");
    }
    if (brigade != NULL)
    {
        gsbSaveBrigade(gInstance, brigade, sampleSaveBrigadeCallback, brigadeId);
        gNumOperations++;
        processTasks();
    }
}

///////////////////////////////////////////////////////////////////////////////
void sampleCreateBrigade(GSBInstancePtr instance, 
                         UCS2String brigadeName, 
                         UCS2String brigadeTag, 
                         UCS2String brigadeUrl, 
                         UCS2String brigadeMOTD, 
                         GSBRecruitingType recruitingType,
                         gsi_u32    *brigadeId) // userData
{
    GSBBrigade *newBrigade = NULL ;
    wprintf(L"\n============== Testing Start: Create Brigade ===========\n");

    newBrigade = (GSBBrigade *) gsimalloc(sizeof(GSBBrigade));

    memset(newBrigade, 0, sizeof(GSBBrigade));

    newBrigade->mName    = goawstrdup(brigadeName);
    newBrigade->mTag     = goawstrdup(brigadeTag);
    newBrigade->mUrl     = goawstrdup(brigadeUrl);
    newBrigade->mMessageOfTheDay = goawstrdup(brigadeMOTD);
    newBrigade->mRecruitingType  = recruitingType;
    //Important: The brigade passed to the API call is updated by the callback
    //It should only be deleted when no longer needed after the callback completes.
    //For this particular sample, it is released in the callback.
    sampleSaveBrigade(instance, newBrigade, gsi_false, brigadeId);
}

///////////////////////////////////////////////////////////////////////////////
void sampleGetEntitlementListCallback(GSResult operationResult, 
                                      GSResultSet *resultSet, 
                                      GSBEntitlementList *entitlementList, 
                                      void *userData)
{    
    gsi_u32 i = 0;
    wprintf(L"=============[ Get Entitlement List By Game Id Response START ]================\n");
    if(!handleRequestResult(operationResult, resultSet, "gsbGetEntitlementList"))
    {
        wprintf (L"*** [ Get Entitlement List By Game Id FAILED] \n");
        gsbFreeEntitlementList(entitlementList);
    }
    else if (GS_SUCCEEDED(operationResult))
    {
        wprintf (L"*** [ Get Entitlement List By Game Id SUCCESS] \n");
        wprintf(L"EntitlementList Count = %d\n",entitlementList->mCount);  
        wprintf(L"=======================[Entitlement List ]======================\n");
        for (i=0; i< entitlementList->mCount; i++)
        {
            wprintf(L"=======================[Entitlement %d ]======================\n", (i+1));

            displayEntitlement(&entitlementList->mEntitlements[i]);
        }
        if (userData != NULL)
        {
            *(GSBEntitlementList **) userData = entitlementList;
        }
        else
        {
            gsbFreeEntitlementList(entitlementList);
        }
    }
    wprintf(L"=============[ Get Entitlement List By Game Id END ]================\n");
    GSI_UNUSED(userData);
}
//////////////////////////////////////////////////////////////////////////
void sampleGetEntitlementListByGameId(GSBInstancePtr gInstance, void *userdata)
{
    wprintf(L"\n============== Testing Start: Get Entitlement List ===========\n");
    gsbGetEntitlementList(gInstance,sampleGetEntitlementListCallback, userdata);
    gNumOperations++;
    processTasks();
}
///////////////////////////////////////////////////////////////////////////////
void sampleDisplayRoleEntitlement(GSBRoleEntitlement *roleEntitlement)
{
    wprintf(L"\tRole Id             : %d\n", roleEntitlement->mRoleId);
    wprintf(L"\tEnttilement Id      : %d\n",roleEntitlement->mEntitlementId);
    wprintf(L"\tRole-Entitlement Id : %d\n",roleEntitlement->mRoleEntitlementId);
};
///////////////////////////////////////////////////////////////////////////////
void sampleGetRoleEntitlementListByEntitlementCallback(GSResult operationResult, 
                                                       GSResultSet *resultSet, 
                                                       GSBRoleIdList *roleIdList, 
                                                       void *userData)
{    
    gsi_u32 i = 0;
    wprintf(L"=============[ Get Role Id List By Entitlement Id Response START ]================\n");
    if(!handleRequestResult(operationResult, resultSet, "gsbGetEntitlementList"))
    {
        wprintf (L"*** [Get Role Id List By Entitlement Id FAILED] \n");
        
        // Free the memory allocated by the SDK.
        gsbFreeRoleIdList(roleIdList);
    }
    else if (GS_SUCCEEDED(operationResult))
    {
        wprintf (L"*** [Get Role Id List By Entitlement Id SUCCESS] \n");
        printf("Number of Roles Found = %d\n", roleIdList->mCount);
        wprintf(L"=======================[Role Id List ]======================\n");
        for (i=0; i< roleIdList->mCount; i++)
        {
            wprintf(L"\tRoleId[%d ] = %d\n",i, roleIdList->mRoleIds[i]);
        }
        if (userData)
        {
            *(GSBRoleIdList **) userData = roleIdList;
        }
        else
        {
            // user data is null so free the roleIdList 
            // so free the memory allocated by the SDK.
            gsbFreeRoleIdList(roleIdList);
        }
    }
    wprintf(L"=============[ Get Role Id List By Entitlement Id END ]================\n");

    GSI_UNUSED(userData);
}
//////////////////////////////////////////////////////////////////////////
void sampleGetRoleEntitlementListByEntitlementId(GSBInstancePtr instance, gsi_u32 brigadeId, gsi_u32 entitlementId)
{
    wprintf(L"\n============== Testing Start: Get Role Id List By Entitlement Id ===========\n");
    wprintf(L"\tEntitlement Id    : %d\n", entitlementId);
    gsbGetRoleIdListByEntitlementId(instance,
                                    brigadeId, 
                                    entitlementId, 
                                    sampleGetRoleEntitlementListByEntitlementCallback, 
                                    NULL);
    gNumOperations++;
    processTasks();
    wprintf(L"============== Testing End: Get Role Id List By Entitlement Id ===========\n");
}
///////////////////////////////////////////////////////////////////////////////
void sampleGetRoleEntitlementListByRoleIdCallback(GSResult operationResult, 
                                                  GSResultSet *resultSet, 
                                                  GSBEntitlementIdList *entitlementIdList, 
                                                  void *userData)
{    
    gsi_u32 i = 0;
    wprintf(L"=============[ Get Entitlement Id List By Role Id Response START ]================\n");
    if(!handleRequestResult(operationResult, resultSet, "gsbGetEntitlementList"))
    {
        wprintf (L"*** [Get Entitlement Id List By Role Id FAILED] \n");
        //Developers must free the memory allocated by the SDK.
        gsbFreeEntitlementIdList(entitlementIdList);
    }
    else if (GS_SUCCEEDED(operationResult))
    {
        wprintf (L"*** [Get Entitlement Id List By Role Id SUCCESS] \n");
        printf("Number of of entitlement ids found = %d\n", entitlementIdList->mCount);
        wprintf(L"=======================[Entitlement Id List ]======================\n");
        for (i=0; i< entitlementIdList->mCount; i++)
        {
            printf("\tEntitlementIds[%d] = %d\n", i, entitlementIdList->mEntitlementIds[i]);
        }
        if (userData)
        {
            *(GSBEntitlementIdList **)userData = entitlementIdList;
        }
        else
        {
            //Developers must free the memory allocated by the SDK.
            gsbFreeEntitlementIdList(entitlementIdList);
        }
    }
    wprintf(L"=============[ Get Entitlement Id List By Role Id END ]================\n");
    GSI_UNUSED(userData);
}
//////////////////////////////////////////////////////////////////////////
void sampleGetRoleEntitlementListByRoleId(GSBInstancePtr instance, 
                                          gsi_u32 brigadeId, 
                                          gsi_u32 roleId)
{
    wprintf(L"\n============== Testing Start: Get Entitlement Id List By Role Id ===========\n");
    wprintf(L"Role Id    : %d\n", roleId);

    gsbGetEntitlementIdListByRoleId(instance,
                                    brigadeId, 
                                    roleId, 
                                    sampleGetRoleEntitlementListByRoleIdCallback, 
                                    NULL);
    gNumOperations++;
    processTasks();
    wprintf(L"============== Testing End: Get Entitlement Id List By Role Id ===========\n");
}

///////////////////////////////////////////////////////////////////////////////
void sampleSendMessageToBrigadeCallback(GSResult operationResult, 
                                        GSResultSet *resultSet, 
                                        void * userData)
{
    wprintf(L"=============[ Send Brigade Message Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbSendMessageToBrigade"))
    {
        wprintf (L"*** [Send Message to Brigade FAILED] \n");
    }
    else if (GS_SUCCEEDED(operationResult))
    {
        wprintf (L"*** [Send Message to Member SUCCESS] \n");
    }
    wprintf(L"=============[ Send Brigade Message Response END ]================\n");
    GSI_UNUSED(userData);
}
//////////////////////////////////////////////////////////////////////////
void sampleSendMessageToBrigade(GSBInstancePtr gInstance, 
                                gsi_u32 brigadeId, 
                                UCS2String msg)
{
    wprintf(L"\n============== Testing: Send Message to Member ===========\n");
    wprintf(L"\tBrigade Id  : %d\n", brigadeId);
    wprintf(L"\tmsg         : %s\n", msg);
   
    gsbSendMessageToBrigade(gInstance, 
                            brigadeId, 
                            msg, 
                            sampleSendMessageToBrigadeCallback, 
                            NULL );
    gNumOperations++;
    processTasks();
}

///////////////////////////////////////////////////////////////////////////////
void sampleSendMessageToMemberCallback(GSResult operationResult, 
                                       GSResultSet *resultSet, 
                                       void * userData)
{
    wprintf(L"=============[ Send Member Message Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbSendMessageToBrigade"))
    {
        wprintf (L"*** [Send Message to Member FAILED] \n");
    }
    else if (GS_SUCCEEDED(operationResult))
    {
        wprintf(L"*** [Send Message to Member SUCCESS] \n");
    }
    wprintf(L"=============[ Send Member Message Response END ]================\n");
    GSI_UNUSED(userData);
}
//////////////////////////////////////////////////////////////////////////
void sampleSendMessageToMember(GSBInstancePtr gInstance, 
                               gsi_u32 to, 
                               UCS2String msg)
{
    wprintf(L"\n============== Testing: Send Message to Member ===========\n");
    wprintf(L"\tTo  : %d\n", to);
    wprintf(L"\tmsg : %s\n", msg);
    gsbSendMessageToMember(gInstance, 
                           to , 
                           msg, 
                           sampleSendMessageToMemberCallback, 
                           NULL );
    gNumOperations++;
    processTasks();
}

//////////////////////////////////////////////////////////////////////////
void sampleGetBrigadesByProfileIdCallback(GSResult operationResult, 
                                          GSResultSet *resultSet, 
                                          GSBBrigadeList *brigadeList, 
                                          void * userData)
{
    wprintf(L"=============[ Get Brigades By ProfileId Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbGetBrigadesByProfileId"))
    {
        wprintf (L"FAILED \n");
        //Developer must free the memory allocated by the SDK when no longer needed.
        gsbFreeBrigadeList(brigadeList);
    }
    else if (GS_SUCCEEDED(operationResult))
    {
        gsi_u32 i = 0;

        wprintf(L"=====================[Brigade List ]==========================\n");
        for (i = 0; i < brigadeList->mCount; i++)
        {
            wprintf(L"=============================[Brigade %d ]==========================\n", (i+1));
            displayBrigade(&brigadeList->mBrigades[i]);
        }
        // This particular callback passes the brigadeList back in the user data
        if (userData != NULL)
        {
            *(GSBBrigadeList **)userData = brigadeList;
        }
        else
        {
            //Developer must free the memory allocated by the SDK when no longer needed.
            gsbFreeBrigadeList(brigadeList);
        }
    }
    wprintf(L"=============[ Get Brigades By ProfileId Response END ]================\n");
    
    
    GSI_UNUSED(userData);
}
//////////////////////////////////////////////////////////////////////////
void sampleGetBrigadesByProfileId(GSBInstancePtr instance, 
                                  gsi_u32 profileId, 
                                  GSBBrigadeList **theBrigadeList)
{
    //_CrtMemCheckpoint( &s1 );

    gsbGetBrigadesByProfileId(instance, profileId, sampleGetBrigadesByProfileIdCallback, theBrigadeList);
    gNumOperations++;
    processTasks();
}

//////////////////////////////////////////////////////////////////////////
void sampleGetBrigadeHistoryCallback(GSResult operationResult, 
                                     GSResultSet *resultSet, 
                                     GSBBrigadeHistoryList *historyList, 
                                     void * userData)
{
    wprintf(L"=============[ Get Brigades History Response START ]================\n");
    if(!handleRequestResult(operationResult, resultSet, "gsbGetBrigadeHistory"))
    {
        wprintf(L"***[GetBrigadeHistory FAILED!]\n");
        // The developer must free the memory allocated for the historyList
        gsbFreeBrigadeHistoryList(historyList);

    }
    else if (GS_SUCCEEDED(operationResult))
    {    
        gsi_u32 i = 0;
        
        wprintf(L"***[GetBrigadeHistory SUCCESS!]");

        wprintf(L"=====================[History List ]==========================\n");
        for (i = 0; i < historyList->mCount; i++)
        {
            wprintf(L"=======================[History Entry %d ]=====================\n", (i+1));
            displayBrigadeHistory(&historyList->mBrigadeHistory[i]);

            // set the test value for the gMatchIdForMatchHistory
            if (wcsncmp(historyList->mBrigadeHistory[i].mAccessLevel, L"Match", wcslen(L"Match")) == 0)
            {
                if ((historyList->mBrigadeHistory[i].mReferenceId != 0) &&
                    (gMatchIdForMatchHistory == 0))
                {
                   gMatchIdForMatchHistory = historyList->mBrigadeHistory[i].mReferenceId;
                }
            }
        }
        if (userData)
        {
            *(GSBBrigadeHistoryList **)userData = historyList;
        }
        else
        {
            gsbFreeBrigadeHistoryList(historyList);
        }
    }
    wprintf(L"=============[ Get Brigades History Response END ]================\n");

    GSI_UNUSED(userData);
     
}
//////////////////////////////////////////////////////////////////////////
void sampleGetBrigadeHistory(GSBInstancePtr gInstance, 
                             gsi_u32 brigadeId, 
                             gsi_u32 profileId, 
                             GSBBrigadeHistoryAccessLevel entryType)
{
    printf("Get Brigade History\n\tProfile Id = %d\n\tBrigade Id = %d\n\tAccess Level = ", profileId, brigadeId);
    switch(entryType)
    {

        case GSBBrigadeHistoryAccessLevel_Admin:
            printf("Admin \n");
            break;
        case GSBBrigadeHistoryAccessLevel_Match :
            printf("Match \n");
            break;
        case GSBBrigadeHistoryAccessLevel_Leader :
            printf("Leader \n");
            break;
        case GSBBrigadeHistoryAccessLevel_Public:
            printf("Public \n");
            break;
        case GSBBrigadeHistoryAccessLevel_Member:
            printf("Member \n");
            break;
        case GSBBrigadeHistoryAccessLevel_All:
            printf("All \n");
            break;
        case GSBBrigadeHistoryAccessLevel_None:
            printf("None \n");
            break;
    }
    gsbGetBrigadeHistory(gInstance, 
                         brigadeId, 
                         profileId, 
                         entryType,  
                         sampleGetBrigadeHistoryCallback, 
                         NULL);
    gNumOperations++;
    processTasks();  
}
//////////////////////////////////////////////////////////////////////////
void sampleGetBrigadeMatchHistoryCallback(GSResult operationResult, 
                                          GSResultSet *resultSet, 
                                          GSBBrigadeHistoryList *historyList, 
                                          void * userData)
{
    wprintf(L"=============[ Get Brigades History Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbGetBrigadeHistory"))
    {
        wprintf (L"FAILED \n");
        // The developer must free the memory allocated for the historyList
        gsbFreeBrigadeHistoryList(historyList);
    }
    else if (GS_SUCCEEDED(operationResult))
    {
        gsi_u32 i = 0;
        wprintf(L"=====================[Brigade List ]==========================\n");
        for (i = 0; i < historyList->mCount; i++)
        {
            wprintf(L"=============================[Brigade %d ]==========================\n", (i+1));
            displayBrigadeHistory(&historyList->mBrigadeHistory[i]);
        }
        if (userData)
        {
            *(GSBBrigadeHistoryList **) userData = historyList;
        }
        else
        {
            gsbFreeBrigadeHistoryList(historyList);
        }
    }
    wprintf(L"=============[ Get Brigades History Response END ]================\n");

    GSI_UNUSED(userData);
}

//////////////////////////////////////////////////////////////////////////
void sampleGetBrigadeMatchHistory(GSBInstancePtr gInstance, 
                                  gsi_u32 matchId)
{
    gsbGetBrigadeMatchHistory(gInstance, 
                              matchId, 
                              sampleGetBrigadeMatchHistoryCallback, 
                              NULL);
    gNumOperations++;
    processTasks();
}

//////////////////////////////////////////////////////////////////////////
void sampleUpdateBrigade( GSBInstancePtr instance, GSBBrigade *brigade)
{
    wprintf(L"============== Testing Start: Update a Brigade ===========\n");
    sampleSaveBrigade(instance, brigade, gsi_true, NULL);
    wprintf(L"============== Testing End: Update a Brigade ===========\n");
}

///////////////////////////////////////////////////////////////////////////////
void sampleUpdateMemberNickAndEmailCallback(GSResult operationResult, GSResultSet *resultSet, void * userData)
{
    wprintf(L"=============[ Update Nick and Email Response START ]================\n");

    if(!handleRequestResult(operationResult, resultSet, "gsbAcceptJoin"))
    {
        printf(" *** [ UpdateMemberNickAndEmail FAILED!!! ] \n");
        if (userData != NULL) *(gsi_bool *)userData = gsi_false; 
    }
    else
    {
        printf(" *** [ UpdateMemberNickAndEmail SUCCESS!!! ] \n");
        if (userData!= NULL) *(gsi_bool *)userData = gsi_true;
    }
    
    wprintf(L"=============[ Update Nick and Email Response END ]================\n");
    GSI_UNUSED(userData);
}

///////////////////////////////////////////////////////////////////////////////
gsi_bool sampleUpdateMemberNickAndEmail(GSBInstancePtr gInstance, gsi_u32 brigadeId )
{
    gsi_bool success = gsi_false;
    wprintf(L"============== Testing : Update Nick Name and Email ===========\n");
    gsbUpdateMemberEmailAndNick(gInstance,
                                brigadeId,
                                sampleUpdateMemberNickAndEmailCallback, 
                                &success);
    gNumOperations++;
    processTasks();
    return success;
}
//////////////////////////////////////////////////////////////////////////
// Helper Functions for the tests
//////////////////////////////////////////////////////////////////////////

//Authenticate a player
int login(playerInfo *player, gsi_char *loginNick, gsi_char *loginPass)
{
	if(player->mBrigadeInstance != NULL) gsbShutdown(player->mBrigadeInstance);

	// Authenticate player via Auth Service to get Login Certificate
	if (!authenticatePlayer(player, loginNick, loginPass))
	    return cleanupAndQuit(-1);

	
	gsbInitialize(&player->mBrigadeInstance, 
                  GAMEID, 
                  player->mLogin->mLoginCert, 
                  player->mLogin->mLoginPrivDat);

	return 1;
}

// As part of the test case initialization, either create or retrieve 
// and return the brigade Id for the "Leader"
gsi_u32 getLeaderBrigadeId()
{

    gsi_u32 brigadeId = 0;

    sampleGetBrigadesByProfileId(Leader.mBrigadeInstance, 
                                 Leader.mLogin->mLoginCert->mProfileId, 
                                 &Leader.mBrigadeList);
    if (Leader.mBrigadeList != NULL && Leader.mBrigadeList->mCount > 0 )
    {
        // Assume the first one in the list is the brigade we want
        brigadeId = Leader.mBrigadeList->mBrigades[0].mBrigadeId;
        printf("Leader's brigade Id = %d ", brigadeId);
    }
    else
    {
        UCS2String brigadeName = gsimalloc(sizeof(unsigned short)*24);
        UCS2String tagName= gsimalloc(sizeof(unsigned short)*24);         
        time_t now = current_time();

        wsprintf(brigadeName,L"B%d", now);
        wsprintf(tagName, L"T%d",now);

        printf("Leader is creating a brigade \n");
        // Leader profile does not have any brigades so create a new brigade for testing
        // Now create a moderated brigade
        sampleCreateBrigade(Leader.mBrigadeInstance, 
                            brigadeName, 
                            tagName, 
                            L"", 
                            L"This is a test brigade", 
                            GSBRecruitingStatus_Moderated, 
                            &brigadeId );

        if (brigadeId != 0)
        {
            GSBBrigade   *leaderBrigade = NULL ; 
            // Now check if we were successful
            sampleGetBrigadeById(Leader.mBrigadeInstance,brigadeId, &leaderBrigade );
            if (leaderBrigade == NULL)
            {
                // we are failed
                wprintf (L"%s : Failed to create and retrieve brigade", __FUNCTION__);
                brigadeId = 0;
            }
            gsbFreeBrigade(leaderBrigade);
        }
        gsifree(brigadeName);
        gsifree(tagName); 
    }
    // no need to keep the brigade list around.
    gsbFreeBrigadeList(Leader.mBrigadeList);
    Leader.mBrigadeList = NULL;
    return brigadeId;
}
// Test case initialization
gsi_bool e2eTestInit()
{
    //////////////////////////////////////////////////////////////////////////
    // 1. initialize the GS core for web services
    gsCoreInitialize();

    // Leader and player login, each login creates a brigade instance
    login(&Leader, LEADER_LOGIN_NICK, LEADER_LOGIN_PASSWORD);
    login(&Member, MEMBER_LOGIN_NICK, MEMBER_LOGIN_PASSWORD);
    Leader.mBrigadeId = getLeaderBrigadeId();
    printf("Leader Profile Id: %d\n", Leader.mLogin->mLoginCert->mProfileId);
    printf("Leader Brigade Id: %d\n", Leader.mBrigadeId);
    printf("Player Profile Id: %d\n", Member.mLogin->mLoginCert->mProfileId);
    return(Leader.mBrigadeId != 0);
}

// Display the test result
void displayVerdict(gsi_bool verdictPass, const char* testcase)
{
    if (verdictPass)
    {
        printf ("PASSED: TestCase %s\n", testcase);
    }
    else
    {
        printf ("FAILED: TestCase %s\n", testcase);
    }
}

// Check whether the "Member" is in the expected status
gsi_bool testStepVerifyMemberStatus(GSBInstancePtr instance, 
                                    gsi_u32 brigadeId, 
                                    gsi_u32 status)
{

    GSBBrigadeMemberList *memberList = NULL;
    gsi_bool found = gsi_false;

    sampleGetBrigadeMemberList(instance, brigadeId, status, &memberList);
    if (memberList != NULL)
    {
        gsi_u32 i = 0;

        for (i = 0; (i < memberList->mCount) && !found; i++)
        {
            if (memberList->mBrigadeMembers[i].mProfileId == Member.mLogin->mLoginCert->mProfileId)
            {
                found = gsi_true;
            }
        }

        gsbFreeBrigadeMemberList(memberList);
        memberList = NULL;
    }
    return found;
}

//Given the brigade id, profile id and desired status returns 
// a pointer to the member's information if found
GSBBrigadeMember* testStepGetBrigadeMember(GSBInstancePtr instance, 
                                           gsi_u32 brigadeId, 
                                           gsi_u32 memberProfileId, 
                                           gsi_u32 status)
{

    GSBBrigadeMemberList *memberList = NULL;
    GSBBrigadeMember     *member = NULL;
    gsi_bool found = gsi_false;

    sampleGetBrigadeMemberList(instance, brigadeId, status, &memberList);
    if (memberList != NULL)
    {
        gsi_u32 i = 0;

        for (i = 0; (i < memberList->mCount) && !found; i++)
        {
            if (memberList->mBrigadeMembers[i].mProfileId == memberProfileId)
            {
                found = gsi_true;
                gsbCloneBrigadeMember(&member,&memberList->mBrigadeMembers[i]);
                printf("____Found Brigade Member:\n");
                displayBrigadeMember(member);
            }
        }
        gsbFreeBrigadeMemberList(memberList);
        memberList = NULL;
    }
    return member;
}

// Rerieve the member id of player given the profile and brigade id
gsi_u32 testStepGetMemberId(GSBBrigade *instance, 
                            gsi_u32 brigadeId, 
                            gsi_u32 profileId)
{ 
    GSBBrigadeMemberList *memberList = NULL;
    gsi_u32 memberId = 0;    

    sampleGetBrigadeMemberList(instance, 
                               brigadeId,
                               GSBBrigadeMemberStatus_Active, 
                               &memberList);
    if (memberList != NULL)
    {
        gsi_u32 i = 0;

        for (i = 0; (i < memberList->mCount) ; i++)
        {
            if (memberList->mBrigadeMembers[i].mProfileId == profileId)
            {
                memberId = memberList->mBrigadeMembers[i].mBrigadeMemberId;
                break;
            }
        }

    }
    gsbFreeBrigadeMemberList(memberList);
    memberList = NULL;
    return memberId;
}

// Make sure that player is not a member in a brigade
gsi_bool clearPlayerBrigades(playerInfo *player)
{
    gsi_bool testSuccess = gsi_true;

    // clear player's brigade list
    if (player->mBrigadeList)
    {
        gsbFreeBrigadeList(player->mBrigadeList);
        player->mBrigadeList = NULL;
    }

    // retrieve the list of brigades this player is a member of 
    sampleGetBrigadesByProfileId(player->mBrigadeInstance, 
        player->mLogin->mLoginCert->mProfileId, 
        &player->mBrigadeList );

    // Verify if the member has any brigades
    if (player->mBrigadeList != NULL)
    {
        // if the member is a member of a brigade then member should leave the brigade
        gsi_u32 i = 0;
        for (i= 0 ; ((i< player->mBrigadeList->mCount) && testSuccess) ; i++ )
        {
            GSResult   result = GS_SUCCESS;
            GSBBrigade brigade = player->mBrigadeList->mBrigades[i] ;

            sampleLeaveBrigade(player->mBrigadeInstance, brigade.mBrigadeId, &result);
            if (result != GS_SUCCESS)
            {
                // we have failed this test case 
                printf("FAILURE in %s : Player with profile id %d could not leave brigade with id %d\n", 
                    __FUNCTION__, player->mLogin->mLoginCert->mProfileId, player->mBrigadeId);
                testSuccess = gsi_false;

            }
        }
        gsbFreeBrigadeList(player->mBrigadeList);
        player->mBrigadeList = NULL;
    }
    return testSuccess;
}

//////////////////////////////////////////////////////////////////////////
// End-to-end Test for 
// Requesting to join and being accepted or rejected to a brigade
//////////////////////////////////////////////////////////////////////////
gsi_bool e2eTestJoinAndAnswerJoin(gsi_bool acceptJoin)
{

    gsi_bool testSuccess = gsi_true;

    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // +++ Test Setup
    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();
    if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

    if (testSuccess)
    {
        printf("%s:Making sure that player does not have any brigades\n", __FUNCTION__);
        testSuccess = clearPlayerBrigades(&Member);
        if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);


        // Verify that member does not have any brigades associated with his profile id 
        if (testSuccess)
        {
            // Retrieve the members brigades     
            sampleGetBrigadesByProfileId(Member.mBrigadeInstance, 
                                         Member.mLogin->mLoginCert->mProfileId, 
                                         &Member.mBrigadeList );
            testSuccess = ((Member.mBrigadeList != NULL) && (Member.mBrigadeList->mCount == 0));
            gsbFreeBrigadeList(Member.mBrigadeList);
            Member.mBrigadeList = NULL;
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }
        // +++ Start test +++
        printf("%s:Test Case Body\n", __FUNCTION__);

        //////////////////////////////////////////////////////////////////////////
        // 4. "Member" player sends a join request the "Leader's" brigade
        if (testSuccess )
        {

            printf("%s : Player profile Id %d request to join brigade Id %d \n", 
                __FUNCTION__, Member.mLogin->mLoginCert->mProfileId, Leader.mBrigadeId);
            sampleJoinBrigade(Member.mBrigadeInstance,Leader.mBrigadeId);

            // Verify the "Member's" status is RequestJoin 
            testSuccess = testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                     Leader.mBrigadeId, 
                                                     GSBBrigadeMemberStatus_RequestJoin);
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }

        // Leader gets a list of the pending members in RequestJoin or Invited state.
        if (testSuccess)
        {
            GSBBrigadeMemberList *pendingMemberList = NULL;
            gsi_bool found = gsi_false;

            // Leader can retrieve his/her brigade's pending join requests by calling
            // gsbGetBrigadeMemberList
            sampleGetBrigadeMemberList(Leader.mBrigadeInstance, 
                                           Leader.mBrigadeId, 
                                           (GSBBrigadeMemberStatus_RequestJoin | GSBBrigadeMemberStatus_Invited),
                                           &pendingMemberList);
            if (pendingMemberList != NULL)
            {
                gsi_u32 i = 0;
                for (i = 0; (i< pendingMemberList->mCount ) && !found; i++)
                {
                    if(pendingMemberList->mBrigadeMembers[i].mStatus == GSBBrigadeMemberStatus_RequestJoin)
                    {
                       // For the testing purposes, 
                       // Leader is only interested in answering the request from the test Member we setup.
                       if (pendingMemberList->mBrigadeMembers[i].mProfileId == Member.mLogin->mLoginCert->mProfileId )
                       {
                           found = gsi_true;
                       }
                    }
                }
            }
            gsbFreeBrigadeMemberList(pendingMemberList);
            pendingMemberList = NULL;
            testSuccess = found;
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }
        
        // Leader answers join
        if (testSuccess)
        {
            gsi_bool expectedResult = acceptJoin ? gsi_true : gsi_false;

            printf("%s : Player profile Id %d request to join brigade Id %d \n", 
                __FUNCTION__, Member.mLogin->mLoginCert->mProfileId, Leader.mBrigadeId);

            sampleAnswerJoin(Leader.mBrigadeInstance, 
                             Leader.mBrigadeId, 
                             Member.mLogin->mLoginCert->mProfileId, 
                             acceptJoin);

            // Verify the "Member's" status is the expected status
            testSuccess = (expectedResult ==  
                           testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                     Leader.mBrigadeId, 
                                                     GSBBrigadeMemberStatus_Active)); 
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }
    }
    
    displayVerdict(testSuccess, __FUNCTION__);
    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);

    // +++ Return Test Verdict
    return testSuccess;
}

//////////////////////////////////////////////////////////////////////////
// End-to-end Test for Inviting and accepting/declining invitation
//////////////////////////////////////////////////////////////////////////
gsi_bool e2eTestInviteAndAnswerInvite(gsi_bool acceptInvite)
{
    gsi_bool testSuccess = gsi_true;

    // +++ Test Setup

    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // +++ Test Setup
    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();
    if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

    if (testSuccess)
    {
        printf("%s:Making sure that player does not have any brigades\n", __FUNCTION__);
        testSuccess = clearPlayerBrigades(&Member);
        if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);


        // Verify that member does not have any brigades associated with his profile id 
        if (testSuccess)
        {
            // Retrieve the members brigades     
            sampleGetBrigadesByProfileId(Member.mBrigadeInstance, 
                                         Member.mLogin->mLoginCert->mProfileId, 
                                         &Member.mBrigadeList );
            testSuccess = ((Member.mBrigadeList != NULL) && (Member.mBrigadeList->mCount == 0));
            gsbFreeBrigadeList(Member.mBrigadeList);
            Member.mBrigadeList = NULL;
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }
        // +++ Start test +++
        printf("%s:Test Case Body\n", __FUNCTION__);

        //////////////////////////////////////////////////////////////////////////
        // 4. invite player
        if (testSuccess )
        {
            printf("%s : Player profile Id %d is invited to brigade Id %d \n", 
                __FUNCTION__, Member.mLogin->mLoginCert->mProfileId, Leader.mBrigadeId);

            sampleInviteToBrigade( Leader.mBrigadeInstance, Leader.mBrigadeId, Member.mLogin->mLoginCert->mProfileId );

            // Verify the "Member's" status is RequestJoin 
            testSuccess = testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                     Leader.mBrigadeId, 
                                                     GSBBrigadeMemberStatus_Invited);
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }

        ////////////////////////////////////////////////////////////////////////
        // 5. Member retrieves his/her invites
        if (testSuccess)
        {
            GSBBrigadePendingActionsList *actionList = NULL;
            gsi_bool found = gsi_false;

            sampleGetMyPendingInvitesAndJoins(Member.mBrigadeInstance, &actionList);
            if (actionList != NULL)
            {
                gsi_u32 i = 0;
                for (i = 0; (i< actionList->mCount ) && !found; i++)
                {
                    if((actionList->mPendingActions[i].mStatus == GSBBrigadeMemberStatus_Invited) &&
                       (Leader.mBrigadeId == actionList->mPendingActions[i].mBrigadeId))

                    {
                        found = gsi_true;
                    }
                }
            }
            gsbFreePendingActionsList(actionList);
            actionList = NULL;
            testSuccess = found;
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }

        //////////////////////////////////////////////////////////////////////////
        // 5. Member answers invitation
        if (testSuccess)
        {
            gsi_bool expectedResult = acceptInvite ? gsi_true : gsi_false;

            // Member answers his invite
            sampleAnswerInvite(Member.mBrigadeInstance, Leader.mBrigadeId, acceptInvite);

            // Verify the "Member's" status is the expected status
            testSuccess = (expectedResult == 
                           testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                     Leader.mBrigadeId, 
                                                     GSBBrigadeMemberStatus_Active)); 
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }
    }

    displayVerdict(testSuccess, __FUNCTION__);

    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);

    // +++ Return Test Verdict
    return testSuccess;
}

//////////////////////////////////////////////////////////////////////////
// End-to-end Test for Inviting and withdrawing invitation
//////////////////////////////////////////////////////////////////////////
int e2eTestInviteAndRescindInvite()
{
    gsi_bool testSuccess = gsi_true;
    

    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // +++ Test Setup
    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();
    if (testSuccess)
    {
        printf("%s:Making sure that player does not have any brigades\n", __FUNCTION__);
        testSuccess = clearPlayerBrigades(&Member);
        if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);
       
        // Verify that member does not have any brigades associated with his profile id 
        if (testSuccess)
        {
            // Retrieve the members brigades     
            sampleGetBrigadesByProfileId(Member.mBrigadeInstance, 
                                         Member.mLogin->mLoginCert->mProfileId, 
                                         &Member.mBrigadeList );
            testSuccess = ((Member.mBrigadeList != NULL) && (Member.mBrigadeList->mCount == 0));
            gsbFreeBrigadeList(Member.mBrigadeList);
            Member.mBrigadeList = NULL;
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }
        // +++ Start test +++
        printf("%s:Test Case Body\n", __FUNCTION__);

        //////////////////////////////////////////////////////////////////////////
        // 4. invite player
        if (testSuccess )
        {
            printf("%s : Player profile Id %d is invited to brigade Id %d \n", 
                __FUNCTION__, Member.mLogin->mLoginCert->mProfileId, Leader.mBrigadeId);

            sampleInviteToBrigade( Leader.mBrigadeInstance, Leader.mBrigadeId, Member.mLogin->mLoginCert->mProfileId );

            // Verify the "Member's" status is RequestJoin 
            testSuccess = testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                     Leader.mBrigadeId, 
                                                     GSBBrigadeMemberStatus_Invited);
            if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

        }
    }

    // Verify that member does not have any brigades associated with his profile id 
    if (testSuccess)
    {
        // Retrieve the members brigades     
        sampleGetBrigadesByProfileId(Member.mBrigadeInstance, 
                                     Member.mLogin->mLoginCert->mProfileId, 
                                     &Member.mBrigadeList );
        testSuccess = ((Member.mBrigadeList != NULL) && (Member.mBrigadeList->mCount == 0));
        gsbFreeBrigadeList(Member.mBrigadeList);
        Member.mBrigadeList = NULL;
        if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

    }

    //////////////////////////////////////////////////////////////////////////
    // 5. withdraw the invitation
    if (testSuccess )
    {
        sampleRescindInvite( Leader.mBrigadeInstance, 
                             Leader.mBrigadeId, 
                             Member.mLogin->mLoginCert->mProfileId );

        // Verify the "Member's" status is NOT Invited
        testSuccess = !testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                  Leader.mBrigadeId, 
                                                  GSBBrigadeMemberStatus_Invited);
        if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);

    }
     
    // +++ Verdict

    //////////////////////////////////////////////////////////////////////////
    // 7. Player retrieves his invites and finds none from the Leader's brigade
    if (testSuccess)
    {
        GSBBrigadePendingActionsList *actionList = NULL;
        gsi_bool found = gsi_false;

        sampleGetMyPendingInvitesAndJoins(Member.mBrigadeInstance, &actionList);
        if (actionList != NULL)
        {
            gsi_u32 i = 0;
            for (i = 0; (i< actionList->mCount ) && !found; i++)
            {
                if((actionList->mPendingActions[i].mStatus == GSBBrigadeMemberStatus_Invited) &&
                    (Leader.mBrigadeId == actionList->mPendingActions[i].mBrigadeId))
                {
                    found = gsi_true;
                }
            }
        }
        gsbFreePendingActionsList(actionList);
        actionList = NULL;
        testSuccess = !found;
        if (!testSuccess) printf("%s : TEST - (%d) \n", (testSuccess?"PASSED":"FAILED"), __LINE__);
    }

    // +++ End of test
    displayVerdict(testSuccess, __FUNCTION__);

    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);

    // +++ Return Test Verdict
    return testSuccess;
}
//////////////////////////////////////////////////////////////////////////
// Tests for Managing Roles and Entitlements
//////////////////////////////////////////////////////////////////////////
gsi_bool testRolesAndEntitlements()
{
    gsi_bool testSuccess = gsi_true;
    int testNum = 0 ;

    GSBEntitlementList *gameEntitlementList = NULL;
        
    GSBRoleList *brigadeRoleList = NULL;
    gsi_u32     roleIdforUpdate = 0;
    gsi_u32     roleIdforDelete = 0;

    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // +++ Test Setup
    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();
    
    // TEST - Get EntitlementList By Game Id
    if (testSuccess)
    {
        printf("TEST %d - Get EntitlementList By Game Id\n", ++testNum);
        sampleGetEntitlementListByGameId(Leader.mBrigadeInstance, &gameEntitlementList);
        testSuccess = (gameEntitlementList != NULL) && (gameEntitlementList->mCount>0);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }

    // TEST - Leader retrieves brigade roles
    if (testSuccess)
    {
        // TEST - Now get all the roles for the brigade
        printf("TEST %d -  Get Role List for brigade id %d\n", ++testNum, Leader.mBrigadeId);
        sampleGetRoleListByBrigadeId(Leader.mBrigadeInstance, Leader.mBrigadeId, &brigadeRoleList);
        testSuccess = (brigadeRoleList!= NULL) && (brigadeRoleList->mCount>0);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    }

    // TEST - Leader retrieves brigade roles associated with an entitlement id
    if (testSuccess)
    {
        gsi_u32 i = 0;

        // TEST - Now get all the roles for the brigade
        printf("TEST %d - Get Brigade Role List associated with entitlement id\n", ++testNum);
        for (i=0; i< gameEntitlementList->mCount; i++) 
        {
            wprintf(L"\tEntitlement Name = %s \n", gameEntitlementList->mEntitlements[i].mEntitlementName);
            printf("\tEntitlement Id   = %d \n", gameEntitlementList->mEntitlements[i].mEntitlementId);
            sampleGetRoleEntitlementListByEntitlementId(Leader.mBrigadeInstance, 
                                                        Leader.mBrigadeId, 
                                                        gameEntitlementList->mEntitlements[i].mEntitlementId);
        }
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }

    // TEST - Leader retrieves brigade entitlements associated with a role id
    if (testSuccess)
    {
        gsi_u32 i = 0;

         // TEST - Now get all the roles for the brigade
        printf("TEST %d - Get Brigade Entitlement List associated with role id\n", ++testNum);
        for (i=0; i< brigadeRoleList->mCount; i++) 
        {
            wprintf(L"\tRole Name = %s \n", brigadeRoleList->mRoles[i].mRoleName);
            printf("\tRole Id   = %d \n", brigadeRoleList->mRoles[i].mRoleId);
            sampleGetRoleEntitlementListByRoleId(Leader.mBrigadeInstance, 
                                                Leader.mBrigadeId, 
                                                brigadeRoleList->mRoles[i].mRoleId);
        }
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    }

    //TEST - Create a New Custom Role
    if (testSuccess)
    {
        gsi_u32 i = 0;
        GSBRole *newRole = NULL ;
        GSBEntitlementIdList *eIdList = NULL;
        printf("TEST %d - Create a New Custom Role\n", ++testNum);
        newRole = (GSBRole *) gsimalloc(sizeof(GSBRole));
        memset(newRole, 0, sizeof(GSBRole));
        newRole->mBrigadeId = Leader.mBrigadeId;
        newRole->mRoleId = 0;
        newRole->mIsDefault = gsi_false;
        newRole->mRoleName = goawstrdup(L"Paramedic");
        newRole->mIsGameRole = gsi_false;

        displayRole(newRole);
        // create the entitlementId list from the entitlement list
        eIdList = (GSBEntitlementIdList*)gsimalloc(sizeof(GSBEntitlementIdList));
        eIdList->mCount = gameEntitlementList->mCount;
        eIdList->mEntitlementIds = (gsi_u32 *) gsimalloc(eIdList->mCount * sizeof(gsi_u32));
        for (i=0; i<eIdList->mCount; i++)
        {
            eIdList->mEntitlementIds[i] = gameEntitlementList->mEntitlements[i].mEntitlementId;
        }

        sampleCreateCustomRole(Leader.mBrigadeInstance, newRole, eIdList);
        testSuccess = (newRole->mRoleId >0);

        roleIdforUpdate = newRole->mRoleId;
        gsbFreeRole(newRole);
        newRole = NULL;
        gsbFreeEntitlementIdList(eIdList);
        eIdList = NULL;
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }

    //TEST - Update a Custom Role
    if (testSuccess)
    {
        gsi_u32 i = 0;
        GSBRole *roleToUpdate = gsimalloc(sizeof(GSBRole)) ;
        GSBEntitlementIdList *eIdList = (GSBEntitlementIdList*) gsimalloc(sizeof(GSBEntitlementIdList));
        
        printf("TEST %d -  Update a Custom Role\n", ++testNum);

        memset(roleToUpdate, 0, sizeof(GSBRole));
        roleToUpdate->mBrigadeId   = Leader.mBrigadeId;
        roleToUpdate->mRoleId      = roleIdforUpdate;
        roleToUpdate->mIsDefault   = gsi_false;
        roleToUpdate->mRoleName    = goawstrdup(L"Surgeon");
        roleToUpdate->mIsGameRole  = gsi_false;
        displayRole(roleToUpdate);

        // create the entitlementId list from the entitlement list
        eIdList->mEntitlementIds = gsimalloc(gameEntitlementList->mCount * sizeof(gsi_u32));
        eIdList->mCount = gameEntitlementList->mCount;
        for (i=0; i<eIdList->mCount; i++)
        {
            eIdList->mEntitlementIds[i] = gameEntitlementList->mEntitlements[i].mEntitlementId;
        }

        sampleUpdateCustomRole(Leader.mBrigadeInstance, roleToUpdate, eIdList);
        roleIdforDelete = roleToUpdate->mRoleId;
        gsbFreeRole(roleToUpdate);
        roleToUpdate = NULL;
        gsbFreeEntitlementIdList(eIdList);
        eIdList = NULL;
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }

    //TEST - Remove a Custom Role
    if (testSuccess)
    {
        printf("TEST %d - Remove a Custom Role\n", ++testNum);
        printf("\tRole Id = %d\n", roleIdforDelete);
        sampleRemoveRole(Leader.mBrigadeInstance, Leader.mBrigadeId, roleIdforDelete);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }

    displayVerdict(testSuccess, __FUNCTION__);
    
    // Free allocated memory
     gsbFreeEntitlementList(gameEntitlementList);
     gsbFreeRoleList(brigadeRoleList);

    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);
    return testSuccess;
}

//////////////////////////////////////////////////////////////////////////
// Tests for Retrieving History
//////////////////////////////////////////////////////////////////////////
gsi_bool testRetrievingHistory()
{

    gsi_bool testSuccess = gsi_true;
    int testNum = 0;

    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();

    if (testSuccess)
    {

    printf("TEST %d - Get History\n", ++testNum);
    sampleGetBrigadeHistory(Leader.mBrigadeInstance, 
                                Leader.mBrigadeId, 
                                Member.mLogin->mLoginCert->mProfileId, 
                                GSBBrigadeHistoryAccessLevel_All);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
   
    printf("TEST %d - Get History\n", ++testNum);
    sampleGetBrigadeHistory(Leader.mBrigadeInstance, 
                            Leader.mBrigadeId, 
                            0, 
                            GSBBrigadeHistoryAccessLevel_Member);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    
    printf("TEST %d - Get History\n", ++testNum);
    sampleGetBrigadeHistory(Member.mBrigadeInstance, 
                            Leader.mBrigadeId, 
                            0, 
                            GSBBrigadeHistoryAccessLevel_Leader);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    
    printf("TEST %d - Get History\n", ++testNum);
    sampleGetBrigadeHistory(Leader.mBrigadeInstance, 
                            Leader.mBrigadeId, 
                            0, 
                            GSBBrigadeHistoryAccessLevel_Public);

    printf("TEST %d - Get History\n", ++testNum);
    sampleGetBrigadeHistory(Leader.mBrigadeInstance, 
                            Leader.mBrigadeId, 
                            0, 
                            GSBBrigadeHistoryAccessLevel_Match);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }

    // there is really no verdict here we cannot automate the verification
    // of the existence of a certain history 
    displayVerdict(testSuccess, __FUNCTION__);

    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);
    return testSuccess;
}

//////////////////////////////////////////////////////////////////////////
// Tests for Brigade Search
//////////////////////////////////////////////////////////////////////////
gsi_bool testBrigadeSearch()
{

    gsi_bool testSuccess = gsi_true;
    int testNum = 0;

    time_t now = current_time();
    time_t tenDaysAgoInMsecs = now -((time_t)10*24*60*60*1000);
    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();

    printf("TEST %d - Search Brigades\n", ++testNum);
    sampleSearchBrigade(Leader.mBrigadeInstance, L"B",0, GSBRecruitingStatus_Max, 0, NULL, 0, 0);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    
    printf("TEST %d - Search Brigades\n", ++testNum);
    sampleSearchBrigade(Leader.mBrigadeInstance, L"MWENG",0, GSBRecruitingStatus_Max, 0, NULL, 0, 0);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    
    printf("TEST %d - Search Brigades\n", ++testNum);
    sampleSearchBrigade(Leader.mBrigadeInstance, L"",Leader.mBrigadeId, GSBRecruitingStatus_Max, 0, NULL, 0, 0);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    printf("TEST %d - Search Brigades\n", ++testNum);
    sampleSearchBrigade(Leader.mBrigadeInstance, L"",0, GSBRecruitingStatus_Max, 0, NULL,0, 10);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    printf("TEST %d - Search Brigades\n", ++testNum);
    sampleSearchBrigade(Leader.mBrigadeInstance, L"",0, GSBRecruitingStatus_Max, tenDaysAgoInMsecs, NULL, 0, 0);  
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    printf("TEST %d - Search Brigades\n", ++testNum);
    sampleSearchBrigade(Leader.mBrigadeInstance, L"",0, GSBRecruitingStatus_Max, 0, NULL, 0, 4);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    
    printf("TEST %d - Search Brigades\n", ++testNum);
    sampleSearchBrigade(Leader.mBrigadeInstance, L"",0, GSBRecruitingStatus_Moderated, 0, NULL, 0, 5);
    printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    displayVerdict(testSuccess, __FUNCTION__);

    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);
    return testSuccess;
}
//////////////////////////////////////////////////////////////////////////
// Tests for Searching Players
//////////////////////////////////////////////////////////////////////////
gsi_bool testPlayerSearch()
{

    gsi_bool testSuccess = gsi_true;
    int testNum = 0;
    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();
    
    printf("TEST %d - Search Players\n", ++testNum);
    // TEST - Search for Players
    
    sampleSearchPlayers(Leader.mBrigadeInstance, L"", Leader.mBrigadeId, NULL, 0, 5);

    printf("TEST %d - Search Players\n", ++testNum);
    sampleSearchPlayers(Leader.mBrigadeInstance, L"",0, NULL, 0, 10);

    displayVerdict(testSuccess, __FUNCTION__);

    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);

    return testSuccess;
}

//////////////////////////////////////////////////////////////////////////
// Tests for Brigade management
//////////////////////////////////////////////////////////////////////////
gsi_bool testBrigadeCreationAndUpdate()
{
    int testNum = 0;
    gsi_bool testSuccess = gsi_true;
    gsi_u32  brigadeId   = 0;
    GSBBrigade  *brigade = NULL;
    printf("EXECUTING: Test case %s\n", __FUNCTION__);
    
    // +++ Test Setup
    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();
    if (testSuccess)
    {

        printf("%s:Making sure that player does not have any brigades\n", __FUNCTION__);
        testSuccess = clearPlayerBrigades(&Member);

        // Verify that member does not have any brigades associated with his profile id         
        if (testSuccess)
        {
            // Retrieve the members brigades     
            sampleGetBrigadesByProfileId(Member.mBrigadeInstance, 
                                         Member.mLogin->mLoginCert->mProfileId, 
                                         &Member.mBrigadeList );
            testSuccess = ((Member.mBrigadeList != NULL) && (Member.mBrigadeList->mCount == 0));
            gsbFreeBrigadeList(Member.mBrigadeList);
            Member.mBrigadeList = NULL;
        }
    }
    if (testSuccess)
    {
        UCS2String brigadeName = gsimalloc(sizeof(unsigned short)*24);
        UCS2String tagName= gsimalloc(sizeof(unsigned short)*24);         
        time_t now = current_time();

        printf("TEST %d - Create a new Brigade\n", ++testNum);
        wsprintf(brigadeName,L"B%d", now);
        wsprintf(tagName, L"T%d",now);

        // TEST - Create a new Brigade
        brigadeId = 0;
        sampleCreateBrigade(Member.mBrigadeInstance, 
                        brigadeName, 
                        tagName, 
                        L"www.poweredbygamespy.com", 
                        L"We Rock!",
                        GSBRecruitingStatus_Open,
                        &brigadeId);

        testSuccess = (brigadeId != 0);
        
        gsifree(brigadeName); 
        gsifree(tagName);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    }
    if (testSuccess)
    {
        GSBBrigade *updatedBrigade= NULL;

        printf ("TEST %d -  Update brigade ( id = %d)\n", ++testNum, brigadeId);

        sampleGetBrigadeById(Member.mBrigadeInstance, brigadeId, &brigade);

        // update the brigade
        if(brigade->mMessageOfTheDay) gsifree(brigade->mMessageOfTheDay);
        brigade->mMessageOfTheDay = goawstrdup(L"Welcome to the brigade");
        brigade->mRecruitingType  = GSBRecruitingStatus_Moderated;

        sampleUpdateBrigade(Member.mBrigadeInstance, brigade);
        
        // Retrieve the brigade to check whether the brigade exists
        sampleGetBrigadeById(Member.mBrigadeInstance, brigadeId, &updatedBrigade);
        testSuccess = (updatedBrigade != NULL)&&(updatedBrigade->mRecruitingType == GSBRecruitingStatus_Moderated);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    
        if (testSuccess)
        {
// Temporary - exclude upload logo for the time being.

//             // TEST Uploading a brigade logo to the backend
//             GSBBrigade *logoBrigade = NULL;
//             // upload a brigade logo 
//             printf ("TEST %d -  Uploading of Brigade Logo to the the backend\n", ++testNum);
//             sampleUploadBrigadeLogo(Member.mBrigadeInstance, _T("./Avatar.jpg") , updatedBrigade->mBrigadeId);
//             
//             sampleGetBrigadeById(Member.mBrigadeInstance, updatedBrigade->mBrigadeId, &logoBrigade);
// 
//             testSuccess = (logoBrigade->mLogoList.mCount>updatedBrigade->mLogoList.mCount);
//             printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
// 
//             if (testSuccess)
//             {
//                 printf ("TEST %d - Downloading of Brigade Logo to the the backend\n", ++testNum);
// 
//                 // download the same logo in a different file name
//                 sampleDownloadBrigadeLogo(Member.mBrigadeInstance, 
//                                           "./Avartar2.jpg",
//                                           &logoBrigade->mLogoList.mLogos[logoBrigade->mLogoList.mCount-1]);
//             }
// 
//             // free the logo test brigade
//             gsbFreeBrigade(logoBrigade);

        }
        gsbFreeBrigade(updatedBrigade);

    }
    if (testSuccess)
    {
        GSBBrigade *disbandedBrigade = NULL;
        printf ("TEST %d -  Disband a brigade\n", ++testNum);

        // Disband a brigade
        sampleDisbandBrigade(Member.mBrigadeInstance, brigadeId);
        // Retrieve the brigade to check whether the brigade exists
        sampleGetBrigadeById(Member.mBrigadeInstance, brigadeId, &disbandedBrigade);
        testSuccess = (disbandedBrigade != NULL)&&(disbandedBrigade->mDisbanded);
        gsbFreeBrigade(disbandedBrigade);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    }
    
    displayVerdict(testSuccess, __FUNCTION__);
    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);
    return testSuccess;
}

//////////////////////////////////////////////////////////////////////////
// Tests for Brigade Member Operation such promoting, member update 
// and status change
//////////////////////////////////////////////////////////////////////////
gsi_bool testBrigadeMemberOperations()
{

    gsi_bool testSuccess = gsi_true;
    int testNum = 0;

    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // +++ Test Setup
    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();

    if (testSuccess)
    {
        printf("%s:Making sure that player does not have any brigades\n", __FUNCTION__);
        testSuccess = clearPlayerBrigades(&Member);
    }

    if (testSuccess )
    {

        printf("TEST %d - JOIN Brigade\n\tPlayer profile Id = %d\n\tBrigade Id = %d \n", 
            ++testNum, Member.mLogin->mLoginCert->mProfileId, Leader.mBrigadeId);
        sampleJoinBrigade(Member.mBrigadeInstance,Leader.mBrigadeId);
        sampleAnswerJoin(Leader.mBrigadeInstance, 
            Leader.mBrigadeId, 
            Member.mLogin->mLoginCert->mProfileId, 
            gsi_true);

        // Verify the "Member's" status is RequestJoin 
        testSuccess = testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
            Leader.mBrigadeId,
            GSBBrigadeMemberStatus_Active); 
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }

    if (testSuccess)
    {
        gsi_u32     memberid = 0;
        GSBBrigade  *brigade = NULL;
        
        // TEST : Promote to leader
        printf("TEST %d - Promote to leader\n", ++testNum);

        memberid = testStepGetMemberId(Leader.mBrigadeInstance, 
            Leader.mBrigadeId, 
            Member.mLogin->mLoginCert->mProfileId);

        samplePromoteToLeader(Leader.mBrigadeInstance, Leader.mBrigadeId, memberid);
        sampleGetBrigadeById(Leader.mBrigadeInstance, 
                            Leader.mBrigadeId,
                            &brigade);
        testSuccess = (brigade->mLeaderProfileId == Member.mLogin->mLoginCert->mProfileId) ;
    
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

        gsbFreeBrigade(brigade);
    }

    if (testSuccess)
    {
        gsi_u32     memberid = 0;
        GSBBrigade  *brigade = NULL;
       
        // Now revert the brigade leader to the original
        printf("TEST %d- Revert the leadership back to the original leader\n", ++testNum);

        // Get the former leaders brigade member id
        memberid = testStepGetMemberId(Member.mBrigadeInstance, 
                                       Leader.mBrigadeId, 
                                       Leader.mLogin->mLoginCert->mProfileId);

        // The current leader promote the former leader back into leadership
        samplePromoteToLeader(Member.mBrigadeInstance, Leader.mBrigadeId, memberid);
        sampleGetBrigadeById(Leader.mBrigadeInstance, 
                            Leader.mBrigadeId,
                            &brigade);

        testSuccess = (brigade->mLeaderProfileId == Leader.mLogin->mLoginCert->mProfileId) ;
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
        gsbFreeBrigade(brigade);

    }
    
    //TEST - Member leaves the brigade
    if (testSuccess)
    {
        GSResult result;
        printf("TEST %d - Member leaves the brigade\n", ++testNum);
        sampleLeaveBrigade(Member.mBrigadeInstance, Leader.mBrigadeId, &result);

        testSuccess = (result == GS_SUCCESS)&&
                      testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                 Leader.mBrigadeId,
                                                 GSBBrigadeMemberStatus_Inactive);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }
   
    // TEST - Rejoin Brigade
    if (testSuccess)
    {
        printf("TEST %d - Rejoin brigade after leaving\n", ++testNum);
        sampleJoinBrigade(Member.mBrigadeInstance,Leader.mBrigadeId);
        sampleAnswerJoin(Leader.mBrigadeInstance, 
                        Leader.mBrigadeId, 
                        Member.mLogin->mLoginCert->mProfileId, 
                        gsi_true);

        // Verify the "Member's" status is RequestJoin 
        testSuccess = testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                 Leader.mBrigadeId,
                                                 GSBBrigadeMemberStatus_Active);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }
    if (testSuccess)
    {
        printf("TEST %d - Member is removed from the brigade\n", ++testNum);

        sampleRemoveBrigadeMember(Leader.mBrigadeInstance,
                                  Member.mLogin->mLoginCert->mProfileId, 
                                  Leader.mBrigadeId);
        // Verify the "Member's" status is RequestJoin 
        testSuccess = testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                 Leader.mBrigadeId,
                                                 (GSBBrigadeMemberStatus_Inactive | 
                                                  GSBBrigadeMemberStatus_Kicked));
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    }

    if (testSuccess)
    {
        printf("TEST %d - Rejoin brigade after being removed\n", ++testNum);
        sampleJoinBrigade(Member.mBrigadeInstance,Leader.mBrigadeId);
        sampleAnswerJoin(Leader.mBrigadeInstance, 
            Leader.mBrigadeId, 
            Member.mLogin->mLoginCert->mProfileId, 
            gsi_true);

        // Verify the "Member's" status is RequestJoin 
        testSuccess = testStepVerifyMemberStatus(Leader.mBrigadeInstance, 
                                                 Leader.mBrigadeId,
                                                 GSBBrigadeMemberStatus_Active);        
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    }
    if (testSuccess)
    {

        GSBBrigadeMember *brigadeMember = NULL;

        printf("TEST %d - Update a brigade member\n", ++testNum);
            
        brigadeMember = testStepGetBrigadeMember(Member.mBrigadeInstance, 
                                                 Leader.mBrigadeId, 
                                                 Member.mLogin->mLoginCert->mProfileId, 
                                                 GSBBrigadeMemberStatus_Active);
        testSuccess = (brigadeMember!= NULL);
        if (!testSuccess)         
            printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

        if (testSuccess)
        {
            GSBBrigadeMember *updatedMember = NULL; 
            // now modify the brigade member
            if (brigadeMember->mTitle != NULL) gsifree(brigadeMember->mTitle);
            brigadeMember->mTitle = goawstrdup(L"Co-pilot");
            // role of a brigade member can be changed also.
            sampleUpdateBrigadeMember(Leader.mBrigadeInstance, brigadeMember);
            updatedMember = testStepGetBrigadeMember(Member.mBrigadeInstance, 
                                                    Leader.mBrigadeId, 
                                                    Member.mLogin->mLoginCert->mProfileId, 
                                                    GSBBrigadeMemberStatus_Active);
            printf("____Updated Brigade member: \n");
            displayBrigadeMember(updatedMember);
            testSuccess =(wcscmp(brigadeMember->mTitle, updatedMember->mTitle)== 0);
            printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
            gsbFreeBrigadeMember(updatedMember);

       }
        gsbFreeBrigadeMember(brigadeMember);

    }
    // TEST Scenario for banMember
    
//     if (testSuccess)
//     {
//         GSBBrigadeMember *bannedMember = NULL;
// 
//         printf("TEST %d - Ban Member\n", ++testNum);
//         printf("\tMember Profile Id = %d\n", Member.mLogin->mLoginCert->mProfileId);
//         printf("\tBrigade Id        = %d\n", Leader.mBrigadeId);
// 
//         sampleBanMember(Leader.mBrigadeInstance, Leader.mBrigadeId, Member.mLogin->mLoginCert->mProfileId);
//         bannedMember = testStepGetBrigadeMember(Leader.mBrigadeInstance, 
//                                                 Leader.mBrigadeId, 
//                                                 Member.mLogin->mLoginCert->mProfileId, 
//                                                 GSBBrigadeMemberStatus_Blocked);
//         testSuccess = (bannedMember != NULL);
//         if (bannedMember) 
//             gsbFreeBrigadeMember(bannedMember);
//         
//         printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
//     }
    
    displayVerdict(testSuccess, __FUNCTION__);
    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);
    return testSuccess;
}

//////////////////////////////////////////////////////////////////////////
// Tests for Send Message API
//////////////////////////////////////////////////////////////////////////
gsi_bool testMessaging()
{
    gsi_bool testSuccess = gsi_true;
    int testNum = 0;

    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // +++ Test Setup
    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();
    if (testSuccess)
    {
        printf("TEST % d - Send a message to brigade\n", ++testNum);
        // TEST - Send Message to all the team members Test
        sampleSendMessageToBrigade(Leader.mBrigadeInstance, Leader.mBrigadeId, L"Hi Team!");
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

        printf("TEST % d - Send a message to Member\n", ++testNum);
        // TEST - Search Message to a brigade member 
        sampleSendMessageToMember(Leader.mBrigadeInstance, Member.mLogin->mLoginCert->mProfileId , L"Having fun?"); 
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);

    }

    displayVerdict(testSuccess, __FUNCTION__);
    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);
    return testSuccess;
}

///////////////////////////////////////////////////////////////////////////
// Update Brigade Member Email and Nick Name
///////////////////////////////////////////////////////////////////////////
gsi_bool testUpdateMemberNickNameAndEmail()
{

    gsi_bool testSuccess = gsi_true;
    int testNum = 0;

    printf("EXECUTING: Test case %s\n", __FUNCTION__);

    // Leader and member logs in and leader has a valid brigade Id.
    printf("%s:Initialize Leader and Player\n", __FUNCTION__);
    testSuccess = e2eTestInit();

    if (testSuccess)
    {

        printf("TEST %d - Synchronize/Update Nick Name and Email\n", ++testNum);
        testSuccess = sampleUpdateMemberNickAndEmail(Leader.mBrigadeInstance, Leader.mBrigadeId);
        printf("%s : TEST - %d (%d) \n", (testSuccess?"PASSED":"FAILED"),testNum, __LINE__);
    }

    // there is really no verdict here we cannot automate the verification
    // of the existence of a certain history 
    displayVerdict(testSuccess, __FUNCTION__);

    cleanupAndQuit(0);
    _CrtMemDumpAllObjectsSince(NULL);
    return testSuccess;
}



//////////////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////////////
int test_main(int argc, char *argv[])
{	
	static GSBBrigade		myBrigade;
    static GSBBrigadeMember myMember;
	static GSBRole			myRole;

    //FILE                   *logFile = NULL;
    GSI_UNUSED(argc);
    GSI_UNUSED(argv);

    // Set debug output options
	gsSetDebugFile(stdout);
	gsSetDebugLevel(GSIDebugCat_All, GSIDebugType_All, GSIDebugLevel_Debug);

	// enable Win32 C Runtime debugging 

#if defined(_WIN32) && defined(USE_CRTDBG)
    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_LEAK_CHECK_DF);
#endif
    // Perform the required availability check
	if (!isBackendAvailable())
        return cleanupAndQuit(-1);  
    //
    // The following are end to end tests to verify member status
    //

    // Player wants to join and leader accepts join
    e2eTestJoinAndAnswerJoin(gsi_true);

    // Player wants to join and gets rejected by the brigade leader
    e2eTestJoinAndAnswerJoin(gsi_false);

    // Leader invites a player to the brigade and invite is rejected.
    e2eTestInviteAndAnswerInvite(gsi_false);

    // Leader invites a player to the brigade and invite is accepted.
    e2eTestInviteAndAnswerInvite(gsi_true);

    // Leader invites a player to the brigade and withdraws the invitation.
    e2eTestInviteAndRescindInvite();

    // The following a group of end-to-end tests
    testUpdateMemberNickNameAndEmail(); 
    testBrigadeCreationAndUpdate();
    testMessaging();
    testRolesAndEntitlements();
    testBrigadeSearch();
    testPlayerSearch();    
    testBrigadeMemberOperations();
    testRetrievingHistory();

    // keep thinking
    printf("Finalizing any incomplete tasks\n");
    processTasks();

    // print out final results
    printResults();

    // Cleanup SDK
//    return cleanupAndQuit(0);
    return 0;
}


