#include "stdafx.h"
#include "Manager.h"


class CFileManager : public CManager
{
public:
	void OnReceive(LPBYTE lpBuffer, UINT nSize);
	CFileManager(CClientSocket *pClient,DWORD managerHandle);
	virtual ~CFileManager();
	DWORD  m_managerHandle;
private:
};