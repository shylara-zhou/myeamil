// SMTPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMTP.h"
#include "SMTPDlg.h"
#include "POP3.h"
#include "Mail.h"
#include "BlackList.h"
#include "FriendList.h"
#include "Base64.h"
#include "shlwapi.h"

#pragma comment(lib, "shlwapi.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CString strMail;
CStringArray BlackList_list;
CStringArray FriendList_list;
CString USER;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMTPDlg dialog

CSMTPDlg::CSMTPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMTPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMTPDlg)
	m_send = false;
	m_html = false;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMTPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMTPDlg)
	DDX_Control(pDX, IDC_LIST, m_listmail);
	DDX_Check(pDX, IDC_CHECK3, m_send);
	//DDX_Check(pDX, IDC_CHECK1, m_html);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSMTPDlg, CDialog)
	//{{AFX_MSG_MAP(CSMTPDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSEND, OnSend)
	//IDC_CHECK3
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
//	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_ADDFILE, OnAddfile)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_GETEMAIL, OnGetemail)
	ON_BN_CLICKED(IDC_LOGIN, OnLogin)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_BN_CLICKED(IDC_BlackList, OnBlackList)
	ON_BN_CLICKED(IDC_FriendList, OnFriendList)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CSMTPDlg::OnLvnItemchangedList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMTPDlg message handlers

BOOL CSMTPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CComboBox* pComSmtp=(CComboBox*)GetDlgItem(IDC_COMBO_SERVER);
	pComSmtp->AddString("@126.com");
	pComSmtp->AddString("@qq.com");
	pComSmtp->AddString("@163.com");
	pComSmtp->SetCurSel(2);

	
	

	CString eusername;
	CString epassword;
	CString path = TEXT("./e.txt");

	CFile fp;//ʹ��MFC��CFile������д���ļ�
	fp.Open(path, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	fp.SeekToBegin();//�ƶ��ļ�ָ��λ�����ļ�ͷ

	CArchive ar(&fp, CArchive::load);//ʹ��CArchive���ж�ȡ�ļ�	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(FALSE);
	bool flag = false;//���
	ar.ReadString(eusername);
	ar.ReadString(epassword);
	ar.ReadString(epassword);
	ar.Close();
	fp.Close();
	SetDlgItemText(IDC_ACCOUNT,eusername );
	SetDlgItemText(IDC_PASSWORD, epassword);
	
	m_html=false;
	m_send = false;
	


	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ���� 
	m_listmail.SetExtendedStyle(m_listmail.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strText;
	strText = _T("���");
	m_listmail.InsertColumn(0,strText,LVCFMT_CENTER,50);
	strText = _T("�ʼ�����");
	m_listmail.InsertColumn(1,strText,LVCFMT_CENTER,300);
	strText = _T("���ͷ�");
	m_listmail.InsertColumn(2,strText,LVCFMT_CENTER,150);
	strText = _T("�ʼ���С");
	m_listmail.InsertColumn(3,strText,LVCFMT_CENTER,100);
	USER="";
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSMTPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSMTPDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
	//	CDialog::OnPaint();
		  CPaintDC   dc(this);   
          CRect   rect;   
          GetClientRect(&rect);   //��ȡ���ڿͻ����Ĵ�С
          CDC   dcMem;   
          dcMem.CreateCompatibleDC(&dc);   
          CBitmap   bmpBackground;   
          bmpBackground.LoadBitmap(IDB_BITMAP4);   
                  //IDB_BITMAP�����Լ���ͼ��Ӧ��ID   
          BITMAP   bitmap;   
          bmpBackground.GetBitmap(&bitmap);   
          CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
          dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);   
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSMTPDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSMTPDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	SMTP smtp;				// smtp����
	CString strSmtpAddr;	// smtp��������ַ
	CString strAccount;		// �ʺ�
	CString strPassword;	// ����
	CString strSendfrom;	// ����Դ��ַ
	CString strSendto;		// Ŀ�ص�ַ
	CString strSendto2;		// Ŀ�ص�ַ
	CString strSenderName;	// ����������
	CString strReceiverName="";// ����������
	CString strSubject;		// �ʼ�����
	CString strContent;		// �ʼ�����
	CString strserver;

	//�ռ���Ϣ

	GetDlgItemText(IDC_COMBO_SERVER   , strserver);
	GetDlgItemText(IDC_ACCOUNT     , strAccount);
	GetDlgItemText(IDC_PASSWORD    , strPassword);
	GetDlgItemText(IDC_SEND_NAME  , strSenderName);
	GetDlgItemText(IDC_RCV_ADDRESS      , strSendto);
	GetDlgItemText(IDC_RCV_ADDRESS2, strSendto2);
	GetDlgItemText(IDC_SUBJECT     , strSubject);
	GetDlgItemText(IDC_CONTENT     , strContent);

	//��֤��Ϣ
	
	strSmtpAddr="smtp."+strserver.Right(strserver.GetLength()-1);
	strSendfrom=strAccount+strserver;
	if(strAccount.IsEmpty())
	{
		AfxMessageBox("����д�ʺ�!"); 
		return ;
	}
	if(strPassword.IsEmpty())
	{
		AfxMessageBox("����д����"); 
		return ;
	}

	if(strSendto.IsEmpty())
	{
		AfxMessageBox("����дĿ�ص�ַ!"); 
		return ;
	}

	//���ӷ�����
	if(!smtp.Connect(string(strSmtpAddr.GetBuffer(strSmtpAddr.GetLength())),25)){
		AfxMessageBox("���ӷ�����ʧ��!"); return ;	
	}

	//��֤�û�������
	if(!smtp.isvalid(string(strAccount.GetBuffer(strAccount.GetLength())),string(strPassword.GetBuffer(strPassword.GetLength())))){
		AfxMessageBox("�û������������!"); return ;	
	}

	//����Ŀ�ص�ַ�б�
	if (m_send) {
		strSendto = strSendto + "," + strSendto2;
	}
	vector<string> strMails;
	int L=strSendto.GetLength(),p=0;
	string str="";
	for(int i=0;i<L;++i)
	{
		if(strSendto[i]==',')
		{
			strMails.push_back(str);
			str="";
			continue;
		}
		str+=strSendto[i];
	}
	if(str!="") strMails.push_back(str);
	//����
	if(!smtp.Send(string(strSendfrom.GetBuffer(strSendfrom.GetLength())),strMails,string(strSenderName.GetBuffer(strSenderName.GetLength())),string(strReceiverName.GetBuffer(strReceiverName.GetLength())),string(strSubject.GetBuffer(strSubject.GetLength())),string(strContent.GetBuffer(strContent.GetLength())),files,m_html))
	{
		//!smtp.Send(string(strSendfrom.GetBuffer(strSendfrom.GetLength())), strMails, string(strSenderName.GetBuffer(strSenderName.GetLength())), string(strReceiverName.GetBuffer(strReceiverName.GetLength())), string(strSubject.GetBuffer(strSubject.GetLength())), string(strContent.GetBuffer(strContent.GetLength())), files, m_html);
		AfxMessageBox("�ʼ�����ʧ��!");
		return ;
	}
	AfxMessageBox("�ʼ����ͳɹ�!");
	//if (m_send) {
	//	//����Ŀ�ص�ַ�б�
	//	vector<string> strMails2;
	//	int L = strSendto2.GetLength(), p = 0;
	//	string str2 = "";
	//	for (int i = 0; i < L; ++i)
	//	{
	//		if (strSendto2[i] == ',')
	//		{
	//			strMails2.push_back(str2);
	//			str2 = "";
	//			continue;
	//		}
	//		str2 += strSendto2[i];
	//	}
	//	if (str2 != "") strMails2.push_back(str2);
	//	//����
	//	if (!smtp.Send(string(strSendfrom.GetBuffer(strSendfrom.GetLength())), strMails2, string(strSenderName.GetBuffer(strSenderName.GetLength())), string(strReceiverName.GetBuffer(strReceiverName.GetLength())), string(strSubject.GetBuffer(strSubject.GetLength())), string(strContent.GetBuffer(strContent.GetLength())), files, m_html))
	//	{
	//		//!smtp.Send(string(strSendfrom.GetBuffer(strSendfrom.GetLength())), strMails, string(strSenderName.GetBuffer(strSenderName.GetLength())), string(strReceiverName.GetBuffer(strReceiverName.GetLength())), string(strSubject.GetBuffer(strSubject.GetLength())), string(strContent.GetBuffer(strContent.GetLength())), files, m_html);
	//		AfxMessageBox("�ʼ�����ʧ��!");
	//		return;
	//	}
	//	AfxMessageBox("�ʼ����ͳɹ�!");
	//}
}


//void CSMTPDlg::OnCheck1() 
//{
//	m_html=!m_html;	
//}

void CSMTPDlg::OnCheck3()
{
	m_send = !m_send;
}

void CSMTPDlg::OnAddfile()
{
	CString cstrsucstring;
	int i, j;
	vector<string> tmp;
	//CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, "All Files (*.*)|*.*||");

	TCHAR *pBuffer = new TCHAR[260*20];//�������ͬʱ��20���ļ�
	//filedlg.m_ofn.lpstrFile = pBuffer;
	//filedlg.m_ofn.nMaxFile = 260 * 20;
	//filedlg.m_ofn.lpstrFile[0] = '\0';
	//static TCHAR BASED_CODE szFilter[] = _T("Chart Files (*.xlc)\0*.xlc\0Worksheet Files (*.xls)\0*.xls\0Data Files (*.xlc;*.xls)\0*.xlc; *.xls\0WordFile (*.doc;*.docx)\0*.doc; *.docx\0All Files (*.*)\0*.*\0\0");
	//static TCHAR BASED_CODE szFilter[] = _T("TEXT FILE (*.txt)\0*.txt\0Chart Files (*.xlc)\0*.xlc\0Worksheet Files (*.xls)\0*.xls\0Data Files (*.xlc;*.xls)\0*.xlc; *.xls\0WordFile (*.doc;*.docx)\0*.doc; *.docx\0");
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, "TEXT FILE (*.txt)|*.txt|Chart Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls|WordFile (*.doc;*.docx)|*.doc; *.docx||");
	//dlgFile.GetOFN().lpstrFilter = szFilter;
	dlgFile.m_ofn.lpstrFile = pBuffer;
	dlgFile.m_ofn.nMaxFile = 260 * 20;
	dlgFile.m_ofn.lpstrFile[0] = '\0';
	//dlgFile.DoModal();
	string str;
	if (dlgFile.DoModal() == IDOK)
	{
		CString cstrfilepath = _T("");
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			cstrfilepath = dlgFile.GetNextPathName(pos);//ȡ���ļ�·��
			str = string(cstrfilepath.GetBuffer(cstrfilepath.GetLength()));
			str = str.substr(str.find_last_of("\\") + 1);
			tmp.push_back(str);
		}
	}
	for (i = 0; i < tmp.size(); ++i)
	{
		for (j = 0; j < files.size(); ++j)
			if (tmp[i] == files[j]) break;
		if (j != files.size()) continue;
		files.push_back(tmp[i]);
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FILE);
	pCombo->ResetContent();
	for (i = 0; i < files.size(); ++i)
		pCombo->AddString(files[i].substr(files[i].find_last_of("\\") + 1).c_str());
	pCombo->SetCurSel(files.size() - 1);
}

void CSMTPDlg::OnDelete() 
{
	if(files.size()>0)
		files.erase(files.begin()+pCombo->GetCurSel());
	pCombo->ResetContent();
	for(int i=0;i<files.size();++i)
		pCombo->AddString(files[i].substr(files[i].find_last_of("\\")+1).c_str());
	pCombo->SetCurSel(files.size()-1);	
}

void CSMTPDlg::OnGetemail() 
{
	if(!pop3.Noop())
	{
		AfxMessageBox("���ȵ�½����!");
		return;
	}
	if(!pop3.STAT())
	{
		AfxMessageBox("����STAT������Ϣʧ��!");
		return;
	}
	if(!pop3.LIST())
	{
		AfxMessageBox("����LIST������Ϣʧ��!");
		return;
	}
	if(!pop3.TOP())
	{
		AfxMessageBox("����TOP������Ϣʧ��!");
		return;
	}
	
	m_listmail.DeleteAllItems();
	UINT n=pop3.GetMailNum(),cnt=0;
	CString str,tmp;
	for(UINT i=0;i<n;++i)
	{
		tmp=pop3.GetMailSender(i);
		bool flag=1;
		for(int j=0;j<BlackList_list.GetSize();++j)
			if(tmp==BlackList_list.GetAt(j)) {flag=0;break;}
		if(!flag) continue;

		str.Format("%d",cnt);
		m_listmail.InsertItem(cnt,str);
		m_listmail.SetItemText(cnt,1,pop3.GetMailSubject(i));
		m_listmail.SetItemText(cnt,2,tmp);
		m_listmail.SetItemText(cnt,3,pop3.GetMailSize(i));
		cnt++;
	}

}

void CSMTPDlg::OnLogin() 
{
	// TODO: Add your control notification handler code here
	UpdateData();


	CString strSmtpAddr;	// ��������ַ
	CString strAccount;		// �ʺ�
	CString strPassword;	// ����
	CString strserver;

	//�ռ���Ϣ

	GetDlgItemText(IDC_COMBO_SERVER   , strserver);
	GetDlgItemText(IDC_ACCOUNT     , strAccount);
	GetDlgItemText(IDC_PASSWORD    , strPassword);

	//��֤��Ϣ
	if(strserver.Find("qq")==-1)	//ע��qq����ķ�����������ͷΪpop
		strSmtpAddr="pop3."+strserver.Right(strserver.GetLength()-1);
	else 
		strSmtpAddr="pop."+strserver.Right(strserver.GetLength()-1);
	
	
	if(strAccount.IsEmpty())
	{
		AfxMessageBox("����д�ʺ�!"); 
		return ;
	}
	strAccount+=strserver;
	if(strPassword.IsEmpty())
	{
		AfxMessageBox("����д����"); 
		return ;
	}

	if(!pop3.Connect(string(strSmtpAddr.GetBuffer(strSmtpAddr.GetLength())),110)){
		AfxMessageBox("���ӷ�����ʧ��!"); return ;	
	}

	//��֤�û�������
	if(!pop3.isvalid(string(strAccount.GetBuffer(strAccount.GetLength())),string(strPassword.GetBuffer(strPassword.GetLength())))){
		AfxMessageBox("�û������������!"); return ;	
	}
	AfxMessageBox("��½�ɹ�!");
	USER=strAccount;


	//��ȡ�ڡ��������б�
	CStdioFile file;
	CString filepath,filename,str;
	filepath=_T("C:\\LIST"); //�ļ�λ��
	if(!PathFileExists(filepath))
	{
		if(!CreateDirectory(filepath,NULL))
		{
			AfxMessageBox("δ�ܴ������Ŀ¼:"+filepath);
			return;
		}
	}
	filepath=filepath+"\\"+USER;
	if(!PathFileExists(filepath))
	{
		if(!CreateDirectory(filepath,NULL))
		{
			AfxMessageBox("δ�ܴ������Ŀ¼:"+filepath);
			return ;
		}
	}
	filename="BlackListlist.txt";  //�ļ���
	filename=filepath+"\\"+filename;  //�ļ�·��

	if(!file.Open(filename,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))//�����ļ�
	{
		AfxMessageBox("δ�ҵ�����ļ�!");
		return ;
	}

	BlackList_list.RemoveAll();
	while(file.ReadString(str))
	{
		BlackList_list.Add(str);
	}
	file.Close();  //�ر��ļ�


	filepath=_T("C:\\LIST"); //�ļ�λ��
	filepath=filepath+"\\"+USER;
	filename="FriendListlist.txt";  //�ļ���
	filename=filepath+"\\"+filename;  //�ļ�·��

	if(!file.Open(filename,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))//�����ļ�
	{
		AfxMessageBox("δ�ҵ�����ļ�!");
		return ;
	}

	FriendList_list.RemoveAll();
	while(file.ReadString(str))
	{
		FriendList_list.Add(str);
	}
	file.Close();  //�ر��ļ�
}

void CSMTPDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	if (m_listmail.GetSelectedCount() != 1)
	{
		AfxMessageBox("����ȷҪ�򿪵�email!");
		return;
	}

	if(!pop3.Noop())
	{
		AfxMessageBox("������������ѶϿ�,�볢�����µ�½");
		return;
	}

	POSITION pos = m_listmail.GetFirstSelectedItemPosition();
	UINT nIndex = m_listmail.GetNextSelectedItem(pos);
	if(pop3.RETR(nIndex + 1,strMail))
	{
		Base64 base64;
		int L;
		string tmp=base64.Decode((char *)strMail.GetBuffer(strMail.GetLength()),strMail.GetLength(),L);
		if(!pop3.ishtml) 
			strMail=tmp.c_str();
		Mail dlg;
		dlg.DoModal();
	}
	*pResult = 0;
}

void CSMTPDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	CString strLangName;    // ѡ�����Ե������ַ���   
    NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;   
  
    if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
    {   
        // ��ȡ��ѡ���б����һ��������ı�   
        strLangName = m_listmail.GetItemText(pNMListView->iItem, 0);   
        // ��ѡ���������ʾ��༭����   
        SetDlgItemText(IDC_LIST, strLangName);   
    }   
}

void CSMTPDlg::OnBlackList() 
{
	// TODO: Add your control notification handler code here
	if(pop3.Noop())
	{
		CBlackList dlg;
		dlg.DoModal();
	}
	else
	{
		AfxMessageBox("���½");
	}
}

void CSMTPDlg::OnFriendList() 
{
	// TODO: Add your control notification handler code here
	if(pop3.Noop())
	{
		CFriendList dlg;
		dlg.DoModal();
	}
	else
	{
		AfxMessageBox("���½");
	}
}

void CSMTPDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	pop3.QUIT();
	CDialog::OnClose();
}

HBRUSH CSMTPDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	   switch(nCtlColor)

       {

              case CTLCOLOR_STATIC: //�����о�̬�ı��ؼ�������

              {

                     pDC->SetBkMode(TRANSPARENT);//���ñ���Ϊ͸��            

                     pDC->SetTextColor(RGB(0,0,0)); //����������ɫ

                     return HBRUSH(GetStockObject(HOLLOW_BRUSH)); // ����

              }

              case CTLCOLOR_DLG: //�����о�̬�ı��ؼ�������

              {

                     pDC->SetBkMode(TRANSPARENT); 

                     return hbr; //���ػ�ˢ���

              }

              default:

              return CDialog::OnCtlColor(pDC,pWnd, nCtlColor);

       }
	return hbr;
}


void CSMTPDlg::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


