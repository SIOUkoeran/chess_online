
// Network_Chess.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "ServerSock.h"
#include "ClientSock.h"



// CChessApp:
// 이 클래스의 구현에 대해서는 Network_Chess.cpp을 참조하십시오.
//

class CChessApp : public CWinApp
{
public:
	CChessApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
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