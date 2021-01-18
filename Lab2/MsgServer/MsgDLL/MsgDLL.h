// MsgDLL.h: основной файл заголовка для библиотеки DLL MsgDLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMsgDLLApp
// Реализацию этого класса см. в файле MsgDLL.cpp
//

class CMsgDLLApp : public CWinApp
{
public:
	CMsgDLLApp();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
