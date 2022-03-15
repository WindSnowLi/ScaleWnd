# MFC等比例放大控件与文字，等比例自适应控件位置

## 头文件定义

```cpp
#pragma once

#include "framework.h"

#include <map>
#include <tuple>

class CScaleWnd : public CDialogEx
{
public:
    // 标准构造函数
    CScaleWnd();
    CScaleWnd(UINT nIDTemplate, CWnd* pParent = NULL);
protected:
    // 原始窗口宽高
    int m_width, m_heigth;
    // 控件ID映射原始窗口位置、字体信息
    std::map<int, CRect> m_table;
    // 控件ID映射原始字体
    std::map<int, CFont*> m_fontTable;
    // 控件ID映射原始字体
    std::map<int, CFont*> m_fontNewTable;
    // 重设刷新控件位置
    void ReSize(void);
    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()
};


```

## 具体实现

```cpp
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
```

## 测试效果

1. 初始

![在这里插入图片描述](https://img-blog.csdnimg.cn/b1f6f867532640c1980f20a14c59db9e.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAd2luZFNub3dMaQ==,size_19,color_FFFFFF,t_70,g_se,x_16)

2. 横向拉宽

![在这里插入图片描述](https://img-blog.csdnimg.cn/854bed1bd84e4c768c1be2e983ff0827.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAd2luZFNub3dMaQ==,size_20,color_FFFFFF,t_70,g_se,x_16)

3. 纵向拉高

![在这里插入图片描述](https://img-blog.csdnimg.cn/36f716b9e0414375ba0e317160e45105.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAd2luZFNub3dMaQ==,size_20,color_FFFFFF,t_70,g_se,x_16)

4. 整体缩小

![在这里插入图片描述](https://img-blog.csdnimg.cn/ba0a71687fe84eee9f0dec484836fe09.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAd2luZFNub3dMaQ==,size_11,color_FFFFFF,t_70,g_se,x_16)
