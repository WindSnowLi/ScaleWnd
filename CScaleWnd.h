#pragma once

#include "framework.h"

#include <map>
#include <tuple>

class CScaleWnd : public CDialogEx
{
public:
	// ��׼���캯��
	CScaleWnd();
	CScaleWnd(UINT nIDTemplate, CWnd* pParent = NULL);
	void SetFont(CWnd* c, CFont* font);
protected:
	// ԭʼ���ڿ��
	int m_width, m_heigth;
	// �ؼ�ָ��(ID����Ĭ��IDΪ0)ӳ��ԭʼ����λ�á�������Ϣ
	std::map<CWnd*, CRect> m_table;
	// �ؼ�ָ��ӳ��ԭʼ����
	std::map<CWnd*, CFont*> m_fontTable;
	// �ؼ�ָ��ӳ��ԭʼ����
	std::map<CWnd*, CFont*> m_fontNewTable;
	// ����ˢ�¿ؼ�λ��
	void ReSize(void);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

