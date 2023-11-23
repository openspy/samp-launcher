///////////////////////////////////////////////////////////////////////////////
// File:	StdAfx.h
// SDK:		GameSpy Transport 2 SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2002-2009 GameSpy Industries, Inc.
// ------------------------------------
// Include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently.
//

#if !defined(AFX_STDAFX_H__7F1931C4_E9E9_49E1_A6DC_8ACC78E87145__INCLUDED_)
#define AFX_STDAFX_H__7F1931C4_E9E9_49E1_A6DC_8ACC78E87145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../gt2.h"
#include "../../darray.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7F1931C4_E9E9_49E1_A6DC_8ACC78E87145__INCLUDED_)
