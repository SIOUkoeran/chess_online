
// Network_ChessDlg.h : ��� ����
//

/**
 * @author : ���¿� (xodnjs6854@gmail.com)  ���Ǵ� ���Ϸ� ���ּ���.
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


// CChessDlg ��ȭ ����
class CChessDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CChessDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NETWORK_CHESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	////////////////////////////////// ���� ���� //////////////////////////////////
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
	/////////////////////////////////////// ��Ʈ�� ���� �� DC ////////////////////////////////////////////
	CDC *pDC;  //������DC (�� �׷��� dmemDC�� pDC�� �ҷ��ͼ� ���) - �׸��� �����̴� ���� ����(������۸�)
	CDC memDC;  //memDC - �ϳ��ϳ��� �׷��� DC
	CDC dmemDC;   //memDC�� �ҷ��� Bitmap�� ��� �׸� DC
    CBitmap backGround;  //�� ��� (�� ��濡 �׸��� ��� �׸��� �ѹ��� ����� ������۸� ������ �����Ѵ�.
	CBitmap bBB, bBK, bBKN, bBP, bBQ, bBR;  // ������� ����
	CBitmap bWR, bWK, bWQ, bWP, bWB, bWKN;  // ������� ����
	CBitmap bitmapMAP; // ü����
	CBitmap RMask, KMask, QMask, KNMask, PMask, BMask;  // ���� ���� ����ũ (��� ����ȭ�� ���� ����ũ)
	CBitmap moveArea, moveArea_Mask; // �̵� ���� ���� ǥ��
	CBitmap dangerArea; // ���� �ִ� ��ġ ǥ��
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	bool isMove; //�̵� ���� ���� �Ǵ�
	bool turn;   //���� �������� �Ǵ�	
	bool rbutton_possible;   //������ ���콺 ��ư Ŭ�� ���� ���� �Ǵ�
	
	bool castling_Possible_W, castling_Possible_w; // ĳ���� ���� ���� �Ǵ� (��� ���� ������ �� false)
	bool castling_Possible_B, castling_Possible_b; // ĳ���� ���� ���� �Ǵ� (��� ���� ������ �� false)
	bool castling_Possible_BK , castling_Possible_WK; // ĳ���� ���� ���� �Ǵ� (ŷ�� ������ �� false)

	bool inPassing_Possible_BPR,inPassing_Possible_BPL; // inPassing�� ���� ���� �Ǵ�  (BPR�� ������, BPL�� ����)
	bool inPassing_Possible_WPR,inPassing_Possible_WPL; // inPassing�� ���� ���� �Ǵ�  (WPR�� ������, WPL�� ����)

	bool check_unit[8][8]; // ������ �̵��� �� �ִ� ��ġ�� bool���� true�� ���� (üũ Ȯ���� ���� ����)
	
	int board[8][8];     // �������� ������ �����ϱ� ���� int�� �迭
	int (*ptr_board)[8];  // ������ board �迭 ������ �����ϱ� ���� ������
	bool move_point[8][8]; //�̵������� ������ �����ϴ� bool �迭

	int SetPointX;  // �̵��� �� �ʱ� ��ġ�� X��ǥ�� ����
	int SetPointY;  // �̵��� �� �ʱ� ��ġ�� Y��ǥ�� ����
	int SetA; // inPassing �˰��򿡼� ó�� ��ġ�� �����ϱ� ���� int�� ����
	int SetB; // inPassing �˰��򿡼� �̵��� ��ġ�� �����ϱ� ���� int�� ����

	CString str_location; // ��ǥ���� �����ϱ� ���� CString�� ����
	CString turn_throw;  // ���� ���濡�� �����ϱ� ���� CString�� ����

	CStatic m_bScore;    // ���ھ ��ȭ ���ڿ� ��Ÿ����
	CStatic m_wScore;    // ���ھ ��ȭ ���ڿ� ��Ÿ����

	int wScore,bScore;   // ���ھ �����ϴ� int�� ����

	CString wScore_str,bScore_str; //����� ���ھ ��ȭ ���ڿ� ��Ÿ���� ���� CString�� ����

	int set_Bking_x, set_Bking_y; //��ŷ�� ��ġ������ ���� int�� ����
	int set_Wking_x, set_Wking_y; //ȭ��Ʈŷ�� ��ġ������ ���� int�� ����

	bool chating; //ä�� ���� ���� �Ǵ�

	CListBox m_listChat;  //ä�� ����Ʈ �ڽ�
	
	////////////////////��ĳ���� ��ư ����////////////////////
	CButton m_Blcbtn;
	CButton m_Bscbtn;
	CButton m_Wlcbtn;
	CButton m_Wscbtn;
	//////////////////////////////////////////////////////////

	CEdit m_timer; //Ÿ�̸Ӹ� ����� CEdit ����
	int ss; //�ʸ� �������� int�� ����
	CString yww,ybw; //������ �̰�ٴ� �޼����� �����ֱ� ���� ����
	BOOL m_bRun;

	CString m_EditCtrl_Val; //ä���Է��� 20�� ���� ���ѽ�Ű�� ����
	int pen_x,pen_y; // �̵��� ���� �ʱ���ġ�� ����
	int penMove_x,penMove_y; // �̵��� ���� ������ġ�� ���� (pen_x,pen_y������ ����� ������ �׾��ֱ� ����)	

	void Rule(int &setX, int &setY);  // ���� ���� �� ��Ģ�� ����Ǵ� �Լ�
	void Swap(int &x,int &y, int &Move_x, int &Move_y); //���� ��ġ�� ��ȯ �����ִ� �Լ� (inPassing �˰��� ����)
	void Move(int &x,int &y);  // ������ �̵� �˰����� ���ǵ� �Լ�
	void Reset_mPoint(); // move_point�迭�� �ʱ�ȭ
	void UnitDraw();    // �� ��� ���� ü���ǰ� ü������ �ѹ��� �׷��ִ� �Լ�
	void Promotion();    // ���θ�� �˰����� ���ǵ� �Լ� 
	void Set_Game();      // ������ �������ִ� �Լ�
	void BlackCheck();  // ������� �̵� ������ Ȯ���ϴ� �Լ�
	void WhiteCheck();   // ������� �̵� ������ Ȯ���ϴ� �Լ�
	void Check();        // üũ ���°� �Ǹ� �޼����� �����ϴ� �Լ�
	void Reset_Check();   // check_unit�迭�� �ʱ�ȭ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy(); //������ Ÿ�̸ӵ� ���� ��Ű�� ����
	void Timer(); //Ÿ�̸Ӹ� ���۽�Ű�� ���������ִ� ��
	void delay(clock_t n); //������ �Լ� (�񵿱�� �����̹Ƿ� ������ �����͸� ���� �� �� �����Ͱ� ��ġ�� ������ ���� ����)
	///////////////////////////////////// ��ĳ���� ��ư //////////////////////////
	afx_msg void OnBnClickedButtonWlc();
	afx_msg void OnBnClickedButtonWsc();
	afx_msg void OnBnClickedButtonBlc();
	afx_msg void OnBnClickedButtonBsc();
	//////////////////////////////////////////////////////////////////////////////
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // ���� ���콺 ��ư Ŭ�� (�̵����)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point); // ������ ���콺 ��ư Ŭ�� (�̵���� �ʱ�ȭ)
	virtual void OnOK(); // ����Ű�� â�� ������ ����
	afx_msg void OnBnClickedResetbutton(); // �������� �������ִ� ��ư
public:
	afx_msg void OnBnClickedButton2();
	// primarykey �г���
	CString m_strNickname;
};
