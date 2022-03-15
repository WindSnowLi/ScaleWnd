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
protected:
	// ԭʼ���ڿ��
	int m_width, m_heigth;
	// �ؼ�IDӳ��ԭʼ����λ�á�������Ϣ
	std::map<int, CRect> m_table;
	// �ؼ�IDӳ��ԭʼ����
	std::map<int, CFont*> m_fontTable;
	// �ؼ�IDӳ��ԭʼ����
	std::map<int, CFont*> m_fontNewTable;
	// ����ˢ�¿ؼ�λ��
	void ReSize(void);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

