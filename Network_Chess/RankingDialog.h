#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "mysql.h"
#include  "Network_ChessDlg.h"
// CRankingDialog 대화 상자입니다.

class CRankingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRankingDialog)

public:
	CRankingDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRankingDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RANKING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	MYSQL m_mysql;

	virtual BOOL OnInitDialog();

	CListBox m_data_list;
	CString user;
};
