#pragma once
#include "KernelManager.h"
#include "FileManager.h"
#include "CmdManager.h"
#include <wininet.h>


UINT WINAPI Loop_ShellManager(void* managerHandle);


UINT WINAPI Loop_FileManager(void* managerHandle);
