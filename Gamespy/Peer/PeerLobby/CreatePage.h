///////////////////////////////////////////////////////////////////////////////
// File:	CreatePage.h
// SDK:		GameSpy Peer SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2009 GameSpy Industries, Inc.

#if !defined(AFX_CREATEPAGE_H__4FC1C8CA_9B4C_47F6_B226_C211DC74D504__INCLUDED_)
#define AFX_CREATEPAGE_H__4FC1C8CA_9B4C_47F6_B226_C211DC74D504__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCreatePage dialog

class CCreatePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCreatePage)

// Construction
public:
	CCreatePage();
	~CCreatePage();

// Dialog Data
	//{{AFX_DATA(CCreatePage)
	enum { IDD = IDD_CREATE_PAGE };
	CString	m_name;
	int		m_maxPlayers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCreatePage)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardBack();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCreatePage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern CCreatePage * CreatePage;

#endif // !defined(AFX_CREATEPAGE_H__4FC1C8CA_9B4C_47F6_B226_C211DC74D504__INCLUDED_)
