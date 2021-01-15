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

#define COPYRIGHT "SMTP"	//��Ȩ��Ϣ
#define BOUNDARY "SMTP��MAIL"	//�߽��ַ���
#define READ_BUFFER 17100	//��������������ֵ����Ϊ57��������
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
	bool isvalid(const string username,const string password);	//��֤�û�������
	bool Connect(const string addr,const int port);		//���ӷ�����
	//��������
	bool Send(const string sendaddr,
			  const vector<string> rcvlist,
			  const string sendname,
			  const string rcvname,
			  const string subject,
			  const string content,
			  const vector<string> file,
			  int ishtml);
	
	

private:
	bool CreateSocket();	//�����׽���
	void ReleaseSocket();	//�ͷ��׽���
	bool CheckResponse(const char * code);	//��鷵�ص�״̬���������״̬���Ƿ�һ��

private:
	SOCKET m_socket;	//�׽���
	WSADATA m_wsadata;	//���SOCKET�ĳ�ʼ����Ϣ
	HOSTENT* m_hostent;	//��������Լ���ַ
	SOCKADDR_IN m_sockaddr_in;	//��ŵ�ַЭ�顢ip���˿�

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTP_H__7C042B6F_1852_4AA8_8A44_8941FBA73780__INCLUDED_)
