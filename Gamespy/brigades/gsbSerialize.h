///////////////////////////////////////////////////////////////////////////////
// File:	gsbSerialize.h
// SDK:		GameSpy Brigades SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2009 GameSpy Industries, Inc.

#ifndef __GSBSERIALIZE_H__
#define __GSBSERIALIZE_H__

#include "../common/gsSoap.h"
#include "../common/gsCrypt.h"
#include "../common/gsLargeInt.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Parsing utility functions

GSResult gsbiParseResponseResult(GSXmlStreamReader theResponseXml, 
								 GSResultSet **resultSet, 
								 const char *theRequestName);

GSResult gsbiParseResponseCodes(GSXmlStreamReader theResponseXml,
								GSResultSet **resultSet);

GSResult gsbiParseBrigade(GSXmlStreamReader theResponseXml,
						  GSBBrigade        *theBrigade);

GSResult gsbiParseEntitlements(GSXmlStreamReader    theResponseXml,
							   GSBEntitlementList   *entitlementList);

GSResult gsbiParseRoleEntitlements(GSXmlStreamReader        theResponseXml,
                                   GSBRoleEntitlementList	*memberEntitlementList);

GSResult gsbiParseRoleEntitlementsForEntitlementIds(GSXmlStreamReader		theResponseXml,
													GSBEntitlementIdList	*entitlementIdList);

GSResult gsbiParseRoleEntitlementsForRoleIds(GSXmlStreamReader		theResponseXml,
											 GSBRoleIdList	*roleIdList);

GSResult gsbiParseRoleList(GSXmlStreamReader	theResponseXml,
						   GSBRoleList			**roleList);

GSResult gsbiParsePlayers(GSXmlStreamReader     theResponseXml,
                          GSBBrigadeMemberList	*memberList) ;

GSResult gsbiParseBrigadeMembers(GSXmlStreamReader              theResponseXml,
								 GSBBrigadeMemberList	        *memberList);

GSResult gsbiParsePendingAction(GSXmlStreamReader			theResponseXml,
								GSBBrigadePendingActions    *pendingAction);

GSResult gsbiParsePendingActions(GSXmlStreamReader              theResponseXml,
								 GSBBrigadePendingActionsList	*actionList);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// XML Serialization utility functions

gsi_bool gsbiStartBaseRequest(GSBInternalInstance         *theInstance,
                              GSXmlStreamWriter   theWriter);

gsi_bool gsbiSerializeBrigade(GSBInternalInstance       *theInstance,
                              GSXmlStreamWriter theWriter,
                              GSBBrigade        *thegBrigade);

gsi_bool gsbiSerializeBrigadeMember(GSXmlStreamWriter theWriter,
                                    GSBBrigadeMember  *theMember);

gsi_bool gsbiSerializeRole( GSXmlStreamWriter theWriter,
                            GSBRole           *theRole);

gsi_bool gsbiSerializeBrigadeLogoList(GSXmlStreamWriter theWriter, 
									  GSBBrigadeLogoList *brigadeLogoList);

gsi_bool gsbiSerializeEntitlementList(GSXmlStreamWriter theWriter,
									  GSBEntitlementList *entitlementList);

gsi_bool gsbiSerializeEntitlementIDs(GSXmlStreamWriter theWriter, 
									 GSBEntitlementIdList *entitlementIdList);

GSResult gsbiParseHistoryList (GSXmlStreamReader        theResponseXml,
                               GSBBrigadeHistoryList	*historyList); 
GSResult gsbiParseHistoryEntry (GSXmlStreamReader theResponseXml,
                                GSBBrigadeHistoryEntry	*historyEntry) ;

UCS2String gsbiParseXmlToUCString(GSXmlStreamReader theResponseXml, const char *matchtag, gsi_bool required);
#endif // __GSBSERIALIZE_H__
