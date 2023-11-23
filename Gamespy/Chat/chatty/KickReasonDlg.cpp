///////////////////////////////////////////////////////////////////////////////
// File:	KickReasonDlg.cpp
// SDK:		GameSpy Chat SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 1999-2009 GameSpy Industries, Inc.

#include "stdafx.h"
#include "chatty.h"
#include "KickReasonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKickReasonDlg dialog

CKickReasonDlg::CKickReasonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKickReasonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKickReasonDlg)
	m_reason = _T("");
	//}}AFX_DATA_INIT
}


void CKickReasonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKickReasonDlg)
	DDX_Text(pDX, IDC_REASON, m_reason);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKickReasonDlg, CDialog)
	//{{AFX_MSG_MAP(CKickReasonDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKickReasonDlg message handlers
