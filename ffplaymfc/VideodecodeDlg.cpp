/* 
 * FFplay for MFC
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 *
 * http://blog.csdn.net/leixiaohua1020
 * 
 * 本工程将ffmpeg项目中的ffplay播放器（ffplay.c）移植到了VC的环境下。
 * 并且使用MFC做了一套简单的界面。
 * This software transplant ffplay to Microsoft VC++ environment. 
 * And use MFC to build a simple Graphical User Interface. 
 */

#include "stdafx.h"
#include "ffplaymfc.h"
#include "VideodecodeDlg.h"
#include "afxdialogex.h"


// VideodecodeDlg 对话框

IMPLEMENT_DYNAMIC(VideodecodeDlg, CDialogEx)

VideodecodeDlg::VideodecodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(VideodecodeDlg::IDD, pParent)
{

}

VideodecodeDlg::~VideodecodeDlg()
{
}

void VideodecodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIDEODECODE_LIST, m_videodecodelist);
}


BEGIN_MESSAGE_MAP(VideodecodeDlg, CDialogEx)
	ON_NOTIFY ( NM_CUSTOMDRAW,IDC_VIDEODECODE_LIST, VideodecodeDlg::OnCustomdrawMyList )
    ON_WM_SIZE()
END_MESSAGE_MAP()


// VideodecodeDlg 消息处理程序
BOOL VideodecodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//整行选择；有表格线；表头；单击激活
	DWORD dwExStyle=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE;
	//报表风格；单行选择；高亮显示选择行


    MoveWindow(GetSystemMetrics(SM_CXSCREEN)/2 + 400, 200, 400, 300);
    

    CRect rc;
    GetClientRect(&rc);
    GetDlgItem(IDC_VIDEODECODE_LIST)->MoveWindow(rc);//用对话框客户区大小重新定义列表控件的位置

    m_videodecodelist.GetClientRect(&rc);
    int nColumnWidth = rc.Width() / 5;

	//多国语言支持
	CString resloader;
	resloader.LoadString(IDS_VIDEODECODE);
	SetWindowText(resloader);

	m_videodecodelist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_videodecodelist.SetExtendedStyle(dwExStyle);
	resloader.LoadString(IDS_VIDEODECODE_NUM);
    m_videodecodelist.InsertColumn(0, resloader, LVCFMT_CENTER, nColumnWidth, 0);
	resloader.LoadString(IDS_VIDEODECODE_FRAMETYPE);
    m_videodecodelist.InsertColumn(1, resloader, LVCFMT_CENTER, nColumnWidth, 0);
	resloader.LoadString(IDS_VIDEODECODE_KEYFRAME);
    m_videodecodelist.InsertColumn(2, resloader, LVCFMT_CENTER, nColumnWidth, 0);
	resloader.LoadString(IDS_VIDEODECODE_CODENUM);
    m_videodecodelist.InsertColumn(3, resloader, LVCFMT_CENTER, nColumnWidth, 0);
	resloader.LoadString(IDS_VIDEODECODE_PTS);
    m_videodecodelist.InsertColumn(4, resloader, LVCFMT_CENTER, nColumnWidth, 0);
  
	return TRUE;
}

//ListCtrl加颜色
void VideodecodeDlg::OnCustomdrawMyList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	//This code based on Michael Dunn's excellent article on
	//list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{

		COLORREF clrNewTextColor, clrNewBkColor;

		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		CString strTemp = m_videodecodelist.GetItemText(nItem,1);
		if(strTemp.Compare(_T("I"))==0){
			clrNewTextColor = RGB(0,0,0);		//Set the text
			clrNewBkColor = RGB(255,0,0);		//背景设置成红色
		}
		else if(strTemp.Compare(_T("P"))==0){
			clrNewTextColor = RGB(0,0,0);		
			clrNewBkColor = RGB(0,255,255);		//背景设置成青色
		}
		else if(strTemp.Compare(_T("B"))==0){
			clrNewTextColor = RGB(0,0,0);		
			clrNewBkColor = RGB(0,255,0);		//背景设置成绿色
		}else{
			clrNewTextColor = RGB(0,0,0);		
			clrNewBkColor = RGB(255,255,255);
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;


		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;


	}
}

void VideodecodeDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    
    // TODO:  在此处添加消息处理程序代码

    CWnd *pWnd;
    pWnd = GetDlgItem(IDC_VIDEODECODE_LIST);
    if (pWnd&&nType != SIZE_MINIMIZED)//当对话框工程刚打开时不执行此代码
    {
        CRect rc;
        GetClientRect(&rc);
        pWnd->MoveWindow(rc);

        m_videodecodelist.GetClientRect(&rc);
        int nColumnWidth = rc.Width() / 5;

        m_videodecodelist.SetColumnWidth(0, nColumnWidth);
        m_videodecodelist.SetColumnWidth(1, nColumnWidth);
        m_videodecodelist.SetColumnWidth(2, nColumnWidth);
        m_videodecodelist.SetColumnWidth(3, nColumnWidth);
        m_videodecodelist.SetColumnWidth(4, nColumnWidth);
    }

}
