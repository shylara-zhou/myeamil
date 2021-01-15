; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCONTENT
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "smtp.h"
LastPage=0

ClassCount=6
Class1=CCONTENT
Class2=CSMTPApp
Class3=CAboutDlg
Class4=CSMTPDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDD_SMTP_DIALOG
Resource3=IDD_CONTENT_DIALOG
Class5=CBLACK
Resource4=IDD_DIALOG_BLACK
Class6=CWHITE
Resource5=IDD_DIALOG_WHITE

[CLS:CCONTENT]
Type=0
BaseClass=CDialog
HeaderFile=CCONTENT.h
ImplementationFile=CCONTENT.cpp
LastObject=CCONTENT
Filter=D
VirtualFilter=dWC

[CLS:CSMTPApp]
Type=0
BaseClass=CWinApp
HeaderFile=SMTP.h
ImplementationFile=SMTP.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=SMTPDlg.cpp
ImplementationFile=SMTPDlg.cpp
LastObject=CAboutDlg

[CLS:CSMTPDlg]
Type=0
BaseClass=CDialog
HeaderFile=SMTPDlg.h
ImplementationFile=SMTPDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CSMTPDlg

[DLG:IDD_CONTENT_DIALOG]
Type=1
Class=CCONTENT
ControlCount=1
Control1=IDC_EDIT_CONTENT,edit,1353779396

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SMTP_DIALOG]
Type=1
Class=CSMTPDlg
ControlCount=26
Control1=IDC_COMBO_SERVER,combobox,1344340227
Control2=IDC_ACCOUNT,edit,1350631554
Control3=IDC_PASSWORD,edit,1350631586
Control4=IDC_SEND_NAME,edit,1350631552
Control5=IDC_RCV_ADDRESS,edit,1350631552
Control6=IDC_SUBJECT,edit,1350631552
Control7=IDC_ADDFILE,button,1342242816
Control8=IDC_CONTENT,edit,1353781380
Control9=IDSEND,button,1342242816
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_COMBO_FILE,combobox,1344340227
Control17=IDC_DELETE,button,1342242816
Control18=IDC_CHECK1,button,1342242819
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,button,1342177287
Control21=IDC_GETEMAIL,button,1342242816
Control22=IDC_STATIC,button,1342177287
Control23=IDC_LOGIN,button,1342242816
Control24=IDC_LIST,SysListView32,1350633473
Control25=IDC_BLACK,button,1342242816
Control26=IDC_WHITE,button,1342242816

[DLG:IDD_DIALOG_BLACK]
Type=1
Class=CBLACK
ControlCount=4
Control1=IDC_LIST_BLACK,listbox,1353777411
Control2=IDC_BUTTON_ADD,button,1342242816
Control3=IDC_BUTTON_DELETE,button,1342242816
Control4=IDC_EDIT1,edit,1350631552

[CLS:CBLACK]
Type=0
HeaderFile=BLACK.h
ImplementationFile=BLACK.cpp
BaseClass=CDialog
Filter=D
LastObject=CBLACK
VirtualFilter=dWC

[DLG:IDD_DIALOG_WHITE]
Type=1
Class=CWHITE
ControlCount=4
Control1=IDC_LIST_WHITE,listbox,1353777411
Control2=IDC_BUTTON_ADD,button,1342242816
Control3=IDC_BUTTON_DELETE2,button,1342242816
Control4=IDC_EDIT1,edit,1350631552

[CLS:CWHITE]
Type=0
HeaderFile=WHITE.h
ImplementationFile=WHITE.cpp
BaseClass=CDialog
Filter=D
LastObject=CWHITE
VirtualFilter=dWC

