/*
Paul Tang

CPSC 351
November 23, 2019

Assignment#4
*/


// ChatDlg.h: 
//

#pragma once
#define WM_RECVDATA WM_USER+1


struct RECVPARAM
{
	SOCKET sock;
	HWND hwnd;
};


class CChatDlg : public CDialogEx
{

public:
	CChatDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	



protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool InitSocket(void);
	DWORD static WINAPI RecvProc(LPVOID lpParameter);
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);
	
private:
	SOCKET m_socket;
public:
	
	afx_msg void OnBnClickedBtnSend();
	
};
