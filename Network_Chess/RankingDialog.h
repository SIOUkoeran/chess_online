#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "mysql.h"
#include  "Network_ChessDlg.h"
// CRankingDialog ��ȭ �����Դϴ�.

class CRankingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRankingDialog)

public:
	CRankingDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRankingDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RANKING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	MYSQL m_mysql;

	virtual BOOL OnInitDialog();

	CListBox m_data_list;
	CString user;
};
