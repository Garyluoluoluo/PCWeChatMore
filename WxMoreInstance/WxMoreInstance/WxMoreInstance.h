
// WxMoreInstance.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "MessageHandleUI.h"

// CWxMoreInstanceApp:
// �йش����ʵ�֣������ WxMoreInstance.cpp
//

class CWxMoreInstanceApp : public CWinAppEx
{
public:
	CWxMoreInstanceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWxMoreInstanceApp theApp;