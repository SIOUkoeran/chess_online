
// Network_ChessDlg.h : 헤더 파일
//

/**
 * @author : 양태원 (xodnjs6854@gmail.com)  문의는 메일로 해주세요.
*/
#pragma once
#include "afxwin.h"
#define XY_SIZE 83
#define BOARD_SIZE 90
#define MOVEMASK_SIZE 97
#define BOARD_XSIZE 660
#define BOARD_YSIZE 750
#define BOARD_YSIZE_TOP 100


#define MY_TIMER 100


// CChessDlg 대화 상자
class CChessDlg : public CDialogEx
{
// 생성입니다.
public:
	CChessDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NETWORK_CHESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	////////////////////////////////// 소켓 전송 //////////////////////////////////
	CIPAddressCtrl m_IPAddress;
	int m_nChatMode;
	SOCKET mh_listen_socket;
	CString m_strMyIP;
	CString m_strOtherIP;
	afx_msg void OnClickedRadioServer();
	afx_msg void OnClickedRadioClient();
	afx_msg void OnClickedButtonConnect();
	afx_msg void OnClickedButtonSend();
	void ReceiveData(CString strReceive);
	void Accept(CString strSock);
	//////////////////////////////////////////////////////////////////////////////

private:
	/////////////////////////////////////// 비트맵 변수 및 DC ////////////////////////////////////////////
	CDC *pDC;  //포인터DC (다 그려진 dmemDC를 pDC로 불러와서 출력) - 그림이 깜빡이는 현상 제거(더블버퍼링)
	CDC memDC;  //memDC - 하나하나를 그려줄 DC
	CDC dmemDC;   //memDC로 불러온 Bitmap을 모두 그릴 DC
    CBitmap backGround;  //흰 배경 (이 배경에 그림을 모두 그리고 한번해 출력해 더블버퍼링 현상을 제거한다.
	CBitmap bBB, bBK, bBKN, bBP, bBQ, bBR;  // 흑색팀의 말들
	CBitmap bWR, bWK, bWQ, bWP, bWB, bWKN;  // 백색팀의 말들
	CBitmap bitmapMAP; // 체스판
	CBitmap RMask, KMask, QMask, KNMask, PMask, BMask;  // 말에 대한 마스크 (배경 투명화를 위한 마스크)
	CBitmap moveArea, moveArea_Mask; // 이동 범위 지역 표시
	CBitmap dangerArea; // 적이 있는 위치 표시
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	bool isMove; //이동 가능 여부 판단
	bool turn;   //누구 차례인지 판단	
	bool rbutton_possible;   //오른쪽 마우스 버튼 클릭 가능 여부 판단
	
	bool castling_Possible_W, castling_Possible_w; // 캐슬링 가능 여부 판단 (백색 룩이 움직일 때 false)
	bool castling_Possible_B, castling_Possible_b; // 캐슬링 가능 여부 판단 (흑색 룩이 움직일 때 false)
	bool castling_Possible_BK , castling_Possible_WK; // 캐슬링 가능 여부 판단 (킹이 움직일 때 false)

	bool inPassing_Possible_BPR,inPassing_Possible_BPL; // inPassing이 가능 여부 판단  (BPR은 오른쪽, BPL은 왼쪽)
	bool inPassing_Possible_WPR,inPassing_Possible_WPL; // inPassing이 가능 여부 판단  (WPR은 오른쪽, WPL은 왼쪽)

	bool check_unit[8][8]; // 유닛이 이동할 수 있는 위치를 bool값에 true로 저장 (체크 확인을 위한 변수)
	
	int board[8][8];     // 보드판의 내용을 정의하기 위한 int형 배열
	int (*ptr_board)[8];  // 보드판 board 배열 변수에 접근하기 위한 포인터
	bool move_point[8][8]; //이동가능한 범위를 저장하는 bool 배열

	int SetPointX;  // 이동할 때 초기 위치의 X좌표를 저장
	int SetPointY;  // 이동할 때 초기 위치의 Y좌표를 저장
	int SetA; // inPassing 알고리즘에서 처음 위치를 저장하기 위한 int형 변수
	int SetB; // inPassing 알고리즘에서 이동할 위치를 저장하기 위한 int형 변수

	CString str_location; // 좌표값을 전송하기 위한 CString형 변수
	CString turn_throw;  // 턴을 상대방에게 전송하기 위한 CString형 변수

	CStatic m_bScore;    // 스코어를 대화 상자에 나타내줌
	CStatic m_wScore;    // 스코어를 대화 상자에 나타내줌

	int wScore,bScore;   // 스코어를 저장하는 int형 변수

	CString wScore_str,bScore_str; //저장된 스코어를 대화 상자에 나타내기 위한 CString형 변수

	int set_Bking_x, set_Bking_y; //블랙킹의 위치정보를 담은 int형 변수
	int set_Wking_x, set_Wking_y; //화이트킹의 위치정보를 담은 int형 변수

	bool chating; //채팅 가능 여부 판단

	CListBox m_listChat;  //채팅 리스트 박스
	
	////////////////////룩캐슬링 버튼 선언////////////////////
	CButton m_Blcbtn;
	CButton m_Bscbtn;
	CButton m_Wlcbtn;
	CButton m_Wscbtn;
	//////////////////////////////////////////////////////////

	CEdit m_timer; //타이머를 출력할 CEdit 선언
	int ss; //초를 세기위한 int형 변수
	CString yww,ybw; //상대방이 이겼다는 메세지를 보내주기 위한 변수
	BOOL m_bRun;

	CString m_EditCtrl_Val; //채팅입력을 20자 내로 제한시키는 변수
	int pen_x,pen_y; // 이동한 말의 초기위치를 저장
	int penMove_x,penMove_y; // 이동한 말의 나중위치를 저장 (pen_x,pen_y변수에 저장된 값부터 그어주기 위함)	

	void Rule(int &setX, int &setY);  // 게임 순서 및 규칙이 적용되는 함수
	void Swap(int &x,int &y, int &Move_x, int &Move_y); //말의 위치를 변환 시켜주는 함수 (inPassing 알고리즘도 적용)
	void Move(int &x,int &y);  // 말들의 이동 알고리즘이 정의된 함수
	void Reset_mPoint(); // move_point배열을 초기화
	void UnitDraw();    // 흰 배경 위에 체스판과 체스말을 한번에 그려주는 함수
	void Promotion();    // 프로모션 알고리즘이 정의된 함수 
	void Set_Game();      // 게임을 셋팅해주는 함수
	void BlackCheck();  // 흑색말의 이동 범위를 확인하는 함수
	void WhiteCheck();   // 백색말의 이동 범위를 확인하는 함수
	void Check();        // 체크 상태가 되면 메세지를 전송하는 함수
	void Reset_Check();   // check_unit배열을 초기화
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy(); //꺼질때 타이머도 정지 시키기 위함
	void Timer(); //타이머를 동작시키고 정지시켜주는 함
	void delay(clock_t n); //딜레이 함수 (비동기식 전송이므로 없으면 데이터를 전송 할 때 데이터가 겹치는 현상을 막기 위함)
	///////////////////////////////////// 룩캐슬링 버튼 //////////////////////////
	afx_msg void OnBnClickedButtonWlc();
	afx_msg void OnBnClickedButtonWsc();
	afx_msg void OnBnClickedButtonBlc();
	afx_msg void OnBnClickedButtonBsc();
	//////////////////////////////////////////////////////////////////////////////
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // 왼쪽 마우스 버튼 클릭 (이동모드)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point); // 오른쪽 마우스 버튼 클릭 (이동모드 초기화)
	virtual void OnOK(); // 엔터키로 창이 꺼짐을 막음
	afx_msg void OnBnClickedResetbutton(); // 새게임을 시작해주는 버튼
public:
	afx_msg void OnBnClickedButton2();
	// primarykey 닉네임
	CString m_strNickname;
};
