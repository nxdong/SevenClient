#include "stdafx.h"
#include "Manager.h"
typedef struct _DRIVE_LIST
{
	BYTE		bToken;
	wchar_t		DriveString[256];

}DRIVE_LIST;
typedef struct _FILE_DAtA
{
	BYTE		bToken;
	LPCTSTR		pData;
}FILE_DATA;
class CFileManager : public CManager
{
public:
	void OnReceive(LPBYTE lpBuffer, UINT nSize);
	CFileManager(CClientSocket *pClient,DWORD managerHandle);
	virtual ~CFileManager();
	DWORD  m_managerHandle;
private:
public:
	UINT SendDriveList();
	UINT SendFilesList(LPCTSTR lpszDirectory);
};