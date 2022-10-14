#pragma once

class CBKeys
{
public:
	CBKeys();
	~CBKeys();

	void Render();
	void Clear();
	void Show(bool bShow);
	
private:
	bool m_bIsItemShow;
	bool m_bIsActive;
};
