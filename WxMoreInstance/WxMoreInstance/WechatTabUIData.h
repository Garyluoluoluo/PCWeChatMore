#pragma once
#include <string>

// WechatTab ��������ݲ�

class WechatTabUIData
{
public:
	WechatTabUIData();
	~WechatTabUIData(void);

	void Init(HWND hw);
	void AddItem(std::wstring name);	//����һ��table ����

private:
	HWND m_UIHwnd;		//����HWND
	int m_max_width;	//�����
};

