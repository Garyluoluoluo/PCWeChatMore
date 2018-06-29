#include "stdafx.h"
#include "ProcessHandleHelper.h"
#include "stdio.h"


ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQuerySystemInformation");
NTQUERYOBJECT    NtQueryObject = (NTQUERYOBJECT)GetProcAddress(GetModuleHandleA("ntdll.dll"),"NtQueryObject");

//������Ȩ
BOOL ElevatePrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
		return FALSE;
	LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid);
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if(!AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(TOKEN_PRIVILEGES),NULL,NULL))
	{
		return FALSE;
	}

	return TRUE;
}

/*
 *	���㸴���κν��̾��
 */
HANDLE DuplicateHandleEx(DWORD pid, HANDLE h, DWORD flags)
{
	HANDLE hHandle = NULL;

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if(hProc)
	{
		if(!DuplicateHandle(hProc,
			(HANDLE)h, GetCurrentProcess(),
			&hHandle, 0, FALSE, /*DUPLICATE_SAME_ACCESS*/flags))
		{
			hHandle = NULL;
		}
	}

	CloseHandle(hProc);
	return hHandle;
}

/*
 *	����ϵͳ���еľ��
 */
PVOID ZwQuerySystemInformationEx(){

	DWORD dwSize = 0;
	PVOID pbuffer = NULL;
	NTSTATUS Status;

	pbuffer = VirtualAlloc(NULL, 0x8000, MEM_COMMIT, PAGE_READWRITE);	//��С�˲��У��᷵��һ������ֵ

	Status = ZwQuerySystemInformation(SystemHandleInformation, pbuffer, 0x8000, &dwSize);

	if(!NT_SUCCESS(Status))
	{
		VirtualFree(pbuffer, 0, MEM_RELEASE);		//�ͷŵ������·���
		if (false)
		{
			return NULL;
		}
		else
		{
			
			if (dwSize*2 > 0x4000000)  // MAXSIZE
			{
				return NULL;
			}

			pbuffer = VirtualAlloc(NULL, dwSize*2, MEM_COMMIT, PAGE_READWRITE);

			if(!pbuffer)
			{
				return NULL;
			}

			Status = ZwQuerySystemInformation(SystemHandleInformation, pbuffer, dwSize*2, NULL);

			if(!NT_SUCCESS(Status))
			{
				if (NULL != pbuffer)
				{
					VirtualFree(pbuffer, 0, MEM_RELEASE);
				}
				return NULL;  
			}
			return pbuffer;
		}
	}

	return pbuffer;
}

HANDLE GetProcessHandleInternal(PVOID pbuffer, int pid, wchar_t* name){

	DWORD dwSize = 0;
	POBJECT_NAME_INFORMATION pNameInfo;
	POBJECT_NAME_INFORMATION pNameType;
	NTSTATUS Status;
	ULONG nIndex = 0;
	DWORD dwFlags = 0;
	char szType[128] = {0};
	char szName[512] = {0};

	if(pbuffer == NULL || pid == 0 || name == NULL){
		return NULL;
	}


	PSYSTEM_HANDLE_INFORMATION1 pHandleInfo = (PSYSTEM_HANDLE_INFORMATION1)pbuffer;
	for(nIndex = 0; nIndex < pHandleInfo->NumberOfHandles; nIndex++)
	{
		//char info[256];
		//sprintf_s(info,"�����ȶ���:%d %d", pHandleInfo->Handles[nIndex].UniqueProcessId, pid);
		//OutputDebugStringA(info);
		if(pHandleInfo->Handles[nIndex].UniqueProcessId == pid)
		{
			//
			HANDLE hHandle = DuplicateHandleEx(pHandleInfo->Handles[nIndex].UniqueProcessId, 
				(HANDLE)pHandleInfo->Handles[nIndex].HandleValue,
				DUPLICATE_SAME_ACCESS
				);
			if(hHandle == NULL) continue;

			Status = NtQueryObject(hHandle, ObjectNameInformation, szName, 512, &dwFlags);

			if (!NT_SUCCESS(Status))
			{
				CloseHandle(hHandle);
				continue;
			}

			Status = NtQueryObject(hHandle, ObjectTypeInformation, szType, 128, &dwFlags);

			CloseHandle(hHandle);	//����Ҫ�����
			hHandle = NULL;

			if (!NT_SUCCESS(Status))
			{
				continue;
			}

			pNameInfo = (POBJECT_NAME_INFORMATION)szName;
			pNameType = (POBJECT_NAME_INFORMATION)szType;

			WCHAR TypName[1024] = {0};		//���ͣ������ж�Ҳ���Բ��жϣ����ܳ��ֲ�ͬ����ͬһ�����ְ�
			WCHAR Name[1024] = {0};

			wcsncpy(TypName, (WCHAR*)pNameType->Name.Buffer, pNameType->Name.Length/2);
			wcsncpy(Name, (WCHAR*)pNameInfo->Name.Buffer, pNameInfo->Name.Length/2);

			if (wcsstr(Name, name))//_WeChat_App_Instance_Identity_Mutex_Name
			{
				return (HANDLE)pHandleInfo->Handles[nIndex].HandleValue;
			}
		}

	}

	return NULL;
}

/*
 *	�ر�ָ�����̵�ĳ�������Ҳ�����Լ���
 */
void CloseProcessHandle(DWORD pid, HANDLE handle){
	HANDLE hHandle = DuplicateHandleEx(pid, 
		handle,
		DUPLICATE_CLOSE_SOURCE		//���ƹ����Ĺر�ԭ���ľ��,���û����������������������������õ�����
		);

	CloseHandle(hHandle);
}

/****************************************public���֣�c�����о����鷳��ϰ�߱�����ԣ��е㲻ϰ����******************************************************/

HANDLE GetProcessHandle(int pid, wchar_t* name){
	PVOID pbuffer = NULL;
	ElevatePrivileges();

	pbuffer = ZwQuerySystemInformationEx();
	if(pbuffer == NULL){
		return NULL;
	}

	HANDLE h = GetProcessHandleInternal(pbuffer, pid, name);
	VirtualFree(pbuffer, 0, MEM_RELEASE);
	return h;
}


void CloseProcessHandle(int pid, wchar_t* name){
	HANDLE h = GetProcessHandle(pid, name);		//��ȡ����α��������Բ��ùر�
	//OutputDebugString(L"��ȡ����");
	if(h != NULL){
		CloseProcessHandle(pid,h);
		//OutputDebugString(L"�ر�����");
	}

	//OutputDebugString(L"ִ�к��������0000000000");


}