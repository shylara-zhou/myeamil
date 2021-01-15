// POP3.h: interface for the POP3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POP3_H__294FB1D1_8FB1_4602_AEAB_51D118E9B51F__INCLUDED_)
#define AFX_POP3_H__294FB1D1_8FB1_4602_AEAB_51D118E9B51F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class POP3  
{
public:
	bool QUIT();
	bool ishtml;
	CString GetMailSender(UINT i);
	bool RETR(UINT nIndex,CString &strMsg);
	CString GetMailSize(UINT i);
	CString GetMailSubject(UINT i);
	bool TOP();
	bool LIST();
	unsigned int GetMailNum();
	bool STAT();
	bool Noop();
	bool isvalid(const string username,const string password);
	bool Connect(const string addr,const int port);
	POP3();
	virtual ~POP3();
private:
	bool CheckResponse(bool bDouble);
	void ReleaseSocket();
	bool CreateSocket();
	SOCKET m_socket;	//套接字
	WSADATA m_wsadata;	//存放SOCKET的初始化信息
	HOSTENT* m_hostent;	//存放主机以及地址
	SOCKADDR_IN m_sockaddr_in;	//存放地址协议、ip、端口
	unsigned int mailnum,mailsize;	//邮件总数、总大小
	CString m_response;			//存放响应信息
	CStringArray m_subject;		//CString数组存放各个邮件主题
	CStringArray m_size;	//存放各个邮件大小
	CStringArray m_sender;	//存放发送方
	CStringArray m_date;	//存放发送时间
};

#endif // !defined(AFX_POP3_H__294FB1D1_8FB1_4602_AEAB_51D118E9B51F__INCLUDED_)
