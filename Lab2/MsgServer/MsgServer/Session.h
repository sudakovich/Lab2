#pragma once

struct Session
{
	int m_ID;
	string m_Name;
	clock_t m_Time;
	queue<Message> m_Messages;
	CRITICAL_SECTION m_CS;


	Session(int ID, string Name, clock_t Time)
		:m_ID(ID), m_Name(Name), m_Time(Time)
	{
		InitializeCriticalSection(&m_CS);
	}

	~Session() 
	{
		DeleteCriticalSection(&m_CS);
	}

	void SetTime(clock_t Time) 
	{
		EnterCriticalSection(&m_CS);
		m_Time = Time;
		LeaveCriticalSection(&m_CS);
	}

	clock_t GetTime()
	{
		return m_Time;
	}

	void Add(Message& m)
	{
		EnterCriticalSection(&m_CS);
		m_Messages.push(m);
		LeaveCriticalSection(&m_CS);
	}

	void Send(CSocket& s)
	{
		EnterCriticalSection(&m_CS);
		if (m_Messages.empty())
		{
			Message::Send(s, m_ID, M_BROKER, M_NODATA);
		}
		else
		{
			m_Messages.front().Send(s);
			m_Messages.pop();
		}
		LeaveCriticalSection(&m_CS);
	}

};