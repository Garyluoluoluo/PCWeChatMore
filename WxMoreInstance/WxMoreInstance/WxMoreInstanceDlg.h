
// WxMoreInstanceDlg.h : ͷ�ļ�
//

#pragma once
#include "WechatTab.h"

// CWxMoreInstanceDlg �Ի���
class CWxMoreInstanceDlg : public CDialog
{
// ����
public:
	CWxMoreInstanceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WXMOREINSTANCE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnMore();

private:
	WechatTab m_tab_ui;
	CMessageHandleUI m_MessageHandleUI;
public:
	afx_msg void OnBnClickedButtonAdvance();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
