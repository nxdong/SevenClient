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
		return;
	}

}