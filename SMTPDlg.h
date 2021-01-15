// SMTPDlg.h : header file
//
#include "POP3.h"
#include "resource.h"
#include<vector>
#include<list>
#if !defined(AFX_SMTPDLG_H__4A3A0BE6_0227_4B33_BDB4_FE528F7D38E5__INCLUDED_)
#define AFX_SMTPDLG_H__4A3A0BE6_0227_4B33_BDB4_FE528F7D38E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSMTPDlg dialog
extern CString strMail;
extern CStringArray BlackList_list;
extern CStringArray FriendList_list;
extern CString USER;

class CSMTPDlg : public CDialog
{
// Construction
public:
	CSMTPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSMTPDlg)
	enum { IDD = IDD_SMTP_DIALOG };
	CListCtrl	m_listmail;
	BOOL	m_html;
	BOOL	m_send;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMTPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSMTPDlg)
	virtual BOOL OnInitDialog();//启动
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);//打开about界面
	afx_msg void OnPaint();//创建窗口
	afx_msg HCURSOR OnQueryDragIcon();//图标设置IDR_MAINFRAME           ICON                    "res\\SMTP.ico"
	afx_msg void OnSend();//smtp发送
	afx_msg void OnCheck3();//抄送
	afx_msg void OnAddfile();//附件打开
	afx_msg void OnDelete();//删除
	afx_msg void OnGetemail();//获得邮件
	afx_msg void OnLogin();//pop3登录服务器
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);//双击打开邮件内容
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBlackList();//黑名单
	afx_msg void OnFriendList();//好友列表
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	vector<string> files;
	CComboBox* pCombo;
	POP3 pop3;				// pop对像
	
public:
	afx_msg void OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTPDLG_H__4A3A0BE6_0227_4B33_BDB4_FE528F7D38E5__INCLUDED_)
