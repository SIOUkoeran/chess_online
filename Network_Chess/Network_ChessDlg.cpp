
// Network_ChessDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Network_Chess.h"
#include "Network_ChessDlg.h"
#include "afxdialogex.h"
#include "RankingDialog.h"
#include "time.h"
#include "SignUp.h"
#include <Windows.h>
#include <atlbase.h>
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CChessDlg ��ȭ ����




CChessDlg::CChessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChessDlg::IDD, pParent)
	, m_strMyIP(_T(""))
	, m_strOtherIP(_T(""))
	, m_EditCtrl_Val(_T(""))
	, m_strNickname(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nChatMode = 0;
}

void CChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_IPAddress);
	DDX_Radio(pDX, IDC_RADIO_SERVER, m_nChatMode);
	DDX_Control(pDX, IDC_bScore, m_bScore);
	DDX_Control(pDX, IDC_wScore, m_wScore);
	DDX_Control(pDX, IDC_LIST_CHAT, m_listChat);
	DDX_Control(pDX, IDC_BUTTON_BLC, m_Blcbtn);
	DDX_Control(pDX, IDC_BUTTON_BSC, m_Bscbtn);
	DDX_Control(pDX, IDC_BUTTON_WLC, m_Wlcbtn);
	DDX_Control(pDX, IDC_BUTTON_WSC, m_Wscbtn);
	DDX_Control(pDX, IDC_EDIT_TIMER, m_timer);
	DDX_Text(pDX, IDC_SEND, m_EditCtrl_Val);
	DDV_MaxChars(pDX, m_EditCtrl_Val, 20);

	DDX_Text(pDX, IDC_EDIT1, m_strNickname);
	DDX_Control(pDX, IDC_BUTTON2, m_RankingDialog);
}

BEGIN_MESSAGE_MAP(CChessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_SERVER, &CChessDlg::OnClickedRadioServer)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, &CChessDlg::OnClickedRadioClient)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CChessDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChessDlg::OnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_WLC, &CChessDlg::OnBnClickedButtonWlc)
	ON_BN_CLICKED(IDC_BUTTON_WSC, &CChessDlg::OnBnClickedButtonWsc)
	ON_BN_CLICKED(IDC_BUTTON_BLC, &CChessDlg::OnBnClickedButtonBlc)
	ON_BN_CLICKED(IDC_BUTTON_BSC, &CChessDlg::OnBnClickedButtonBsc)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_RESETBUTTON, &CChessDlg::OnBnClickedResetbutton)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//ON_EN_CHANGE(IDC_EDIT_TIMER, &CChessDlg::OnEnChangeEditTimer)



	ON_BN_CLICKED(IDC_BUTTON2, &CChessDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CChessDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CChessDlg::OnBnClickedButton1)
END_MESSAGE_MAP()




// CChessDlg �޽��� ó����

BOOL CChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	mysql_init(&m_mysql);
	mysql_set_character_set(&m_mysql, "euckr");
	MYSQL *conn = mysql_real_connect(&m_mysql, "localhost", "user1", "pass01",
		"db1", 3306, (char *)NULL, 0);
	if (conn == NULL) {
		AfxMessageBox((LPCTSTR)mysql_error(&m_mysql));
		::PostQuitMessage(0);
		return FALSE;
		
	}
	
	

	

	Set_Game(); //�� �ʱ� ��ġ ����

	isMove= false;
	rbutton_possible=false;

	chating=false;

	inPassing_Possible_BPR=false;
	inPassing_Possible_BPL=false;
	inPassing_Possible_WPR=false;
	inPassing_Possible_WPL=false;

	SetPointX=0;
	SetPointY=0;

	SetA=0;
	SetB=0;

	set_Bking_x=0;
	set_Bking_y=0;

	set_Wking_x=0;
	set_Wking_y=0;

	turn = true; //true�� �� ��� ���� false�� �� ��� ����

	bitmapMAP.LoadBitmap(IDB_BITMAP1); // ü����
	bWB.LoadBitmap(IDB_BITMAP2); //ȭ��Ʈ���
	bWK.LoadBitmap(IDB_BITMAP3); //ȭ��Ʈŷ
	bWKN.LoadBitmap(IDB_BITMAP4); //ȭ��Ʈ����Ʈ
	bWP.LoadBitmap(IDB_BITMAP5); //ȭ��Ʈ��
	bWQ.LoadBitmap(IDB_BITMAP6); //ȭ��Ʈ��
	bWR.LoadBitmap(IDB_BITMAP7); //ȭ��Ʈ��

	bBB.LoadBitmap(IDB_BITMAP8); //�����
	bBK.LoadBitmap(IDB_BITMAP9); //��ŷ
	bBKN.LoadBitmap(IDB_BITMAP10); //������Ʈ
	bBP.LoadBitmap(IDB_BITMAP11); //����
	bBQ.LoadBitmap(IDB_BITMAP12); //����
	bBR.LoadBitmap(IDB_BITMAP13); //����
	
	BMask.LoadBitmap(IDB_BITMAP14); //��󸶽�ũ
	KMask.LoadBitmap(IDB_BITMAP15); //ŷ����ũ
	KNMask.LoadBitmap(IDB_BITMAP16); //����Ʈ����ũ
	PMask.LoadBitmap(IDB_BITMAP17); //������ũ
	QMask.LoadBitmap(IDB_BITMAP18); //������ũ
	RMask.LoadBitmap(IDB_BITMAP19); //�踶��ũ

	moveArea.LoadBitmap(IDB_BITMAP20); //�̵�����
	moveArea_Mask.LoadBitmap(IDB_BITMAP21); //�̵����� ����ũ
	dangerArea.LoadBitmap(IDB_BITMAP22); //�� ������ �ִ� ���� ǥ��
	backGround.LoadBitmap(IDB_BITMAP23); // ��� ���� �׸� �� ���
	Reset_mPoint(); // �̵����� �ʱ�ȭ
	Reset_Check(); // üũ���� �ʱ�ȭ
	ptr_board=board; // �����ͷ� board������ ����

	wScore=0;
	bScore=0;
	wScore_str.Format(_T("%d"),wScore);
	bScore_str.Format(_T("%d"),bScore);
	m_wScore.SetWindowTextW(wScore_str);
	m_bScore.SetWindowTextW(bScore_str);

	char name[255];
	PHOSTENT hostinfo;
	if(gethostname(name, sizeof(name))==0){
		if((hostinfo = gethostbyname(name)) != NULL)
		{
			m_strMyIP = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
		}
	}
	m_IPAddress.SetWindowText(m_strMyIP);
	m_IPAddress.EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT,_T("Open"));

	pen_x=10;
	pen_y=10;
	penMove_x=10;
	penMove_y=10;

	ss=61;
	yww="yww";
	ybw="ybw";
	m_bRun = FALSE;

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CChessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	UnitDraw();
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChessDlg::OnClickedRadioServer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_IPAddress.SetWindowTextW(m_strMyIP);
	m_IPAddress.EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT,_T("Open"));
	isMove = false;
	turn = false;
	rbutton_possible=false;
}

void CChessDlg::OnClickedRadioClient()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_IPAddress.SetWindowTextW(m_strMyIP);
	m_IPAddress.EnableWindow(TRUE);
	SetDlgItemText(IDC_BUTTON_CONNECT,_T("Connect"));
	isMove = false;
	turn = false;
}

void CChessDlg::OnClickedButtonConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	if(!m_nChatMode){
		((CChessApp*)AfxGetApp())->InitServer();
		GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	} 
	else
	{
		CString strIP;
		GetDlgItemText(IDC_IPADDRESS_SERVER,strIP);
		if(strIP!=_T("0.0.0.0")){
			GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
			((CChessApp*)AfxGetApp())->Connect(strIP);
			m_strOtherIP=strIP;

			delay(500);
			CString accept("accept");
			theApp.SendData(accept);

			CString strInsert;
			strInsert.Format(_T("**************GAME START**************"));
			int sel=m_listChat.InsertString(-1,strInsert);
			m_listChat.SetCurSel(sel);
		}
		else
		{
			AfxMessageBox(_T("������ ������ IP�ּҸ� �Է��ϼ���"));
		}
	}
}

void CChessDlg::OnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strSend, strInsert, strChat;

	strChat="*chat*";
	theApp.SendData(strChat);
	delay(300);

	GetDlgItemText(IDC_SEND,strSend);
	strInsert.Format(_T("[��]:%s"), strSend);
	theApp.SendData(strSend);
	int sel=m_listChat.InsertString(-1,strInsert);
	m_listChat.SetCurSel(sel);
	SetDlgItemText(IDC_SEND,_T(""));
}

void CChessDlg::ReceiveData(CString strReceive)
{
	CString x,y,m_x,m_y,m_unit1,m_unit2;
	int net_x,net_y,netMove_x,netMove_y,netMove_unit1,netMove_unit2;

	x="";
	y="";
	m_x="";
	m_y="";
	m_unit1="";
	m_unit2="";
	net_x=0;
	net_y=0;
	netMove_x=0;
	netMove_y=0;
	netMove_unit1=0;
	netMove_unit2=0;

	if(strReceive=="*chat*"){
		chating = true;
		delay(5);
	}else if(chating){   //ä�� �޼����� �� ���
		CString strInsert;
		//strInsert.Format(_T("[%s]:%s"),nickName, strReceive);
		strInsert.Format(_T("[����]:%s"), strReceive);
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);
		chating=false;
	}
	else if(strReceive=="W"){  //White���� ���� �ѱ�
		delay(5);
		UnitDraw();
		isMove=true;
		turn=true;
		rbutton_possible=true;
		ss=61;
		Timer();
	}
	else if(strReceive=="B"){ //Black���� ���� �ѱ�
		delay(5);
		UnitDraw();
		isMove=true;
		turn=false;
		rbutton_possible=true;
		ss=61;
		Timer();
	}
	else if(strReceive=="ipwL"){
		inPassing_Possible_WPL=true;
	}
	else if(strReceive=="ipwR"){
		inPassing_Possible_WPR=true;
	}
	else if(strReceive=="ipbL"){
		inPassing_Possible_BPL=true;
	}
	else if(strReceive=="ipbR"){
		inPassing_Possible_BPR=true;
	}
	else if(strReceive=="white"){ //üũ
		delay(5);
		CString strInsert;
		strInsert.Format(L"********Check! White King�� �����մϴ�.********");
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);
	}
	else if(strReceive=="black"){
		delay(5);
		CString strInsert;
		strInsert.Format(L"********Check! Black King�� �����մϴ�.********");
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);
	}
	else if(strReceive=="ww"){  //��� �¸�
		delay(5);
		GetDlgItem(IDC_RESETBUTTON)->EnableWindow(true);
		isMove=false;
		rbutton_possible=false;
		wScore++;
		//wScore NickName �� ������ �߰�!
		wScore_str.Format(_T("%d"),wScore);
		m_wScore.SetWindowTextW(wScore_str);
			
		MessageBox(L"You lose.");
		Timer();
	}
	else if(strReceive=="bw"){  //��� �¸�
		delay(5);
		GetDlgItem(IDC_RESETBUTTON)->EnableWindow(true);
		isMove=false;
		rbutton_possible=false;
		bScore++;
		bScore_str.Format(_T("%d"),bScore);
		m_bScore.SetWindowTextW(bScore_str);

		MessageBox(L"You lose.");
		Timer();
	}
	else if(strReceive=="yww"){  //��� ��ǽ�
		
		CString query_win;
		query_win.Format(_T("update member set wins = wins+1 where id = '%s'"), externUsername);

		int status_win = mysql_query(&m_mysql, CW2A(query_win));
		delay(5);
		MessageBox(L"Victory!!!");
		isMove=false;
		rbutton_possible=false;
		wScore++;
		wScore_str.Format(_T("%d"),wScore);
		m_wScore.SetWindowTextW(wScore_str);
		
	
	}
	else if(strReceive=="ybw"){  //��� ��ǽ�
		CString query_win;
		query_win.Format(_T("update member set wins = wins+1 where id = '%s'"), externUsername);

		int status_win = mysql_query(&m_mysql, CW2A(query_win));

		delay(5);
		MessageBox(L"Victory!!!");

		isMove=false;
		rbutton_possible=false;
		bScore++;
		bScore_str.Format(_T("%d"),bScore);
		m_bScore.SetWindowTextW(bScore_str);
		
	}
	else if(strReceive=="reset"){  //���� �����
		delay(5);
		Set_Game(); 
		UnitDraw();
		Reset_mPoint();

		CString strInsert;
		strInsert.Format(L"***************GAME START***************");
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);
	}
	else if(strReceive=="accept"){  // Client(Black)�� ������ ���
		delay(5);
		CString strInsert;
		strInsert.Format(_T("***************GAME START***************"));
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);

		isMove = true;
		turn = true;
		rbutton_possible=true;

		Timer();
	}
	else if(chating==false){  //ä���� �ƴ� ��� (���� ������)

		if(strReceive=="wlc"){
				ptr_board[0][0]=0;
				ptr_board[1][0]=0;
				ptr_board[2][0]=1;
				ptr_board[3][0]=5;
				ptr_board[4][0]=0;
		}else if(strReceive=="wsc"){
				ptr_board[4][0]=0;
				ptr_board[6][0]=1;
				ptr_board[5][0]=5;
				ptr_board[7][0]=0;
		}else if(strReceive=="blc"){
				ptr_board[0][7]=0;
				ptr_board[1][7]=0;
				ptr_board[2][7]=11;
				ptr_board[3][7]=15;
				ptr_board[4][7]=0;
		}else if(strReceive=="bsc"){
				ptr_board[4][7]=0;
				ptr_board[6][7]=11;
				ptr_board[5][7]=15;
				ptr_board[7][7]=0;
		}

		else{
		AfxExtractSubString(x,strReceive,0,'/');
		AfxExtractSubString(y,strReceive,1,'/');
		AfxExtractSubString(m_x,strReceive,2,'/');
		AfxExtractSubString(m_y,strReceive,3,'/');
		AfxExtractSubString(m_unit1,strReceive,4,'/');
		AfxExtractSubString(m_unit2,strReceive,5,'/');

		net_x=_ttoi(x);
		net_y=_ttoi(y);
		netMove_x=_ttoi(m_x);
		netMove_y=_ttoi(m_y);
		netMove_unit1=_ttoi(m_unit1);
		netMove_unit2=_ttoi(m_unit2);

		if(net_y==4 && ptr_board[net_x][net_y]==6 && ptr_board[netMove_x][netMove_y-1]==16 && 0<netMove_x<7)  // inPassing �˰���
		{
			ptr_board[netMove_x][netMove_y-1]=0;
		}else if(net_y==3 && ptr_board[net_x][net_y]==16 && ptr_board[netMove_x][netMove_y+1]==6 && 0<netMove_x<7){  // inPassing �˰���
			ptr_board[netMove_x][netMove_y+1]=0;
		}


		ptr_board[net_x][net_y]=netMove_unit1;
		ptr_board[netMove_x][netMove_y]=netMove_unit2;

		pen_x=net_x;
		pen_y=net_y;
		penMove_x=netMove_x;
		penMove_y=netMove_y;
		}
	}
}

void CChessDlg::Accept(CString strSock)
{
	m_strOtherIP=strSock;
}

void CChessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int ax = point.x;
	int ay = point.y;
	
	int board_X=0;
	int board_Y=0;
	int a=0;
	int b=0;

	for(int i=0; i<8; i++){
		a=(XY_SIZE+(XY_SIZE*i));
		b=XY_SIZE*i;
		if(ax>b && ax<a){
			board_X= i;
			break;
		}
	}

	for(int j=0; j<8; j++){
		a=(XY_SIZE+(XY_SIZE*j))+BOARD_SIZE;
		b=(XY_SIZE*j)+BOARD_SIZE;
		if(ay>b && ay<a){
			board_Y= j;
			break;
		}
	}

	if(ax<XY_SIZE*8 && ay>BOARD_SIZE && ay<(XY_SIZE*8)+BOARD_SIZE) {
		Rule(board_X, board_Y);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CChessDlg::Rule(int &setX, int &setY){

	CClientDC dc(this);
	CString help;

	if(isMove && setX<8 && setY<8){

	SetPointX=setX;
	SetPointY=setY;

	help.Format(_T("�̵� �� ���� ������ �ּ���.(�ٽ� �����Ϸ��� 'Right Mouse Button Click.')"));
	if((ptr_board[setX][setY])==0){  // �ƹ��͵� ���� ���� Ŭ���ϸ� �̵���� ����
		isMove=true;
	}
	if(turn){   // ������� �̵�
		if((ptr_board[setX][setY]) != 0 && ptr_board[setX][setY] <10){
			{
				dc.TextOut(20,20,help);
				isMove=false;
				Move(setX,setY);
			}
		
		}
	} else {   // ������� �̵�
		if((ptr_board[setX][setY])>10){
			dc.TextOut(20,20,help);
			isMove=false;
			Move(setX,setY);
		}
	}
	}else {

		if(move_point[setX][setY]==true){

		Swap(SetPointX,SetPointY,setX,setY);	

		inPassing_Possible_WPL=false;
		inPassing_Possible_WPR=false;
		inPassing_Possible_BPL=false;
		inPassing_Possible_BPR=false;

		isMove=true;
		Promotion();
		UnitDraw();
		Reset_mPoint();

		//��ǥ�� ����
		str_location.Format(_T("%d/%d/%d/%d/%d/%d"),SetPointX,SetPointY,setX,setY,ptr_board[SetPointX][SetPointY],ptr_board[setX][setY]);
		theApp.SendData(str_location);
		delay(300);

		WhiteCheck();
		BlackCheck();
		Check();
		Reset_Check();

		if(turn){
			turn=false;
			isMove=false;
			turn_throw="B";
			theApp.SendData(turn_throw);
			delay(300);

			m_timer.SetWindowText(L"���� �ð�:60��");

			ss=61;
			Timer();
			delay(300);
			rbutton_possible=false;
		
		}else {
			turn=true;
			isMove=false;
			turn_throw="W";
			theApp.SendData(turn_throw);
			delay(300);

			m_timer.SetWindowText(L"���� �ð�:60��");

			ss=61;
			Timer();
			delay(300);
			rbutton_possible=false;
			}	

		}
	}
	
}

void CChessDlg::Swap(int &x,int &y, int &move_X, int &move_Y){
	
	
	 //////////// inPassing �˰��� /////////////

	 if(y==4 && ptr_board[x][y]==6 && ptr_board[move_X][move_Y-1]==16 && inPassing_Possible_WPL){
		 SetA = ptr_board[x][y];

		 ptr_board[x][y]=0;
		 ptr_board[move_X][move_Y-1]=0;
		 ptr_board[move_X][move_Y]=SetA;
	 }else if(y==4 && ptr_board[x][y]==6 && ptr_board[move_X][move_Y-1]==16 && inPassing_Possible_WPR){
		 SetA = ptr_board[x][y];

		 ptr_board[x][y]=0;
		 ptr_board[move_X][move_Y-1]=0;
		 ptr_board[move_X][move_Y]=SetA;
	 }
	 else if(y==3 && ptr_board[x][y]==16 && ptr_board[move_X][move_Y+1]==6 && inPassing_Possible_BPL){
		 SetA = ptr_board[x][y];

		 ptr_board[x][y]=0;
		 ptr_board[move_X][move_Y+1]=0;
		 ptr_board[move_X][move_Y]=SetA;
	 }else if(y==3 && ptr_board[x][y]==16 && ptr_board[move_X][move_Y+1]==6 && inPassing_Possible_BPR){
		 SetA = ptr_board[x][y];

		 ptr_board[x][y]=0;
		 ptr_board[move_X][move_Y+1]=0;
		 ptr_board[move_X][move_Y]=SetA;
	 }
	 else {
			CString iPWL,iPWR,iPBL,iPBR;
			iPWL="ipwL";
			iPWR="ipwR";
			iPBL="ipbL";
			iPBR="ipbR";
			if(ptr_board[x][1]==6 && move_Y==3 && ptr_board[move_X+1][move_Y]==16){
			theApp.SendData(iPBL);
			delay(300);
			}
			if(ptr_board[x][1]==6 && move_Y==3 && ptr_board[move_X-1][move_Y]==16){
			theApp.SendData(iPBR);
			delay(300);
			} 
			if(ptr_board[x][6]==16 && move_Y==4 && ptr_board[move_X+1][move_Y]==6){
			theApp.SendData(iPWL);
			delay(300);
			}
			if(ptr_board[x][6]==16 && move_Y==4 && ptr_board[move_X-1][move_Y]==6){
			theApp.SendData(iPWR);
			delay(300);
			}
			////////////////////////////////////////////
			SetA = ptr_board[x][y];
			SetB = ptr_board[move_X][move_Y];
	 
			ptr_board[x][y] = SetB;
			ptr_board[move_X][move_Y] = SetA;

			if(SetB>0){
				ptr_board[x][y]=0;  // �� ��ġ�� ��������
			}	
	 }


	 CString bwin, wwin;
	 bwin="bw";
	 wwin="ww";

	 if(SetB==1){ //��� �¸�
		 USES_CONVERSION;




		 CString query_win;
		 query_win.Format(_T("update member set wins = wins+1 where id = '%s'"), externUsername);

		 int status_win = mysql_query(&m_mysql, CW2A(query_win));
		 theApp.SendData(bwin);
		

		 delay(300);
		 isMove=false;
		 rbutton_possible=false;
		 bScore++;
		 bScore_str.Format(_T("%d"),bScore);
		 m_bScore.SetWindowTextW(bScore_str);
		 KillTimer(IDC_STOPWATCH);
		 MessageBox(L"Victory!!!");
		

	 }

	 if(SetB==11){ //��� �¸�
		 CString query_win;
		 query_win.Format(_T("update member set wins = wins+1 where id = '%s'"), externUsername);

		 int status_win = mysql_query(&m_mysql, CW2A(query_win));
		 theApp.SendData(wwin);
		
		 delay(300);
		 isMove=false;
		 rbutton_possible=false;
		 wScore++;
		 wScore_str.Format(_T("%d"),wScore);
		 m_wScore.SetWindowTextW(wScore_str);
		 KillTimer(IDC_STOPWATCH);
		 MessageBox(L"Victory!!!");
		

	 }
}

void CChessDlg::Move(int &x,int &y){
	CClientDC dc(this);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	switch(ptr_board[x][y])
	{
	case 1:  //ȭ��Ʈ ŷ

		if(y<7){
			if(ptr_board[x][y+1]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x][y+1]=true;
			}else if(ptr_board[x][y+1]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x][y+1]=true;
			}
		}
		if(y>0){
			if(ptr_board[x][y-1]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x][y-1]=true;
			}else if(ptr_board[x][y-1]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x][y-1]=true;
			}
		}

		if(x<7){
			if(ptr_board[x+1][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x+1][y]=true;
			}else if(ptr_board[x+1][y]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x+1][y]=true;
			}
		}
		if(x>0){
			if(ptr_board[x-1][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x-1][y]=true;
			}else if(ptr_board[x-1][y]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x-1][y]=true;
			}
		}
		if(x<7 && y<7){
			if(ptr_board[x+1][y+1]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x+1][y+1]=true;
			}else if(ptr_board[x+1][y+1]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x+1][y+1]=true;
			}
		}

		if(x<7 && y>0){
			if(ptr_board[x+1][y-1]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x+1][y-1]=true;
			}else if(ptr_board[x+1][y-1]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x+1][y-1]=true;
			}
		}

		if(x>0 && y<7){
			if(ptr_board[x-1][y+1]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x-1][y+1]=true;
			}else if(ptr_board[x-1][y+1]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x-1][y+1]=true;
			}
		}

		if(x>0 && y>0){
			if(ptr_board[x-1][y-1]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x-1][y-1]=true;
			}else if(ptr_board[x-1][y-1]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
				move_point[x-1][y-1]=true;
			}
		}


		break;


	case 11:  //�� ŷ
			if(y<7){
				if(ptr_board[x][y+1]==0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(moveArea);
					dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x][y+1]=true;
				}else if(ptr_board[x][y+1]<10 && ptr_board[x][y+1]!=0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x][y+1]=true;
				}
			}
			if(y>0){
				if(ptr_board[x][y-1]==0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(moveArea);
					dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x][y-1]=true;
				}else if(ptr_board[x][y-1]<10 && ptr_board[x][y-1]!=0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x][y-1]=true;
				}
			}

			if(x<7){
				if(ptr_board[x+1][y]==0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(moveArea);
					dc.BitBlt((x+1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x+1][y]=true;
				}else if(ptr_board[x+1][y]<10 && ptr_board[x+1][y]!=0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x+1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x+1][y]=true;
				}
			}
			if(x>0){
				if(ptr_board[x-1][y]==0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(moveArea);
					dc.BitBlt((x-1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x-1][y]=true;
				}else if(ptr_board[x-1][y]<10 && ptr_board[x-1][y]!=0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x-1)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x-1][y]=true;
				}
			}
			if(x<7 && y<7){
				if(ptr_board[x+1][y+1]==0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(moveArea);
					dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x+1][y+1]=true;
				}else if(ptr_board[x+1][y+1]<10 && ptr_board[x+1][y+1]!=0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x+1][y+1]=true;
				}
			}

			if(x<7 && y>0){
				if(ptr_board[x+1][y-1]==0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(moveArea);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x+1][y-1]=true;
				}else if(ptr_board[x+1][y-1]<10 && ptr_board[x+1][y-1]!=0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x+1][y-1]=true;
				}
			}

			if(x>0 && y<7){
				if(ptr_board[x-1][y+1]==0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(moveArea);
					dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x-1][y+1]=true;
				}else if(ptr_board[x-1][y+1]<10 && ptr_board[x-1][y+1]!=0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x-1][y+1]=true;
				}
			}

			if(x>0 && y>0){
				if(ptr_board[x-1][y-1]==0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(moveArea);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x-1][y-1]=true;
				}else if(ptr_board[x-1][y-1]<10&& ptr_board[x-1][y-1]!=0){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);				
					move_point[x-1][y-1]=true;
				}
			}

		break;




	case 2:  //ȭ��Ʈ ��

		for(int i=1; i<8; i++){   ///���� �˻�
			if(y+i>7)break;
			if(ptr_board[x][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y+i]=true;


			} else if(ptr_board[x][y+i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y+i]=true;

				break;
			} else if(ptr_board[x][y+i]<10){  //����϶�
				move_point[x][y+i]=false;
				break;
			} 
		}

		for(int j=1; j<8; j++){  //���� �˻�
			if(y-j<0)break;
			if(ptr_board[x][y-j]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y-j]=true;
			} else if(ptr_board[x][y-j]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-j]=true;
				break;
			} else if(ptr_board[x][y-j]<10){  //����϶�
				move_point[x][y-j]=false;
				break;
			} 
		}
		for(int a=1; a<8; a++){  //�ϴ��� �˻�
			if(x+a>7)break;
			if(ptr_board[x+a][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+a][y]=true;
			} else if(ptr_board[x+a][y]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+a][y]=true;
				break;
			} else if(ptr_board[x+a][y]<10){  //����϶�
				move_point[x+a][y]=false;
				break;
			} 
		}

		for(int b=1; b<8; b++){  //����� �˻�
			if(x-b<0)break;
			if(ptr_board[x-b][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-b][y]=true;
			} else if(ptr_board[x-b][y]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-b][y]=true;
				break;
			} else if(ptr_board[x-b][y]<10){  //����϶�
				move_point[x-b][y]=false;
				break;
			} 
		}
		for(int i=1; i<8; i++){  //������ �Ʒ� �밢�� �˻�
			if(x+i>7 || y+i>7 )break;
			if(ptr_board[x+i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y+i]=true;
			} else if(ptr_board[x+i][y+i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y+i]=true;
				break;
			} else if(ptr_board[x+i][y+i]<10){  //����϶�
				move_point[x+i][y+i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //������ �� �밢�� �˻� 
			if(x+i>7 || y-i<0 )break;
			if(ptr_board[x+i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y-i]=true;
			} else if(ptr_board[x+i][y-i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y-i]=true;
				break;
			} else if(ptr_board[x+i][y-i]<10){  //����϶�
				move_point[x+i][y-i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //���� �� �밢�� �˻� 
			if(x-i<0 || y-i<0 )break;
			if(ptr_board[x-i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y-i]=true;
			} else if(ptr_board[x-i][y-i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y-i]=true;
				break;
			} else if(ptr_board[x-i][y-i]<10){  //����϶�
				move_point[x-i][y-i]=false;
				break;
			} 
		}
		for(int i=1; i<8; i++){   //���� �Ʒ� �밢�� �˻� 
			if(x-i<0 || y+i>7 )break;
			if(ptr_board[x-i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y+i]=true;
			} else if(ptr_board[x-i][y+i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y+i]=true;
				break;
			} else if(ptr_board[x-i][y+i]<10){  //����϶�
				move_point[x-i][y+i]=false;
				break;
			} 
		}
		break;

	case 12:  //����
		for(int i=1; i<8; i++){   ///���� �˻�
			if(y+i>7)break;
			if(ptr_board[x][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y+i]=true;
			} else if(ptr_board[x][y+i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y+i]=true;
				break;
			} else if(ptr_board[x][y+i]>10){  //����϶�
				move_point[x][y+i]=false;
				break;
			} 
		}
		for(int j=1; j<8; j++){  //���� �˻�
			if(y-j<0)break;
			if(ptr_board[x][y-j]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y-j]=true;
			} else if(ptr_board[x][y-j]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-j]=true;
				break;
			} else if(ptr_board[x][y-j]>10){  //����϶�
				move_point[x][y-j]=false;
				break;
			} 
		}
		for(int a=1; a<8; a++){  //�ϴ��� �˻�
			if(x+a>7)break;
			if(ptr_board[x+a][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+a][y]=true;
			} else if(ptr_board[x+a][y]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+a][y]=true;
				break;
			} else if(ptr_board[x+a][y]>10){  //����϶�
				move_point[x+a][y]=false;
				break;
			} 
		}
		for(int b=1; b<8; b++){  //����� �˻�
			if(x-b<0)break;
			if(ptr_board[x-b][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x-b][y]=true;
			} else if(ptr_board[x-b][y]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-b][y]=true;
				break;
			} else if(ptr_board[x-b][y]>10){  //����϶�
				move_point[x-b][y]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){  //������ �Ʒ� �밢�� �˻�
			if(x+i>7 || y+i>7 )break;
			if(ptr_board[x+i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y+i]=true;
			} else if(ptr_board[x+i][y+i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y+i]=true;
				break;
			} else if(ptr_board[x+i][y+i]>10){  //����϶�
				move_point[x+i][y+i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //������ �� �밢�� �˻� 
			if(x+i>7 || y-i<0 )break;
			if(ptr_board[x+i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y-i]=true;
			} else if(ptr_board[x+i][y-i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y-i]=true;
				break;
			} else if(ptr_board[x+i][y-i]>10){  //����϶�
				move_point[x+i][y-i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //���� �� �밢�� �˻� 
			if(x-i<0 || y-i<0 )break;
			if(ptr_board[x-i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y-i]=true;
			} else if(ptr_board[x-i][y-i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y-i]=true;
				break;
			} else if(ptr_board[x-i][y-i]>10){  //����϶�
				move_point[x-i][y-i]=false;
				break;
			} 
		}


		for(int i=1; i<8; i++){   //���� �Ʒ� �밢�� �˻� 
			if(x-i<0 || y+i>7 )break;
			if(ptr_board[x-i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y+i]=true;
			} else if(ptr_board[x-i][y+i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y+i]=true;
				break;
			} else if(ptr_board[x-i][y+i]>10){  //����϶�
				move_point[x-i][y+i]=false;
				break;
			} 
		}


		break;


	case 3:  //ȭ��Ʈ ���
		for(int i=1; i<8; i++){  //������ �Ʒ� �밢�� �˻�
			if(x+i>7 || y+i>7 )break;
			if(ptr_board[x+i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y+i]=true;
			} else if(ptr_board[x+i][y+i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y+i]=true;
				break;
			} else if(ptr_board[x+i][y+i]<10){  //����϶�
				move_point[x+i][y+i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //������ �� �밢�� �˻� 
			if(x+i>7 || y-i<0 )break;
			if(ptr_board[x+i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y-i]=true;
			} else if(ptr_board[x+i][y-i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y-i]=true;
				break;
			} else if(ptr_board[x+i][y-i]<10){  //����϶�
				move_point[x+i][y-i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //���� �� �밢�� �˻� 
			if(x-i<0 || y-i<0 )break;
			if(ptr_board[x-i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y-i]=true;
			} else if(ptr_board[x-i][y-i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y-i]=true;
				break;
			} else if(ptr_board[x-i][y-i]<10){  //����϶�
				move_point[x-i][y-i]=false;
				break;
			} 
		}


		for(int i=1; i<8; i++){   //���� �Ʒ� �밢�� �˻� 
			if(x-i<0 || y+i>7 )break;
			if(ptr_board[x-i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y+i]=true;
			} else if(ptr_board[x-i][y+i]>10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y+i]=true;
				break;
			} else if(ptr_board[x-i][y+i]<10){  //����϶�
				move_point[x-i][y+i]=false;
				break;
			} 
		}


		break;

	case 13:  //�� ���

		for(int i=1; i<8; i++){  //������ �Ʒ� �밢�� �˻�
			if(x+i>7 || y+i>7 )break;
			if(ptr_board[x+i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y+i]=true;
			} else if(ptr_board[x+i][y+i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y+i]=true;
				break;
			} else if(ptr_board[x+i][y+i]>10){  //����϶�
				move_point[x+i][y+i]=false;
				break;
			} 
		}
		for(int i=1; i<8; i++){   //������ �� �밢�� �˻� 
			if(x+i>7 || y-i<0 )break;
			if(ptr_board[x+i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y-i]=true;
			} else if(ptr_board[x+i][y-i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y-i]=true;
				break;
			} else if(ptr_board[x+i][y-i]>10){  //����϶�
				move_point[x+i][y-i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //���� �� �밢�� �˻� 
			if(x-i<0 || y-i<0 )break;
			if(ptr_board[x-i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y-i]=true;
			} else if(ptr_board[x-i][y-i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y-i]=true;
				break;
			} else if(ptr_board[x-i][y-i]>10){  //����϶�
				move_point[x-i][y-i]=false;
				break;
			} 
		}


		for(int i=1; i<8; i++){   //���� �Ʒ� �밢�� �˻� 
			if(x-i<0 || y+i>7 )break;
			if(ptr_board[x-i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y+i]=true;
			} else if(ptr_board[x-i][y+i]<10){  //����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y+i]=true;
				break;
			} else if(ptr_board[x-i][y+i]>10){  //����϶�
				move_point[x-i][y+i]=false;
				break;
			} 
		}


		break;

	case 4:  //ȭ��Ʈ����Ʈ
		if(x<7 && y<6){
			if(ptr_board[x+1][y+2]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y+2]=true;
			} else if(ptr_board[x+1][y+2]>10){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y+2]=true;
			} else if(ptr_board[x+1][y+2]<10){
				if(x+1>7 || y+2>7) break;
				move_point[x+1][y+2]=false;
			}
		}
		if(x<7 && y>1){
			if(ptr_board[x+1][y-2]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y-2]=true;
			}else if(ptr_board[x+1][y-2]>10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y-2]=true;
			} else if(ptr_board[x+1][y-2]<10){

				move_point[x+1][y-2]=false;
			}
		}


		if(x>0 && y<6){
			if(ptr_board[x-1][y+2]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y+2]=true;
			} else if(ptr_board[x-1][y+2]>10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y+2]=true;
			} else if(ptr_board[x-1][y+2]<10){

				move_point[x-1][y+2]=false;
			}
		}

		if(x>0 && y>1){
			if(ptr_board[x-1][y-2]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y-2]=true;
			}else if(ptr_board[x-1][y-2]>10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y-2]=true;
			} else if(ptr_board[x-1][y-2]<10){

				move_point[x-1][y-2]=false;
			}
		}




		if(x<6 && y<7){
			if(ptr_board[x+2][y+1]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+2][y+1]=true;
			}else if(ptr_board[x+2][y+1]>10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+2][y+1]=true;
			} else if(ptr_board[x+2][y+1]<10){

				move_point[x+2][y+1]=false;
			}

		}

		if(x<6 && y>0){
			if(ptr_board[x+2][y-1]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+2][y-1]=true;
			}else if(ptr_board[x+2][y-1]>10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+2][y-1]=true;
			} else if(ptr_board[x+2][y-1]<10){

				move_point[x+2][y-1]=false;
			}
		}

		if(x>1 && y<7){

			if(ptr_board[x-2][y+1]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-2][y+1]=true;
			}else if(ptr_board[x-2][y+1]>10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-2][y+1]=true;
			} else if(ptr_board[x-2][y+1]<10){

				move_point[x-2][y+1]=false;
			}
		}

		if(x>1 && y>0){
			if(ptr_board[x-2][y-1]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-2][y-1]=true;
			}else if(ptr_board[x-2][y-1]>10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-2][y-1]=true;
			} else if(ptr_board[x-2][y-1]<10){

				move_point[x-2][y-1]=false;
			}
		}



		break;

	case 14: //������Ʈ

		if(x<7 && y<6){
			if(ptr_board[x+1][y+2]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y+2]=true;
			} else if(ptr_board[x+1][y+2]<10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y+2]=true;
			} else if(ptr_board[x+1][y+2]>10){
				if(x+1>7 || y+2>7) break;
				move_point[x+1][y+2]=false;
			}
		}


		if(x<7 && y>1){
			if(ptr_board[x+1][y-2]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y-2]=true;
			}else if(ptr_board[x+1][y-2]<10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y-2]=true;
			} else if(ptr_board[x+1][y-2]>10){

				move_point[x+1][y-2]=false;
			}
		}


		if(x>0 && y<6){
			if(ptr_board[x-1][y+2]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y+2]=true;
			} else if(ptr_board[x-1][y+2]<10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-1)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y+2]=true;
			} else if(ptr_board[x-1][y+2]>10){

				move_point[x-1][y+2]=false;
			}
		}

		if(x>0 && y>1){
			if(ptr_board[x-1][y-2]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y-2]=true;
			}else if(ptr_board[x-1][y-2]<10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-1)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y-2]=true;
			} else if(ptr_board[x-1][y-2]>10){

				move_point[x-1][y-2]=false;
			}
		}




		if(x<6 && y<7){
			if(ptr_board[x+2][y+1]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+2][y+1]=true;
			}else if(ptr_board[x+2][y+1]<10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+2][y+1]=true;
			} else if(ptr_board[x+2][y+1]>10){

				move_point[x+2][y+1]=false;
			}

		}

		if(x<6 && y>0){
			if(ptr_board[x+2][y-1]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+2][y-1]=true;
			}else if(ptr_board[x+2][y-1]<10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+2][y-1]=true;
			} else if(ptr_board[x+2][y-1]>10){

				move_point[x+2][y-1]=false;
			}
		}

		if(x>1 && y<7){

			if(ptr_board[x-2][y+1]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-2][y+1]=true;
			}else if(ptr_board[x-2][y+1]<10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-2)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-2][y+1]=true;
			} else if(ptr_board[x-2][y+1]>10){

				move_point[x-2][y+1]=false;
			}
		}

		if(x>1 && y>0){
			if(ptr_board[x-2][y-1]==0){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-2][y-1]=true;
			}else if(ptr_board[x-2][y-1]<10){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-2)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-2][y-1]=true;
			} else if(ptr_board[x-2][y-1]>10){

				move_point[x-2][y-1]=false;
			}
		}



		break;



	case 5:  //ȭ��Ʈ��
		for(int i=1; i<8; i++){
			if(y+i>7)break;
			if(ptr_board[x][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y+i]=true;

			} else if(ptr_board[x][y+i]>10){  //�����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y+i]=true;
				break;
			} else if(ptr_board[x][y+i]<10){  //�Ʊ��϶�
				move_point[x][y+i]=false;
				break;
			} 


		}
		for(int j=1; j<8; j++){
			if(y-j<0)break;
			if(ptr_board[x][y-j]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x][y-j]=true;
			} else if(ptr_board[x][y-j]>10){  //�����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-j]=true;
				break;
			} else if(ptr_board[x][y-j]<10){  //�Ʊ��϶�
				move_point[x][y-j]=false;
				break;
			} 
		}
		for(int a=1; a<8; a++){
			if(x+a>7)break;
			if(ptr_board[x+a][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x+a][y]=true;
			} else if(ptr_board[x+a][y]>10){  //�����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+a][y]=true;
				break;
			} else if(ptr_board[x+a][y]<10){  //�Ʊ��϶�
				move_point[x+a][y]=false;
				break;
			} 
		}

		for(int b=1; b<8; b++){
			if(x-b<0)break;
			if(ptr_board[x-b][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x-b][y]=true;
			} else if(ptr_board[x-b][y]>10){  //�����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-b][y]=true;
				break;
			} else if(ptr_board[x-b][y]<10){  //�Ʊ��϶�
				move_point[x-b][y]=false;
				break;
			} 
		}

		break;


	case 15:  //����

		for(int i=1; i<8; i++){
			if(y+i>7)break;
			if(ptr_board[x][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x][y+i]=true;
			} else if(ptr_board[x][y+i]<10){  //�����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y+i]=true;
				break;
			} else if(ptr_board[x][y+i]>10){  //�Ʊ��϶�
				move_point[x][y+i]=false;
				break;
			} 


		}
		for(int j=1; j<8; j++){
			if(y-j<0)break;
			if(ptr_board[x][y-j]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x][y-j]=true;
			} else if(ptr_board[x][y-j]<10){  //�����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-j]=true;
				break;
			} else if(ptr_board[x][y-j]>10){  //�Ʊ��϶�
				move_point[x][y-j]=false;
				break;
			} 
		}
		for(int a=1; a<8; a++){
			if(x+a>7)break;
			if(ptr_board[x+a][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x+a][y]=true;
			} else if(ptr_board[x+a][y]<10){  //�����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+a][y]=true;
				break;
			} else if(ptr_board[x+a][y]>10){  //�Ʊ��϶�
				move_point[x+a][y]=false;
				break;
			} 
		}

		for(int b=1; b<8; b++){
			if(x-b<0)break;
			if(ptr_board[x-b][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x-b][y]=true;
			} else if(ptr_board[x-b][y]<10){  //�����϶�
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-b][y]=true;
				break;
			} else if(ptr_board[x-b][y]>10){  //�Ʊ��϶�
				move_point[x-b][y]=false;
				break;
			} 
		}

		break;

	case 6: //ȭ��Ʈ��

		if(y==1){      //�ѹ��� �������� ���� ȭ��Ʈ��

			if(ptr_board[x][y+1]+ptr_board[x][y+2]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dc.BitBlt((x)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);


				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				dc.BitBlt((x)*XY_SIZE,(y+2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y+1]=true;
				move_point[x][y+2]=true;
			} else if((ptr_board[x][y+1]==0)){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);

				move_point[x][y+1]=true;
			}

			if(x<7 && y<7){
				if(10<ptr_board[x+1][y+1] && ptr_board[x+1][y+1]<20 ){

					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

					move_point[x+1][y+1]=true;
				}
			}

			if(x>0 && y<7){
				if(10<ptr_board[x-1][y+1] && ptr_board[x-1][y+1]<20 ){

					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

					move_point[x-1][y+1]=true;
				}
			}


		}

		else {   //������ ��

			if(ptr_board[x][y+1]==0 && y<7){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x][y+1]=true;
			}




			if(10<ptr_board[x+1][y+1] && ptr_board[x+1][y+1]<20 && x<7 && y<7){      

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+1][y+1]= true;	
			}

			if(10<ptr_board[x-1][y+1] && ptr_board[x-1][y+1]<20 && x>0 && y<7){

				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-1][y+1]= true;

			}


			if(y==4){

				if(ptr_board[x+1][y]==16 && x<7 && inPassing_Possible_WPR){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x+1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
					move_point[x+1][y+1]= true;	

				}

				if(ptr_board[x-1][y]==16 && x>0 && inPassing_Possible_WPL){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x-1)*XY_SIZE,(y+1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
					move_point[x-1][y+1]= true;

				}


			}
		}
		break;
	case 16: //����   
		if(y==6){  //�ѹ��� �������� ���� ����

			if(ptr_board[x][y-1]+ptr_board[x][y-2]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dc.BitBlt((x)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);


				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				dc.BitBlt((x)*XY_SIZE,(y-2)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y-1]=true;
				move_point[x][y-2]=true;

			} else if((ptr_board[x][y-1]==0)){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-1]=true;

			}

			if(x>0 && y>0){
				if(1<=ptr_board[x-1][y-1] && ptr_board[x-1][y-1]<10){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
					move_point[x-1][y-1]=true;

				}
			}

			if(x<7 && y>0){

				if(1<=ptr_board[x+1][y-1] && ptr_board[x+1][y-1]<10){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
					move_point[x+1][y-1]=true;
				}
			}
		} else {  //������ ����

			if(ptr_board[x][y-1]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y-1]=true;

			}

			if(x>0 && y>0){
				if(0<ptr_board[x-1][y-1] && ptr_board[x-1][y-1]<10){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
					move_point[x-1][y-1]=true;
				}
			}

			if(x<7 && y>0){
				if(0<ptr_board[x+1][y-1] && ptr_board[x+1][y-1]<10){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
					move_point[x+1][y-1]=true;
				}
			}

			if(y==3){

				if(ptr_board[x+1][y]==6 && x<7 && inPassing_Possible_BPR){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x+1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
					move_point[x+1][y-1]=true;
				}

				if(ptr_board[x-1][y]==6 && x>0 && inPassing_Possible_BPL){
					dcMem.SelectObject(moveArea_Mask);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);

					dcMem.SelectObject(dangerArea);
					dc.BitBlt((x-1)*XY_SIZE,(y-1)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
					move_point[x-1][y-1]=true;


				}
			}

		}


		break;



	}
}

void CChessDlg::UnitDraw(){
	
	pDC=GetDC();
	CBitmap *oldBitmap;

	CPen Pen;
	Pen.CreatePen(PS_SOLID, 5, RGB(255,0,0) );

	memDC.CreateCompatibleDC(pDC);
	oldBitmap = memDC.SelectObject(&backGround);
	dmemDC.CreateCompatibleDC(&memDC);

	dmemDC.SelectObject(&bitmapMAP);
	memDC.BitBlt(0,0,860,860,&dmemDC,0,0,SRCCOPY);

	for(int x=0; x<=7; x++){
		for(int y=0;y<=7;y++){
			switch(ptr_board[x][y])
		{
		case 1:	//ŷ				ȭ��Ʈ
			dmemDC.SelectObject(&KMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWK);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);

			//�� ĳ���� �˰���
			if(ptr_board[0][0]==5 && ptr_board[1][0]==0 && ptr_board[2][0] ==0 && ptr_board[3][0] == 0 && castling_Possible_WK && castling_Possible_W){
				GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(true);
			} else GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(false);

			if(ptr_board[5][0] == 0 && ptr_board[6][0]==0 && ptr_board[7][0]==5 && castling_Possible_WK && castling_Possible_w){
				GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(true);
			} else GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(false);

			set_Bking_x=x;  //üũ�� Ȯ���ϱ� ���� ���� ����
			set_Bking_y=y;  //üũ�� Ȯ���ϱ� ���� ���� ����

			if(x!=4 || y!=0){
				//�� ĳ���� �˰���
				castling_Possible_WK=false;
				GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(castling_Possible_WK);
				GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(castling_Possible_WK);
			}
			break;
		case 2:	//��
			dmemDC.SelectObject(&QMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWQ);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 3: //���
			dmemDC.SelectObject(&BMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWB);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 4:	//����Ʈ
			dmemDC.SelectObject(&KNMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWKN);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 5:	//��			
			dmemDC.SelectObject(&RMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWR);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			//�� ĳ���� �˰���
			if(x==0 && y!=0) {
				castling_Possible_W = false;
				GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(false);
			}else if(x==7 && y!=0) {
				castling_Possible_w = false;
				GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(false);
			}else if(x!=0 && y==0) {
				if(x<4){
					castling_Possible_W = false;
					GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(false);
				}
				if(x>4 && x<7){
					castling_Possible_w = false;
					GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(false);
				}
			}
			break;

		case 11: //ŷ                     //��
			dmemDC.SelectObject(&KMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBK);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);

			//�� ĳ���� �˰���
			if(ptr_board[0][7]==15 &&ptr_board[1][7]==0 && ptr_board[2][7] ==0 && ptr_board[3][7] == 0 && castling_Possible_BK && castling_Possible_B){
				GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(true);
			} else GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(false);

			if(ptr_board[5][7] == 0 && ptr_board[6][7]==0 && ptr_board[7][7]==15 && castling_Possible_BK && castling_Possible_b){
				GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(true);
			} else GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(false);

			set_Wking_x=x;
			set_Wking_y=y;

			if(x!=4 || y!=7){
				//�� ĳ���� �˰���
				castling_Possible_BK=false;
				GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(castling_Possible_BK);
				GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(castling_Possible_BK);
			}
			break;
		case 12:	//��
			dmemDC.SelectObject(&QMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBQ);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 13:	//���
			dmemDC.SelectObject(&BMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBB);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 14:	//����Ʈ
			dmemDC.SelectObject(&KNMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBKN);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 15:	//��
			dmemDC.SelectObject(&RMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBR);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);


			//�� ĳ���� �˰���
			if(x==0 && y!=7) {
				castling_Possible_B = false;
				GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(false);
			}else if(x==7 && y!=7) {
				castling_Possible_b = false;
				GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(false);
			}else if(x!=0 && y==7)
				if(x<4){
					castling_Possible_B = false;
					GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(false);
				}
				if(x>4 && x<7){
					castling_Possible_b = false;
					GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(false);
				}
			break;

		case 6: //��                     //ȭ��Ʈ
			dmemDC.SelectObject(&PMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWP);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;

		case 16: //��                   //��	
			dmemDC.SelectObject(&PMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBP);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		}

		}
	}

	CPen* oldPen = memDC.SelectObject( &Pen );
	memDC.MoveTo(pen_x*XY_SIZE+38,pen_y*XY_SIZE+BOARD_SIZE+38);
	memDC.LineTo(penMove_x*XY_SIZE+38,penMove_y*XY_SIZE+BOARD_SIZE+38);


	pDC->BitBlt(0,0,860, 860, &memDC, 0, 0, SRCCOPY);

	dmemDC.DeleteDC();
	memDC.DeleteDC();
}

void CChessDlg::Reset_mPoint(){
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			move_point[i][j]=false;
		}
	}
}

void CChessDlg::Reset_Check(){
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			check_unit[i][j]=false;
		}
	}
}

void CChessDlg::Promotion(){
	for(int i=0;i<8;i++)
		{   //white pawn		
		if(ptr_board[i][7]==6){
			ptr_board[i][7] = 2;
		}
	}

	for(int j=0;j<8;j++)
		{   //black pawn		
		if(ptr_board[j][0]==16){
			ptr_board[j][0] = 12;
		}
	}
}

void CChessDlg::Set_Game(){  //�ʱⰪ ���� �� ���� �ٽý��� �Ҽ� �ְ� ���ִ� �޼ҵ�

	board[0][0] = 5;  //��
	board[1][0] = 4;  //����Ʈ
	board[2][0] = 3;  //���				//����� �ʱ�ȭ
	board[3][0] = 2;  //��
	board[4][0] = 1;  //ŷ
	board[5][0] = 3;  //���
	board[6][0] = 4;  //����Ʈ
	board[7][0] = 5;  //��

	board[0][7] = 15;  //��
	board[1][7] = 14;
	board[2][7] = 13;
	board[3][7] = 12;					//����� �ʱ�ȭ
	board[4][7] = 11;  //ŷ
	board[5][7] = 13;
	board[6][7] = 14;
	board[7][7] = 15;  //��

	for(int i=0;i<8;i++)
		{//��� ����				
		board[i][1] = 6;
	}
	for(int j=0;j<8;j++){//��� ����
		board[j][6] = 16;
	}

	for(int a=2;a<=5;a++)	{				//�� ���� ���� 0���� ä��
		for(int b=0;b<8;b++){
			board[b][a] = 0;
		}
	}

	castling_Possible_B = true; //��� �� ĳ���� ���� ����
	castling_Possible_b = true; //��� �� ĳ���� ���� ����
	castling_Possible_W = true; //��� �� ĳ���� ���� ����
	castling_Possible_w = true; //��� �� ĳ���� ���� ����

	castling_Possible_BK= true;  //��� ŷ ĳ���� ���� ����
	castling_Possible_WK= true;

	GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(false);

	GetDlgItem(IDC_RESETBUTTON)->EnableWindow(false);
}

void CChessDlg::OnBnClickedButtonWlc()
{
	CString wlc;
	if(turn && rbutton_possible){
	ptr_board[0][0]=0;
	ptr_board[1][0]=0;
	ptr_board[2][0]=1;
	ptr_board[3][0]=5;
	ptr_board[4][0]=0;
	UnitDraw();
	
	wlc="wlc";
	theApp.SendData(wlc);
	delay(300);

	turn=false;
	isMove=false;
	turn_throw="B";
	theApp.SendData(turn_throw);
	rbutton_possible=false;
	} else {
		MessageBox(L"�� ���ʰ� �ƴմϴ�.");
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CChessDlg::OnBnClickedButtonWsc()
{
	CString wsc;
	if(turn && rbutton_possible){
	ptr_board[4][0]=0;
	ptr_board[6][0]=1;
	ptr_board[5][0]=5;
	ptr_board[7][0]=0;
	UnitDraw();

	wsc="wsc";
	theApp.SendData(wsc);
	delay(300);

	turn=false;
	isMove=false;
	turn_throw="B";
	theApp.SendData(turn_throw);
	rbutton_possible=false;
	} else {

		MessageBox(L"�� ���ʰ� �ƴմϴ�.");
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CChessDlg::OnBnClickedButtonBlc()
{
	CString blc;
	 // �� ��ĳ����
	if(turn == false && rbutton_possible){
	ptr_board[0][7]=0;
	ptr_board[1][7]=0;
	ptr_board[2][7]=11;
	ptr_board[3][7]=15;
	ptr_board[4][7]=0;
	UnitDraw();

	blc="blc";
	theApp.SendData(blc);
	delay(300);

	turn=true;
	isMove=false;
	turn_throw="W";
	theApp.SendData(turn_throw);
	rbutton_possible=false;
	} else {
		MessageBox(L"�� ���ʰ� �ƴմϴ�.");
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CChessDlg::OnBnClickedButtonBsc()
{
	CString bsc;
	//�� ��ĳ����
	if(turn == false && rbutton_possible){
	ptr_board[4][7]=0;
	ptr_board[6][7]=11;
	ptr_board[5][7]=15;
	ptr_board[7][7]=0;
	UnitDraw();

	bsc="bsc";
	theApp.SendData(bsc);
	delay(300);

	turn=true;
	isMove=false;
	turn_throw="W";
	theApp.SendData(turn_throw);
	rbutton_possible=false;
	} else {
		MessageBox(L"�� ���ʰ� �ƴմϴ�.");
	}
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CChessDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(rbutton_possible){
	if(turn){
		turn=true;
	}else{
		turn=false;
	}
	isMove=true;
	UnitDraw();
	Reset_mPoint();
	CDialogEx::OnRButtonDown(nFlags, point);
	}
}

void CChessDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return;
}

void CChessDlg::delay(clock_t n)
{
	//�����̸� ������ִ� �޼ҵ�
  clock_t start = clock();
  while(clock() - start < n);
}

void CChessDlg::WhiteCheck(){
	for(int x=0; x<8; x++){
		 for(int y=0; y<8; y++){
			 switch(ptr_board[x][y]) 
			 {
				case 1: //ȭ��Ʈ ŷ
				if(ptr_board[x][y+1]==11 && y<7){
						check_unit[x][y+1]=true;
				}
				else if(ptr_board[x][y-1]==11 && y>1){
						check_unit[x][y-1]=true;
				}
				else if(ptr_board[x+1][y]==11 && x<7){
						check_unit[x+1][y]=true;
				}
				else if(ptr_board[x-1][y]==11 && x>1){
						check_unit[x-1][y]=true;
				}
				else if(ptr_board[x+1][y+1]==11 && x<7 && y<7){
						check_unit[x+1][y+1]=true;
				}
				else if(ptr_board[x+1][y-1]==11 && x<7 && y>1){
						check_unit[x+1][y-1]=true;
				}
				else if(ptr_board[x-1][y+1]==11 && x>1 && y<7){
						check_unit[x-1][y+1]=true;
				}
				else if(ptr_board[x-1][y-1]==11 && x>1 && y>1){
						check_unit[x-1][y-1]=true;
				}
				break;

				case 2:     //ȭ��Ʈ ��
				for(int i=1; i<8; i++){   ///���� �˻�
					if(ptr_board[x][y+i]<17 && ptr_board[x][y+i]!=0 && ptr_board[x][y+i]!=11){  //�Ʊ��϶�
						break;
					}
					else if(ptr_board[x][y+i]==11 && y+i<8){
						check_unit[x][y+i]=true;
					}
				}
				for(int j=1; j<8; j++){  //���� �˻�
					if(ptr_board[x][y-j]<17 && ptr_board[x][y-j]!=0 && ptr_board[x][y-j]!=11){  //�Ʊ��϶�
						break;
					} 
					else if(ptr_board[x][y-j]==11 && y-j>0){
						check_unit[x][y-j]=true;
					}
				}
				for(int a=1; a<8; a++){  //�ϴ��� �˻�
					if(ptr_board[x+a][y]<17 && ptr_board[x+a][y]!=0 && ptr_board[x+a][y]!=11){  //�Ʊ��϶�
						break;
					} 

					else if(ptr_board[x+a][y]==11 && x+a<8){
						check_unit[x+a][y]=true;
					}
				}
			
				for(int b=1; b<8; b++){  //����� �˻�
					if(ptr_board[x-b][y]<17 && ptr_board[x-b][y]!=0 && ptr_board[x-b][y]!=11){  //�Ʊ��϶�
						break;
					}
					else if(ptr_board[x-b][y]==11 && x-b>0){
						check_unit[x-b][y]=true;
					}
				}

				for(int i=1; i<8; i++){  //������ �Ʒ� �밢�� �˻�
						if(ptr_board[x+i][y+i]<17 && ptr_board[x+i][y+i]!=0 && ptr_board[x+i][y+i]!=11){  //�Ʊ��϶�
						break;
						} 
						else if(ptr_board[x+i][y+i]==11 && x+i<8 && y+i<8){
							check_unit[x+i][y+i]=true;
						}
				}

				for(int i=1; i<8; i++){   //������ �� �밢�� �˻� 
						if(ptr_board[x+i][y-i]<17 && ptr_board[x+i][y-i]!=0 && ptr_board[x+i][y-i]!=11){  //�Ʊ��϶�
						break;
						} 
						else if(ptr_board[x+i][y-i]==11 && x+i<8 && y-i>0){
							check_unit[x+i][y-i]=true;
						}
				}

				for(int i=1; i<8; i++){   //���� �� �밢�� �˻� 
						if(ptr_board[x-i][y-i]<17 && ptr_board[x-i][y-i]!=0 && ptr_board[x-i][y-i]!=11 ){  //�Ʊ��϶�
						break;
						} 
						else if(ptr_board[x-i][y-i]==11 && x-i>0 && y-i>0){
							check_unit[x-i][y-i]=true;
						}
				}


				for(int i=1; i<8; i++){   //���� �Ʒ� �밢�� �˻� 
				
						if(ptr_board[x-i][y+i]<17 && ptr_board[x-i][y+i]!=0 && ptr_board[x-i][y+i]!=11 ){  //�Ʊ��϶�
						break;
						} 

						else if(ptr_board[x-i][y+i]==11 && x-i>0 && y+i<8){
							check_unit[x-i][y+i]=true;
						}
					}

			
				break;




				case 3:  //ȭ��Ʈ ���

						
				for(int i=1; i<8; i++){  //������ �Ʒ� �밢�� �˻�
						if(ptr_board[x+i][y+i]<17 && ptr_board[x+i][y+i]!=0 && ptr_board[x+i][y+i]!=11){  //�Ʊ��϶�
						break;
						} 
						else if(ptr_board[x+i][y+i]==11 && x+i<8 && y+i<8){
							check_unit[x+i][y+i]=true;
						}
				}

				for(int i=1; i<8; i++){   //������ �� �밢�� �˻� 
						if(ptr_board[x+i][y-i]<17 && ptr_board[x+i][y-i]!=0 && ptr_board[x+i][y-i]!=11){  //�Ʊ��϶�
						break;
						} 
						else if(ptr_board[x+i][y-i]==11 && x+i<8 && y-i>0){
							check_unit[x+i][y-i]=true;
						}
				}

				for(int i=1; i<8; i++){   //���� �� �밢�� �˻� 
						if(ptr_board[x-i][y-i]<17 && ptr_board[x-i][y-i]!=0 && ptr_board[x-i][y-i]!=11 ){  //�Ʊ��϶�
						break;
						} 
						else if(ptr_board[x-i][y-i]==11&& x-i>0 && y-i>0){
							check_unit[x-i][y-i]=true;
						}
				}


				for(int i=1; i<8; i++){   //���� �Ʒ� �밢�� �˻� 
				
						if(ptr_board[x-i][y+i]<17 && ptr_board[x-i][y+i]!=0 && ptr_board[x-i][y+i]!=11 ){  //�Ʊ��϶�
						break;
						} 

						else if(ptr_board[x-i][y+i]==11 && x-i>0 && y+i<8){
							check_unit[x-i][y+i]=true;
						}
				}
				break;


				case 4:  //ȭ��Ʈ ����Ʈ
			
					if(x<7 && y<6){
						if(ptr_board[x+1][y+2]==11){
						check_unit[x+1][y+2]=true;
						} 
				}
		
				if(x<7 && y>1){
					if(ptr_board[x+1][y-2]==11){
						check_unit[x+1][y-2]=true;
						}
				}


				if(x>0 && y<6){
					if(ptr_board[x-1][y+2]==11){
						check_unit[x-1][y+2]=true;
						}
				}
			
				if(x>0 && y>1){
					if(ptr_board[x-1][y-2]==11){
						check_unit[x-1][y-2]=true;
						}
				}

			

			
				if(x<6 && y<7){
					if(ptr_board[x+2][y+1]==11){
						check_unit[x+2][y+1]=true;
						}
				}

				if(x<6 && y>0){
					if(ptr_board[x+2][y-1]==11){
						check_unit[x+2][y-1]=true;
						}
				}

				if(x>1 && y<7){
					if(ptr_board[x-2][y+1]==11){
						check_unit[x-2][y+1]=true;
						}
				}

				if(x>1 && y>0){
					if(ptr_board[x-2][y-1]==11){
						check_unit[x-2][y-1]=true;
						}
				}
	
		
				break;

				 //ȭ��Ʈ��
				case 5:
					for(int i=1; i<8; i++){   ///���� �˻�
					if(ptr_board[x][y+i]<17 && ptr_board[x][y+i]!=0 && ptr_board[x][y+i]!=11){  //�Ʊ��϶�
						break;
					}
					else if(ptr_board[x][y+i]==11 && y+i<8){
						check_unit[x][y+i]=true;
					}
				}
				for(int j=1; j<8; j++){  //���� �˻�
					if(ptr_board[x][y-j]<17 && ptr_board[x][y-j]!=0 && ptr_board[x][y-j]!=11){  //�Ʊ��϶�
						break;
					} 
					else if(ptr_board[x][y-j]==11 && y-j>0){
						check_unit[x][y-j]=true;
					}
				}
				for(int a=1; a<8; a++){  //�ϴ��� �˻�
					if(ptr_board[x+a][y]<17 && ptr_board[x+a][y]!=0 && ptr_board[x+a][y]!=11){  //�Ʊ��϶�
						break;
					} 

					else if(ptr_board[x+a][y]==11 && x+a<8){
						check_unit[x+a][y]=true;
					}
				}
			
				for(int b=1; b<8; b++){  //����� �˻�
					if(ptr_board[x-b][y]<17 && ptr_board[x-b][y]!=0 && ptr_board[x-b][y]!=11){  //�Ʊ��϶�
						break;
					}
					else if(ptr_board[x-b][y]==11 && x-b>0){
						check_unit[x-b][y]=true;
					}
				}
			
				break;

				 //ȭ��Ʈ��
				case 6:
						if(ptr_board[x+1][y+1]==11 && x<7 && y<7){
						check_unit[x+1][y+1]=true;
						}
						if(ptr_board[x-1][y+1]==11 && x>0 && y<7){
						check_unit[x-1][y+1]=true;
						}
				break;

			}
		}
	}


}

void CChessDlg::BlackCheck(){
	for(int x=0; x<8; x++){
		 for(int y=0; y<8; y++){
			 switch(ptr_board[x][y]) 
			 {
				case 11:  //�� ŷ
				if(ptr_board[x][y+1]==1 && y<7){
						check_unit[x][y+1]=true;
				}
				else if(ptr_board[x][y-1]==1 && y>0){
						check_unit[x][y-1]=true;
				}
				else if(ptr_board[x+1][y]==1 && x<7){
						check_unit[x+1][y]=true;
				}
				else if(ptr_board[x-1][y]==1 && x>0){
						check_unit[x-1][y]=true;
				}
				else if(ptr_board[x+1][y+1]==1 && x<7 && y<7){
						check_unit[x+1][y+1]=true;
				}
				else if(ptr_board[x+1][y-1]==1 && x<7 && y>0){
						check_unit[x+1][y-1]=true;
				}
				else if(ptr_board[x-1][y+1]==1 && x>0 && y<7){
						check_unit[x-1][y+1]=true;
				}
				else if(ptr_board[x-1][y-1]==1 && x>0 && y>0){
						check_unit[x-1][y-1]=true;
				}
				break;
		
				//����		
				case 12:
				for(int i=1; i<8; i++){   ///���� �˻�
					if(ptr_board[x][y+i]>1 && y+i<8 ){ 
						break;
					}
					else if(ptr_board[x][y+i]==1 && y+i<8){
						check_unit[x][y+i]=true;
					}
				}
				for(int j=1; j<8; j++){  //���� �˻�
					if(ptr_board[x][y-j]>1 && y>0){  
						break;
					} 
					else if(ptr_board[x][y-j]==1 && y-j>=0){
						check_unit[x][y-j]=true;
					}
				}
				for(int a=1; a<8; a++){  //�ϴ��� �˻�
					if(ptr_board[x+a][y]>1 && x+a<8){ 
						break;
					} 

					else if(ptr_board[x+a][y]==1 && x+a<8){
						check_unit[x+a][y]=true;
					}
				}
			
				for(int b=1; b<8; b++){  //����� �˻�
					if(ptr_board[x-b][y]>1 && x-b>=0){ 
						break;
					}
					else if(ptr_board[x-b][y]==1 && x-b>=0){
						check_unit[x-b][y]=true;
					}
				}

				for(int i=1; i<8; i++){  //������ �Ʒ� �밢�� �˻�
						if(ptr_board[x+i][y+i]>1 &&x+i<8 && y+i<8){ 
							break;
						} 
						else if(ptr_board[x+i][y+i]==1 && x+i<8 && y+i<8){
							check_unit[x+i][y+i]=true;
						}
				}

				for(int i=1; i<8; i++){   //������ �� �밢�� �˻� 
						if(ptr_board[x+i][y-i]>1 &&x+i<8 && y-i>=0){  
						break;
						} 
						else if(ptr_board[x+i][y-i]==1 && x+i<8 && y-i>=0){
							check_unit[x+i][y-i]=true;
						}
				}

				for(int i=1; i<8; i++){   //���� �� �밢�� �˻� 
						if(ptr_board[x-i][y-i]>1 && x-i>=0 && y-i>=0){  
						break;
						} 
						else if(ptr_board[x-i][y-i]==1 && x-i>=0 && y-i>=0){
							check_unit[x-i][y-i]=true;
						}
				}


				for(int i=1; i<8; i++){   //���� �Ʒ� �밢�� �˻� 
				
						if(ptr_board[x-i][y+i]>1 && x-i>=0 && y+i<8){  
						break;
						} 

						else if(ptr_board[x-i][y+i]==1 && x-i>=0 && y+i<8){
							check_unit[x-i][y+i]=true;
						}
					}

					 break;

		  //�� ���
					case 13:
				for(int i=1; i<8; i++){  //������ �Ʒ� �밢�� �˻�
						if(ptr_board[x+i][y+i]>1 &&x+i<8 && y+i<8){ 
							break;
						} 
						else if(ptr_board[x+i][y+i]==1 && x+i<8 && y+i<8){
							check_unit[x+i][y+i]=true;
						}
				}

				for(int i=1; i<8; i++){   //������ �� �밢�� �˻� 
						if(ptr_board[x+i][y-i]>1 &&x+i<8 && y-i>=0){  
						break;
						} 
						else if(ptr_board[x+i][y-i]==1 && x+i<8 && y-i>=0){
							check_unit[x+i][y-i]=true;
						}
				}

				for(int i=1; i<8; i++){   //���� �� �밢�� �˻� 
						if(ptr_board[x-i][y-i]>1 && x-i>=0 && y-i>=0){  
						break;
						} 
						else if(ptr_board[x-i][y-i]==1 && x-i>=0 && y-i>=0){
							check_unit[x-i][y-i]=true;
						}
				}


				for(int i=1; i<8; i++){   //���� �Ʒ� �밢�� �˻� 
				
						if(ptr_board[x-i][y+i]>1 && x-i>=0 && y+i<8){  
						break;
						} 

						else if(ptr_board[x-i][y+i]==1 && x-i>=0 && y+i<8){
							check_unit[x-i][y+i]=true;
						}
					}
			
					break;



		 //������Ʈ
					case 14:
						if(x<7 && y<6){
							if(ptr_board[x+1][y+2]==1){
							check_unit[x+1][y+2]=true;	
							} 
					}
		
					if(x<7 && y>1){
						if(ptr_board[x+1][y-2]==1){
							check_unit[x+1][y-2]=true;
							}
					}


					if(x>0 && y<6){
						if(ptr_board[x-1][y+2]==1){
							check_unit[x-1][y+2]=true;
							}
					}
			
					if(x>0 && y>1){
						if(ptr_board[x-1][y-2]==1){
							check_unit[x-1][y-2]=true;
							}
					}

			

			
					if(x<6 && y<7){
						if(ptr_board[x+2][y+1]==1){
							check_unit[x+2][y+1]=true;
							}
					}

					if(x<6 && y>0){
						if(ptr_board[x+2][y-1]==1){
							check_unit[x+2][y-1]=true;
							}
					}

					if(x>1 && y<7){
						if(ptr_board[x-2][y+1]==1){
							check_unit[x-2][y+1]=true;
							}
					}

					if(x>1 && y>0){
						if(ptr_board[x-2][y-1]==1){
							check_unit[x-2][y-1]=true;
							}
					}

			
				

					break;

		 //����
					case 15:
				for(int i=1; i<8; i++){   ///���� �˻�
					if(ptr_board[x][y+i]>1 && y<7 && ptr_board[x][y+i]<17){ 
						break;
					}
					else if(ptr_board[x][y+i]==1 && y+i<8){
						check_unit[x][y+i]=true;
					}
				}
				for(int j=1; j<8; j++){  //���� �˻�
					if(ptr_board[x][y-j]>1 && y>0){  
						break;
					} 
					else if(ptr_board[x][y-j]==1 && y-j>=0){
						check_unit[x][y-j]=true;
					}
				}
				for(int a=1; a<8; a++){  //�ϴ��� �˻�
					if(ptr_board[x+a][y]>1 && x+a<8){ 
						break;
					} 

					else if(ptr_board[x+a][y]==1 && x+a<8){
						check_unit[x+a][y]=true;
					}
				}
			
				for(int b=1; b<8; b++){  //����� �˻�
					if(ptr_board[x-b][y]>1 && x-b>=0){ 
						break;
					}
					else if(ptr_board[x-b][y]==1 && x-b>=0){
						check_unit[x-b][y]=true;
					}
				}
					break;

			 //����   
					case 16:
							if(ptr_board[x-1][y-1]==1){
								check_unit[x-1][y-1]=true;
							}			
							if(ptr_board[x+1][y-1]==1){
								check_unit[x+1][y-1]=true;
							}
							
					 break;

					}
			
		}
		
	}

}

void CChessDlg::OnBnClickedResetbutton()
{
	CString reset("reset");
	isMove=true;
	rbutton_possible=true;
	Set_Game(); 
	UnitDraw();
	Reset_mPoint();
	theApp.SendData(reset);

	CString strInsert;
	strInsert.Format(L"***************GAME START***************");
	int sel=m_listChat.InsertString(-1,strInsert);
	m_listChat.SetCurSel(sel);

	ss=61;
	Timer();
}

void CChessDlg::Check(){
	CString check;
	if(turn==false){
					 if(check_unit[set_Bking_x][set_Bking_y]){
						  check="white";
						  theApp.SendData(check);
						  delay(300);
					 }
	}
	else {
					 if(check_unit[set_Wking_x][set_Wking_y]){
						  check="black";
			  			  theApp.SendData(check);
						  delay(300);
					  }				 
	}
}

void CChessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CString szTemp;

    switch (nIDEvent)
    {

    case IDC_STOPWATCH:
		ss--;
		szTemp.Format(L"���� �ð�:%02d��", ss);
		m_timer.SetWindowText(szTemp);

		if (ss == 0)
		{
			isMove=false;
			rbutton_possible=false;
			Timer();
			GetDlgItem(IDC_RESETBUTTON)->EnableWindow(true);

			if(turn){
				theApp.SendData(ybw);
				bScore++;
				bScore_str.Format(_T("%d"),bScore);
				m_bScore.SetWindowTextW(bScore_str);
				
			} else{
				theApp.SendData(yww);
				wScore++;
				wScore_str.Format(_T("%d"),wScore);
				m_wScore.SetWindowTextW(wScore_str);
			}
			MessageBox(L"Time Out. You lose.");
			break;
		}

        break;
		
	default:
		break;

   }

	CDialogEx::OnTimer(nIDEvent);
}

void CChessDlg::Timer(){
	if (m_bRun)
		{
		m_bRun = FALSE;
		KillTimer(IDC_STOPWATCH);
		}
	else
		{
		m_bRun = TRUE;
		SetTimer(IDC_STOPWATCH, 1000, NULL);
		}
}

void CChessDlg::OnDestroy()
{
	KillTimer(IDC_STOPWATCH);
	CDialogEx::OnDestroy();
}

void CChessDlg::OnBnClickedButton2()// RANKING ��ư
{
	CRankingDialog dlg;

	dlg.user = externUsername;


	dlg.DoModal();
	

	
}

//}




void CChessDlg::OnBnClickedButton3()// SIGN_UP ��ư 
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CSignUp dlg;
	
	
	CString m_strUsername;
	CString m_strPassword;
	USES_CONVERSION;

	if (dlg.DoModal() == IDOK) {

		
		CString query1;

		

		query1.Format(_T("insert into member (id,pw) values ('%s', password('%s'))"), dlg.strID, dlg.strPwd);
		//sprintf(query, "insert into member (id,pw) values ('%s', password('%s'))", m_strUsername, m_strPassword);
		//query_stat = mysql_query(conn, query);
		int status = mysql_query(&m_mysql, CW2A(query1));

	}

}


BOOL CChessDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	mysql_close(&m_mysql);
	return CDialogEx::DestroyWindow();
}


void CChessDlg::OnBnClickedButton1()
{
	 //TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strButton1;
	
	CString strPassword;

	this->GetDlgItemText(IDC_BUTTON1, strButton1);

	if (!strButton1.Compare(_T("LOGIN"))) {
		this->SetDlgItemText(IDC_BUTTON1, _T("LOGOUT"));
		this->GetDlgItemText(IDC_EDIT1, externUsername);
		this->GetDlgItemText(IDC_EDIT2, strPassword);

		CString query;
		query.Format(_T("select * from member where id='%s' and pw=password('%s')"), externUsername, strPassword);
		USES_CONVERSION;
		mysql_query(&m_mysql, CW2A(query));
		MYSQL_RES *result = mysql_store_result(&m_mysql);
		int nRowCount = mysql_num_rows(result);
		if (nRowCount > 0) {
			MYSQL_ROW row = mysql_fetch_row(result);
			int score = atoi(row[2]);

			CString strInsert;
			strInsert.Format(_T("������ %d���� %s���� �α��� �Ͽ����ϴ�."), score, externUsername);
			int sel = m_listChat.InsertString(-1, strInsert);
			m_listChat.SetCurSel(sel);

			this->SetDlgItemText(IDC_BUTTON1, _T("LOGOUT"));
		}
	}
	else {
		this->SetDlgItemText(IDC_BUTTON1, _T("LOGIN"));
		CString strInsert;
		strInsert.Format(_T("�α׾ƿ� �Ͽ����ϴ�."));
		int sel = m_listChat.InsertString(-1, strInsert);
		m_listChat.SetCurSel(sel);
	}
	
}
