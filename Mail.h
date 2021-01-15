#if !defined(AFX_Mail_H__DF692300_024F_47D5_BA8F_FBE947BD1DDC__INCLUDED_)
#define AFX_Mail_H__DF692300_024F_47D5_BA8F_FBE947BD1DDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Mail dialog

class Mail : public CDialog
{
// Construction
public:
	Mail(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Mail)
	enum { IDD = IDD_CONTENT_DIALOG };
	CEdit	m_content;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Mail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(Mail)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Mail_H__DF692300_024F_47D5_BA8F_FBE947BD1DDC__INCLUDED_)
