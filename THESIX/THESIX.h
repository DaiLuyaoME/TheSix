
// THESIX.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTHESIXApp:
// �йش����ʵ�֣������ THESIX.cpp
//

class CTHESIXApp : public CWinApp
{
public:
	CTHESIXApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTHESIXApp theApp;