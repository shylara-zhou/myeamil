#include "stdafx.h"
#include "resource.h"

#if !defined(AFX_LOGIN_H__3BDE7082_D933_472F_8A25_16CB0C66E90F__INCLUDED_)
#define AFX_LOGIN_H__3BDE7082_D933_472F_8A25_16CB0C66E90F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 
// _MSC_VER > 1000
// LOGIN.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IDD_SMTP_DIALOG dialog



class CLOGIN : public CDialog
{
	// Construction
public:
	CLOGIN(CWnd* pParent = NULL);   // standard constructor
	int Login(CString username, CString password);
	CString ewirte(CString eusername, CString epassword);
	//CString eread();
// Dialog Data
	//{{AFX_DATA(CBlackList)
	enum { IDD = IDD_LOGIN };
	CListBox	m_list;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlackList)
	BOOL	m_html;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMTPDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	//}}AFX_VIRTUAL

// Implementation
//protected:
//
//	// Generated message map functions
//	//{{AFX_MSG(CBlackList)
//	virtual BOOL OnInitDialog();
//	//afx_msg void OnButtonAdd();
	//afx_msg void OnButtonDelete();
	afx_msg void OnClose();
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRe();

	//CString ewirte(CString eusername, CString epassword);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BlackList_H__3BDE7082_D933_472F_8A25_16CB0C66E90F__INCLUDED_)
