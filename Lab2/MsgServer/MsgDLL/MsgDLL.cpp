// MsgDLL.cpp: определяет процедуры инициализации для библиотеки DLL.
//
#pragma once
#include "pch.h"
#include "framework.h"
#include "MsgDLL.h"
#include "Msg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <windows.h>
#include <afxsock.h>
#include <string>
#include <vector>

//
//TODO: если эта библиотека DLL динамически связана с библиотеками DLL MFC,
//		все функции, экспортированные из данной DLL-библиотеки, которые выполняют вызовы к
//		MFC, должны содержать макрос AFX_MANAGE_STATE в
//		самое начало функции.
//
//		Например:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// тело нормальной функции
//		}
//
//		Важно, чтобы данный макрос был представлен в каждой
//		функции до вызова MFC.  Это означает, что
//		должен стоять в качестве первого оператора в
//		функции и предшествовать даже любым объявлениям переменных объекта,
//		поскольку их конструкторы могут выполнять вызовы к MFC
//		DLL.
//
//		В Технических указаниях MFC 33 и 58 содержатся более
//		подробные сведения.
//

// CMsgDLLApp
#define DllExport __declspec(dllexport)

//CSocket s;
using namespace std;
Message m;

extern "C" DllExport unsigned int GetIDMessage()
{
	return m.m_Header.m_From;
}

extern "C" DllExport unsigned int GetSizeMessage()
{
	return m.m_Data.size();
}

extern "C" DllExport char GetTextMessage(unsigned int i)
{
	return m.m_Data[i];
}


extern "C" DllExport int _stdcall Get(unsigned int m_ID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CSocket s;
	s.Create();
	s.Connect("127.0.0.1", 54321);
	m.Send(s, M_BROKER, m_ID, M_GETDATA);
	switch (m.Receive(s))
	{
	default:
		s.Close();
		return 0;
		break;
	case M_DATA:
		s.Close();
		return 5;
		break;
	}
}


extern "C" DllExport void _stdcall Send(unsigned int m_User,unsigned int m_ID, unsigned int m_Code, const char* m_Text)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CSocket s;
	s.Create();
	s.Connect("127.0.0.1", 54321);
	m.Send(s, m_User, m_ID, m_Code, m_Text);
	s.Close();
}

extern "C" DllExport unsigned int _stdcall OnLaunch()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxSocketInit();
	CSocket s;
	s.Create();
	s.Connect("127.0.0.1", 54321);
	m.Send(s, 0, 0, M_INIT);
	m.Receive(s);
	s.Close();
	return m.m_Header.m_To;
}
 



BEGIN_MESSAGE_MAP(CMsgDLLApp, CWinApp)
END_MESSAGE_MAP()


// Создание CMsgDLLApp

CMsgDLLApp::CMsgDLLApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CMsgDLLApp

CMsgDLLApp theApp;


// Инициализация CMsgDLLApp

BOOL CMsgDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
