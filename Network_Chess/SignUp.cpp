// SignUp.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Network_Chess.h"
#include "SignUp.h"
#include "afxdialogex.h"


// CSignUp ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSignUp, CDialogEx)

CSignUp::CSignUp(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SIGN_UP, pParent)
	
	, strID(_T(""))
	, strPwd(_T(""))
{

}

CSignUp::~CSignUp()
{
}

void CSignUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT1, m_strID);
	//  DDX_Control(pDX, IDC_EDIT2, m_strPwd);
	//  DDX_Control(pDX, IDC_EDIT1, m_strI);
	//  DDX_Control(pDX, IDC_EDIT1, m_strID);
	DDX_Text(pDX, IDC_EDIT1, strID);
	DDX_Text(pDX, IDC_EDIT2, strPwd);
}


BEGIN_MESSAGE_MAP(CSignUp, CDialogEx)
END_MESSAGE_MAP()


// CSignUp �޽��� ó�����Դϴ�.
