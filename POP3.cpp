// POP3.cpp: implementation of the POP3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "smtp.h"
#include "POP3.h"
#include <string>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

POP3::POP3()
{
	mailnum=0;
	mailsize=0;
	ishtml=0;
}

POP3::~POP3()
{

}

bool POP3::Connect(const string addr, const int port)
{
		//�����׽���ʧ��
	
	if(!CreateSocket()) 
		return false;
	//���ݷ��������ƻ�ȡip��ַ
	if((m_hostent=gethostbyname(addr.c_str()))==NULL)
	{
		ReleaseSocket();
		return false;
	}
	//�÷�����������
	if(m_hostent->h_addr_list[0]==NULL)
	{
		ReleaseSocket();
		return false;
	}
	
	//����Э��غͷ�����ip�Լ��˿�
	memset(&m_sockaddr_in,0,sizeof(m_sockaddr_in));
	m_sockaddr_in.sin_family=AF_INET;
	m_sockaddr_in.sin_port=htons(port);
	m_sockaddr_in.sin_addr.S_un.S_addr=*(ULONG *)m_hostent->h_addr_list[0];

	//����
	if(connect(m_socket,(sockaddr *)&m_sockaddr_in,sizeof(m_sockaddr_in))==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	//��鷵�ص�״̬
	if(!CheckResponse(0)) return false;
	
	//���ӷ������ɹ�
	return true;
}

bool POP3::CreateSocket()
{
		//��ʼ��
	if(WSAStartup(MAKEWORD(2,2),&m_wsadata) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return FALSE;
	}
	//�����׽���
	if((m_socket = socket(AF_INET,SOCK_STREAM,0)) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return FALSE;
	}

	return TRUE;
}

void POP3::ReleaseSocket()
{
	shutdown(m_socket,SD_BOTH);
	closesocket(m_socket);
	WSACleanup();
}

bool POP3::CheckResponse(bool bDouble)
{
//���ﲻ��ֱ����������գ���Ϊ���ӳ��Լ����ı��ֶη��͵����
//��Ҫ�趨һ��ʱ�䣬����ʱ���ڽ��յ������ݳ���û�б仯������մ洢����

/*

	TCHAR Respond[100005]={0};
	if(recv(m_socket,Respond,sizeof(Respond),0) == SOCKET_ERROR)
	{
	//	AfxMessageBox("������Ӧ����!");
		ReleaseSocket();
		return false;
	}

	if(memcmp(Respond,"+OK",3)!=0)
	{
	//	AfxMessageBox("��Ӧ��Ϣ����!");
		return false;
	}
	m_response=Respond;
	return true;
*/
	TCHAR pChar[100005];
	CString strTemp;

	// ��ȡ��Ӧ��Ϣ
	BOOL bEnd = FALSE;
	UINT nReceived = 0;
	DWORD dwStart = ::GetTickCount();
	while (!bEnd)
	{
		// ����ʱ�䵽
		if ((::GetTickCount() - dwStart) >	2000)
		{
			pChar[nReceived] = '\0';
			// ���浱ǰ��Ӧ����Ϣ
			m_response = pChar;
			AfxMessageBox("������Ӧ��Ϣ��ʱ");
			return FALSE;
		}

		// ���׽����Ƿ�ɶ�
		timeval timeout = {0, 0};
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(m_socket,&fds);
		int nStatus = select(0,&fds,NULL,NULL,&timeout);

		if (nStatus == SOCKET_ERROR)
		{
			// �׽��ֲ��ɶ�
			AfxMessageBox("�׽��ֲ��ɴ�");
			return FALSE;
		}
		else if (!nStatus)
		{
			// û�н��յ�����
			::Sleep(688);
			continue;
		}

		// ���׽����н�������
		::Sleep(288);
		nStatus = recv(m_socket,pChar + nReceived,sizeof(pChar),0);
		if (nStatus == SOCKET_ERROR)
		{
			pChar[nReceived] = '\0';
			// �׽��ִ���
			// ���浱ǰ��Ӧ��Ϣ
			m_response = pChar; 
			AfxMessageBox("δ�ܴ��׽������յ�����");
			return FALSE; 
		}
		else if (nStatus)
		{
			// ���ü�ʱ��
			dwStart = ::GetTickCount();
			// ���յ���������������
			nReceived += nStatus;
		}

		// ��pChar��Ϊ�ַ�����������CString�͵ı���
		pChar[nReceived] = '\0';
		strTemp = pChar;
								
		// ����Ƿ��յ��˽�����־
		LPCTSTR lpszComp = bDouble ? "\r\n.\r\n" : "\r\n";
		bEnd = (strTemp.Find(lpszComp) != -1);
	}

	// ȥ��������־
	nReceived -= bDouble ? 3 : 0;
	pChar[nReceived] = '\0';

	// ����Ӧ��Ϣ�Ƿ���Ч
	strTemp = pChar;
	strTemp.MakeUpper();
	int nStart = strTemp.Find("+OK");
	if (nStart == -1)
	{
		// �յ���Ч��Ϣ
		AfxMessageBox("��Ӧ��Ϣ��Ч");
		return FALSE;
	}
	// ��ȡ��Ч��Ϣ
	strTemp = pChar;
	m_response = strTemp.Right(strTemp.GetLength() - nStart - 3);
	
	return TRUE;
 }

 //�û���������
bool POP3::isvalid(const string username, const string password)
{
		//����USER + �˻�
	string str="USER "+username+"\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse(0))  return false;
	
	str="PASS "+password+"\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse(0))  return false;
	return true;
}

//NOOP���ȷ������
bool POP3::Noop()
{
	string str="NOOP\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse(0)) return false;
	return true;
}

//STAT�����ȡ�ʼ��������ܴ�С
bool POP3::STAT()
{
	CString str="STAT\r\n";
	if(send(m_socket,str,str.GetLength(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse(0)) return false;
	
	//�Խ��յ�����Ϣ���д�����ȡ�ʼ���Ŀ���Լ��ܴ�С

	char num[20];
	int i,p=0;
	for(i=0;m_response[i]<'0'||m_response[i]>'9';++i);
	for(;m_response[i]!=_T(' ');++i)
		num[p++]=m_response[i];
	num[p]='\0';
	mailnum=atoi(num);
	p=0;
	for(i=i+1;m_response[i]>='0'&&m_response[i]<='9';++i)
		num[p++]=m_response[i];
	num[p]='\0';
	mailsize=atoi(num);

	return true;
}

unsigned int POP3::GetMailNum()
{
	return mailnum;
}

//�г��ʼ���ż���С
bool POP3::LIST()
{
	CString str="LIST\r\n";		//����LIST���������
	if(send(m_socket,str,str.GetLength(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse(1))	//�����Ӧ�Ƿ���ȷ
	{
		ReleaseSocket();	
		return false;
	}
	//��ȡ�����ʼ���С
	m_size.RemoveAll();
	m_size.SetSize(mailnum);
	int pos=0,L=m_response.GetLength();
	UINT cnt=0;
	while((pos=m_response.Find(" ",pos))!=-1)
	{
		str="";
		for(int i=pos+1;m_response[i]>='0'&&m_response[i]<='9';++i)
			str+=m_response[i];
		if(str!="")
		m_size.InsertAt(cnt++,str);
		++pos;
	}
	
	return true;
}

//TOP��������ʼ�ͷ��Ϣ
bool POP3::TOP()
{
	CString str;
	m_subject.RemoveAll();
	m_subject.SetSize(mailnum);
	m_sender.RemoveAll();
	m_sender.SetSize(mailnum);
	for(int i=1;i<=mailnum;++i)
	{
		str.Format("TOP %d 0\r\n",i);
		if(send(m_socket,str,str.GetLength(),0)==SOCKET_ERROR)
		{
			ReleaseSocket();
			return false;
		}
		if(!CheckResponse(0))
		{
			CString nn;
			nn.Format("%d",i);
			AfxMessageBox("��ȡ��"+nn+"���ʼ�ʧ��!");
			continue;
		}

		//��ȡSUBJECT�ֶ�
		str=m_response;
		str.MakeUpper();
	//	AfxMessageBox(str);


		UINT nstart=str.Find("\r\nSUBJECT:");
		if(nstart==-1)
		{
			m_subject.SetAt(i-1,_T(""));
			continue;
		}
		str=m_response.Right(m_response.GetLength()-nstart-10);
		nstart=str.Find("\r\n");
		if(nstart!=-1) str=str.Left(nstart);
		str.TrimLeft();
		str.TrimRight();
		
		nstart=str.Find("=?GBK?B?");
		if(nstart!=-1)
		{
			str=str.Right(str.GetLength()-nstart-8);
			str=str.Left(str.GetLength()-2);
			Base64 base64;
			int PL;
			str=base64.Decode(str.GetBuffer(0),str.GetLength(),PL).c_str();
		}
		nstart=str.Find("=?UTF-8?B?");
		if(nstart!=-1)
		{
			str=str.Right(str.GetLength()-nstart-10);
			str=str.Left(str.GetLength()-2);
			Base64 base64;
			int PL;
			str=base64.Decode((char *)str.GetBuffer(0),str.GetLength(),PL).c_str();
		}
		m_subject.SetAt(i-1,str);

		//��ȡFROM�ֶ�
		
		str=m_response;
		str.MakeUpper();
		nstart=str.Find("\r\nFROM:");
		str=m_response.Right(m_response.GetLength()-nstart-7);
		nstart=str.Find("\r\n");
		if(nstart!=-1) str=str.Left(nstart);
		str.TrimLeft();
		str.TrimRight();
		
		nstart=str.ReverseFind('<');
		str=str.Right(str.GetLength()-nstart-1);
		nstart=str.Find('>');
		if(nstart!=-1) str=str.Left(nstart);
		m_sender.SetAt(i-1,str);
	}
	return true;
}

CString POP3::GetMailSubject(UINT i)
{
	return m_subject.GetAt(i);
}

CString POP3::GetMailSize(UINT i)
{
	return m_size.GetAt(i);
}

//��ȡ���ΪnIndex���ʼ����ݴ����strMsg��
bool POP3::RETR(UINT nIndex, CString &strMsg)
{
	if(nIndex>mailnum) return FALSE;
	CString str;
	str.Format("RETR %d\r\n",nIndex);
	if (send(m_socket,str,str.GetLength(),0)==SOCKET_ERROR)
	{
		AfxMessageBox("��ȡ�ʼ�ʧ��!");
		ReleaseSocket();
		return FALSE;
	}

	if (!CheckResponse(0))
	{
		AfxMessageBox("�����ʼ�ʧ��!");
		ReleaseSocket();
		return FALSE;
	}

	// ��\r\nΪ��ʼ��־
	ishtml=false;
	if(m_response.Find("Content-Type: text/html")!=-1)
		ishtml=true;
	strMsg = m_response;
	int nStart = strMsg.Find("\r\n\r\n");
	strMsg = m_response.Right(m_response.GetLength() - nStart - 4);

	// ���б�ʾ�ʼ�����
	nStart = strMsg.Find("\r\n\r\n");
	strMsg = strMsg.Right(strMsg.GetLength() - nStart - 4);
	// ���б�ʾ�ʼ����Ľ���
	nStart = strMsg.Find("\r\n\r\n");
	if (nStart != -1) strMsg = strMsg.Left(nStart);
	
	return TRUE;
}

CString POP3::GetMailSender(UINT i)
{
	return m_sender.GetAt(i);
}

bool POP3::QUIT()
{
	if(Noop())
	{
		CString str="QUIT\r\n";
		if (send(m_socket,str,str.GetLength(),0)==SOCKET_ERROR)
		{
			ReleaseSocket();
			return false;
		}
		if(CheckResponse(0))
		{
			ReleaseSocket();
			return false;
		}
	}
	ReleaseSocket();
	return true;
}
