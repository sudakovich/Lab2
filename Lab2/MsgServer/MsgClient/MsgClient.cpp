// MsgClient.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "framework.h"
#include "MsgClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <afxsock.h>
#include "Msg.h"


// Единственный объект приложения

CWinApp theApp;

using namespace std;
unsigned int m_ID;
string Addr = "127.0.0.1";
Message m;
int ID;
bool flag1;


void MyThread() 
{
    CSocket s;
    //cout << "Thread is working!" << endl;
    while (true) 
    {
        s.Create();
        s.Connect("127.0.0.1", 54321);
        m.Send(s, M_BROKER, m_ID, M_GETDATA);
        switch (m.Receive(s)) 
        {
        case M_DATA:
            cout << m_ID << endl;
            cout << "Message from " << m.m_Header.m_From << " " << m.m_Data << endl;
            if (!flag1)
            {
                cout << "ID(10 - all, x - other) = ";
            }
            else 
            {
                cout << "ID(10 - all, x - other) = " << ID << endl;
                cout << "Text = ";
            }
            break;
        case M_EXIT:
            return;
        }
        s.Close();
        Sleep(1000);
    }
}

void OnInit() 
{
    AfxSocketInit();
    CSocket s;
    s.Create();
    s.Connect("127.0.0.1", 54321);
    m.Send(s, 0, 0, M_INIT);
    m.Receive(s);
    s.Close();
    m_ID = m.m_Header.m_To;
    cout << m_ID << endl;
    thread t(MyThread);
    t.detach();
}

void start() 
{
    CSocket s;
    OnInit();
    while (true) 
    {
        flag1 = false;
        cout << "ID(10 - all, x(>100) - other) = ";
        cin >> ID;
        cout << endl;
        flag1 = true;
        cout << "Text = ";
        string Text;
        cin >> Text;
        cout << endl;
        s.Create();
        s.Connect("127.0.0.1", 54321);
        m.Send(s, ID, m_ID, M_DATA, Text);
        s.Close();
    }
}

BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType) // Обработчик на закрытие
{
    if (dwCtrlType == CTRL_CLOSE_EVENT)
    {
        CSocket s;
        s.Create();
        s.Connect(LPCTSTR("127.0.0.1"), 54321);
        m.Send(s, 0, m_ID, M_EXIT);
        s.Close();
        return 1;
    }
}

int main()
{
    int nRetCode = 0;
    SetConsoleCtrlHandler(HandlerRoutine, 1);
    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // инициализировать MFC, а также печать и сообщения об ошибках про сбое
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: вставьте сюда код для приложения.
            wprintf(L"Критическая ошибка: сбой при инициализации MFC\n");
            nRetCode = 1;
        }
        else
        {
            start();
        }
    }
    else
    {
        // TODO: измените код ошибки в соответствии с потребностями
        wprintf(L"Критическая ошибка: сбой GetModuleHandle\n");
        nRetCode = 1;
    }

    return nRetCode;
}
