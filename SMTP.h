// SMTP.h : main header file for the SMTP application
//

#if !defined(AFX_SMTP_H__7C042B6F_1852_4AA8_8A44_8941FBA73780__INCLUDED_)
#define AFX_SMTP_H__7C042B6F_1852_4AA8_8A44_8941FBA73780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#pragma warning(disable : 4786)
#pragma comment(lib,"ws2_32.lib")
#include "resource.h"		// main symbols
#include "Base64.h"
#include <winsock2.h>
#include <vector>
#include <string>

#define COPYRIGHT "SMTP"	//版权信息
#define BOUNDARY "SMTP—MAIL"	//边界字符串
#define READ_BUFFER 17100	//附件缓冲区，该值必须为57的整数倍
/////////////////////////////////////////////////////////////////////////////
// CSMTPApp:
// See SMTP.cpp for the implementation of this class
//

class CSMTPApp : public CWinApp
{
public:
	CSMTPApp();

	BOOL InitInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMTPApp)
	public:
	//virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSMTPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////


class SMTP  
{
public:

	SMTP();
	virtual ~SMTP();

public:
	bool isvalid(const string username,const string password);	//验证用户名密码
	bool Connect(const string addr,const int port);		//连接服务器
	//发送数据
	bool Send(const string sendaddr,
			  const vector<string> rcvlist,
			  const string sendname,
			  const string rcvname,
			  const string subject,
			  const string content,
			  const vector<string> file,
			  int ishtml);
	
	

private:
	bool CreateSocket();	//创建套接字
	void ReleaseSocket();	//释放套接字
	bool CheckResponse(const char * code);	//检查返回的状态码和期望的状态码是否一致

private:
	SOCKET m_socket;	//套接字
	WSADATA m_wsadata;	//存放SOCKET的初始化信息
	HOSTENT* m_hostent;	//存放主机以及地址
	SOCKADDR_IN m_sockaddr_in;	//存放地址协议、ip、端口

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTP_H__7C042B6F_1852_4AA8_8A44_8941FBA73780__INCLUDED_)
