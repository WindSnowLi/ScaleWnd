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
	void SetFont(CWnd* c, CFont* font);
protected:
	// 原始窗口宽高
	int m_width, m_heigth;
	// 控件指针(ID会有默认ID为0)映射原始窗口位置、字体信息
	std::map<CWnd*, CRect> m_table;
	// 控件指针映射原始字体
	std::map<CWnd*, CFont*> m_fontTable;
	// 控件指针映射原始字体
	std::map<CWnd*, CFont*> m_fontNewTable;
	// 重设刷新控件位置
	void ReSize(void);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

