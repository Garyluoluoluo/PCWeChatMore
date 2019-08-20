#pragma once

#include <vector>
#include "WeChatProcessInfo.h"
// WechatTab �Ի���
// ΢��tab�������
class WechatTab : public CDialogEx
{

	const static int WM_MAIN_WECHAT_HWND_MSG = WM_USER + 1;			//�Զ���Э��
	const static int WM_MAIN_WECHAT_HWND_CLOSE_MSG = WM_USER + 2;			//�Զ���Э��
	const static int IDC_BUTTON_AUTO_TAB = WM_USER + 3;				//��̬������ID

	DECLARE_DYNAMIC(WechatTab)

public:
	WechatTab(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WechatTab();

	bool AddItemUI(CString name, HWND hw_chat);
	void RemoveItemUI(HWND hw_chat);



// �Ի�������
	enum { IDD = IDD_DIALOG_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnWeChatHWNDMessage(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnWeChatHWNDCloseMessage(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnAutoTabButtonClick();

	DECLARE_MESSAGE_MAP()

private:
	int CalcTabWidth();								//���㵱ǰtab�Ŀ��
	CRect CalcTabRect(int index);					//����Tab�ĸ߶ȺͿ��
	CRect CalcNewTabRect();							//������һ���µ�Tab��rect
	CButton* CreateTabItem(CString name);
	void RefreshTabUI();
	bool IsCotain(HWND hw);
	HWND FindWechatFromTab();						//ͨ�������ȡ��Ӧ��΢������
	std::vector<WeChatProcessInfo> CheckWechatIsBad();		//���΢�Ŵ����Ƿ���Ч��������Ч�Ĵ���

private:
	std::vector<WeChatProcessInfo>	m_tab_button_list;
	int	m_begin_tab_index;							//��ǰ�����һ����ʼ������
	int m_last_tab_width;							//��¼�ϴ�tab���
};
