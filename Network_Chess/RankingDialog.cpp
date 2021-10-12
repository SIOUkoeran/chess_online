// RankingDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Network_Chess.h"
#include "RankingDialog.h"
#include "afxdialogex.h"
#include "Network_ChessDlg.h"



// CRankingDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRankingDialog, CDialogEx)

CRankingDialog::CRankingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RANKING, pParent)
	
	
{

	user = _T("");
}

CRankingDialog::~CRankingDialog()
{
}

void CRankingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	//  DDX_Control(pDX, IDC_LIST3, m_chat_list);

	//  DDX_Control(pDX, IDC_LIST4, m_ranklist);

	DDX_Control(pDX, IDC_LIST5, m_data_list);
}


BEGIN_MESSAGE_MAP(CRankingDialog, CDialogEx)
	
END_MESSAGE_MAP()


// CRankingDialog 메시지 처리기입니다.





BOOL CRankingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	mysql_init(&m_mysql);
	mysql_set_character_set(&m_mysql, "euckr");
	MYSQL *conn = mysql_real_connect(&m_mysql, "localhost", "user1", "pass01",
		"db1", 3306, (char *)NULL, 0);
	if (conn == NULL) {
		AfxMessageBox((LPCTSTR)mysql_error(&m_mysql));
		::PostQuitMessage(0);
		return FALSE;

	}


	USES_CONVERSION;
	CString query;
	query.Format(_T("select * from member order by wins DESC limit 5"));
	
	int status = mysql_query(&m_mysql,CW2A(query));
	MYSQL_RES *result = mysql_store_result(&m_mysql);
	if (result) {
		int nFieldCount = mysql_num_fields(result);
		MYSQL_FIELD *fields = mysql_fetch_fields(result);
		MYSQL_ROW row;
		int no = 0;
		
		while ((row = mysql_fetch_row(result))) {
			CString username= CA2W(row[0]);
			int score = atoi(row[2]);
			
			CString strInsert;
			strInsert.Format(_T("NO: %d ID : %s  WINS : %d"),no, username, score);
			int sel = m_data_list.InsertString(-1, strInsert);
			m_data_list.SetCurSel(sel);
			no += 1;
			
			}
			
		}
	CString query_find;
	query_find.Format(_T("select * from member order by wins DESC"));

	int status1 = mysql_query(&m_mysql, CW2A(query_find));
	MYSQL_RES * result1 = mysql_store_result(&m_mysql);
	if (result1) {
		int nFieldCount1 = mysql_num_fields(result1);
		MYSQL_FIELD *fields1 = mysql_fetch_fields(result1);
		MYSQL_ROW row1;
		int no1 = 0;

		while ((row1 = mysql_fetch_row(result1))) {
			CString username = CA2W(row1[0]);
			int score = atoi(row1[2]);

			
			//strInsert.Format(_T("NO: %d ID : %s  WINS : %d"), no, username, score);
			if (user == username) {
				CString strInsert1;
				strInsert1.Format(_T("%s님의 순위는 %d입니다."), username,no1+1);
				int sel1 = m_data_list.InsertString(-1, strInsert1);
				m_data_list.SetCurSel(sel1);
			}
			no1 += 1;
		}
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
