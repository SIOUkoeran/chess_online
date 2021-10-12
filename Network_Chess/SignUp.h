#pragma once
#include "afxwin.h"


// CSignUp 대화 상자입니다.

class CSignUp : public CDialogEx
{
	DECLARE_DYNAMIC(CSignUp)

public:
	CSignUp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSignUp();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGN_UP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	CEdit m_strID;
//	CString m_strPwd;
//	CString m_strI;
//	CString m_strID;
	CString strID;
	CString strPwd;
};
