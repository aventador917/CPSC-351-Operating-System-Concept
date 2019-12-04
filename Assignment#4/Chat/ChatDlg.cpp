/*
Paul Tang

CPSC 351
November 23, 2019

Assignment#4
*/


//

#include "stdafx.h"
#include "Chat.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include<string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   


protected:

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()




CChatDlg::CChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_RECVDATA, OnRecvData)
	ON_BN_CLICKED(IDC_BTN_SEND, &CChatDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()




DWORD WINAPI CChatDlg::RecvProc(LPVOID lpParameter)
{
	
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
	HWND  hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	
	SOCKADDR_IN addrFrom;
	int len = sizeof(SOCKADDR);

	CHAR recvBuf[200];
	char tempBuf[300];
	int retval;

	while (true)
	{
		retval = recvfrom(sock, recvBuf, 200, 0, (SOCKADDR*)&addrFrom, &len);
		if (SOCKET_ERROR == retval)
		{
			break;
		}
		sprintf(tempBuf, "%s say： %s", inet_ntoa(addrFrom.sin_addr), recvBuf);
		::PostMessage(hwnd, WM_RECVDATA, 0, (LPARAM)tempBuf);
	}

	return 0;
}



 LRESULT CChatDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
	CString str = CString((LPCSTR)lParam);//(_TCHAR*)lParam;
	CString strTemp;

	GetDlgItemText(IDC_EDIT_RECV, strTemp);
	str += "\r\n";
	str += strTemp;


	SetDlgItemText(IDC_EDIT_RECV, str);

	return 0;
}


BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		
		CString strAboutMenu;
	    strAboutMenu.LoadString(IDS_ABOUTBOX);
		
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	if (!InitSocket())
	{
		return false;
	}
	
	RECVPARAM *pRecvparam = new RECVPARAM;
	pRecvparam->sock = this->m_socket;
	pRecvparam->hwnd = m_hWnd;

	HANDLE hThread = CreateThread(NULL, 0, RecvProc, (LPVOID)pRecvparam, 0, NULL);
	CloseHandle(hThread);

	return TRUE;  
}

void CChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}



void CChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CChatDlg::InitSocket(void)
{
	this->m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
	if (this->m_socket== SOCKET_ERROR)
	{
		AfxMessageBox(_T("create failure"));
		return false;
	}
	SOCKADDR_IN addrSock;
	addrSock.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrSock.sin_family = AF_INET;
	addrSock.sin_port = htons(3514);
	int retval;
	retval = bind(m_socket, (SOCKADDR*)&addrSock, sizeof(SOCKADDR));  
	if (SOCKET_ERROR == retval)
	{
		closesocket(m_socket);
		MessageBox(_T("bind failure"));
		return false;
	}
	return true;
}




void CChatDlg::OnBnClickedBtnSend()
{
	
	SOCKADDR_IN addrTo;
	addrTo.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(3514);

	CString msg;
	GetDlgItemText(IDC_EDIT_SENDMESSAGE, msg);
	std::string strMsg(msg.GetBuffer());

	sendto(m_socket, strMsg.c_str(), msg.GetLength()+1, 0, (SOCKADDR*)&addrTo, sizeof(SOCKADDR));
	SetDlgItemText(IDC_EDIT_SENDMESSAGE, _T(""));
	
}


