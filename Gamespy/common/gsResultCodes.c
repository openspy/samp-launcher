///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include "gsCommon.h"
#include "gsResultCodes.h"


void gsResultCodesFreeResultSet(GSResultSet *resultSet)
{
	if (resultSet)
	{
		gsifree(resultSet->mErrorMessage);
		gsifree(resultSet->mResults);
		resultSet->mNumResults = 0;
	}	
}

GSResult gsResultCodesCreateResultSet(GSResultSet **resultSet)
{
	GS_ASSERT(resultSet);
	if (!resultSet)
	{
		gsDebugFormat(GSIDebugCat_Common, GSIDebugType_State, GSIDebugLevel_WarmError, "resultSet is NULL");
		return GS_ERROR(GSResultSDK_Common, GSResultSection_State, GSResultCode_InvalidParameters);
	}

	*resultSet = (GSResultSet *)gsimalloc(sizeof(GSResultSet));
	if (!(*resultSet))
	{
		gsDebugFormat(GSIDebugCat_Common, GSIDebugType_Memory, GSIDebugLevel_WarmError, "Failed to allocate memory for resultSet");
		return GS_ERROR(GSResultSDK_Common, GSResultSection_Memory, GSResultCode_InvalidParameters);
	}
	memset(*resultSet, 0, sizeof(GSResultSet));
	return GS_SUCCESS;
}