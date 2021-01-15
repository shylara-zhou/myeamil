// SMTP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SMTP.h"
#include "SMTPDlg.h"
#include <stdio.h>
#include"Login.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMTPApp

BEGIN_MESSAGE_MAP(CSMTPApp, CWinApp)
	//{{AFX_MSG_MAP(CSMTPApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMTPApp construction

CSMTPApp::CSMTPApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSMTPApp object

CSMTPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSMTPApp initialization
//����
BOOL CSMTPApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CLOGIN dlg;
	//m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK


			CSMTPDlg sdlg;
			m_pMainWnd = &sdlg;
	
				sdlg.DoModal();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	//CSMTPDlg sdlg;
	//m_pMainWnd = &sdlg;
	//if (dlg.Login()) {
	//	sdlg.DoModal();
	//}


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// SMTP Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SMTP::SMTP()
{

}

SMTP::~SMTP()
{

}

bool SMTP::CreateSocket()	//�����׽���
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

void SMTP::ReleaseSocket() //�ͷ��׽���
{
	shutdown(m_socket,SD_BOTH);
	closesocket(m_socket);
	WSACleanup();
}

//���ӵ�������
bool SMTP::Connect(const string addr, const int port)
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

	//��鷵�ص�״̬���ǲ���220:������׼������
	if(!CheckResponse("220")) return false;

	//���ͻ�ӦHELO
	string str="HELO "+addr+"\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	
	//��鷵�ص�״̬���ǲ���250:�����������
	if(!CheckResponse("250")) 
		return false;
	
	//���ӷ������ɹ�
	return true;
}


bool SMTP::isvalid(const string username, const string password)//��֤�û���������
{
	Base64 base64;
	string str="AUTH LOGIN\r\n";

	//����AUTH LOGIN
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}

	//��鷵�ص�״̬���ǲ���334:�ȴ��û�������֤��Ϣ
	if(!CheckResponse("334")) return false;
	
	//���û�����base64����
	str="";
	str=base64.Encode((unsigned char *)username.c_str(),username.length())+"\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334")) return false;

	//��������base64����
	str="";
	str=base64.Encode((unsigned char *)password.c_str(),password.length())+"\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	//235:�û���֤�ɹ�
	if(!CheckResponse("235")) return false;
	
	return true;
}

bool SMTP::Send(const string sendaddr,
	 const vector<string> rcvlist,
	 const string sendname,
	 const string rcvname,
	 const string subject,
	 const string content,
	 const vector<string> file,
	 int ishtml)
{
	int i;
	if(sendaddr.length()==0||rcvlist.size()<=0) return false;
	string str;
	Base64 base64;

	//��д���͵�ַ
	str="MAIL FROM:<"+sendaddr+">\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250")) return false;
	
	//��д���շ�
	for(i=0;i<rcvlist.size();++i)
	{
		str="RCPT TO:<"+rcvlist[i]+">\r\n";
		if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
		{
			ReleaseSocket();
			return false;
		}
		if(!CheckResponse("250")) return false;
	}

	//DATA
	str="DATA\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	//354:��ʼ�ʼ�����
	if(!CheckResponse("354")) return false;
	
	//��д�ʼ���Ϣ
	str="from:"+sendname+"<"+sendaddr+">\r\n";
	str+="subject:"+subject+"\r\n";
	str+="MIME_Version:1.0\r\n";
	str=str+"X-Mailer:"+COPYRIGHT+"\r\n";
	str=str+"Content-type:multipart/mixed;Boundary="+BOUNDARY+"\r\n\r\n";
	
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}

	//�ʼ�����
	str="--";
	str+=BOUNDARY;
	str+="\r\n";
	str+=ishtml?"Content-type:text/html;Charset=gb2312\r\n":"Content-type:text/plain;Charset=gb2312\r\n";
	str+="Content-Transfer-Encoding:Base64\r\n\r\n";
	//����
	string Content=base64.Encode((unsigned char *)content.c_str(),content.length());
	str+=Content+"\r\n\r\n";

	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}

	//����
	for(i=0;i<file.size();++i)
	{
		str="--";
		str+=BOUNDARY;
		str+="\r\n";
		str+="Content-Type;application/octet-stream;Name=";
		str+=file[i].substr(file[i].find_last_of("\\")+1).c_str();
		str+="\r\n";
		str+="Content-Disposition:attachment;FileName=";
		str+=file[i].substr(file[i].find_last_of("\\")+1).c_str();
		str+="\r\n";
		str+="Content-Transfer-Encoding:Base64\r\n\r\n";
		if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
		{
			ReleaseSocket();
			return false;
		}

		FILE* fin=fopen(file[i].c_str(),"rb");
		char *buf=new char[READ_BUFFER+5];
		memset(buf,0,READ_BUFFER+5);
		int Size=fread(buf,1,READ_BUFFER,fin);
		while(Size==READ_BUFFER)
		{
			str=base64.Encode((unsigned char*)buf,Size);
			if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
			{
				ReleaseSocket();
				return false;
			}
			memset(buf,0,READ_BUFFER+5);
			Size=fread(buf,1,READ_BUFFER,fin);
		}
		if(Size>0)
		{
			str=base64.Encode((unsigned char*)buf,Size)+"\r\n\r\n";
			if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
			{
				ReleaseSocket();
				return false;
			}
		}
		delete[] buf;
		fclose(fin);
	}
	str="--";
	str+=BOUNDARY;
	str+="--\r\n.\r\n";

	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250")) return false;

	str="QUIT\r\n";
	if(send(m_socket,str.c_str(),str.length(),0)==SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	
	//221:�ر�����
	if(!CheckResponse("221")) return false;
	ReleaseSocket();
	return true;
}

bool SMTP::CheckResponse(const char *code)	//��鷵�ص�״̬���������״̬���Ƿ�һ��
{
	char rcvcode[1024]={0};		//����̫С�����
	if(recv(m_socket,rcvcode,sizeof(rcvcode),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(code[0] == rcvcode[0] && code[1] == rcvcode[1] && code[2] == rcvcode[2])
		return true;
	return false;
}
