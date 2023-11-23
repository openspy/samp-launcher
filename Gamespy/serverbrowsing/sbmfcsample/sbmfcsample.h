///////////////////////////////////////////////////////////////////////////////
// File:	sbmfcsample.h
// SDK:		GameSpy Server Browsing SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2009 GameSpy Industries, Inc.
// ------------------------------------
// Main header file for the SBMFCSAMPLE application.

#if !defined(AFX_SBMFCSAMPLE_H__F2EE1BD5_6089_4F00_9E83_2D0A7C9AA592__INCLUDED_)
#define AFX_SBMFCSAMPLE_H__F2EE1BD5_6089_4F00_9E83_2D0A7C9AA592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSbmfcsampleApp:
// See sbmfcsample.cpp for the implementation of this class
//

class CSbmfcsampleApp : public CWinApp
{
public:
	CSbmfcsampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSbmfcsampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSbmfcsampleApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SBMFCSAMPLE_H__F2EE1BD5_6089_4F00_9E83_2D0A7C9AA592__INCLUDED_)
