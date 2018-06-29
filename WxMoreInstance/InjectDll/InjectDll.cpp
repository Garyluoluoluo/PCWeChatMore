// InjectDll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "InjectDll.h"
#include <stdio.h>  
#include <cstdlib>
#include "ProcessHandleHelper.h"


bool IsWeChat();
void CloseWxHandle(HANDLE mutex);
void UnHookLib();

// ���ǵ���������һ��ʾ��
INJECTDLL_API int nInjectDll=0;

// ���ǵ���������һ��ʾ����
INJECTDLL_API int fnInjectDll(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� InjectDll.h
CInjectDll::CInjectDll()
{
	return;
}

void HandleWxInstanceMutex();
void CloseWxHandle(HANDLE mutex);

extern HMODULE g_Module;
HHOOK g_Hook = 0;
bool isKilled = false;			//�Ƿ�Ѿ��ɱ��


//���ӻص�����
LRESULT CALLBACK HOOKProc(
							  _In_  int code,
							  _In_  WPARAM wParam,
							  _In_  LPARAM lParam
							  )
{
	HandleWxInstanceMutex();
	return CallNextHookEx(g_Hook, code, wParam, lParam);
}

bool StartHook()
{
	if (g_Hook == 0)
	{
		g_Hook = SetWindowsHookEx(WH_GETMESSAGE, HOOKProc, g_Module, 0);
		int error = GetLastError();

		wchar_t exeName[300] = {0};
		WCHAR szInfo[512] = { 0 };
		swprintf_s(szInfo, _countof(szInfo), L"����ʧ������:%d", error);
		//�����������debug��Ϣ��
		OutputDebugString(szInfo);

		return true;
	}
	return false;
}

bool UnHook()
{
	if (g_Hook!=0)
	{
		return UnhookWindowsHookEx(g_Hook) ? true : false;
	}
	return false;
}

/*
 *	�������΢�ŵľ��ͷŵ�---�����ڻص��ͷţ�ֻ������Ϣ������ܰ�
 */
void UnHookLib(){
	if(!IsWeChat()){
		FreeLibrary(g_Module);
	}
}

/*
 *	�Ƿ�΢�Ž���
 */
bool IsWeChat(){
	wchar_t exeName[300] = {0};
	GetModuleFileName(NULL,exeName, 300);
	if(wcsstr(exeName, L"WeChat.exe") != NULL){
		return true;

	}else{
		return false;
	}
}

void HandleWxInstanceMutex(){
	if(IsWeChat()){
		if(!isKilled){
			//_WeChat_App_Instance_Identity_Mutex_Name
			HANDLE hmutex = CreateMutex(NULL,TRUE,L"_WeChat_App_Instance_Identity_Mutex_Name");
			int err = GetLastError();
			if (err == ERROR_ALREADY_EXISTS)
			{
				
				CloseWxHandle(hmutex);
				CloseHandle(hmutex);
				isKilled = true;
			}else{
				
				OutputDebugString(L"�����ڣ���ҲҪ�ر��˾��");
				CloseHandle(hmutex);	//ҲҪ�ص�
			}
		}
	}
}

void CloseWxHandle(HANDLE mutex){ 

	OutputDebugString(L"ɱ��΢��APP");
	CloseProcessHandle(GetCurrentProcessId(), L"_WeChat_App_Instance_Identity_Mutex_Name");
}


