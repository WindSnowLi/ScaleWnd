#include "pch.h"

#include "CScaleWnd.h"

CScaleWnd::CScaleWnd()
{
}

CScaleWnd::CScaleWnd(UINT nIDTemplate, CWnd* pParent) : CDialogEx(nIDTemplate, pParent)
{
}

BEGIN_MESSAGE_MAP(CScaleWnd, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CScaleWnd::ReSize(void)
{
	float fsp[2]{};
	POINT Newp{}; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / m_width;
	fsp[1] = (float)Newp.y / m_heigth;
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);
	while (hwndChild) {
		int woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		CRect rect = m_table[woc];
		rect.left = long(rect.TopLeft().x * fsp[0]);
		rect.top = long(rect.TopLeft().y * fsp[1]);
		rect.right = long(rect.BottomRight().x * fsp[0]);
		rect.bottom = long(rect.BottomRight().y * fsp[1]);
		GetDlgItem(woc)->MoveWindow(rect, TRUE);

		CFont* oldFont = m_fontTable[woc];
		LOGFONT lf;
		oldFont->GetLogFont(&lf);
		float change = fsp[0] > fsp[1] ? fsp[1] : fsp[0];
		lf.lfWidth = lf.lfWidth * change;
		lf.lfHeight = lf.lfHeight * change;

		CFont* font = new CFont();
		font->CreateFontIndirectW(&lf);
		GetDlgItem(woc)->SetFont(font);
		::DeleteObject(m_fontNewTable[woc]);
		delete m_fontNewTable[woc];
		m_fontNewTable[woc] = font;
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}

BOOL CScaleWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;
	GetClientRect(&rect);  //获取对话框的大小
	m_width = rect.right - rect.left;
	m_heigth = rect.bottom - rect.top;
	CPoint  tlPoint, brPoint;
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);
	while (hwndChild) {
		int woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(&rect);
		LOGFONT lf;
		GetDlgItem(woc)->GetFont()->GetLogFont(&lf);
		ScreenToClient(&rect);
		tlPoint = rect.TopLeft();
		brPoint = rect.BottomRight();
		rect.SetRect(tlPoint, brPoint);
		m_table.insert(std::make_pair(woc, rect));
		CFont* font = new CFont();
		font->CreateFontIndirectW(&lf);
		m_fontTable.emplace(std::make_pair(woc, GetDlgItem(woc)->GetFont()));
		m_fontNewTable.emplace(std::make_pair(woc, font));
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	return TRUE;
}


void CScaleWnd::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);
	//窗口最小化不操作
	if (nType != SIZE_MINIMIZED)
	{
		ReSize();
	}
}