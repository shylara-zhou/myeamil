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

	CFile fp;//使用MFC的CFile将数据写入文件
	fp.Open(path, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	fp.SeekToBegin();//移动文件指针位置至文件头

	CArchive ar(&fp, CArchive::load);//使用CArchive进行读取文件	
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(FALSE);
	bool flag = false;//标记
	ar.ReadString(eusername);
	ar.ReadString(epassword);
	ar.ReadString(epassword);
	ar.Close();
	fp.Close();
	SetDlgItemText(IDC_ACCOUNT,eusername );
	SetDlgItemText(IDC_PASSWORD, epassword);
	
	m_html=false;
	m_send = false;
	


	// 为列表视图控件添加全行选中和栅格风格 
	m_listmail.SetExtendedStyle(m_listmail.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strText;
	strText = _T("序号");
	m_listmail.InsertColumn(0,strText,LVCFMT_CENTER,50);
	strText = _T("邮件主题");
	m_listmail.InsertColumn(1,strText,LVCFMT_CENTER,300);
	strText = _T("发送方");
	m_listmail.InsertColumn(2,strText,LVCFMT_CENTER,150);
	strText = _T("邮件大小");
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
          GetClientRect(&rect);   //获取窗口客户区的大小
          CDC   dcMem;   
          dcMem.CreateCompatibleDC(&dc);   
          CBitmap   bmpBackground;   
          bmpBackground.LoadBitmap(IDB_BITMAP4);   
                  //IDB_BITMAP是你自己的图对应的ID   
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

	SMTP smtp;				// smtp对像
	CString strSmtpAddr;	// smtp服务器地址
	CString strAccount;		// 帐号
	CString strPassword;	// 密码
	CString strSendfrom;	// 发送源地址
	CString strSendto;		// 目地地址
	CString strSendto2;		// 目地地址
	CString strSenderName;	// 发送者姓名
	CString strReceiverName="";// 接收者姓名
	CString strSubject;		// 邮件主题
	CString strContent;		// 邮件正文
	CString strserver;

	//收集信息

	GetDlgItemText(IDC_COMBO_SERVER   , strserver);
	GetDlgItemText(IDC_ACCOUNT     , strAccount);
	GetDlgItemText(IDC_PASSWORD    , strPassword);
	GetDlgItemText(IDC_SEND_NAME  , strSenderName);
	GetDlgItemText(IDC_RCV_ADDRESS      , strSendto);
	GetDlgItemText(IDC_RCV_ADDRESS2, strSendto2);
	GetDlgItemText(IDC_SUBJECT     , strSubject);
	GetDlgItemText(IDC_CONTENT     , strContent);

	//验证信息
	
	strSmtpAddr="smtp."+strserver.Right(strserver.GetLength()-1);
	strSendfrom=strAccount+strserver;
	if(strAccount.IsEmpty())
	{
		AfxMessageBox("请填写帐号!"); 
		return ;
	}
	if(strPassword.IsEmpty())
	{
		AfxMessageBox("请填写密码"); 
		return ;
	}

	if(strSendto.IsEmpty())
	{
		AfxMessageBox("请填写目地地址!"); 
		return ;
	}

	//连接服务器
	if(!smtp.Connect(string(strSmtpAddr.GetBuffer(strSmtpAddr.GetLength())),25)){
		AfxMessageBox("连接服务器失败!"); return ;	
	}

	//验证用户名密码
	if(!smtp.isvalid(string(strAccount.GetBuffer(strAccount.GetLength())),string(strPassword.GetBuffer(strPassword.GetLength())))){
		AfxMessageBox("用户名或密码错误!"); return ;	
	}

	//处理目地地址列表
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
	//发送
	if(!smtp.Send(string(strSendfrom.GetBuffer(strSendfrom.GetLength())),strMails,string(strSenderName.GetBuffer(strSenderName.GetLength())),string(strReceiverName.GetBuffer(strReceiverName.GetLength())),string(strSubject.GetBuffer(strSubject.GetLength())),string(strContent.GetBuffer(strContent.GetLength())),files,m_html))
	{
		//!smtp.Send(string(strSendfrom.GetBuffer(strSendfrom.GetLength())), strMails, string(strSenderName.GetBuffer(strSenderName.GetLength())), string(strReceiverName.GetBuffer(strReceiverName.GetLength())), string(strSubject.GetBuffer(strSubject.GetLength())), string(strContent.GetBuffer(strContent.GetLength())), files, m_html);
		AfxMessageBox("邮件发送失败!");
		return ;
	}
	AfxMessageBox("邮件发送成功!");
	//if (m_send) {
	//	//处理目地地址列表
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
	//	//发送
	//	if (!smtp.Send(string(strSendfrom.GetBuffer(strSendfrom.GetLength())), strMails2, string(strSenderName.GetBuffer(strSenderName.GetLength())), string(strReceiverName.GetBuffer(strReceiverName.GetLength())), string(strSubject.GetBuffer(strSubject.GetLength())), string(strContent.GetBuffer(strContent.GetLength())), files, m_html))
	//	{
	//		//!smtp.Send(string(strSendfrom.GetBuffer(strSendfrom.GetLength())), strMails, string(strSenderName.GetBuffer(strSenderName.GetLength())), string(strReceiverName.GetBuffer(strReceiverName.GetLength())), string(strSubject.GetBuffer(strSubject.GetLength())), string(strContent.GetBuffer(strContent.GetLength())), files, m_html);
	//		AfxMessageBox("邮件抄送失败!");
	//		return;
	//	}
	//	AfxMessageBox("邮件抄送成功!");
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

	TCHAR *pBuffer = new TCHAR[260*20];//最多允许同时打开20个文件
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
			cstrfilepath = dlgFile.GetNextPathName(pos);//取得文件路径
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
		AfxMessageBox("请先登陆邮箱!");
		return;
	}
	if(!pop3.STAT())
	{
		AfxMessageBox("接收STAT返回信息失败!");
		return;
	}
	if(!pop3.LIST())
	{
		AfxMessageBox("接收LIST返回信息失败!");
		return;
	}
	if(!pop3.TOP())
	{
		AfxMessageBox("接收TOP返回信息失败!");
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


	CString strSmtpAddr;	// 服务器地址
	CString strAccount;		// 帐号
	CString strPassword;	// 密码
	CString strserver;

	//收集信息

	GetDlgItemText(IDC_COMBO_SERVER   , strserver);
	GetDlgItemText(IDC_ACCOUNT     , strAccount);
	GetDlgItemText(IDC_PASSWORD    , strPassword);

	//验证信息
	if(strserver.Find("qq")==-1)	//注意qq邮箱的服务器域名开头为pop
		strSmtpAddr="pop3."+strserver.Right(strserver.GetLength()-1);
	else 
		strSmtpAddr="pop."+strserver.Right(strserver.GetLength()-1);
	
	
	if(strAccount.IsEmpty())
	{
		AfxMessageBox("请填写帐号!"); 
		return ;
	}
	strAccount+=strserver;
	if(strPassword.IsEmpty())
	{
		AfxMessageBox("请填写密码"); 
		return ;
	}

	if(!pop3.Connect(string(strSmtpAddr.GetBuffer(strSmtpAddr.GetLength())),110)){
		AfxMessageBox("连接服务器失败!"); return ;	
	}

	//验证用户名密码
	if(!pop3.isvalid(string(strAccount.GetBuffer(strAccount.GetLength())),string(strPassword.GetBuffer(strPassword.GetLength())))){
		AfxMessageBox("用户名或密码错误!"); return ;	
	}
	AfxMessageBox("登陆成功!");
	USER=strAccount;


	//获取黑、白名单列表
	CStdioFile file;
	CString filepath,filename,str;
	filepath=_T("C:\\LIST"); //文件位置
	if(!PathFileExists(filepath))
	{
		if(!CreateDirectory(filepath,NULL))
		{
			AfxMessageBox("未能创建相关目录:"+filepath);
			return;
		}
	}
	filepath=filepath+"\\"+USER;
	if(!PathFileExists(filepath))
	{
		if(!CreateDirectory(filepath,NULL))
		{
			AfxMessageBox("未能创建相关目录:"+filepath);
			return ;
		}
	}
	filename="BlackListlist.txt";  //文件名
	filename=filepath+"\\"+filename;  //文件路径

	if(!file.Open(filename,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))//建立文件
	{
		AfxMessageBox("未找到相关文件!");
		return ;
	}

	BlackList_list.RemoveAll();
	while(file.ReadString(str))
	{
		BlackList_list.Add(str);
	}
	file.Close();  //关闭文件


	filepath=_T("C:\\LIST"); //文件位置
	filepath=filepath+"\\"+USER;
	filename="FriendListlist.txt";  //文件名
	filename=filepath+"\\"+filename;  //文件路径

	if(!file.Open(filename,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))//建立文件
	{
		AfxMessageBox("未找到相关文件!");
		return ;
	}

	FriendList_list.RemoveAll();
	while(file.ReadString(str))
	{
		FriendList_list.Add(str);
	}
	file.Close();  //关闭文件
}

void CSMTPDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	if (m_listmail.GetSelectedCount() != 1)
	{
		AfxMessageBox("请明确要打开的email!");
		return;
	}

	if(!pop3.Noop())
	{
		AfxMessageBox("与服务器连接已断开,请尝试重新登陆");
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
	CString strLangName;    // 选择语言的名称字符串   
    NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;   
  
    if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
    {   
        // 获取被选择列表项第一个子项的文本   
        strLangName = m_listmail.GetItemText(pNMListView->iItem, 0);   
        // 将选择的语言显示与编辑框中   
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
		AfxMessageBox("请登陆");
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
		AfxMessageBox("请登陆");
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

              case CTLCOLOR_STATIC: //对所有静态文本控件的设置

              {

                     pDC->SetBkMode(TRANSPARENT);//设置背景为透明            

                     pDC->SetTextColor(RGB(0,0,0)); //设置字体颜色

                     return HBRUSH(GetStockObject(HOLLOW_BRUSH)); // 必须

              }

              case CTLCOLOR_DLG: //对所有静态文本控件的设置

              {

                     pDC->SetBkMode(TRANSPARENT); 

                     return hbr; //返回画刷句柄

              }

              default:

              return CDialog::OnCtlColor(pDC,pWnd, nCtlColor);

       }
	return hbr;
}


void CSMTPDlg::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


