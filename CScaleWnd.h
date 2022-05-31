#pragma once

#ifndef SCALE_HEAD
#include <map>
#define SCALE_HEAD                            \
    int m_width, m_heigth;                    \
    std::map<CWnd *, CRect> m_scale_table;          \
    std::map<CWnd *, CFont *> m_scale_fontTable;    \
    std::map<CWnd *, CFont *> m_scale_fontNewTable; \
    void ReSize(void);
#endif // !SCALE_HEAD

#ifndef SCALE_CPP_INI

#define SCALE_CPP_INI                                      \
    CRect rect;                                            \
    GetClientRect(&rect);                                  \
    m_width = rect.right - rect.left;                      \
    m_heigth = rect.bottom - rect.top;                     \
    CPoint tlPoint, brPoint;                               \
    HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);        \
    while (hwndChild)                                      \
    {                                                      \
        CWnd *wnd = FromHandle(hwndChild);                 \
        wnd->GetWindowRect(&rect);                         \
        ScreenToClient(&rect);                             \
        tlPoint = rect.TopLeft();                          \
        brPoint = rect.BottomRight();                      \
        rect.SetRect(tlPoint, brPoint);                    \
        m_scale_table.insert(std::make_pair(wnd, rect));         \
        LOGFONT lf;                                        \
        CFont *pfont = wnd->GetFont();                     \
        if (pfont == nullptr)                              \
        {                                                  \
            pfont = new CFont();                           \
            pfont->CreatePointFont(200, L"ËÎÌו");          \
        }                                                  \
        wnd->SetFont(pfont);                               \
        pfont->GetLogFont(&lf);                            \
        CFont *font = new CFont();                         \
        font->CreateFontIndirectW(&lf);                    \
        m_scale_fontTable.emplace(std::make_pair(wnd, pfont));   \
        m_scale_fontNewTable.emplace(std::make_pair(wnd, font)); \
        hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);   \
    }

#endif // !SCALE_CPP_INI

#ifndef RESIZE
#define RESIZE(CLASS_DLG)                                        \
    void CLASS_DLG::ReSize(void)                                 \
    {                                                            \
        float fsp[2]{};                                          \
        POINT Newp{};                                            \
        CRect recta;                                             \
        GetClientRect(&recta);                                   \
        Newp.x = recta.right - recta.left;                       \
        Newp.y = recta.bottom - recta.top;                       \
        fsp[0] = (float)Newp.x / m_width;                        \
        fsp[1] = (float)Newp.y / m_heigth;                       \
        HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);          \
        while (hwndChild)                                        \
        {                                                        \
            CWnd *wnd = FromHandle(hwndChild);                   \
            CRect rect = m_scale_table[wnd];                           \
            rect.left = long(rect.TopLeft().x * fsp[0]);         \
            rect.top = long(rect.TopLeft().y * fsp[1]);          \
            rect.right = long(rect.BottomRight().x * fsp[0]);    \
            rect.bottom = long(rect.BottomRight().y * fsp[1]);   \
            wnd->MoveWindow(rect, TRUE);                         \
            CFont *oldFont = m_scale_fontTable[wnd];                   \
            if (oldFont == nullptr)                              \
            {                                                    \
                hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT); \
                continue;                                        \
            }                                                    \
            LOGFONT lf;                                          \
            oldFont->GetLogFont(&lf);                            \
            float change = fsp[0] > fsp[1] ? fsp[1] : fsp[0];    \
            lf.lfWidth = lf.lfWidth * change;                    \
            lf.lfHeight = lf.lfHeight * change;                  \
            CFont *font = new CFont();                           \
            font->CreateFontIndirectW(&lf);                      \
            wnd->SetFont(font);                                  \
            m_scale_fontNewTable[wnd]->DeleteObject();                 \
            CFont *temp = m_scale_fontNewTable[wnd];                   \
            delete temp;                                         \
            m_scale_fontNewTable[wnd] = font;                          \
            hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);     \
            wnd->Invalidate();                                   \
        }                                                        \
    }

#endif // !RESIZE

#ifndef SCALE_ONSIZE
#define SCALE_ONSIZE                                        \
    if (m_scale_table.size() == 0)                                \
    {                                                       \
        return;                                             \
    }                                                       \
    if (nType != SIZE_MINIMIZED && this->IsWindowVisible()) \
    {                                                       \
        ReSize();                                           \
    }
#endif // !SCALE_ONSIZE