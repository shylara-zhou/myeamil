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
		//创建套接字失败
	
	if(!CreateSocket()) 
		return false;
	//根据服务器名称获取ip地址
	if((m_hostent=gethostbyname(addr.c_str()))==NULL)
	{
		ReleaseSocket();
		return false;
	}
	//该服务器不存在
	if(m_hostent->h_addr_list[0]==NULL)
	{
		ReleaseSocket();
		return false;
	}
	
	//设置协议簇和服务器ip以及端口
	memset(&m_sockaddr_in,0,sizeof(m_sockaddr_in));
	m_sockaddr_in.sin_family=AF_INET;
	m_sockaddr_in.sin_port=htons(port);
	m_sockaddr_in.sin_addr.S_un.S_addr=*(ULONG *)m_hostent->h_addr_list[0];

	//连接
	if(connect(m_socket,(sockaddr *)&m_sockaddr_in,sizeof(m_sockaddr_in))==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	//检查返回的状态
	if(!CheckResponse(0)) return false;
	
	//连接服务器成功
	return true;
}

bool POP3::CreateSocket()
{
		//初始化
	if(WSAStartup(MAKEWORD(2,2),&m_wsadata) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return FALSE;
	}
	//创建套接字
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
//这里不能直接用数组接收，因为有延迟以及长文本分段发送的情况
//需要设定一个时间，若在时间内接收到的数据长度没有变化，则接收存储数据

/*

	TCHAR Respond[100005]={0};
	if(recv(m_socket,Respond,sizeof(Respond),0) == SOCKET_ERROR)
	{
	//	AfxMessageBox("接收响应出错!");
		ReleaseSocket();
		return false;
	}

	if(memcmp(Respond,"+OK",3)!=0)
	{
	//	AfxMessageBox("响应信息出错!");
		return false;
	}
	m_response=Respond;
	return true;
*/
	TCHAR pChar[100005];
	CString strTemp;

	// 读取回应信息
	BOOL bEnd = FALSE;
	UINT nReceived = 0;
	DWORD dwStart = ::GetTickCount();
	while (!bEnd)
	{
		// 尝试时间到
		if ((::GetTickCount() - dwStart) >	2000)
		{
			pChar[nReceived] = '\0';
			// 保存当前回应的消息
			m_response = pChar;
			AfxMessageBox("接收响应消息超时");
			return FALSE;
		}

		// 看套接字是否可读
		timeval timeout = {0, 0};
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(m_socket,&fds);
		int nStatus = select(0,&fds,NULL,NULL,&timeout);

		if (nStatus == SOCKET_ERROR)
		{
			// 套接字不可读
			AfxMessageBox("套接字不可达");
			return FALSE;
		}
		else if (!nStatus)
		{
			// 没有接收到数据
			::Sleep(688);
			continue;
		}

		// 从套接字中接收数据
		::Sleep(288);
		nStatus = recv(m_socket,pChar + nReceived,sizeof(pChar),0);
		if (nStatus == SOCKET_ERROR)
		{
			pChar[nReceived] = '\0';
			// 套接字错误
			// 保存当前回应信息
			m_response = pChar; 
			AfxMessageBox("未能从套接字中收到数据");
			return FALSE; 
		}
		else if (nStatus)
		{
			// 重置计时器
			dwStart = ::GetTickCount();
			// 已收到的数据又增加了
			nReceived += nStatus;
		}

		// 将pChar设为字符串，并赋给CString型的变量
		pChar[nReceived] = '\0';
		strTemp = pChar;
								
		// 检查是否收到了结束标志
		LPCTSTR lpszComp = bDouble ? "\r\n.\r\n" : "\r\n";
		bEnd = (strTemp.Find(lpszComp) != -1);
	}

	// 去掉结束标志
	nReceived -= bDouble ? 3 : 0;
	pChar[nReceived] = '\0';

	// 检查回应信息是否有效
	strTemp = pChar;
	strTemp.MakeUpper();
	int nStart = strTemp.Find("+OK");
	if (nStart == -1)
	{
		// 收到无效信息
		AfxMessageBox("回应消息无效");
		return FALSE;
	}
	// 提取有效信息
	strTemp = pChar;
	m_response = strTemp.Right(strTemp.GetLength() - nStart - 3);
	
	return TRUE;
 }

 //用户名密码检测
bool POP3::isvalid(const string username, const string password)
{
		//发送USER + 账户
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

//NOOP命令，确认连接
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

//STAT命令，获取邮件总数及总大小
bool POP3::STAT()
{
	CString str="STAT\r\n";
	if(send(m_socket,str,str.GetLength(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse(0)) return false;
	
	//对接收到的信息进行处理，获取邮件数目、以及总大小

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

//列出邮件序号及大小
bool POP3::LIST()
{
	CString str="LIST\r\n";		//发送LIST命令到服务器
	if(send(m_socket,str,str.GetLength(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse(1))	//检查响应是否正确
	{
		ReleaseSocket();	
		return false;
	}
	//读取各个邮件大小
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

//TOP命令，返回邮件头信息
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
			AfxMessageBox("获取第"+nn+"封邮件失败!");
			continue;
		}

		//获取SUBJECT字段
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

		//获取FROM字段
		
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

//获取序号为nIndex的邮件内容存放于strMsg中
bool POP3::RETR(UINT nIndex, CString &strMsg)
{
	if(nIndex>mailnum) return FALSE;
	CString str;
	str.Format("RETR %d\r\n",nIndex);
	if (send(m_socket,str,str.GetLength(),0)==SOCKET_ERROR)
	{
		AfxMessageBox("收取邮件失败!");
		ReleaseSocket();
		return FALSE;
	}

	if (!CheckResponse(0))
	{
		AfxMessageBox("接收邮件失败!");
		ReleaseSocket();
		return FALSE;
	}

	// 以\r\n为起始标志
	ishtml=false;
	if(m_response.Find("Content-Type: text/html")!=-1)
		ishtml=true;
	strMsg = m_response;
	int nStart = strMsg.Find("\r\n\r\n");
	strMsg = m_response.Right(m_response.GetLength() - nStart - 4);

	// 空行表示邮件正文
	nStart = strMsg.Find("\r\n\r\n");
	strMsg = strMsg.Right(strMsg.GetLength() - nStart - 4);
	// 空行表示邮件正文结束
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
