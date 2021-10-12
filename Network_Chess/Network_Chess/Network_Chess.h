
// Network_Chess.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "ServerSock.h"
#include "ClientSock.h"



// CChessApp:
// �� Ŭ������ ������ ���ؼ��� Network_Chess.cpp�� �����Ͻʽÿ�.
//

class CChessApp : public CWinApp
{
public:
	CChessApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
public:
	CServerSock* m_pServer;
	CClientSock* m_pClient;
	void InitServer();
	void Accept();
	void CleanUp();
	void Connect(CString strIP);
	void ReceiveData();
	void SendData(CString strData);
	void CloseChild();


	DECLARE_MESSAGE_MAP()
};

extern CChessApp theApp;