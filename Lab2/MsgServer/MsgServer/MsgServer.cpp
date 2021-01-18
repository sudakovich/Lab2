// MsgServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "MsgServer.h"
#include "Msg.h"
#include "Session.h"
#include <ctime>
#include <queue>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int gMaxID = M_USER;
map<int, shared_ptr<Session>> gSessions;


void ProcessClient(SOCKET hSock)
{
    CSocket s;
    s.Attach(hSock);
    Message m;
    switch (m.Receive(s))
        {
        case M_INIT:
        {
            int NewID;
            NewID = ++gMaxID;
            auto pSession = make_shared<Session>(NewID, m.m_Data, clock());
            cout << gMaxID << endl;
            gSessions[pSession->m_ID] = pSession;
            Message::Send(s, pSession->m_ID, M_BROKER, M_CONFIRM);
            break;
        }
        case M_EXIT:
        {
            gSessions.erase(m.m_Header.m_From);
            Message::Send(s, m.m_Header.m_From, M_BROKER, M_EXIT);
            cout << m.m_Header.m_From << " Exited" << endl;
            return;
        }
        case M_GETDATA:
        {
            if (gSessions.find(m.m_Header.m_From) != gSessions.end())
            {
                gSessions[m.m_Header.m_From]->SetTime(clock());
                gSessions[m.m_Header.m_From]->Send(s);
            }
            break;
        }
        default:
        {
            if (gSessions.find(m.m_Header.m_From) != gSessions.end())
            {
                gSessions[m.m_Header.m_From]->SetTime(clock());
                if (gSessions.find(m.m_Header.m_To) != gSessions.end())
                {
                    cout << m.m_Header.m_From << " " << m.m_Data << endl;
                    gSessions[m.m_Header.m_To]->Add(m);
                }
                else if (m.m_Header.m_To == M_ALL)
                {
                    for (auto& [id, Session] : gSessions)
                    {
                        if (id != m.m_Header.m_From)
                            Session->Add(m);
                    }
                }
            }
            break;
        }
        }
}

void LaunchClient()
{
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    CreateProcessA((LPSTR)"C:\\Users\\Сергей\\Desktop\\tris_2020-master\\MsgServer\\Debug\\MsgClient.exe", NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi); // Путь
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

void CheckTime() 
{
    while (true)
    {
        for (int i = 100; i <= gMaxID; i++)
        {
            if (gSessions.find(i) != gSessions.end())
            {
                double  workTime = (double)(clock() - gSessions[i]->GetTime()) / CLOCKS_PER_SEC;
                if (workTime > 60)
                {
                    cout << i << " Disconnected" << endl;
                    gSessions.erase(i);
                }
            }
        }
        Sleep(10000);
    }
}

void Server()
{
    AfxSocketInit();

    CSocket Server;
    Server.Create(54321);
    /*
    for (int i = 0; i < 2; ++i)
    {
        LaunchClient();                        //Опциональный цикл для запуска клиентов. 
    }
    */
    thread Time(CheckTime);
    Time.detach();

    while (true)
    {
        if (!Server.Listen())
            break;
        CSocket s;
        Server.Accept(s);
        thread t(ProcessClient, s.Detach());
        t.detach();
    }
}


// The one and only application object

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            Server();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
