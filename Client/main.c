#include <Windows.h>

#include "stdafx.h"

#include "../Common/common.h"
#include "client.h"
#include "device.h"



/**
 *
 */
int main()
{
	xlog(LOG_INFO, L"Starting %s v%.02f...\n", CFB_PROGRAM_NAME_SHORT, CFB_VERSION);
	xlog(LOG_INFO, L"- by <%s>\n", CFB_AUTHOR);

	if (!OpenCfbDevice())
	{
		xlog(LOG_CRITICAL, L"Failed to get a handle to '%s'\n", CFB_USER_DEVICE_NAME);
		xlog(LOG_INFO, L"Hint: is the driver registered?\n");
		return -1;
	}

	DWORD dwCliTid;

	HANDLE hCli = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunInterpreter, NULL, 0, &dwCliTid);
	if( !hCli )
	{
		PrintError(L"CreateThread()");
	}
	else 
	{
		xlog(LOG_SUCCESS, L"Interpreter thread started as TID=%d\n", dwCliTid);
		WaitForSingleObject(hCli, INFINITE);
	}

	CloseCfbDevice();

	xlog(LOG_SUCCESS, L"Thanks for using %s, have a nice day!\n- %s\n", CFB_PROGRAM_NAME_SHORT, CFB_AUTHOR);

    return 0;
}

