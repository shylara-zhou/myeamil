// Mail.cpp : implementation file
//

#include "stdafx.h"
#include "smtp.h"
#include "Mail.h"
#include "SMTPDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Mail dialog


Mail::Mail(CWnd* pParent /*=NULL*/)
	: CDialog(Mail::IDD, pParent)
{
	//{{AFX_DATA_INIT(Mail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Mail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Mail)
	DDX_Control(pDX, IDC_EDIT_CONTENT, m_content);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Mail, CDialog)
	//{{AFX_MSG_MAP(Mail)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Mail message handlers

BOOL Mail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDIT_CONTENT,strMail);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Mail::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
  
          CRect   rect;   
          GetClientRect(&rect);   
          CDC   dcMem;   
          dcMem.CreateCompatibleDC(&dc);   
          CBitmap   bmpBackground;   
          bmpBackground.LoadBitmap(IDB_BITMAP1);   
                  //IDB_BITMAP是你自己的图对应的ID   
          BITMAP   bitmap;   
          bmpBackground.GetBitmap(&bitmap);   
          CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
          dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);   
}
