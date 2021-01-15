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
	SOCKET m_socket;	//�׽���
	WSADATA m_wsadata;	//���SOCKET�ĳ�ʼ����Ϣ
	HOSTENT* m_hostent;	//��������Լ���ַ
	SOCKADDR_IN m_sockaddr_in;	//��ŵ�ַЭ�顢ip���˿�
	unsigned int mailnum,mailsize;	//�ʼ��������ܴ�С
	CString m_response;			//�����Ӧ��Ϣ
	CStringArray m_subject;		//CString�����Ÿ����ʼ�����
	CStringArray m_size;	//��Ÿ����ʼ���С
	CStringArray m_sender;	//��ŷ��ͷ�
	CStringArray m_date;	//��ŷ���ʱ��
};

#endif // !defined(AFX_POP3_H__294FB1D1_8FB1_4602_AEAB_51D118E9B51F__INCLUDED_)
