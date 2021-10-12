#pragma once

// CClientSock 명령 대상입니다.

class CClientSock : public CAsyncSocket
{
public:
	CClientSock();
	virtual ~CClientSock();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


