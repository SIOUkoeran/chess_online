#pragma once
#include "afxwin.h"


// CSignUp ��ȭ �����Դϴ�.

class CSignUp : public CDialogEx
{
	DECLARE_DYNAMIC(CSignUp)

public:
	CSignUp(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSignUp();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGN_UP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
//	CEdit m_strID;
//	CString m_strPwd;
//	CString m_strI;
//	CString m_strID;
	CString strID;
	CString strPwd;
};
