#pragma once

class CCustomServer
{
public:
	CCustomServer();
	~CCustomServer();

	void Render();
	void Clear();
	void Show(bool bShow);
	
public:
	bool				m_bIsActive;
	char*				address;
	char*				nick;
	char*				pass;
	int					port;
	bool				m_bFixer;
	bool				m_bFixer2;
	bool 				m_bLamb;
};