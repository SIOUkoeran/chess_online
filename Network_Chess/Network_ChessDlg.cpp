
// Network_ChessDlg.cpp : 구현 파일
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


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CChessDlg 대화 상자




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




// CChessDlg 메시지 처리기

BOOL CChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	mysql_init(&m_mysql);
	mysql_set_character_set(&m_mysql, "euckr");
	MYSQL *conn = mysql_real_connect(&m_mysql, "localhost", "user1", "pass01",
		"db1", 3306, (char *)NULL, 0);
	if (conn == NULL) {
		AfxMessageBox((LPCTSTR)mysql_error(&m_mysql));
		::PostQuitMessage(0);
		return FALSE;
		
	}
	
	

	

	Set_Game(); //말 초기 위치 설정

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

	turn = true; //true일 때 백색 차례 false일 때 흑색 차례

	bitmapMAP.LoadBitmap(IDB_BITMAP1); // 체스맵
	bWB.LoadBitmap(IDB_BITMAP2); //화이트비숍
	bWK.LoadBitmap(IDB_BITMAP3); //화이트킹
	bWKN.LoadBitmap(IDB_BITMAP4); //화이트나이트
	bWP.LoadBitmap(IDB_BITMAP5); //화이트폰
	bWQ.LoadBitmap(IDB_BITMAP6); //화이트퀸
	bWR.LoadBitmap(IDB_BITMAP7); //화이트룩

	bBB.LoadBitmap(IDB_BITMAP8); //블랙비숍
	bBK.LoadBitmap(IDB_BITMAP9); //블랙킹
	bBKN.LoadBitmap(IDB_BITMAP10); //블랙나이트
	bBP.LoadBitmap(IDB_BITMAP11); //블랙폰
	bBQ.LoadBitmap(IDB_BITMAP12); //블랙퀸
	bBR.LoadBitmap(IDB_BITMAP13); //블랙룩
	
	BMask.LoadBitmap(IDB_BITMAP14); //비숍마스크
	KMask.LoadBitmap(IDB_BITMAP15); //킹마스크
	KNMask.LoadBitmap(IDB_BITMAP16); //나이트마스크
	PMask.LoadBitmap(IDB_BITMAP17); //폰마스크
	QMask.LoadBitmap(IDB_BITMAP18); //퀸마스크
	RMask.LoadBitmap(IDB_BITMAP19); //룩마스크

	moveArea.LoadBitmap(IDB_BITMAP20); //이동지점
	moveArea_Mask.LoadBitmap(IDB_BITMAP21); //이동지점 마스크
	dangerArea.LoadBitmap(IDB_BITMAP22); //적 잡을수 있는 영역 표시
	backGround.LoadBitmap(IDB_BITMAP23); // 모든 것을 그릴 흰 배경
	Reset_mPoint(); // 이동보드 초기화
	Reset_Check(); // 체크보드 초기화
	ptr_board=board; // 포인터로 board변수에 접근

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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	UnitDraw();
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChessDlg::OnClickedRadioServer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_IPAddress.SetWindowTextW(m_strMyIP);
	m_IPAddress.EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT,_T("Open"));
	isMove = false;
	turn = false;
	rbutton_possible=false;
}

void CChessDlg::OnClickedRadioClient()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_IPAddress.SetWindowTextW(m_strMyIP);
	m_IPAddress.EnableWindow(TRUE);
	SetDlgItemText(IDC_BUTTON_CONNECT,_T("Connect"));
	isMove = false;
	turn = false;
}

void CChessDlg::OnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
			AfxMessageBox(_T("접속할 서버의 IP주소를 입력하세요"));
		}
	}
}

void CChessDlg::OnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strSend, strInsert, strChat;

	strChat="*chat*";
	theApp.SendData(strChat);
	delay(300);

	GetDlgItemText(IDC_SEND,strSend);
	strInsert.Format(_T("[나]:%s"), strSend);
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
	}else if(chating){   //채팅 메세지로 온 경우
		CString strInsert;
		//strInsert.Format(_T("[%s]:%s"),nickName, strReceive);
		strInsert.Format(_T("[상대방]:%s"), strReceive);
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);
		chating=false;
	}
	else if(strReceive=="W"){  //White에게 차례 넘김
		delay(5);
		UnitDraw();
		isMove=true;
		turn=true;
		rbutton_possible=true;
		ss=61;
		Timer();
	}
	else if(strReceive=="B"){ //Black에게 차례 넘김
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
	else if(strReceive=="white"){ //체크
		delay(5);
		CString strInsert;
		strInsert.Format(L"********Check! White King이 위험합니다.********");
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);
	}
	else if(strReceive=="black"){
		delay(5);
		CString strInsert;
		strInsert.Format(L"********Check! Black King이 위험합니다.********");
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);
	}
	else if(strReceive=="ww"){  //백색 승리
		delay(5);
		GetDlgItem(IDC_RESETBUTTON)->EnableWindow(true);
		isMove=false;
		rbutton_possible=false;
		wScore++;
		//wScore NickName 의 순위에 추가!
		wScore_str.Format(_T("%d"),wScore);
		m_wScore.SetWindowTextW(wScore_str);
			
		MessageBox(L"You lose.");
		Timer();
	}
	else if(strReceive=="bw"){  //흑색 승리
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
	else if(strReceive=="yww"){  //백색 기권승
		
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
	else if(strReceive=="ybw"){  //흑색 기권승
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
	else if(strReceive=="reset"){  //게임 재시작
		delay(5);
		Set_Game(); 
		UnitDraw();
		Reset_mPoint();

		CString strInsert;
		strInsert.Format(L"***************GAME START***************");
		int sel=m_listChat.InsertString(-1,strInsert);
		m_listChat.SetCurSel(sel);
	}
	else if(strReceive=="accept"){  // Client(Black)이 접속한 경우
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
	else if(chating==false){  //채팅이 아닌 경우 (말의 움직임)

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

		if(net_y==4 && ptr_board[net_x][net_y]==6 && ptr_board[netMove_x][netMove_y-1]==16 && 0<netMove_x<7)  // inPassing 알고리즘
		{
			ptr_board[netMove_x][netMove_y-1]=0;
		}else if(net_y==3 && ptr_board[net_x][net_y]==16 && ptr_board[netMove_x][netMove_y+1]==6 && 0<netMove_x<7){  // inPassing 알고리즘
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

	help.Format(_T("이동 할 곳을 선택해 주세요.(다시 선택하려면 'Right Mouse Button Click.')"));
	if((ptr_board[setX][setY])==0){  // 아무것도 없는 곳을 클릭하면 이동모드 유지
		isMove=true;
	}
	if(turn){   // 백색팀의 이동
		if((ptr_board[setX][setY]) != 0 && ptr_board[setX][setY] <10){
			{
				dc.TextOut(20,20,help);
				isMove=false;
				Move(setX,setY);
			}
		
		}
	} else {   // 흑색팀의 이동
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

		//좌표값 전송
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

			m_timer.SetWindowText(L"남은 시간:60초");

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

			m_timer.SetWindowText(L"남은 시간:60초");

			ss=61;
			Timer();
			delay(300);
			rbutton_possible=false;
			}	

		}
	}
	
}

void CChessDlg::Swap(int &x,int &y, int &move_X, int &move_Y){
	
	
	 //////////// inPassing 알고리즘 /////////////

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
				ptr_board[x][y]=0;  // 말 겹치면 지워버림
			}	
	 }


	 CString bwin, wwin;
	 bwin="bw";
	 wwin="ww";

	 if(SetB==1){ //흑색 승리
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

	 if(SetB==11){ //백색 승리
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
	case 1:  //화이트 킹

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


	case 11:  //블랙 킹
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




	case 2:  //화이트 퀸

		for(int i=1; i<8; i++){   ///우측 검사
			if(y+i>7)break;
			if(ptr_board[x][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y+i]=true;


			} else if(ptr_board[x][y+i]>10){  //흑색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y+i]=true;

				break;
			} else if(ptr_board[x][y+i]<10){  //백색일때
				move_point[x][y+i]=false;
				break;
			} 
		}

		for(int j=1; j<8; j++){  //좌측 검사
			if(y-j<0)break;
			if(ptr_board[x][y-j]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y-j]=true;
			} else if(ptr_board[x][y-j]>10){  //흑색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-j]=true;
				break;
			} else if(ptr_board[x][y-j]<10){  //백색일때
				move_point[x][y-j]=false;
				break;
			} 
		}
		for(int a=1; a<8; a++){  //하단측 검사
			if(x+a>7)break;
			if(ptr_board[x+a][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+a][y]=true;
			} else if(ptr_board[x+a][y]>10){  //흑색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+a][y]=true;
				break;
			} else if(ptr_board[x+a][y]<10){  //백색일때
				move_point[x+a][y]=false;
				break;
			} 
		}

		for(int b=1; b<8; b++){  //상단측 검사
			if(x-b<0)break;
			if(ptr_board[x-b][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-b][y]=true;
			} else if(ptr_board[x-b][y]>10){  //흑색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-b][y]=true;
				break;
			} else if(ptr_board[x-b][y]<10){  //백색일때
				move_point[x-b][y]=false;
				break;
			} 
		}
		for(int i=1; i<8; i++){  //오른쪽 아래 대각선 검사
			if(x+i>7 || y+i>7 )break;
			if(ptr_board[x+i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y+i]=true;
			} else if(ptr_board[x+i][y+i]>10){  //흑색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y+i]=true;
				break;
			} else if(ptr_board[x+i][y+i]<10){  //백색일때
				move_point[x+i][y+i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //오른쪽 위 대각선 검사 
			if(x+i>7 || y-i<0 )break;
			if(ptr_board[x+i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y-i]=true;
			} else if(ptr_board[x+i][y-i]>10){  //흑색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y-i]=true;
				break;
			} else if(ptr_board[x+i][y-i]<10){  //백색일때
				move_point[x+i][y-i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //왼쪽 위 대각선 검사 
			if(x-i<0 || y-i<0 )break;
			if(ptr_board[x-i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y-i]=true;
			} else if(ptr_board[x-i][y-i]>10){  //흑색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y-i]=true;
				break;
			} else if(ptr_board[x-i][y-i]<10){  //백색일때
				move_point[x-i][y-i]=false;
				break;
			} 
		}
		for(int i=1; i<8; i++){   //왼쪽 아래 대각선 검사 
			if(x-i<0 || y+i>7 )break;
			if(ptr_board[x-i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y+i]=true;
			} else if(ptr_board[x-i][y+i]>10){  //흑색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y+i]=true;
				break;
			} else if(ptr_board[x-i][y+i]<10){  //백색일때
				move_point[x-i][y+i]=false;
				break;
			} 
		}
		break;

	case 12:  //블랙퀸
		for(int i=1; i<8; i++){   ///우측 검사
			if(y+i>7)break;
			if(ptr_board[x][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y+i]=true;
			} else if(ptr_board[x][y+i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y+i]=true;
				break;
			} else if(ptr_board[x][y+i]>10){  //흑색일때
				move_point[x][y+i]=false;
				break;
			} 
		}
		for(int j=1; j<8; j++){  //좌측 검사
			if(y-j<0)break;
			if(ptr_board[x][y-j]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y-j]=true;
			} else if(ptr_board[x][y-j]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-j]=true;
				break;
			} else if(ptr_board[x][y-j]>10){  //흑색일때
				move_point[x][y-j]=false;
				break;
			} 
		}
		for(int a=1; a<8; a++){  //하단측 검사
			if(x+a>7)break;
			if(ptr_board[x+a][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+a][y]=true;
			} else if(ptr_board[x+a][y]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+a][y]=true;
				break;
			} else if(ptr_board[x+a][y]>10){  //흑색일때
				move_point[x+a][y]=false;
				break;
			} 
		}
		for(int b=1; b<8; b++){  //상단측 검사
			if(x-b<0)break;
			if(ptr_board[x-b][y]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x-b][y]=true;
			} else if(ptr_board[x-b][y]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-b][y]=true;
				break;
			} else if(ptr_board[x-b][y]>10){  //흑색일때
				move_point[x-b][y]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){  //오른쪽 아래 대각선 검사
			if(x+i>7 || y+i>7 )break;
			if(ptr_board[x+i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y+i]=true;
			} else if(ptr_board[x+i][y+i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y+i]=true;
				break;
			} else if(ptr_board[x+i][y+i]>10){  //흑색일때
				move_point[x+i][y+i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //오른쪽 위 대각선 검사 
			if(x+i>7 || y-i<0 )break;
			if(ptr_board[x+i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y-i]=true;
			} else if(ptr_board[x+i][y-i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y-i]=true;
				break;
			} else if(ptr_board[x+i][y-i]>10){  //흑색일때
				move_point[x+i][y-i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //왼쪽 위 대각선 검사 
			if(x-i<0 || y-i<0 )break;
			if(ptr_board[x-i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y-i]=true;
			} else if(ptr_board[x-i][y-i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y-i]=true;
				break;
			} else if(ptr_board[x-i][y-i]>10){  //흑색일때
				move_point[x-i][y-i]=false;
				break;
			} 
		}


		for(int i=1; i<8; i++){   //왼쪽 아래 대각선 검사 
			if(x-i<0 || y+i>7 )break;
			if(ptr_board[x-i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y+i]=true;
			} else if(ptr_board[x-i][y+i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y+i]=true;
				break;
			} else if(ptr_board[x-i][y+i]>10){  //흑색일때
				move_point[x-i][y+i]=false;
				break;
			} 
		}


		break;


	case 3:  //화이트 비숍
		for(int i=1; i<8; i++){  //오른쪽 아래 대각선 검사
			if(x+i>7 || y+i>7 )break;
			if(ptr_board[x+i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y+i]=true;
			} else if(ptr_board[x+i][y+i]>10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y+i]=true;
				break;
			} else if(ptr_board[x+i][y+i]<10){  //흑색일때
				move_point[x+i][y+i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //오른쪽 위 대각선 검사 
			if(x+i>7 || y-i<0 )break;
			if(ptr_board[x+i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y-i]=true;
			} else if(ptr_board[x+i][y-i]>10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y-i]=true;
				break;
			} else if(ptr_board[x+i][y-i]<10){  //흑색일때
				move_point[x+i][y-i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //왼쪽 위 대각선 검사 
			if(x-i<0 || y-i<0 )break;
			if(ptr_board[x-i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y-i]=true;
			} else if(ptr_board[x-i][y-i]>10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y-i]=true;
				break;
			} else if(ptr_board[x-i][y-i]<10){  //흑색일때
				move_point[x-i][y-i]=false;
				break;
			} 
		}


		for(int i=1; i<8; i++){   //왼쪽 아래 대각선 검사 
			if(x-i<0 || y+i>7 )break;
			if(ptr_board[x-i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y+i]=true;
			} else if(ptr_board[x-i][y+i]>10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y+i]=true;
				break;
			} else if(ptr_board[x-i][y+i]<10){  //흑색일때
				move_point[x-i][y+i]=false;
				break;
			} 
		}


		break;

	case 13:  //블랙 비숍

		for(int i=1; i<8; i++){  //오른쪽 아래 대각선 검사
			if(x+i>7 || y+i>7 )break;
			if(ptr_board[x+i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y+i]=true;
			} else if(ptr_board[x+i][y+i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y+i]=true;
				break;
			} else if(ptr_board[x+i][y+i]>10){  //흑색일때
				move_point[x+i][y+i]=false;
				break;
			} 
		}
		for(int i=1; i<8; i++){   //오른쪽 위 대각선 검사 
			if(x+i>7 || y-i<0 )break;
			if(ptr_board[x+i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x+i][y-i]=true;
			} else if(ptr_board[x+i][y-i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+i][y-i]=true;
				break;
			} else if(ptr_board[x+i][y-i]>10){  //흑색일때
				move_point[x+i][y-i]=false;
				break;
			} 
		}

		for(int i=1; i<8; i++){   //왼쪽 위 대각선 검사 
			if(x-i<0 || y-i<0 )break;
			if(ptr_board[x-i][y-i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y-i]=true;
			} else if(ptr_board[x-i][y-i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y-i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y-i]=true;
				break;
			} else if(ptr_board[x-i][y-i]>10){  //흑색일때
				move_point[x-i][y-i]=false;
				break;
			} 
		}


		for(int i=1; i<8; i++){   //왼쪽 아래 대각선 검사 
			if(x-i<0 || y+i>7 )break;
			if(ptr_board[x-i][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x-i][y+i]=true;
			} else if(ptr_board[x-i][y+i]<10){  //백색일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-i)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-i][y+i]=true;
				break;
			} else if(ptr_board[x-i][y+i]>10){  //흑색일때
				move_point[x-i][y+i]=false;
				break;
			} 
		}


		break;

	case 4:  //화이트나이트
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

	case 14: //블랙나이트

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



	case 5:  //화이트룩
		for(int i=1; i<8; i++){
			if(y+i>7)break;
			if(ptr_board[x][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	
				move_point[x][y+i]=true;

			} else if(ptr_board[x][y+i]>10){  //적군일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y+i]=true;
				break;
			} else if(ptr_board[x][y+i]<10){  //아군일때
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
			} else if(ptr_board[x][y-j]>10){  //적군일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-j]=true;
				break;
			} else if(ptr_board[x][y-j]<10){  //아군일때
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
			} else if(ptr_board[x+a][y]>10){  //적군일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+a][y]=true;
				break;
			} else if(ptr_board[x+a][y]<10){  //아군일때
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
			} else if(ptr_board[x-b][y]>10){  //적군일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-b][y]=true;
				break;
			} else if(ptr_board[x-b][y]<10){  //아군일때
				move_point[x-b][y]=false;
				break;
			} 
		}

		break;


	case 15:  //블랙룩

		for(int i=1; i<8; i++){
			if(y+i>7)break;
			if(ptr_board[x][y+i]==0){
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(moveArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);	

				move_point[x][y+i]=true;
			} else if(ptr_board[x][y+i]<10){  //적군일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y+i)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y+i]=true;
				break;
			} else if(ptr_board[x][y+i]>10){  //아군일때
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
			} else if(ptr_board[x][y-j]<10){  //적군일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x)*XY_SIZE,(y-j)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x][y-j]=true;
				break;
			} else if(ptr_board[x][y-j]>10){  //아군일때
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
			} else if(ptr_board[x+a][y]<10){  //적군일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x+a)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x+a][y]=true;
				break;
			} else if(ptr_board[x+a][y]>10){  //아군일때
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
			} else if(ptr_board[x-b][y]<10){  //적군일때
				dcMem.SelectObject(moveArea_Mask);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCAND);
				dcMem.SelectObject(dangerArea);
				dc.BitBlt((x-b)*XY_SIZE,(y)*XY_SIZE+MOVEMASK_SIZE,73,73,&dcMem,0,0,SRCPAINT);
				move_point[x-b][y]=true;
				break;
			} else if(ptr_board[x-b][y]>10){  //아군일때
				move_point[x-b][y]=false;
				break;
			} 
		}

		break;

	case 6: //화이트폰

		if(y==1){      //한번도 움직이지 않은 화이트폰

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

		else {   //움직인 폰

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
	case 16: //블랙폰   
		if(y==6){  //한번도 움직이지 않은 블랙폰

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
		} else {  //움직인 블랙폰

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
		case 1:	//킹				화이트
			dmemDC.SelectObject(&KMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWK);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);

			//룩 캐슬링 알고리즘
			if(ptr_board[0][0]==5 && ptr_board[1][0]==0 && ptr_board[2][0] ==0 && ptr_board[3][0] == 0 && castling_Possible_WK && castling_Possible_W){
				GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(true);
			} else GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(false);

			if(ptr_board[5][0] == 0 && ptr_board[6][0]==0 && ptr_board[7][0]==5 && castling_Possible_WK && castling_Possible_w){
				GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(true);
			} else GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(false);

			set_Bking_x=x;  //체크를 확인하기 위한 변수 저장
			set_Bking_y=y;  //체크를 확인하기 위한 변수 저장

			if(x!=4 || y!=0){
				//룩 캐슬링 알고리즘
				castling_Possible_WK=false;
				GetDlgItem(IDC_BUTTON_WSC)->EnableWindow(castling_Possible_WK);
				GetDlgItem(IDC_BUTTON_WLC)->EnableWindow(castling_Possible_WK);
			}
			break;
		case 2:	//퀸
			dmemDC.SelectObject(&QMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWQ);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 3: //비숍
			dmemDC.SelectObject(&BMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWB);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 4:	//나이트
			dmemDC.SelectObject(&KNMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWKN);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 5:	//룩			
			dmemDC.SelectObject(&RMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWR);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			//룩 캐슬링 알고리즘
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

		case 11: //킹                     //블랙
			dmemDC.SelectObject(&KMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBK);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);

			//룩 캐슬링 알고리즘
			if(ptr_board[0][7]==15 &&ptr_board[1][7]==0 && ptr_board[2][7] ==0 && ptr_board[3][7] == 0 && castling_Possible_BK && castling_Possible_B){
				GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(true);
			} else GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(false);

			if(ptr_board[5][7] == 0 && ptr_board[6][7]==0 && ptr_board[7][7]==15 && castling_Possible_BK && castling_Possible_b){
				GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(true);
			} else GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(false);

			set_Wking_x=x;
			set_Wking_y=y;

			if(x!=4 || y!=7){
				//룩 캐슬링 알고리즘
				castling_Possible_BK=false;
				GetDlgItem(IDC_BUTTON_BSC)->EnableWindow(castling_Possible_BK);
				GetDlgItem(IDC_BUTTON_BLC)->EnableWindow(castling_Possible_BK);
			}
			break;
		case 12:	//퀸
			dmemDC.SelectObject(&QMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBQ);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 13:	//비숍
			dmemDC.SelectObject(&BMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBB);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 14:	//나이트
			dmemDC.SelectObject(&KNMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBKN);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;
		case 15:	//룩
			dmemDC.SelectObject(&RMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bBR);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);


			//룩 캐슬링 알고리즘
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

		case 6: //폰                     //화이트
			dmemDC.SelectObject(&PMask);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCAND);

			dmemDC.SelectObject(&bWP);
			memDC.BitBlt(x*XY_SIZE,y*XY_SIZE-70+BOARD_SIZE,200,200,&dmemDC,0,0,SRCPAINT);
			break;

		case 16: //폰                   //블랙	
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

void CChessDlg::Set_Game(){  //초기값 설정 및 게임 다시시작 할수 있게 해주는 메소드

	board[0][0] = 5;  //룩
	board[1][0] = 4;  //나이트
	board[2][0] = 3;  //비숍				//백색말 초기화
	board[3][0] = 2;  //퀸
	board[4][0] = 1;  //킹
	board[5][0] = 3;  //비숍
	board[6][0] = 4;  //나이트
	board[7][0] = 5;  //룩

	board[0][7] = 15;  //룩
	board[1][7] = 14;
	board[2][7] = 13;
	board[3][7] = 12;					//흑색말 초기화
	board[4][7] = 11;  //킹
	board[5][7] = 13;
	board[6][7] = 14;
	board[7][7] = 15;  //룩

	for(int i=0;i<8;i++)
		{//백색 졸들				
		board[i][1] = 6;
	}
	for(int j=0;j<8;j++){//흑색 졸들
		board[j][6] = 16;
	}

	for(int a=2;a<=5;a++)	{				//말 없는 공간 0으로 채움
		for(int b=0;b<8;b++){
			board[b][a] = 0;
		}
	}

	castling_Possible_B = true; //흑색 롱 캐슬링 가능 여부
	castling_Possible_b = true; //흑색 숏 캐슬링 가능 여부
	castling_Possible_W = true; //백색 롱 캐슬링 가능 여부
	castling_Possible_w = true; //백색 숏 캐슬링 가능 여부

	castling_Possible_BK= true;  //흑색 킹 캐슬링 가능 여부
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
		MessageBox(L"내 차례가 아닙니다.");
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

		MessageBox(L"내 차례가 아닙니다.");
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CChessDlg::OnBnClickedButtonBlc()
{
	CString blc;
	 // 블랙 롱캐슬링
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
		MessageBox(L"내 차례가 아닙니다.");
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CChessDlg::OnBnClickedButtonBsc()
{
	CString bsc;
	//블랙 숏캐슬링
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
		MessageBox(L"내 차례가 아닙니다.");
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;
}

void CChessDlg::delay(clock_t n)
{
	//딜레이를 만들어주는 메소드
  clock_t start = clock();
  while(clock() - start < n);
}

void CChessDlg::WhiteCheck(){
	for(int x=0; x<8; x++){
		 for(int y=0; y<8; y++){
			 switch(ptr_board[x][y]) 
			 {
				case 1: //화이트 킹
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

				case 2:     //화이트 퀸
				for(int i=1; i<8; i++){   ///우측 검사
					if(ptr_board[x][y+i]<17 && ptr_board[x][y+i]!=0 && ptr_board[x][y+i]!=11){  //아군일때
						break;
					}
					else if(ptr_board[x][y+i]==11 && y+i<8){
						check_unit[x][y+i]=true;
					}
				}
				for(int j=1; j<8; j++){  //좌측 검사
					if(ptr_board[x][y-j]<17 && ptr_board[x][y-j]!=0 && ptr_board[x][y-j]!=11){  //아군일때
						break;
					} 
					else if(ptr_board[x][y-j]==11 && y-j>0){
						check_unit[x][y-j]=true;
					}
				}
				for(int a=1; a<8; a++){  //하단측 검사
					if(ptr_board[x+a][y]<17 && ptr_board[x+a][y]!=0 && ptr_board[x+a][y]!=11){  //아군일때
						break;
					} 

					else if(ptr_board[x+a][y]==11 && x+a<8){
						check_unit[x+a][y]=true;
					}
				}
			
				for(int b=1; b<8; b++){  //상단측 검사
					if(ptr_board[x-b][y]<17 && ptr_board[x-b][y]!=0 && ptr_board[x-b][y]!=11){  //아군일때
						break;
					}
					else if(ptr_board[x-b][y]==11 && x-b>0){
						check_unit[x-b][y]=true;
					}
				}

				for(int i=1; i<8; i++){  //오른쪽 아래 대각선 검사
						if(ptr_board[x+i][y+i]<17 && ptr_board[x+i][y+i]!=0 && ptr_board[x+i][y+i]!=11){  //아군일때
						break;
						} 
						else if(ptr_board[x+i][y+i]==11 && x+i<8 && y+i<8){
							check_unit[x+i][y+i]=true;
						}
				}

				for(int i=1; i<8; i++){   //오른쪽 위 대각선 검사 
						if(ptr_board[x+i][y-i]<17 && ptr_board[x+i][y-i]!=0 && ptr_board[x+i][y-i]!=11){  //아군일때
						break;
						} 
						else if(ptr_board[x+i][y-i]==11 && x+i<8 && y-i>0){
							check_unit[x+i][y-i]=true;
						}
				}

				for(int i=1; i<8; i++){   //왼쪽 위 대각선 검사 
						if(ptr_board[x-i][y-i]<17 && ptr_board[x-i][y-i]!=0 && ptr_board[x-i][y-i]!=11 ){  //아군일때
						break;
						} 
						else if(ptr_board[x-i][y-i]==11 && x-i>0 && y-i>0){
							check_unit[x-i][y-i]=true;
						}
				}


				for(int i=1; i<8; i++){   //왼쪽 아래 대각선 검사 
				
						if(ptr_board[x-i][y+i]<17 && ptr_board[x-i][y+i]!=0 && ptr_board[x-i][y+i]!=11 ){  //아군일때
						break;
						} 

						else if(ptr_board[x-i][y+i]==11 && x-i>0 && y+i<8){
							check_unit[x-i][y+i]=true;
						}
					}

			
				break;




				case 3:  //화이트 비숍

						
				for(int i=1; i<8; i++){  //오른쪽 아래 대각선 검사
						if(ptr_board[x+i][y+i]<17 && ptr_board[x+i][y+i]!=0 && ptr_board[x+i][y+i]!=11){  //아군일때
						break;
						} 
						else if(ptr_board[x+i][y+i]==11 && x+i<8 && y+i<8){
							check_unit[x+i][y+i]=true;
						}
				}

				for(int i=1; i<8; i++){   //오른쪽 위 대각선 검사 
						if(ptr_board[x+i][y-i]<17 && ptr_board[x+i][y-i]!=0 && ptr_board[x+i][y-i]!=11){  //아군일때
						break;
						} 
						else if(ptr_board[x+i][y-i]==11 && x+i<8 && y-i>0){
							check_unit[x+i][y-i]=true;
						}
				}

				for(int i=1; i<8; i++){   //왼쪽 위 대각선 검사 
						if(ptr_board[x-i][y-i]<17 && ptr_board[x-i][y-i]!=0 && ptr_board[x-i][y-i]!=11 ){  //아군일때
						break;
						} 
						else if(ptr_board[x-i][y-i]==11&& x-i>0 && y-i>0){
							check_unit[x-i][y-i]=true;
						}
				}


				for(int i=1; i<8; i++){   //왼쪽 아래 대각선 검사 
				
						if(ptr_board[x-i][y+i]<17 && ptr_board[x-i][y+i]!=0 && ptr_board[x-i][y+i]!=11 ){  //아군일때
						break;
						} 

						else if(ptr_board[x-i][y+i]==11 && x-i>0 && y+i<8){
							check_unit[x-i][y+i]=true;
						}
				}
				break;


				case 4:  //화이트 나이트
			
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

				 //화이트룩
				case 5:
					for(int i=1; i<8; i++){   ///우측 검사
					if(ptr_board[x][y+i]<17 && ptr_board[x][y+i]!=0 && ptr_board[x][y+i]!=11){  //아군일때
						break;
					}
					else if(ptr_board[x][y+i]==11 && y+i<8){
						check_unit[x][y+i]=true;
					}
				}
				for(int j=1; j<8; j++){  //좌측 검사
					if(ptr_board[x][y-j]<17 && ptr_board[x][y-j]!=0 && ptr_board[x][y-j]!=11){  //아군일때
						break;
					} 
					else if(ptr_board[x][y-j]==11 && y-j>0){
						check_unit[x][y-j]=true;
					}
				}
				for(int a=1; a<8; a++){  //하단측 검사
					if(ptr_board[x+a][y]<17 && ptr_board[x+a][y]!=0 && ptr_board[x+a][y]!=11){  //아군일때
						break;
					} 

					else if(ptr_board[x+a][y]==11 && x+a<8){
						check_unit[x+a][y]=true;
					}
				}
			
				for(int b=1; b<8; b++){  //상단측 검사
					if(ptr_board[x-b][y]<17 && ptr_board[x-b][y]!=0 && ptr_board[x-b][y]!=11){  //아군일때
						break;
					}
					else if(ptr_board[x-b][y]==11 && x-b>0){
						check_unit[x-b][y]=true;
					}
				}
			
				break;

				 //화이트폰
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
				case 11:  //블랙 킹
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
		
				//블랙퀸		
				case 12:
				for(int i=1; i<8; i++){   ///우측 검사
					if(ptr_board[x][y+i]>1 && y+i<8 ){ 
						break;
					}
					else if(ptr_board[x][y+i]==1 && y+i<8){
						check_unit[x][y+i]=true;
					}
				}
				for(int j=1; j<8; j++){  //좌측 검사
					if(ptr_board[x][y-j]>1 && y>0){  
						break;
					} 
					else if(ptr_board[x][y-j]==1 && y-j>=0){
						check_unit[x][y-j]=true;
					}
				}
				for(int a=1; a<8; a++){  //하단측 검사
					if(ptr_board[x+a][y]>1 && x+a<8){ 
						break;
					} 

					else if(ptr_board[x+a][y]==1 && x+a<8){
						check_unit[x+a][y]=true;
					}
				}
			
				for(int b=1; b<8; b++){  //상단측 검사
					if(ptr_board[x-b][y]>1 && x-b>=0){ 
						break;
					}
					else if(ptr_board[x-b][y]==1 && x-b>=0){
						check_unit[x-b][y]=true;
					}
				}

				for(int i=1; i<8; i++){  //오른쪽 아래 대각선 검사
						if(ptr_board[x+i][y+i]>1 &&x+i<8 && y+i<8){ 
							break;
						} 
						else if(ptr_board[x+i][y+i]==1 && x+i<8 && y+i<8){
							check_unit[x+i][y+i]=true;
						}
				}

				for(int i=1; i<8; i++){   //오른쪽 위 대각선 검사 
						if(ptr_board[x+i][y-i]>1 &&x+i<8 && y-i>=0){  
						break;
						} 
						else if(ptr_board[x+i][y-i]==1 && x+i<8 && y-i>=0){
							check_unit[x+i][y-i]=true;
						}
				}

				for(int i=1; i<8; i++){   //왼쪽 위 대각선 검사 
						if(ptr_board[x-i][y-i]>1 && x-i>=0 && y-i>=0){  
						break;
						} 
						else if(ptr_board[x-i][y-i]==1 && x-i>=0 && y-i>=0){
							check_unit[x-i][y-i]=true;
						}
				}


				for(int i=1; i<8; i++){   //왼쪽 아래 대각선 검사 
				
						if(ptr_board[x-i][y+i]>1 && x-i>=0 && y+i<8){  
						break;
						} 

						else if(ptr_board[x-i][y+i]==1 && x-i>=0 && y+i<8){
							check_unit[x-i][y+i]=true;
						}
					}

					 break;

		  //블랙 비숍
					case 13:
				for(int i=1; i<8; i++){  //오른쪽 아래 대각선 검사
						if(ptr_board[x+i][y+i]>1 &&x+i<8 && y+i<8){ 
							break;
						} 
						else if(ptr_board[x+i][y+i]==1 && x+i<8 && y+i<8){
							check_unit[x+i][y+i]=true;
						}
				}

				for(int i=1; i<8; i++){   //오른쪽 위 대각선 검사 
						if(ptr_board[x+i][y-i]>1 &&x+i<8 && y-i>=0){  
						break;
						} 
						else if(ptr_board[x+i][y-i]==1 && x+i<8 && y-i>=0){
							check_unit[x+i][y-i]=true;
						}
				}

				for(int i=1; i<8; i++){   //왼쪽 위 대각선 검사 
						if(ptr_board[x-i][y-i]>1 && x-i>=0 && y-i>=0){  
						break;
						} 
						else if(ptr_board[x-i][y-i]==1 && x-i>=0 && y-i>=0){
							check_unit[x-i][y-i]=true;
						}
				}


				for(int i=1; i<8; i++){   //왼쪽 아래 대각선 검사 
				
						if(ptr_board[x-i][y+i]>1 && x-i>=0 && y+i<8){  
						break;
						} 

						else if(ptr_board[x-i][y+i]==1 && x-i>=0 && y+i<8){
							check_unit[x-i][y+i]=true;
						}
					}
			
					break;



		 //블랙나이트
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

		 //블랙룩
					case 15:
				for(int i=1; i<8; i++){   ///우측 검사
					if(ptr_board[x][y+i]>1 && y<7 && ptr_board[x][y+i]<17){ 
						break;
					}
					else if(ptr_board[x][y+i]==1 && y+i<8){
						check_unit[x][y+i]=true;
					}
				}
				for(int j=1; j<8; j++){  //좌측 검사
					if(ptr_board[x][y-j]>1 && y>0){  
						break;
					} 
					else if(ptr_board[x][y-j]==1 && y-j>=0){
						check_unit[x][y-j]=true;
					}
				}
				for(int a=1; a<8; a++){  //하단측 검사
					if(ptr_board[x+a][y]>1 && x+a<8){ 
						break;
					} 

					else if(ptr_board[x+a][y]==1 && x+a<8){
						check_unit[x+a][y]=true;
					}
				}
			
				for(int b=1; b<8; b++){  //상단측 검사
					if(ptr_board[x-b][y]>1 && x-b>=0){ 
						break;
					}
					else if(ptr_board[x-b][y]==1 && x-b>=0){
						check_unit[x-b][y]=true;
					}
				}
					break;

			 //블랙폰   
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString szTemp;

    switch (nIDEvent)
    {

    case IDC_STOPWATCH:
		ss--;
		szTemp.Format(L"남은 시간:%02d초", ss);
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

void CChessDlg::OnBnClickedButton2()// RANKING 버튼
{
	CRankingDialog dlg;

	dlg.user = externUsername;


	dlg.DoModal();
	

	
}

//}




void CChessDlg::OnBnClickedButton3()// SIGN_UP 버튼 
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	mysql_close(&m_mysql);
	return CDialogEx::DestroyWindow();
}


void CChessDlg::OnBnClickedButton1()
{
	 //TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
			strInsert.Format(_T("점수가 %d점인 %s님이 로그인 하였습니다."), score, externUsername);
			int sel = m_listChat.InsertString(-1, strInsert);
			m_listChat.SetCurSel(sel);

			this->SetDlgItemText(IDC_BUTTON1, _T("LOGOUT"));
		}
	}
	else {
		this->SetDlgItemText(IDC_BUTTON1, _T("LOGIN"));
		CString strInsert;
		strInsert.Format(_T("로그아웃 하였습니다."));
		int sel = m_listChat.InsertString(-1, strInsert);
		m_listChat.SetCurSel(sel);
	}
	
}
