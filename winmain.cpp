/*****************************************************************************
 * This file is part of Notepad++ project                                    *
 * Copyright (C)2003 Don HO <don.h@free.fr>                                  *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either                      *
 * version 2 of the License, or (at your option) any later version.          *
 *                                                                           *
 * Note that the GPL places important restrictions on "derived works", yet   *
 * it does not provide a detailed definition of that term.  To avoid         *
 * misunderstandings, we consider an application to constitute a             *
 * "derivative work" for the purpose of this license if it does any of the   *
 * following:                                                                *
 * 1. Integrates source code from Notepad++.                                 *
 * 2. Integrates/includes/aggregates Notepad++ into a proprietary executable *
 *    installer, such as those produced by InstallShield.                    *
 * 3. Links to a library or executes a program that does any of the above.   *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 *
 *****************************************************************************/

/*******************
 * ������          *
 *******************/
#include "Notepad_plus_Window.h"
#include "Processus.h"
/*******************
 * Win32 exception *
 *******************/
#include "Win32Exception.h"

/********************
 * Write dump files *
 ********************/
#include "MiniDumper.h"
#include "verifySignedFile.h"

/****************************************************
 * ��Ϊ�еĿⲻ�ܱ��������ӣ�����ʹ�ö�̬��������� *
 ****************************************************/
#include "Real_Kesic_Lee_Dynamic_Link_Library_Load_And_Proc_Address.h"

#define WindowsApi
#define WIN32EXCEPTION_MESSAGE_FORMATTER TEXT("An exception occured. Notepad++ cannot recover and must be shut down.\r\nThe exception details are as follows:\r\nCode:\t0x%08X\r\nType:\t%S\r\nException address: 0x%p")

// typedef std::vector<generic_string> ParamVector;



namespace
{
	/*
	 * ����WM_COPYDATA��Ϣ������
	 */
	void allowWmCopydataMessages(Notepad_plus_Window & notepad_plus_plus, const NppParameters * pNppParameters, winVer ver)
	{
		#ifndef MSGFLT_ADD
		const DWORD MSGFLT_ADD = 1;
		#endif

		#ifndef MSGFLT_ALLOW
		const DWORD MSGFLT_ALLOW = 1;
		#endif

		// Tell UAC that lower integrity processes are allowed to send WM_COPYDATA messages to this process (or window)
		// This allows opening new files to already opened elevated Notepad++ process via explorer context menu.
		if (ver >= WV_VISTA || ver == WV_UNKNOWN)
		{
			HMODULE hDll = WindowsApi::GetModuleHandle(TEXT("user32.dll"));
			if (hDll)
			{
				// According to MSDN ChangeWindowMessageFilter may not be supported in future versions of Windows,
				// that is why we use ChangeWindowMessageFilterEx if it is available (windows version >= Win7).
				if (pNppParameters->getWinVersion() == WV_VISTA)
				{
					typedef BOOL (WINAPI * MESSAGEFILTERFUNC)(UINT message,DWORD dwFlag);

					MESSAGEFILTERFUNC func = (MESSAGEFILTERFUNC)::GetProcAddress( hDll, "ChangeWindowMessageFilter" );

					if (func)
					{
						/******************************************************************************************************************************************************************************************************
						 * WindowsApi::ChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);                                                                                                                                    *
						 * from : http://msdn.microsoft.com/query/dev15.query?appId=Dev15IDEF1&l=ZH-CN&k=k("WINUSER%2FChangeWindowMessageFilter");k(ChangeWindowMessageFilter);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true *
						 * function                                                                                                                                                                                           *
						 * [Using the ChangeWindowMessageFilter function is not recommended, as it has process-wide                                                                                                           *
						 * scope.Instead, use the ChangeWindowMessageFilterEx(1) function tocontrol access to specific                                                                                                        *
						 * windows as needed. ChangeWindowMessageFilter may not be supported in future versions of                                                                                                            *
						 * Windows.]                                                                                                                                                                                          *
						 * Ϊָ���Ĵ�������/�Ƴ���Ϣ������                                                                                                                                                                    *
						 * ChangeWindowMessageFilterEx(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-changewindowmessagefilterex                                                                     *
						 * Adds or removes a message from the User Interface Privilege Isolation (UIPI) message filter.                                                                                                       *
						 * BOOL ChangeWindowMessageFilter(UINT message, DWORD dwFlag);                                                                                                                                        *
						 * message                                                                                                                                                                                            *
						 * Type: UINT                                                                                                                                                                                         *
						 * The message to add to or remove from the filter.                                                                                                                                                   *
						 * dwFlag                                                                                                                                                                                             *
						 * Type: DWORD                                                                                                                                                                                        *
						 * The action to be performed. One of the following values.                                                                                                                                           *
						 * +---------------+-----------------------------------------------------------------+                                                                                                                *
						 * | Value         | Meaning                                                         |                                                                                                                *
						 * +---------------+-----------------------------------------------------------------+                                                                                                                *
						 * | MSGFLT_ADD    | Adds the message to the filter. This has the effect of allowing |                                                                                                                *
						 * | 1             | the message to be received.                                     |                                                                                                                *
						 * +---------------+-----------------------------------------------------------------+                                                                                                                *
						 * | MSGFLT_REMOVE | Removes the message from the filter. This has the effect of     |                                                                                                                *
						 * | 2             | blocking the message.                                           |                                                                                                                *
						 * +---------------+-----------------------------------------------------------------+                                                                                                                *
						 * Return Value�U                                                                                                                                                                                     *
						 * Type: Type: BOOL                                                                                                                                                                                   *
						 * TRUE if successful; otherwise, FALSE. To get extended error information, call GetLastError(2).                                                                                                     *
						 * GetLastError(2) : https://docs.microsoft.com/windows/desktop/api/errhandlingapi/nf-errhandlingapi-getlasterror                                                                                     *
						 *                                                                                                                                                                                                    *
						 *   Note  A message can be successfully removed from the filter, but that is not a guarantee that                                                                                                    *
						 *   the message will be blocked. See the Remarks section for more details.                                                                                                                           *
						 ******************************************************************************************************************************************************************************************************/
						func(WM_COPYDATA, MSGFLT_ADD);
					}
				}
				else
				{
					typedef BOOL (WINAPI * MESSAGEFILTERFUNCEX)(HWND hWnd,UINT message,DWORD action,VOID* pChangeFilterStruct);

					MESSAGEFILTERFUNCEX func = (MESSAGEFILTERFUNCEX)::GetProcAddress( hDll, "ChangeWindowMessageFilterEx" );

					if (func)
					{
						/**********************************************************************************************************************************************************************************************************
						 * WindowsApi::ChangeWindowMessageFilterEx(notepad_plus_plus.getHSelf(), WM_COPYDATA, MSGFLT_ALLOW, NULL);                                                                                                *
						 * from : http://msdn.microsoft.com/query/dev15.query?appId=Dev15IDEF1&l=ZH-CN&k=k("WINUSER%2FChangeWindowMessageFilterEx");k(ChangeWindowMessageFilterEx);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true *
						 * function                                                                                                                                                                                               *
						 * Modifies the User Interface Privilege Isolation (UIPI) message filter for a specified window.                                                                                                          *
						 * Ϊָ���Ĵ����޸���Ϣ������                                                                                                                                                                             *
						 * BOOL ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct);                                                                                      *
						 * hwnd                                                                                                                                                                                                   *
						 * Type: HWND                                                                                                                                                                                             *
						 * A handle to the window whose UIPI message filter is to be modified.                                                                                                                                    *
						 * message                                                                                                                                                                                                *
						 * Type: UINT                                                                                                                                                                                             *
						 * The message that the message filter allows through or blocks.                                                                                                                                          *
						 * action                                                                                                                                                                                                 *
						 * Type: DWORD                                                                                                                                                                                            *
						 * The action to be performed, and can take one of the following values:                                                                                                                                  *
						 * +------------------+---------------------------------------------------------------+                                                                                                                   *
						 * | Value            | Meaning                                                       |                                                                                                                   *
						 * +------------------+---------------------------------------------------------------+                                                                                                                   *
						 * | MSGFLT_ALLOW     | Allows the message through the filter. This enables the       |                                                                                                                   *
						 * | 1                | message to be received by hWnd, regardless of the source of   |                                                                                                                   *
						 * |                  | the message,even it comes from a lower privileged process.    |                                                                                                                   *
						 * +------------------+---------------------------------------------------------------+                                                                                                                   *
						 * | MSGFLT_DISALLOW  | Blocks the message to be delivered to hWnd if it comes from a |                                                                                                                   *
						 * | 2                | lower privileged process,unless the message is allowed        |                                                                                                                   *
						 * |                  | process-wide by using the ChangeWindowMessageFilter(1)        |                                                                                                                   *
						 * |                  | functionor globally.                                          |                                                                                                                   *
						 * +------------------+---------------------------------------------------------------+                                                                                                                   *
						 * | MSGFLT_RESET     | Resets the window message filter for hWnd to the default. Any |                                                                                                                   *
						 * | 0                | message allowedglobally or process-wide will get through,     |                                                                                                                   *
						 * |                  | but any message not includedin those two categories, and      |                                                                                                                   *
						 * |                  | which comes from a lower privileged process, will be blocked. |                                                                                                                   *
						 * +------------------+---------------------------------------------------------------+                                                                                                                   *
						 * ChangeWindowMessageFilter(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-changewindowmessagefilter                                                                             *
						 * pChangeFilterStruct                                                                                                                                                                                    *
						 * Type: PCHANGEFILTERSTRUCT(2)                                                                                                                                                                           *
						 * Optional pointer to a CHANGEFILTERSTRUCT structure.                                                                                                                                                    *
						 * PCHANGEFILTERSTRUCT(2) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-changefilterstruct                                                                                          *
						 * Return Value�U                                                                                                                                                                                         *
						 * Type: Type: BOOL                                                                                                                                                                                       *
						 * If the function succeeds, it returns TRUE; otherwise, it returns FALSE. To get extended error                                                                                                          *
						 * information, call GetLastError(3).                                                                                                                                                                     *
						 * GetLastError(3) : https://docs.microsoft.com/windows/desktop/api/errhandlingapi/nf-errhandlingapi-getlasterror                                                                                         *
						 **********************************************************************************************************************************************************************************************************/
						func(notepad_plus_plus.getHSelf(), WM_COPYDATA, MSGFLT_ALLOW, NULL);
					}
				}
			}
		}
	}

	/*************************************************************
	 * commandLine should contain path to n++ executable running *
	 * �ѿ���̨��Shell���ݵĲ������в�⣬����ɲμ�argc��argv   *
	 *************************************************************/
	std::vector<generic_string> parseCommandLine(const TCHAR * commandLine)
	{
		/*
		 * ��Ų���������
		 */
		std::vector<generic_string> result;
		if ( commandLine[0] != '\0' )
		{
			int numArgs;
			/*************************************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/shellapi/nf-shellapi-commandlinetoargvw?redirectedfrom=MSDN *
			 * function                                                                                                              *
			 * Parses a Unicode command line string and returns an array of pointers to the command line                             *
			 * arguments, along with a count of such arguments, in a way that is similar to the standard C run-time                  *
			 * argv and argc values.                                                                                                 *
			 * LPWSTR * CommandLineToArgvW(LPCWSTR lpCmdLine, int * pNumArgs);                                                       *
			 * lpCmdLine                                                                                                             *
			 * Type: LPCWSTR                                                                                                         *
			 * Pointer to a null-terminated Unicode string that contains the full command line. If this parameter is                 *
			 * an empty string the function returns the path to the current executable file.                                         *
			 * pNumArgs                                                                                                              *
			 * Type: int *                                                                                                           *
			 * Pointer to an int that receives the number of array elements returned, similar to argc.                               *
			 * Return Value                                                                                                          *
			 * Type: LPWSTR *                                                                                                        *
			 * A pointer to an array of LPWSTR values, similar to argv.                                                              *
			 * If the function fails, the return value is NULL. To get extended error information, call GetLastError(1).             *
			 * GetLastError(1) : https://docs.microsoft.com/windows/desktop/api/errhandlingapi/nf-errhandlingapi-getlasterror        *
			 *************************************************************************************************************************/
			LPWSTR * tokenizedCmdLine = WindowsApi::CommandLineToArgvW( commandLine, &numArgs );
			if ( tokenizedCmdLine != nullptr )
			{
				result.assign( tokenizedCmdLine, tokenizedCmdLine + numArgs );
				WindowsApi::LocalFree( tokenizedCmdLine );
			}
		}
		return result;
	}

	/**************************************************************************************************************
	 * 1. Converts /p to -quickPrint if it exists as the first parameter                                          *
	 * 2. Concatenates all remaining parameters to form a file path, adding appending .txt extension if necessary *
	 * This seems to mirror Notepad's behaviour                                                                   *
	 **************************************************************************************************************/
	std::vector<generic_string> convertParamsToNotepadStyle(PWSTR pCmdLine)
	{
		std::vector<generic_string> params;

		// Notepad accepts both /p and /P, so compare case insensitively
		/***************************************************************************************************************************************************************************************************************************************************************************************************************************************************
		 * from : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/strnicmp-wcsnicmp-mbsnicmp-strnicmp-l-wcsnicmp-l-mbsnicmp-l?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DZH-CN%26k%3Dk(%22TCHAR%2F_tcsnicmp%22);k(_tcsnicmp);k(DevLang-C%2B%2B);k(TargetOS-Windows)%26rd%3Dtrue&view=vs-2019 *
		 * _strnicmp��_wcsnicmp��_mbsnicmp��_strnicmp_l��_wcsnicmp_l��_mbsnicmp_l                                                                                                                                                                                                                                                                          *
		 * �Ƚ������ַ�����ָ����Ŀ���ַ��������Ǵ�Сд����                                                                                                                                                                                                                                                                                                *
		 * ��Ҫ                                                                                                                                                                                                                                                                                                                                            *
		 * _mbsnicmp�� _mbsnicmp_l���������� Windows ����ʱ��ִ�е�Ӧ�ó��� �й���ϸ��                                                                                                                                                                                                                                                                   *
		 * Ϣ�������ͨ�� Windows ƽ̨Ӧ���в�֧�ֵ� CRT ������                                                                                                                                                                                                                                                                                            *
		 * int _strnicmp  (const char          * string1, const char          * string2, size_t count);                                                                                                                                                                                                                                                    *
		 * int _wcsnicmp  (const wchar_t       * string1, const wchar_t       * string2, size_t count);                                                                                                                                                                                                                                                    *
		 * int _mbsnicmp  (const unsigned char * string1, const unsigned char * string2, size_t count);                                                                                                                                                                                                                                                    *
		 * int _strnicmp_l(const char          * string1, const char          * string2, size_t count, _locale_t locale);                                                                                                                                                                                                                                  *
		 * int _wcsnicmp_l(const wchar_t       * string1, const wchar_t       * string2, size_t count, _locale_t locale);                                                                                                                                                                                                                                  *
		 * int _mbsnicmp_l(const unsigned char * string1, const unsigned char * string2, size_t count, _locale_t locale);                                                                                                                                                                                                                                  *
		 * string1�� string2                                                                                                                                                                                                                                                                                                                               *
		 * ����Ƚϵ�����null��ֹ���ַ���(C�����ַ���)��                                                                                                                                                                                                                                                                                                 *
		 * count                                                                                                                                                                                                                                                                                                                                           *
		 * Ҫ�Ƚϵ��ַ�����                                                                                                                                                                                                                                                                                                                                *
		 * locale                                                                                                                                                                                                                                                                                                                                          *
		 * Ҫʹ�õ��������á�                                                                                                                                                                                                                                                                                                                              *
		 * ����ֵ�U                                                                                                                                                                                                                                                                                                                                        *
		 * ָʾ���ַ���֮��Ĺ�ϵ��������ʾ��                                                                                                                                                                                                                                                                                                              *
		 * +--------+-------------------------------------------+                                                                                                                                                                                                                                                                                          *
		 * | ����ֵ |      ����                                 |                                                                                                                                                                                                                                                                                          *
		 * +--------+-------------------------------------------+                                                                                                                                                                                                                                                                                          *
		 * | < 0    | string1�Ӵ�С��string2���ַ�����          |                                                                                                                                                                                                                                                                                          *
		 * +--------+-------------------------------------------+                                                                                                                                                                                                                                                                                          *
		 * | = 0    | string1 substring ��string2�Ӵ���ȫ��ͬ�� |                                                                                                                                                                                                                                                                                          *
		 * +--------+-------------------------------------------+                                                                                                                                                                                                                                                                                          *
		 * | > 0    | string1 �Ӵ�����string2���ַ�����         |                                                                                                                                                                                                                                                                                          *
		 * +--------+-------------------------------------------+                                                                                                                                                                                                                                                                                          *
		 * �ڲ�����֤�����У���Щ�������� _NLSCMPERROR������ string .h <> ��<mbstring.h > �ж��塣                                                                                                                                                                                                                                                         *
		 * ��ע�U                                                                                                                                                                                                                                                                                                                                          *
		 * _Strnicmp������������Ƚ�string1��string2�ĵ�һ�������ַ���ͨ����ÿ���ַ�ת��ΪСд��                                                                                                                                                                                                                                                         *
		 * �в����ִ�Сд�ıȽϡ� _strnicmp��strncmp�Ĳ����ִ�Сд�İ汾������ڱȽϼ����ַ�֮ǰ��                                                                                                                                                                                                                                                         *
		 * ��һ�ַ����дﵽ��ֹ null �ַ�����ȽϽ����� ����ڱȽϼ����ַ�֮ǰ����һ�ַ����дﵽ��                                                                                                                                                                                                                                                         *
		 * ֹ null �ַ�ʱ�ַ�����ȣ���϶̵��ַ�����С��                                                                                                                                                                                                                                                                                                  *
		 *                                                                                                                                                                                                                                                                                                                                                 *
		 * ASCII ���д� 91 �� 96 ���ַ�����[������\������]������^���������͡�`�����ļ�����С��������ĸ�ַ��� ��˳                                                                                                                                                                                                                                        *
		 * ����stricmp��˳����ͬ��                                                                                                                                                                                                                                                                                                                         *
		 *                                                                                                                                                                                                                                                                                                                                                 *
		 * _wcsnicmp�� _mbsnicmp�� _strnicmp�Ŀ��ַ��Ͷ��ֽ��ַ��汾�� _Wcsnicmp�Ĳ����ǿ��ַ���                                                                                                                                                                                                                                                           *
		 * ����; _mbsnicmp����Щ�ַ����Ƕ��ֽ��ַ��ַ����� _mbsnicmp���ݵ�ǰ�Ķ��ֽڴ���ҳʶ���                                                                                                                                                                                                                                                           *
		 * �ֽ��ַ����У����ڷ�������ʱ���� _NLSCMPERROR �� �й���ϸ��Ϣ������Ĵ���ҳ(1)�� ����                                                                                                                                                                                                                                                           *
		 * �������������������ͬ��Ϊ����Щ�����ܵ��������õ�Ӱ�죺û�� _l��׺�İ汾������������                                                                                                                                                                                                                                                           *
		 * ����ص���Ϊʹ�õ�ǰ��������;���� _l��׺�İ汾��Ϊʹ�ô�����������á� �й���ϸ��Ϣ��                                                                                                                                                                                                                                                           *
		 * ����� Locale(2)��                                                                                                                                                                                                                                                                                                                              *
		 * ������Щ��������֤������� ���string1��string2Ϊ��ָ�룬�򽫵�����Ч�������������                                                                                                                                                                                                                                                           *
		 * ������֤�������� �������ִ�м���������Щ���������� _NLSCMPERROR ������Errno����Ϊ                                                                                                                                                                                                                                                              *
		 * EINVAL��                                                                                                                                                                                                                                                                                                                                        *
		 * ����ҳ(1) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/code-pages?view=vs-2019                                                                                                                                                                                                                                                      *
		 * Locale(2) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/locale?view=vs-2019                                                                                                                                                                                                                                                          *
		 * ������֤(3) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/parameter-validation?view=vs-2019                                                                                                                                                                                                                                          *
		 * һ���ı�����ӳ��U                                                                                                                                                                                                                                                                                                                              *
		 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                                                    *
		 * | TCHAR.H ���� | δ���� _UNICODE �� _MBCS | �Ѷ��� _MBCS | �Ѷ��� _UNICODE |                                                                                                                                                                                                                                                                    *
		 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                                                    *
		 * | _tcsncicmp   | _strnicmp                | _mbsnicmp    | _wcsnicmp       |                                                                                                                                                                                                                                                                    *
		 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                                                    *
		 * | _tcsnicmp    | _strnicmp                | _mbsnbicmp   | _wcsnicmp       |                                                                                                                                                                                                                                                                    *
		 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                                                    *
		 * | _tcsncicmp_l | _strnicmp_l              | _mbsnicmp_l  | _wcsnicmp_l     |                                                                                                                                                                                                                                                                    *
		 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                                                    *
		 * Ҫ��U                                                                                                                                                                                                                                                                                                                                          *
		 * +-------------------------+-------------------------+                                                                                                                                                                                                                                                                                           *
		 * | ���������ص�ֵ          | ����ı�ͷ              |                                                                                                                                                                                                                                                                                           *
		 * +-------------------------+-------------------------+                                                                                                                                                                                                                                                                                           *
		 * | _strnicmp�� _strnicmp_l | <string.h>              |                                                                                                                                                                                                                                                                                           *
		 * +-------------------------+-------------------------+                                                                                                                                                                                                                                                                                           *
		 * | _wcsnicmp�� _wcsnicmp_l | <string.h> �� <wchar.h> |                                                                                                                                                                                                                                                                                           *
		 * +-------------------------+-------------------------+                                                                                                                                                                                                                                                                                           *
		 * | _mbsnicmp�� _mbsnicmp_l | <mbstring.h>            |                                                                                                                                                                                                                                                                                           *
		 * +-------------------------+-------------------------+                                                                                                                                                                                                                                                                                           *
		 * �й�������������Ϣ������� ������(4)��                                                                                                                                                                                                                                                                                                          *
		 * ������(4) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/compatibility?view=vs-2019                                                                                                                                                                                                                                                   *
		 * ʾ���U                                                                                                                                                                                                                                                                                                                                          *
		 * ����� strncme ��ʾ����                                                                                                                                                                                                                                                                                                                         *
		 * strncme(5) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/strncmp-wcsncmp-mbsncmp-mbsncmp-l?view=vs-2019                                                                                                                                                                                                                    *
		 ***************************************************************************************************************************************************************************************************************************************************************************************************************************************************/
		if (_tcsnicmp(TEXT("/p"), pCmdLine, 2) == 0 )
		{
			params.emplace_back(TEXT("-quickPrint"));
			pCmdLine += 2; // Length of "/p"
		}

		// Advance to the first non-whitespace character
		while ( iswspace( *pCmdLine ) )
		{
			++pCmdLine;
		}

		// Now form a file name from the remaining commandline (if any is left)
		if ( *pCmdLine != '\0' )
		{
			generic_string str(pCmdLine);
			/*******************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/shlwapi/nf-shlwapi-pathfindextensionw     *
			 * function                                                                                            *
			 * Searches a path for an extension.                                                                   *
			 * LPCWSTR PathFindExtensionW(LPCWSTR pszPath);                                                        *
			 * pszPath                                                                                             *
			 * Type: PTSTR                                                                                         *
			 * A pointer to a null-terminated string of maximum length MAX_PATH that contains the path to          *
			 * search, including the extension being searched for.                                                 *
			 * Return Value�U                                                                                      *
			 * Type: PTSTR                                                                                         *
			 * Returns the address of the "." that precedes the extension within pszPath if an extension is found, *
			 * or the address of the terminating null character otherwise.                                         *
			 *******************************************************************************************************/
			if ( *WindowsApi::PathFindExtension(str.c_str()) == '\0' )
			{
				// If joined path has no extension, Notepad adds a .txt extension
				str.append(TEXT(".txt"));
			}
			params.push_back(std::move(str));
		}
		return params;
	}

	/********************************************************************************************************
	 * �жϲ����б����Ƿ����ָ���Ĳ�����������ڣ�����Ҫ���б����Ƴ��ò���ʱ�����б����Ƴ��ò���������true *
	 * @param token2Find ָ���Ĳ���                                                                         *
	 * @param params     �����б�                                                                           *
	 * @param eraseArg   �Ƿ��Ƴ�ָ���Ĳ���(true : ��ʾ��Ҫ���б����Ƴ�ָ���Ĳ����������ظ�ֻ�Ƴ���һ��)    *                          *
	 ********************************************************************************************************/
	bool isInList(const TCHAR * token2Find, std::vector<generic_string> & params, bool eraseArg = true)
	{
		for (auto it = params.begin(); it != params.end(); ++it)
		{
			/***********************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winbase/nf-winbase-lstrcmpw                   *
			 * function                                                                                                *
			 * Compares two character strings. The comparison is case-sensitive.                                       *
			 * To perform a comparison that is not case-sensitive, use the lstrcmpi(1) function.                       *
			 * lstrcmpi(1) : https://docs.microsoft.com/windows/desktop/api/winbase/nf-winbase-lstrcmpia               *
			 * int lstrcmp(LPCWSTR lpString1, LPCWSTR lpString2);                                                      *
			 * lpString1                                                                                               *
			 * Type: LPCTSTR                                                                                           *
			 * The first null-terminated string to be compared.                                                        *
			 * lpString2                                                                                               *
			 * Type: LPCTSTR                                                                                           *
			 * The second null-terminated string to be compared.                                                       *
			 * Type: int                                                                                               *
			 * If the string pointed to by lpString1 is less than the string pointed to by lpString2, the return value *
			 * is negative. If the string pointed to by lpString1 is greater than the string pointed to by lpString2,  *
			 * the return value is positive. If the strings are equal, the return value is zero.                       *
			 * �����ַ�������(���ִ�Сд)��                                                                            *
			 *     lpString1 <  lpString2�����ظ�����                                                                  *
			 *     lpString1 >  lpString2������������                                                                  *
			 *     lpString1 == lpString2��������                                                                      *
			 ***********************************************************************************************************/
			if (lstrcmp(token2Find, it->c_str()) == 0)
			{
				if (eraseArg)
				{
					params.erase(it);
				}
				return true;
			}
		}
		return false;
	}
	/****************************************************************************
	 * ����ָ������                                                             *
	 * ����ҵ�ָ���Ĳ�������ô��ȡָ��������ֵ��ͬʱ�Ӳ����б����Ƴ���Ӧ�Ĳ��� *
	 ****************************************************************************/
	bool getParamVal(TCHAR c, std::vector<generic_string> & params, generic_string & value)
	{
		value = TEXT("");
		size_t nbItems = params.size();

		for (size_t i = 0; i < nbItems; ++i)
		{
			const TCHAR * token = params.at(i).c_str();
			//dash, and enough chars
			if (lstrlen(token) >= 2 && token[0] == '-' && token[1] == c)
			{
				value = (token+2);
				params.erase(params.begin() + i);
				return true;
			}
		}
		return false;
	}

	/*********************************************************************************************
	 * ���Ҳ�������ָ������str��ͷ���ַ���������ҵ����Ӳ����б����Ƴ�ָ�����ַ�����Ȼ�󷵻�true *
	 *********************************************************************************************/
	bool getParamValFromString(const TCHAR *str, std::vector<generic_string> & params, generic_string & value)
	{
		value = TEXT("");
		size_t nbItems = params.size();

		for (size_t i = 0; i < nbItems; ++i)
		{
			const TCHAR * token = params.at(i).c_str();
			generic_string tokenStr = token;
			size_t pos = tokenStr.find(str);
			if (pos != generic_string::npos && pos == 0)
			{
				value = (token + lstrlen(str));
				params.erase(params.begin() + i);
				return true;
			}
		}
		return false;
	}

	/*
	 * ���Ҳ����е����ԣ����û���ҵ�����L_EXTERNAL
	 */
	LangType getLangTypeFromParam(std::vector<generic_string> & params)
	{
		generic_string langStr;
		if (!getParamVal('l', params, langStr))
		{
			return L_EXTERNAL;
		}
		return NppParameters::getLangIDFromStr(langStr.c_str());
	}

	/*
	 * ���ʻ����������ļ����ļ���
	 */
	generic_string getLocalizationPathFromParam(std::vector<generic_string> & params)
	{
		generic_string locStr;
		if (!getParamVal('L', params, locStr))
		{
			return TEXT("");
		}
		return NppParameters::getLocPathFromStr(locStr.c_str());
	}

	/*
	 * �Ӳ����б��л�ȡ��ֵ�͵Ĳ�������������б���û��ֵ������-1
	 */
	int getNumberFromParam(char paramName, std::vector<generic_string> & params, bool & isParamePresent)
	{
		generic_string numStr;
		if (!getParamVal(paramName, params, numStr))
		{
			isParamePresent = false;
			return -1;
		}
		isParamePresent = true;
		return WindowsApi::generic_atoi(numStr.c_str());
	}

	/*
	 * qn qt qf
	 */
	generic_string getEasterEggNameFromParam(std::vector<generic_string> & params, unsigned char & type)
	{
		generic_string EasterEggName;
		if (!getParamValFromString(TEXT("-qn"), params, EasterEggName))  // get internal easter egg
		{
			if (!getParamValFromString(TEXT("-qt"), params, EasterEggName)) // get user quote from cmdline argument
			{
				if (!getParamValFromString(TEXT("-qf"), params, EasterEggName)) // get user quote from a content of file
				{
					return TEXT("");
				}
				else
				{
					EasterEggName = relativeFilePathToFullFilePath(EasterEggName.c_str());
					type = 2; // quote content in file
				}
			}
			else // commandline quote
			{
				type = 1;
			}
		}
		else // easter egg
		{
			type = 0;
		}

		generic_string percentTwentyStr = TEXT("%20");
		generic_string spaceStr = TEXT(" ");
		size_t start_pos = 0;
		while ((start_pos = EasterEggName.find(percentTwentyStr, start_pos)) != std::string::npos)
		{
			EasterEggName.replace(start_pos, percentTwentyStr.length(), spaceStr);
			// Handles case where 'to' is a substring of 'from'
			start_pos += spaceStr.length();
		}

		return EasterEggName;
	}

	/*
	 * �Ӳ����л�ȡ�����ٶ�
	 */
	int getGhostTypingSpeedFromParam(std::vector<generic_string> & params)
	{
		generic_string speedStr;
		if (!getParamValFromString(TEXT("-qSpeed"), params, speedStr))
		{
			return -1;
		}
	
		int speed = std::stoi(speedStr, 0);
		if (speed <= 0 || speed > 3)
		{
			return -1;
		}

		return speed;
	}

	/*
	 * �����õĲ�����
	 */
	const TCHAR FLAG_MULTI_INSTANCE[]            = TEXT("-multiInst");
	const TCHAR FLAG_NO_PLUGIN[]                 = TEXT("-noPlugin");
	const TCHAR FLAG_READONLY[]                  = TEXT("-ro");
	const TCHAR FLAG_NOSESSION[]                 = TEXT("-nosession");
	const TCHAR FLAG_NOTABBAR[]                  = TEXT("-notabbar");
	const TCHAR FLAG_SYSTRAY[]                   = TEXT("-systemtray");
	const TCHAR FLAG_LOADINGTIME[]               = TEXT("-loadingTime");
	const TCHAR FLAG_HELP[]                      = TEXT("--help");
	const TCHAR FLAG_ALWAYS_ON_TOP[]             = TEXT("-alwaysOnTop");
	const TCHAR FLAG_OPENSESSIONFILE[]           = TEXT("-openSession");
	const TCHAR FLAG_RECURSIVE[]                 = TEXT("-r");
	const TCHAR FLAG_FUNCLSTEXPORT[]             = TEXT("-export=functionList");
	const TCHAR FLAG_PRINTANDQUIT[]              = TEXT("-quickPrint");
	const TCHAR FLAG_NOTEPAD_COMPATIBILITY[]     = TEXT("-notepadStyleCmdline");
	const TCHAR FLAG_OPEN_FOLDERS_AS_WORKSPACE[] = TEXT("-openFoldersAsWorkspace");

	/*
	 * �쳣����
	 */
	void doException(Notepad_plus_Window & notepad_plus_plus)
	{
		//disable exception handler after excpetion, we dont want corrupt data structurs to crash the exception handler
		Win32Exception::removeHandler();
		WindowsApi::MessageBox(Notepad_plus_Window::gNppHWND, TEXT("Notepad++ will attempt to save any unsaved data. However, dataloss is very likely."), TEXT("Recovery initiating"), MB_OK | MB_ICONINFORMATION);

		TCHAR tmpDir[1024];
		WindowsApi::GetTempPath(1024, tmpDir);
		generic_string emergencySavedDir = tmpDir;
		emergencySavedDir += TEXT("\\N++RECOV");

		bool res = notepad_plus_plus.emergency(emergencySavedDir);
		if (res)
		{
			generic_string displayText = TEXT("Notepad++ was able to successfully recover some unsaved documents, or nothing to be saved could be found.\r\nYou can find the results at :\r\n");
			displayText += emergencySavedDir;
			WindowsApi::MessageBox(Notepad_plus_Window::gNppHWND, displayText.c_str(), TEXT("Recovery success"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			WindowsApi::MessageBox(Notepad_plus_Window::gNppHWND, TEXT("Unfortunatly, Notepad++ was not able to save your work. We are sorry for any lost data."), TEXT("Recovery failure"), MB_OK | MB_ICONERROR);
		}
	}

	PWSTR advanceCmdLine(PWSTR pCmdLine, const generic_string & string)
	{
		const size_t len = string.length();
		while (true)
		{
			/*************************************************************************************************************************************************************************************************************************************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/strstr-wcsstr-mbsstr-mbsstr-l?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DZH-CN%26k%3Dk(%22CORECRT_WSTRING%2Fwcsstr%22);k(wcsstr);k(DevLang-C%2B%2B);k(TargetOS-Windows)%26rd%3Dtrue&view=vs-2019 *
			 * ����ָ���ַ����е������ַ����ĵ�һ��ƥ�����ָ�롣                                                                                                                                                                                                                                                                    *
			 *     ��Ҫ                                                                                                                                                                                                                                                                                                              *
			 *         _mbsstr �� _mbsstr_l �޷������� Windows ����ʱ��ִ�е�Ӧ�ó��� �й���ϸ��Ϣ�����                                                                                                                                                                                                                           *
			 *         ��ͨ�� Windows ƽ̨Ӧ���в�֧�ֵ� CRT ����(1)��                                                                                                                                                                                                                                                               *
			 *         ͨ�� Windows ƽ̨Ӧ���в�֧�ֵ� CRT ����(1) : https://docs.microsoft.com/zh-cn/cpp/cppcx/crt-functions-not-supported-in-universal-windows-platform-apps?view=vs-2019                                                                                                                                          *
			 *                                                                                                                                                                                                                                                                                                                       *
			 * char                * strstr   (const char          * str, const char          * strSearch); // C only                                                                                                                                                                                                                *
			 * char                * strstr   (char                * str, const char          * strSearch); // C++ only                                                                                                                                                                                                              *
			 * const char          * strstr   (const char          * str, const char          * strSearch); // C++ only                                                                                                                                                                                                              *
			 *                                                                                                                                                                                                                                                                                                                       *
			 * wchar_t             * wcsstr   (const wchar_t       * str, const wchar_t       * strSearch); // C only                                                                                                                                                                                                                *
			 * wchar_t             * wcsstr   (wchar_t             * str, const wchar_t       * strSearch); // C++ only                                                                                                                                                                                                              *
			 * const wchar_t       * wcsstr   (const wchar_t       * str, const wchar_t       * strSearch); // C++ only                                                                                                                                                                                                              *
			 *                                                                                                                                                                                                                                                                                                                       *
			 * unsigned char       * _mbsstr  (const unsigned char * str, const unsigned char * strSearch); // C only                                                                                                                                                                                                                *
			 * unsigned char       * _mbsstr  (unsigned char       * str, const unsigned char * strSearch); // C++ only                                                                                                                                                                                                              *
			 * const unsigned char * _mbsstr  (const unsigned char * str, const unsigned char * strSearch); // C++ only                                                                                                                                                                                                              *
			 *                                                                                                                                                                                                                                                                                                                       *
			 * unsigned char       * _mbsstr_l(const unsigned char * str, const unsigned char * strSearch, _locale_t locale); // C only                                                                                                                                                                                              *
			 * unsigned char       * _mbsstr_l(unsigned char       * str, const unsigned char * strSearch, _locale_t locale); // C++ only                                                                                                                                                                                            *
			 * const unsigned char * _mbsstr_l(const unsigned char * str, const unsigned char * strSearch, _locale_t locale); // C++ only                                                                                                                                                                                            *
			 *                                                                                                                                                                                                                                                                                                                       *
			 * str                                                                                                                                                                                                                                                                                                                   *
			 * Ҫ������ null ��ֹ���ַ�����                                                                                                                                                                                                                                                                                          *
			 * strSearch                                                                                                                                                                                                                                                                                                             *
			 * Ҫ�������� null ��β���ַ�����                                                                                                                                                                                                                                                                                        *
			 * locale                                                                                                                                                                                                                                                                                                                *
			 * Ҫʹ�õ��������á�                                                                                                                                                                                                                                                                                                    *
			 * ����ָ��str�е�һ�γ��ֵ�strSearch��ָ��; ���strSearchδ������str�У��򷵻� NULL�� ���                                                                                                                                                                                                                              *
			 * strSearchָ�򳤶�Ϊ����ַ���������������str��                                                                                                                                                                                                                                                                      *
			 * ��ע�U                                                                                                                                                                                                                                                                                                                *
			 * ��������һ��ָ�룬��ָ��ָ�� str ��strSearch�ĵ�һ��ƥ��� strstr ������������β�� null �ַ���                                                                                                                                                                                                                      *
			 * wcsstr �ǿ��ַ��汾�� strstr��_mbsstr �Ƕ��ֽ��ַ��汾�� wcsstr �Ĳ����ͷ���ֵ�ǿ��ַ��ַ�����                                                                                                                                                                                                                        *
			 * �� _mbsstr �����Ƕ��ֽ��ַ��ַ����� _mbsstr ����֤������� ���str��strSearchΪ NULL����������                                                                                                                                                                                                                      *
			 * Ч������������������֤(1)�������� �������ִ�м����� _mbsstr������errnoΪ EINVAL��������0��                                                                                                                                                                                                                     *
			 * strstr �� wcsstr ������֤������� �����������������������ͬ��Ϊ��                                                                                                                                                                                                                                                    *
			 * ������֤(1) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/parameter-validation?view=vs-2019                                                                                                                                                                                                                *
			 *     ��Ҫ                                                                                                                                                                                                                                                                                                              *
			 *         ��Щ�������ܴӻ������������������в�� ����������������������ϵͳ����Ϊ���ǿ�������ִ��������룬�����                                                                                                                                                                                                     *
			 *         ����û�б�֤��Ȩ�������� �й���ϸ��Ϣ������� ���⻺�������(2)��                                                                                                                                                                                                                                             *
			 * ���⻺�������(2) : https://docs.microsoft.com/zh-cn/windows/win32/SecBP/avoiding-buffer-overruns                                                                                                                                                                                                                     *
			 * �� C �У���Щ����ʹ�õ�һ�������ĳ���ָ�롣 �� C++ �У����������ؿ��á� ����ָ��const��ָ������ط���ָ��const��ָ��;                                                                                                                                                                                                 *
			 * ����ָ��ǳ�����ָ��İ汾����ָ��ǳ�����ָ�롣 �����Щ�����ĳ����ͷǳ����汾�����ã���ᶨ���                                                                                                                                                                                                                     *
			 * _CRT_CONST_CORRECT_OVERLOADS�� ������������ض� C++��Ҫ�ǳ�����Ϊ���붨����� _CONST_RETURN��                                                                                                                                                                                                                         *
			 * ���ֵ�� LC_CTYPE ����������������õ�Ӱ��;�й���ϸ��Ϣ�������setlocale��_wsetlocale(3)�� û�� _l��׺����Щ�����İ汾                                                                                                                                                                                                *
			 * ��������������ص���Ϊʹ�õ�ǰ��������;���� _l��׺�İ汾��ͬ��ֻ�������Ǹ��ô�����������ò����� �й���ϸ��Ϣ�������                                                                                                                                                                                                 *
			 * Locale(4)��                                                                                                                                                                                                                                                                                                           *
			 * setlocale��_wsetlocale(3) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=vs-2019                                                                                                                                                                                        *
			 * Locale(4) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/locale?view=vs-2019                                                                                                                                                                                                                                *
			 * һ���ı�����ӳ��                                                                                                                                                                                                                                                                                                      *
			 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                          *
			 * | TCHAR.H ���� | δ���� _UNICODE �� _MBCS | �Ѷ��� _MBCS | �Ѷ��� _UNICODE |                                                                                                                                                                                                                                          *
			 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                          *
			 * | _tcsstr      | strstr                   | _mbsstr      | wcsstr          |                                                                                                                                                                                                                                          *
			 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                          *
			 * | ������       | ������                   | _mbsstr_l    | ������          |                                                                                                                                                                                                                                          *
			 * +--------------+--------------------------+--------------+-----------------+                                                                                                                                                                                                                                          *
			 * Ҫ��U                                                                                                                                                                                                                                                                                                                *
			 * +---------------------+-------------------------+                                                                                                                                                                                                                                                                     *
			 * | ���������ص�ֵ      | ����ı�ͷ               |                                                                                                                                                                                                                                                                    *
			 * +---------------------+-------------------------+                                                                                                                                                                                                                                                                     *
			 * | strstr              | <string.h>                |                                                                                                                                                                                                                                                                   *
			 * +---------------------+-------------------------+                                                                                                                                                                                                                                                                     *
			 * | wcsstr              | <string.h> �� <wchar.h> |                                                                                                                                                                                                                                                                     *
			 * +---------------------+-------------------------+                                                                                                                                                                                                                                                                     *
			 * | _mbsstr�� _mbsstr_l | <mbstring.h>             |                                                                                                                                                                                                                                                                    *
			 * +---------------------+-------------------------+                                                                                                                                                                                                                                                                     *
			 * �йؼ����Եĸ�����Ϣ����μ� ������(5)��                                                                                                                                                                                                                                                                              *
			 * ������(5) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/compatibility?view=vs-2019                                                                                                                                                                                                                         *
			 * ʾ��                                                                                                                                                                                                                                                                                                                  *
			 * C                                                                                                                                                                                                                                                                                                                     *
			 * // crt_strstr.c                                                                                                                                                                                                                                                                                                       *
			 * #include <string.h>                                                                                                                                                                                                                                                                                                   *
			 * #include <stdio.h>                                                                                                                                                                                                                                                                                                    *
			 *                                                                                                                                                                                                                                                                                                                       *
			 * char str[] =    "lazy";                                                                                                                                                                                                                                                                                               *
			 * char string[] = "The quick brown dog jumps over the lazy fox";                                                                                                                                                                                                                                                        *
			 * char fmt1[] =   "         1         2         3         4         5";                                                                                                                                                                                                                                                 *
			 * char fmt2[] =   "12345678901234567890123456789012345678901234567890";                                                                                                                                                                                                                                                 *
			 *                                                                                                                                                                                                                                                                                                                       *
			 * int main( void )                                                                                                                                                                                                                                                                                                      *
			 * {                                                                                                                                                                                                                                                                                                                     *
			 *    char *pdest;                                                                                                                                                                                                                                                                                                       *
			 *    int  result;                                                                                                                                                                                                                                                                                                       *
			 *    printf( "String to be searched:\n   %s\n", string );                                                                                                                                                                                                                                                               *
			 *    printf( "   %s\n   %s\n\n", fmt1, fmt2 );                                                                                                                                                                                                                                                                          *
			 *    pdest = strstr( string, str );                                                                                                                                                                                                                                                                                     *
			 *    result = (int)(pdest - string + 1);                                                                                                                                                                                                                                                                                *
			 *    if ( pdest != NULL )                                                                                                                                                                                                                                                                                               *
			 *       printf( "%s found at position %d\n", str, result );                                                                                                                                                                                                                                                             *
			 *    else                                                                                                                                                                                                                                                                                                               *
			 *       printf( "%s not found\n", str );                                                                                                                                                                                                                                                                                *
			 * }                                                                                                                                                                                                                                                                                                                     *
			 * Output                                                                                                                                                                                                                                                                                                                *
			 * String to be searched:                                                                                                                                                                                                                                                                                                *
			 * The quick brown dog jumps over the lazy fox                                                                                                                                                                                                                                                                           *
			 * 1         2         3         4         5                                                                                                                                                                                                                                                                             *
			 * 12345678901234567890123456789012345678901234567890                                                                                                                                                                                                                                                                    *
			 *                                                                                                                                                                                                                                                                                                                       *
			 * lazy found at position 36                                                                                                                                                                                                                                                                                             *
			 *************************************************************************************************************************************************************************************************************************************************************************************************************************/
			PWSTR ignoredString = wcsstr(pCmdLine, string.c_str());
			if (ignoredString == nullptr)
			{
				// Should never happen - tokenized parameters contain string somewhere, so it HAS to match
				// This is there just in case
				break;
			}
	
			// Match the substring only if it matched an entire substring        
			/************************************************************************************************************************************************************************************************************************************************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/isspace-iswspace-isspace-l-iswspace-l?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev15.query%3FappId%3DDev15IDEF1%26l%3DZH-CN%26k%3Dk(%22CORECRT_WCTYPE%2Fiswspace%22);k(iswspace);k(DevLang-C%2B%2B);k(TargetOS-Windows)%26rd%3Dtrue&view=vs-2019 *
			 * isspace��iswspace��_isspace_l��_iswspace_l                                                                                                                                                                                                                                                                                       *
			 * ȷ�������Ƿ��ʾ�հ��ַ���                                                                                                                                                                                                                                                                                                       *
			 * int isspace    (int    c);                                                                                                                                                                                                                                                                                                       *
			 * int iswspace   (wint_t c);                                                                                                                                                                                                                                                                                                       *
			 * int _isspace_l (int    c, _locale_t locale);                                                                                                                                                                                                                                                                                     *
			 * int _iswspace_l(wint_t c, _locale_t locale);                                                                                                                                                                                                                                                                                     *
			 * c                                                                                                                                                                                                                                                                                                                                *
			 * Ҫ���Ե�������                                                                                                                                                                                                                                                                                                                   *
			 * locale                                                                                                                                                                                                                                                                                                                           *
			 * Ҫʹ�õ��������á�                                                                                                                                                                                                                                                                                                               *
			 * ���c�ǿհ��ַ����ض���ʾ��ʽ����ÿ�����̽����ط���ֵ�� ���c�ǿհ��ַ���0X09-0x0D ��0x20����                                                                                                                                                                                                                                    *
			 * ��isspace������һ������ֵ��Isspace�����Ĳ��������Ľ��ȡ�����������õ�LC_CTYPE�������;�й���                                                                                                                                                                                                                                    *
			 * ϸ��Ϣ�������setlocale��_wsetlocale(1) ����Щ���� _l��׺�ĺ����İ汾��������������ص���Ϊʹ                                                                                                                                                                                                                                    *
			 * �õ�ǰ��������;���� _l��׺�İ汾����ͬ�ģ�ֻ��������ʹ�ô�����������á� �й���ϸ��Ϣ�������                                                                                                                                                                                                                                    *
			 * Locale(2)��                                                                                                                                                                                                                                                                                                                      *
			 * setlocale��_wsetlocale(1) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=vs-2019                                                                                                                                                                                                   *
			 * Locale(2) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/locale?view=vs-2019                                                                                                                                                                                                                                           *
			 * ���c�Ƕ�Ӧ�ڱ�׼�հ��ַ��Ŀ��ַ�����iswspace������һ������ֵ��                                                                                                                                                                                                                                                                  *
			 * ���c���� EOF �����0��0xff ����0��0xff������isspace�� _isspace_l����Ϊ�ǲ�ȷ���ġ� ��ʹ�õ���                                                                                                                                                                                                                                   *
			 * CRT �Ⲣ��c������Щֵ֮һʱ���������������ԡ�                                                                                                                                                                                                                                                                                    *
			 * һ���ı�����ӳ��                                                                                                                                                                                                                                                                                                                 *
			 * +--------------+--------------------------+---------------------+----------+                                                                                                                                                                                                                                                     *
			 * | TCHAR.H ���� | δ���� _UNICODE �� _MBCS | �Ѷ��� _MBCS �Ѷ��� | _UNICODE |                                                                                                                                                                                                                                                     *
			 * +--------------+--------------------------+---------------------+----------+                                                                                                                                                                                                                                                     *
			 * | _istspace    | isspace                  | _ismbcspace(3)      | iswspace |                                                                                                                                                                                                                                                     *
			 * +--------------+--------------------------+---------------------+----------+                                                                                                                                                                                                                                                     *
			 * _ismbcspace(3) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/ismbcgraph-functions?view=vs-2019                                                                                                                                                                                                              *
			 * Ҫ��U                                                                                                                                                                                                                                                                                                                           *
			 * +----------------+----------------------------+                                                                                                                                                                                                                                                                                  *
			 * | ���������ص�ֵ | ����ı�ͷ                 |                                                                                                                                                                                                                                                                                  *
			 * +----------------+----------------------------+                                                                                                                                                                                                                                                                                  *
			 * | isspace        | <ctype.h>                     |                                                                                                                                                                                                                                                                               *
			 * +----------------+----------------------------+                                                                                                                                                                                                                                                                                  *
			 * | iswspace       | <ctype.h 1> �� <wchar.h 1> |                                                                                                                                                                                                                                                                                  *
			 * +----------------+----------------------------+                                                                                                                                                                                                                                                                                  *
			 * | _isspace_l     | <ctype.h>                  |                                                                                                                                                                                                                                                                                  *
			 * +----------------+----------------------------+                                                                                                                                                                                                                                                                                  *
			 * | _iswspace_l    | <ctype.h 1> �� <wchar.h 1> |                                                                                                                                                                                                                                                                                  *
			 * +----------------+----------------------------+                                                                                                                                                                                                                                                                                  *
			 * �й�������������Ϣ������� ������(4)��                                                                                                                                                                                                                                                                                           *
			 * ������(4) : https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/compatibility?view=vs-2019                                                                                                                                                                                                                                    *
			 ************************************************************************************************************************************************************************************************************************************************************************************************************************************/
			if ( (ignoredString == pCmdLine || iswspace(*(ignoredString-1)) ) && // Check start
				 (iswspace(*(ignoredString+len)) || *(ignoredString+len) == '\0') )
			{
				ignoredString += len;

				// Advance to the first non-whitespace and not quotation mark character
				while (iswspace( *ignoredString ) || *ignoredString == L'"' )
				{
					++ignoredString;
				}
				pCmdLine = ignoredString;
				break;
			}
			else
			{
				pCmdLine = ignoredString+len; // Just skip this match and resume from another
			}
		}
		return pCmdLine;
	}

	/*
	 * Looks for -z arguments and strips command line arguments following those, if any
	 * Also advances pCmdLine to point after the last ignored parameter
	 * -notepadStyleCmdline is also considered an ignored parameter here,
	 * as we don't want it to be part of the assembled file name
	 */
	PWSTR stripIgnoredParams(std::vector<generic_string> & params, PWSTR pCmdLine)
	{
		for ( auto it = params.begin(); it != params.end(); )
		{
			if (WindowsApi::lstrcmp(it->c_str(), TEXT("-z")) == 0)
			{
				pCmdLine = advanceCmdLine(pCmdLine, *it);

				auto nextIt = std::next(it);
				if ( nextIt != params.end() )
				{
					pCmdLine = advanceCmdLine(pCmdLine, *nextIt);
					params.erase(nextIt);
				}
				it = params.erase(it);
			}
			else if (WindowsApi::lstrcmp(it->c_str(), FLAG_NOTEPAD_COMPATIBILITY) == 0)
			{
				pCmdLine = advanceCmdLine(pCmdLine, *it++);
			}
			else
			{
				++it;
			}
		}
		return pCmdLine;
	}

} // namespace




int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int)
{
	/*
	 * �����б�
	 */
	std::vector<generic_string> params                 = parseCommandLine(pCmdLine);
	PWSTR                       pCmdLineWithoutIgnores = stripIgnoredParams(params, pCmdLine);

	//for debugging purposes.
	MiniDumper mdump;

	/*******************************
	 * Notepad ++ ����ʱ��Ҫ�Ĳ��� *
	 *******************************/
	NppParametersInitial aaa;
	NppParameters * pNppParameters = NppParameters::getInstance();

	/*********************************************
	 * Notepad ++ ����ʱ��Ҫ�Ľ������(���޸�)   *
	 *********************************************/
	NppGUI & nppGui = const_cast<NppGUI &>(pNppParameters->getNppGUI()); /* pNppParameters->getNppGUI() 1 */
	/*********************************************
	 * Notepad ++ ����ʱ��Ҫ�Ľ������(�����޸�) *
	 *********************************************/
	const NppGUI & nppGUI = pNppParameters->getNppGUI(); /* pNppParameters->getNppGUI() 2 */

	/*
	 * Shell������װ
	 */
	CmdLineParams cmdLineParams;

	/******************************
	 * ��ʼ����̬���ӿ�ĺ���ָ�� *
	 ******************************/
	const RealKesicLeeDynamicLinkLibraryLoad & obj = getRealKesicLeeDynamicLinkLibraryLoad();
	initialRealKesicLeeDynamicLinkLibraryLoad(obj);

	/*
	 * Mutex���
	 */
	HANDLE nppMutex = nullptr;

	/*
	 * ��Ҫ�򿪵��ļ��ַ�������˫���� + �ո� + ˫������Ϊ�ָ�
	 */
	generic_string quotFileName = TEXT("");

	/*
	 * ��ʵ���Ƿ��ǵ�����ʵ��
	 * true  : �ǵ�����ʵ��
	 */
	bool TheFirstOne = true;

	/*
	 * ��λ����
	 */
	bool isParamePresent;
	/*
	 * ��ʾ����
	 */
	bool showHelp             = isInList(FLAG_HELP, params);
	/*
	 * �Ƿ��ʵ��
	 */
	bool isMultiInst          = isInList(FLAG_MULTI_INSTANCE, params);
	/*
	 * �Ƿ��ܵ������˳�
	 */
	bool doFunctionListExport = isInList(FLAG_FUNCLSTEXPORT, params);
	/*
	 * �Ƿ��ӡ���˳�
	 */
	bool doPrintAndQuit       = isInList(FLAG_PRINTANDQUIT, params);

	bool doUpdateNpp        = nppGui._autoUpdateOpt._doAutoUpdate; /* �Զ�����NPP  */
	bool doUpdatePluginList = nppGui._autoUpdateOpt._doAutoUpdate; /* �������²�� */

	WindowsApi::SetLastError(NO_ERROR);
	/************************************************************************************************************************************************************
	 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/synchapi/nf-synchapi-createmutexw                                                              *
	 * function                                                                                                                                                 *
	 * Creates or opens a named or unnamed mutex object.                                                                                                        *
	 * To specify an access mask for the object, use the CreateMutexEx(1) function.                                                                             *
	 * HANDLE CreateMutexW(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCWSTR lpName);                                                        *
	 * lpMutexAttributes                                                                                                                                        *
	 * A pointer to a SECURITY_ATTRIBUTES(2) structure. If this parameter is NULL, the handle cannot be                                                         *
	 * inherited by child processes.                                                                                                                            *
	 * The lpSecurityDescriptor member of the structure specifies a security descriptor for the new mutex.                                                      *
	 * If lpMutexAttributes is NULL, the mutex gets a default security descriptor.                                                                              *
	 * The ACLs in the default security descriptor for a mutex come from the primary or impersonation token of the creator.                                     *
	 * For more information, see Synchronization Object Security and Access Rights(3).                                                                          *
	 * SECURITY_ATTRIBUTES(2) : https://docs.microsoft.com/previous-versions/windows/desktop/legacy/aa379560(v=vs.85)                                           *
	 * Synchronization Object Security and Access Rights(3) : https://docs.microsoft.com/windows/desktop/Sync/synchronization-object-security-and-access-rights *
	 * bInitialOwner                                                                                                                                            *
	 * If this value is TRUE and the caller created the mutex, the calling thread obtains initial ownership of the mutex object.                                *
	 * Otherwise, the calling thread does not obtain ownership of the mutex. To determine if the caller created the mutex,                                      *
	 * see the Return Values section.                                                                                                                           *
	 * lpName                                                                                                                                                   *
	 * The name of the mutex object. The name is limited to MAX_PATH characters. Name comparison is case sensitive.                                             *
	 * If lpName matches the name of an existing named mutex object, this function requests the MUTEX_ALL_ACCESS access right.                                  *
	 * In this case, the bInitialOwner parameter is ignored because it has already been set by the creating process.                                            *
	 * If the lpMutexAttributes parameter is not NULL, it determines whether the handle can be inherited,                                                       *
	 * but its security-descriptor member is ignored.                                                                                                           *
	 * If lpName is NULL, the mutex object is created without a name.                                                                                           *
	 * If lpName matches the name of an existing event, semaphore, waitable timer, job, or file-mapping object,                                                 *
	 * the function fails and the GetLastError function returns ERROR_INVALID_HANDLE.                                                                           *
	 * This occurs because these objects share the same namespace.                                                                                              *
	 * The name can have a "Global" or "Local" prefix to explicitly create the object in the global or session namespace.                                       *
	 * The remainder of the name can contain any character except the backslash character ().                                                                   *
	 * For more information, see Kernel Object Namespaces(3). Fast user switching is implemented using Terminal Services sessions.                              *
	 * Kernel object names must follow the guidelines outlined for Terminal Services so that applications can support multiple users.                           *
	 * Kernel Object Namespaces(3) : https://docs.microsoft.com/windows/desktop/TermServ/kernel-object-namespaces                                               *
	 * The object can be created in a private namespace. For more information, see Object Namespaces(4).                                                        *
	 * Object Namespaces(4) : https://docs.microsoft.com/windows/desktop/Sync/object-namespaces                                                                 *
	 * Return Value�U                                                                                                                                           *
	 * If the function succeeds, the return value is a handle to the newly created mutex object.                                                                *
	 * If the function fails, the return value is NULL. To get extended error information, call GetLastError.                                                   *
	 * If the mutex is a named mutex and the object existed before this function call, the return value is a handle to the existing object,                     *
	 * GetLastError returns ERROR_ALREADY_EXISTS, bInitialOwner is ignored, and the calling thread is not granted ownership.                                    *
	 * However, if the caller has limited access rights, the function will fail with ERROR_ACCESS_DENIED and the caller should use                              *
	 * the OpenMutex(5) function.                                                                                                                               *
	 * OpenMutex(5) : https://docs.microsoft.com/windows/desktop/api/winbase/nf-winbase-openmutexa                                                              *
	 * Remarks�U                                                                                                                                                *
	 * The handle returned by CreateMutex has the MUTEX_ALL_ACCESS access right;                                                                                *
	 * it can be used in any function that requires a handle to a mutex object,                                                                                 *
	 * provided that the caller has been granted access.                                                                                                        *
	 * If a mutex is created from a service or a thread that is impersonating a different user,                                                                 *
	 * you can either apply a security descriptor to the mutex when you create it,                                                                              *
	 * or change the default security descriptor for the creating process by changing its default DACL.                                                         *
	 * For more information, see Synchronization Object Security and Access Rights(6).                                                                          *
	 * Synchronization Object Security and Access Rights(6) : https://docs.microsoft.com/windows/desktop/Sync/synchronization-object-security-and-access-rights *
	 * If you are using a named mutex to limit your application to a single instance,                                                                           *
	 * a malicious user can create this mutex before you do and prevent your application from starting.                                                         *
	 * To prevent this situation, create a randomly named mutex and store the name so that it can only be obtained by an                                        *
	 * authorized user. Alternatively, you can use a file for this purpose. To limit your application to one instance per user,                                 *
	 * create a locked file in the user's profile directory.                                                                                                    *
	 * Any thread of the calling process can specify the mutex-object handle in a call to one of the wait functions(7).                                         *
	 * The single-object wait functions return when the state of the specified object is signaled.                                                              *
	 * The multiple-object wait functions can be instructed to return either when any one or when all of the specified objects                                  *
	 * are signaled. When a wait function returns, the waiting thread is released to continue its execution.                                                    *
	 * wait functions(7) : https://docs.microsoft.com/windows/desktop/Sync/wait-functions                                                                       *
	 * The state of a mutex object is signaled when it is not owned by any thread.                                                                              *
	 * The creating thread can use the bInitialOwner flag to request immediate ownership of the mutex.                                                          *
	 * Otherwise, a thread must use one of the wait functions to request ownership. When the mutex's state is signaled,                                         *
	 * one waiting thread is granted ownership, the mutex's state changes to nonsignaled, and the wait function returns.                                        *
	 * Only one thread can own a mutex at any given time. The owning thread uses the ReleaseMutex function to release its ownership.                            *
	 * The thread that owns a mutex can specify the same mutex in repeated wait function calls without blocking its execution.                                  *
	 * Typically, you would not wait repeatedly for the same mutex, but this mechanism prevents a thread from deadlocking itself                                *
	 * while waiting for a mutex that it already owns. However, to release its ownership, the thread must call ReleaseMutex(8) once                             *
	 * for each time that the mutex satisfied a wait.                                                                                                           *
	 * ReleaseMutex(8) : https://docs.microsoft.com/windows/desktop/api/synchapi/nf-synchapi-releasemutex                                                       *
	 * Two or more processes can call CreateMutex to create the same named mutex. The first process actually creates the mutex,                                 *
	 * and subsequent processes with sufficient access rights simply open a handle to the existing mutex.                                                       *
	 * This enables multiple processes to get handles of the same mutex, while relieving the user of the responsibility of                                      *
	 * ensuring that the creating process is started first. When using this technique, you should set the bInitialOwner flag to FALSE;                          *
	 * otherwise, it can be difficult to be certain which process has initial ownership.                                                                        *
	 * Multiple processes can have handles of the same mutex object, enabling use of the object for interprocess synchronization.                               *
	 * The following object-sharing mechanisms are available:                                                                                                   *
	 *     * A child process created by the CreateProcess(9) function can inherit a handle to a mutex object if the lpMutexAttributes                           *
	 *       parameter of CreateMutex enabled inheritance. This mechanism works for both named and unnamed mutexes.                                             *
	 *     * A process can specify the handle to a mutex object in a call to the DuplicateHandle(10) function to create a duplicate handle                      *
	 *       that can be used by another process. This mechanism works for both named and unnamed mutexes.                                                      *
	 *     * A process can specify a named mutex in a call to the OpenMutex(11) or CreateMutex function to retrieve a handle to the mutex object.               *
	 * CreateProcess(9) : https://docs.microsoft.com/windows/desktop/api/processthreadsapi/nf-processthreadsapi-createprocessa                                  *
	 * DuplicateHandle(10) : https://docs.microsoft.com/windows/desktop/api/handleapi/nf-handleapi-duplicatehandle                                              *
	 * OpenMutex(11) : https://docs.microsoft.com/windows/desktop/api/winbase/nf-winbase-openmutexa                                                             *
	 * Use the CloseHandle(12) function to close the handle. The system closes the handle automatically when the process terminates.                            *
	 * The mutex object is destroyed when its last handle has been closed.                                                                                      *
	 * CloseHandle(12) : https://docs.microsoft.com/windows/desktop/api/handleapi/nf-handleapi-closehandle                                                      *
	 * �½�һ���������                                                                                                                                         *
	 * ���ִ�гɹ���������һ�»�����ľ��                                                                                                                     *
	 * ���ִ��ʧ�ܣ������ؿգ�ͬʱ��ʹ��GetLastError�����鿴ʧ��ԭ��                                                                                           *
	 * Examples�U                                                                                                                                               *
	 * For an example that uses CreateMutex, see Using Mutex Objects(13).                                                                                       *
	 * Using Mutex Objects(13) : https://docs.microsoft.com/windows/desktop/Sync/using-mutex-objects                                                            *
	 ************************************************************************************************************************************************************/
	nppMutex = WindowsApi::CreateMutex(NULL, false, TEXT("nppInstance"));
	if (!nppMutex || WindowsApi::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		TheFirstOne = false;
	}

	// Convert commandline to notepad-compatible format, if applicable
	if ( isInList(FLAG_NOTEPAD_COMPATIBILITY, params) )
	{
		params = convertParamsToNotepadStyle(pCmdLineWithoutIgnores);
	}
	/*
	 * ���ز�����������ֻ��дһ�Σ����ֶ���Դ򿪵��ļ�����
	 */
	cmdLineParams._isNoTab                = isInList(FLAG_NOTABBAR,                  params);
	cmdLineParams._isNoPlugin             = isInList(FLAG_NO_PLUGIN,                 params);
	cmdLineParams._isReadOnly             = isInList(FLAG_READONLY,                  params);
	cmdLineParams._isNoSession            = isInList(FLAG_NOSESSION,                 params);
	cmdLineParams._isPreLaunch            = isInList(FLAG_SYSTRAY,                   params);
	cmdLineParams._alwaysOnTop            = isInList(FLAG_ALWAYS_ON_TOP,             params);
	cmdLineParams._showLoadingTime        = isInList(FLAG_LOADINGTIME,               params);
	cmdLineParams._isSessionFile          = isInList(FLAG_OPENSESSIONFILE,           params);
	cmdLineParams._isRecursive            = isInList(FLAG_RECURSIVE,                 params);
	cmdLineParams._openFoldersAsWorkspace = isInList(FLAG_OPEN_FOLDERS_AS_WORKSPACE, params);

	/*
	 * ǰ׺��������
	 */
	cmdLineParams._langType         = getLangTypeFromParam(params);
	cmdLineParams._localizationPath = getLocalizationPathFromParam(params);
	cmdLineParams._easterEggName    = getEasterEggNameFromParam(params, cmdLineParams._quoteType);
	cmdLineParams._ghostTypingSpeed = getGhostTypingSpeedFromParam(params);

	// getNumberFromParam should be run at the end, to not consuming the other params
	/*
	 * ǰ׺����������
	 */
	cmdLineParams._line2go   = getNumberFromParam('n', params, isParamePresent);
	cmdLineParams._column2go = getNumberFromParam('c', params, isParamePresent);
	cmdLineParams._pos2go    = getNumberFromParam('p', params, isParamePresent);
	cmdLineParams._point.x   = getNumberFromParam('x', params, cmdLineParams._isPointXValid);
	cmdLineParams._point.y   = getNumberFromParam('y', params, cmdLineParams._isPointYValid);

	/****************
	 * ��ʾ������Ϣ *
	 ****************/
	if (showHelp)
	{
		WindowsApi::MessageBox(NULL, COMMAND_ARG_HELP, TEXT("Notepad++ Command Argument Help"), MB_OK);
	}

	/*
	 * export functionlist feature will serialize fuctionlist on the disk, then exit Notepad++.
	 * So it's important to not launch into existing instance, and keep it silent.
	 */
	/*
	 * �����ܵ����ʹ�ӡ�˳�
	 */
	if ( ( doFunctionListExport ) || ( doPrintAndQuit ) )
	{
		isMultiInst = true;
		doUpdateNpp = doUpdatePluginList = false;
		cmdLineParams._isNoSession = true;
	}

	if (cmdLineParams._localizationPath != TEXT(""))
	{
		pNppParameters->setStartWithLocFileName(cmdLineParams._localizationPath);
	}

	/***********************************
	 * Notepad ++ ��������ʱ��Ҫ�Ĳ��� *
	 ***********************************/
	pNppParameters->load();

	pNppParameters->setFunctionListExportBoolean(doFunctionListExport);
	pNppParameters->setPrintAndExitBoolean(doPrintAndQuit);

	// override the settings if notepad style is present
	if (pNppParameters->asNotepadStyle())
	{
		isMultiInst                = true;
		cmdLineParams._isNoTab     = true;
		cmdLineParams._isNoSession = true;
	}

	// override the settings if multiInst is choosen by user in the preference dialog
	if (nppGUI._multiInstSetting == multiInst)
	{
		isMultiInst = true;
		// Only the first launch remembers the session
		if (!TheFirstOne)
		{
			cmdLineParams._isNoSession = true;
		}
	}

	// tell the running instance the FULL path to the new files to load
	size_t nbFilesToOpen = params.size();

	for (size_t i = 0; i < nbFilesToOpen; ++i)
	{
		const TCHAR * currentFile = params.at(i).c_str();
		if (currentFile[0])
		{
			//check if relative or full path. Relative paths dont have a colon for driveletter

			quotFileName += TEXT("\"");
			quotFileName += relativeFilePathToFullFilePath(currentFile);
			quotFileName += TEXT("\" ");
		}
	}

	//Only after loading all the file paths set the working directory
	::SetCurrentDirectory(NppParameters::getInstance()->getNppPath().c_str());    //force working directory to path of module, preventing lock

	/*
	 * �ڲ������ʵ����ͬʱ���ֶ��ʵ��ʱ
	 */
	if ((!isMultiInst) && (!TheFirstOne))
	{
		/*
		 * ����ָ���Ĵ�����������ָ���Ĵ���
		 */
		/********************************************************************************************************************************
		 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-findwindoww                                     *
		 * function                                                                                                                     *
		 * Retrieves a handle to the top-level window whose class name and window name match the                                        *
		 * specified strings. This function does not search child windows. This function does not perform a                             *
		 * case-sensitive search.                                                                                                       *
		 * To search child windows, beginning with a specified child window, use the FindWindowEx(1) function.                          *
		 * FindWindowEx(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-findwindowexa                            *
		 * HWND FindWindowW(LPCWSTR lpClassName, LPCWSTR lpWindowName);                                                                 *
		 * ����ָ���Ĵ��������ʹ���������ָ���Ĵ���                                                                                     *
		 * lpClassName                                                                                                                  *
		 * Type: LPCTSTR                                                                                                                *
		 * The class name or a class atom created by a previous call to the RegisterClass or RegisterClassEx                            *
		 * function. The atom must be in the low-order word of lpClassName; the high-order word must be zero.                           *
		 * If lpClassName points to a string, it specifies the window class name. The class name can be any                             *
		 * name registered with RegisterClass or RegisterClassEx, or any of the predefined control-class names.                         *
		 * If lpClassName is NULL, it finds any window whose title matches the lpWindowName parameter.                                  *
		 * lpWindowName                                                                                                                 *
		 * Type: LPCTSTR                                                                                                                *
		 * The window name (the window's title). If this parameter is NULL, all window names match.                                     *
		 * Return Value�U                                                                                                               *
		 * Type: Type: HWND                                                                                                             *
		 * If the function succeeds, the return value is a handle to the window that has the specified class                            *
		 * name and window name.                                                                                                        *
		 * If the function fails, the return value is NULL. To get extended error information, call GetLastError.                       *
		 * Remarks�U                                                                                                                    *
		 * If the lpWindowName parameter is not NULL, FindWindow calls the GetWindowText(2) function to retrieve                        *
		 * the window name for comparison. For a description of a potential problem that can arise, see the                             *
		 * Remarks for GetWindowText.                                                                                                   *
		 * GetWindowText(2) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getwindowtexta                          *
		 * Examples�U                                                                                                                   *
		 * For an example, see Retrieving the Number of Mouse Wheel Scroll Lines(3).                                                    *
		 * Retrieving the Number of Mouse Wheel Scroll Lines(3) : https://docs.microsoft.com/windows/desktop/inputdev/using-mouse-input *
		 ********************************************************************************************************************************/
		HWND hNotepad_plus = ::FindWindow(Notepad_plus_Window::getClassName(), NULL);
		for (int i = 0 ;!hNotepad_plus && i < 5 ; ++i)
		{
			/***************************************************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/synchapi/nf-synchapi-sleep?redirectedfrom=MSDN                            *
			 * function                                                                                                                            *
			 * Suspends the execution of the current thread until the time-out interval elapses.                                                   *
			 * To enter an alertable wait state, use the SleepEx(1) function.                                                                      *
			 * SleepEx(1) : https://docs.microsoft.com/windows/desktop/api/synchapi/nf-synchapi-sleepex                                            *
			 * void Sleep(DWORD dwMilliseconds);                                                                                                   *
			 * dwMilliseconds                                                                                                                      *
			 * The time interval for which execution is to be suspended, in milliseconds.                                                          *
			 * A value of zero causes the thread to relinquish the remainder of its time slice to any other thread that                            *
			 * is ready to run. If there are no other threads ready to run, the function returns immediately,                                      *
			 * and the thread continues execution.Windows XP:  A value of zero causes the thread to relinquish                                     *
			 * the remainder of its time slice to any other thread of equal priority that is ready to run.                                         *
			 * If there are no other threads of equal priority ready to run, the function returns immediately,                                     *
			 * and the thread continues execution. This behavior changed starting with Windows Server 2003.                                        *
			 * A value of INFINITE indicates that the suspension should not time out.                                                              *
			 * Return Value�U                                                                                                                      *
			 * This function does not return a value.                                                                                              *
			 * Remarks�U                                                                                                                           *
			 * This function causes a thread to relinquish the remainder of its time slice and become unrunnable                                   *
			 * for an interval based on the value of dwMilliseconds. The system clock "ticks" at a constant rate.                                  *
			 * If dwMilliseconds is less than the resolution of the system clock, the thread may sleep for less than the                           *
			 * specified length of time. If dwMilliseconds is greater than one tick but less than two, the wait can be                             *
			 * anywhere between one and two ticks, and so on. To increase the accuracy of the sleep interval,                                      *
			 * call the timeGetDevCaps function to determine the supported minimum timer resolution and                                            *
			 * the timeBeginPeriod function to set the timer resolution to its minimum.                                                            *
			 * Use caution when calling timeBeginPeriod, as frequent calls can significantly affect the system clock,                              *
			 * system power usage, and the scheduler. If you call timeBeginPeriod, call it one time early in                                       *
			 * the application and be sure to call the timeEndPeriod function at the very end of the application.                                  *
			 * After the sleep interval has passed, the thread is ready to run.                                                                    *
			 * If you specify 0 milliseconds, the thread will relinquish the remainder of its time slice but remain ready.                         *
			 * Note that a ready thread is not guaranteed to run immediately.                                                                      *
			 * Consequently, the thread may not run until some time after the sleep interval elapses. For more information,                        *
			 * see Scheduling Priorities(1).                                                                                                       *
			 * Scheduling Priorities(1) : https://docs.microsoft.com/windows/desktop/ProcThread/scheduling-priorities                              *
			 * Be careful when using Sleep in the following scenarios:                                                                             *
			 *     * Code that directly or indirectly creates windows (for example, DDE and COM CoInitialize).                                     *
			 *       If a thread creates any windows, it must process messages. Message broadcasts are sent to all windows in the system.          *
			 *       If you have a thread that uses Sleep with infinite delay, the system will deadlock.                                           *
			 *     * Threads that are under concurrency control. For example, an I/O completion port or thread pool limits the number of           *
			 *       associated threads that can run. If the maximum number of threads is already running,                                         *
			 *       no additional associated thread can run until a running thread finishes.                                                      *
			 *       If a thread uses Sleep with an interval of zero to wait for one of the additional associated threads to accomplish some work, *
			 *       the process might deadlock.                                                                                                   *
			 * For these scenarios, use MsgWaitForMultipleObjects(2) or MsgWaitForMultipleObjectsEx(3), rather than Sleep.                         *
			 * MsgWaitForMultipleObjects(2) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-msgwaitformultipleobjects          *
			 * MsgWaitForMultipleObjectsEx(3) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-msgwaitformultipleobjectsex      *
			 * Windows Phone 8.1: This function is supported for Windows Phone Store apps on Windows Phone 8.1 and later.                          *
			 * Windows 8.1 and Windows Server 2012 R2: This function is supported for Windows Store apps on Windows 8.1,                           *
			 * Windows Server 2012 R2, and later.                                                                                                  *
			 * Examples�U                                                                                                                          *
			 * For an example, see Using Thread Local Storage(4).                                                                                  *
			 * Using Thread Local Storage(4) : https://docs.microsoft.com/windows/desktop/ProcThread/using-thread-local-storage                    *
			 * ��ͣ��ǰ���߳�XXX���룬�������Ϊ0�������ǰ��ʱ��Ƭ                                                                                *
			 ***************************************************************************************************************************************/
			::Sleep(100);
			hNotepad_plus = ::FindWindow(Notepad_plus_Window::getClassName(), NULL);
		}

		/*
		 * ����ҵ�����ָ���Ĵ��ڷ���Ϣ��ͬʱ�˳�
		 */
		if (hNotepad_plus)
		{
			// First of all, destroy static object NppParameters
			pNppParameters->destroyInstance();

			int sw = 0;

			/*************************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-iszoomed?redirectedfrom=MSDN *
			 * function                                                                                                  *
			 * Determines whether a window is maximized.                                                                 *
			 * ��ָ���Ĵ������                                                                                        *
			 * �ɹ������棬ʧ�ܷ��ؼ�                                                                                    *
			 * BOOL IsZoomed(HWND hWnd);                                                                                 *
			 * hWnd                                                                                                      *
			 * Type: HWND                                                                                                *
			 * A handle to the window to be tested.                                                                      *
			 * Return Value�U                                                                                            *
			 * Type: Type: BOOL                                                                                          *
			 * If the window is zoomed, the return value is nonzero.                                                     *
			 * If the window is not zoomed, the return value is zero.                                                    *
			 *************************************************************************************************************/
			if (::IsZoomed(hNotepad_plus))
			{
				sw = SW_MAXIMIZE;
			}
			/*************************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-isiconic?redirectedfrom=MSDN *
			 * function                                                                                                  *
			 * Determines whether the specified window is minimized (iconic).                                            *
			 * BOOL IsIconic(HWND hWnd);                                                                                 *
			 * hWnd                                                                                                      *
			 * Type: HWND                                                                                                *
			 * A handle to the window to be tested.                                                                      *
			 * Return Value�U                                                                                            *
			 * Type: Type: BOOL                                                                                          *
			 * If the window is iconic, the return value is nonzero.                                                     *
			 * If the window is not iconic, the return value is zero.                                                    *
			 * ��ָ���Ĵ������                                                                                        *
			 * �ɹ������棬ʧ�ܷ��ؼ�                                                                                    *
			 *************************************************************************************************************/
			else if (::IsIconic(hNotepad_plus))
			{
				sw = SW_RESTORE;
			}

			if (sw != 0)
			{
				/***************************************************************************************************************************
				 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-showwindow?redirectedfrom=MSDN             *
				 * function                                                                                                                *
				 * Sets the specified window's show state.                                                                                 *
				 * BOOL ShowWindow(HWND hWnd, int nCmdShow);                                                                               *
				 * hWnd                                                                                                                    *
				 * Type: HWND                                                                                                              *
				 * A handle to the window.                                                                                                 *
				 * nCmdShow                                                                                                                *
				 * Type: int                                                                                                               *
				 * Controls how the window is to be shown. This parameter is ignored the first time an application calls ShowWindow,       *
				 * if the program that launched the application provides a STARTUPINFO(1) structure. Otherwise,                            *
				 * the first time ShowWindow is called, the value should be the value obtained by the WinMain(2) function in its nCmdShow  *
				 * parameter.                                                                                                              *
				 * In subsequent calls, this parameter can be one of the following values.                                                 *
				 * STARTUPINFO(1) : https://docs.microsoft.com/windows/desktop/api/processthreadsapi/ns-processthreadsapi-startupinfoa     *
				 * WinMain(2) : https://docs.microsoft.com/windows/desktop/api/winbase/nf-winbase-winmain                                  *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | Value              | Meaning                                                                                 |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_FORCEMINIMIZE   | Minimizes a window, even if the thread that owns the window is not responding.          |        *
				 * | 11                 | This flag should only be used when minimizing windows from a different thread.          |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_HIDE            | Hides the window and activates another window.                                          |        *
				 * | 0                  |                                                                                         |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_MAXIMIZE        | Maximizes the specified window.                                                         |        *
				 * | 3                  |                                                                                         |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_MINIMIZE        | Minimizes the specified window and activates the next top-level window in the Z order.  |        *
				 * | 6                  |                                                                                         |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_RESTORE         | Activates and displays the window. If the window is minimized or maximized,             |        *
				 * | 9                  | the system restores it to its original size and position. An application should specify |        *
				 * |                    | this flag when restoring a minimized window.                                            |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_SHOW            | Activates the window and displays it in its current size and position.                  |        *
				 * | 5                  |                                                                                         |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_SHOWDEFAULT     | Sets the show state based on the SW_ value specified in the STARTUPINFO(1) structure    |        *
				 * | 10                 | passed to the CreateProcess(3) function by the program that started the application.    |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_SHOWMAXIMIZED   | Activates the window and displays it as a maximized window.                             |        *
				 * | 3                  |                                                                                         |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_SHOWMINIMIZED   | Activates the window and displays it as a minimized window.                             |        *
				 * | 2                  |                                                                                         |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_SHOWMINNOACTIVE | Displays the window as a minimized window. This value is similar to SW_SHOWMINIMIZED,   |        *
				 * | 7                  | except the window is not activated.                                                     |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_SHOWNA          | Displays the window in its current size and position. This value is similar to SW_SHOW, |        *
				 * | 8                  | except that the window is not activated.                                                |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_SHOWNOACTIVATE  | Displays a window in its most recent size and position.                                 |        *
				 * | 4                  | This value is similar to SW_SHOWNORMAL, except that the window is not activated.        |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * | SW_SHOWNORMAL      | Activates and displays a window. If the window is minimized or maximized,               |        *
				 * | 1                  | the system restores it to its original size and position. An application should specify |        *
				 * |                    | this flag when displaying the window for the first time.                                |        *
				 * +--------------------+-----------------------------------------------------------------------------------------+        *
				 * STARTUPINFO(1) : https://docs.microsoft.com/windows/desktop/api/processthreadsapi/ns-processthreadsapi-startupinfoa     *
				 * CreateProcess(3) : https://docs.microsoft.com/windows/desktop/api/processthreadsapi/nf-processthreadsapi-createprocessa *
				 * Return Value�U                                                                                                          *
				 * Type: Type: BOOL                                                                                                        *
				 * If the window was previously visible, the return value is nonzero.                                                      *
				 * If the window was previously hidden, the return value is zero.                                                          *
				 * ����ָ�����ڵ�չʾ״̬���ɹ������棬ʧ�ܷ��ؼ�                                                                          *
				 * Remarks�U                                                                                                               *
				 *                                                                                                                         *
				 * To perform certain special effects when showing or hiding a window, use AnimateWindow(4).                               *
				 * AnimateWindow(4) :                                                                                                      *
				 * The first time an application calls ShowWindow, it should use the WinMain(2) function's nCmdShow                        *
				 * parameter as its nCmdShow parameter. Subsequent calls to ShowWindow must use one of                                     *
				 * the values in the given list, instead of the one specified by the WinMain function's nCmdShow                           *
				 * parameter.                                                                                                              *
				 * WinMain(2) : https://docs.microsoft.com/windows/desktop/api/winbase/nf-winbase-winmain                                  *
				 * As noted in the discussion of the nCmdShow parameter, the nCmdShow value is ignored in the first                        *
				 * call to ShowWindow if the program that launched the application specifies startup information in                        *
				 * the structure. In this case, ShowWindow uses the information specified in the STARTUPINFO(1)                            *
				 * structure to show the window. On subsequent calls, the application must call ShowWindow with                            *
				 * nCmdShow set to SW_SHOWDEFAULT to use the startup information provided by the program that                              *
				 * launched the application. This behavior is designed for the following situations:                                       *
				 * STARTUPINFO(1) : https://docs.microsoft.com/windows/desktop/api/processthreadsapi/ns-processthreadsapi-startupinfoa     *
				 *     * Applications create their main window by calling CreateWindow(4) with the WS_VISIBLE flag set.                    *
				 *     * Applications create their main window by calling CreateWindow(4) with the WS_VISIBLE flag cleared,                *
				 *       and later call ShowWindow with the SW_SHOW flag set to make it visible.                                           *
				 * CreateWindow(4) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-createwindowa                       *
				 * Examples�U                                                                                                              *
				 * For an example, see Creating a Main Window(5).                                                                          *
				 * Creating a Main Window(5) : https://docs.microsoft.com/windows/desktop/winmsg/using-windows                             *
				 ***************************************************************************************************************************/
				::ShowWindow(hNotepad_plus, sw);
			}

			/*****************************************************************************************************************************      *
			 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-setforegroundwindow?redirectedfrom=MSDN              *
			 * function                                                                                                                          *
			 * Brings the thread that created the specified window into the foreground and activates the window.                                  *
			 * Keyboard input is directed to the window, and various visual cues are changed for the user.                                          *
			 * The system assigns a slightly higher priority to the thread that created the foreground window than it does to other threads.     *
			 * ����ָ���Ĵ���Ϊǰ�ô��ڣ����̵����뽫ֱ�ӽ��뵽�ô��ڣ�ͬʱϵͳ�����ø����ȼ���ִ�иù��̣��ɹ������棬ʧ�ܷ��ؼ�                  *
			 * BOOL SetForegroundWindow(HWND hWnd);                                                                                              *
			 * hWnd                                                                                                                              *
			 * Type: HWND                                                                                                                          *
			 * A handle to the window that should be activated and brought to the foreground.                                                      *
			 * Return Value�U                                                                                                                      *
			 * Type: Type: BOOL                                                                                                                  *
			 * If the window was brought to the foreground, the return value is nonzero.                                                          *
			 * If the window was not brought to the foreground, the return value is zero.                                                          *
			 * Remarks�U                                                                                                                          *
			 * The system restricts which processes can set the foreground window. A process can set the                                          *
			 * foreground window only if one of the following conditions is true:                                                                  *
			 *     * The process is the foreground process.                                                                                      *
			 *     * The process was started by the foreground process.                                                                          *
			 *     * The process received the last input event.                                                                                  *
			 *     * There is no foreground process.                                                                                              *
			 *     * The process is being debugged.                                                                                              *
			 *     * The foreground process is not a Modern Application or the Start Screen.                                                      *
			 *     * The foreground is not locked (see LockSetForegroundWindow(1)).                                                              *
			 *     * The foreground lock time-out has expired (see SPI_GETFOREGROUNDLOCKTIMEOUT in SystemParametersInfo(2)).                      *
			 *     * No menus are active.                                                                                                          *
			 * LockSetForegroundWindow(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-locksetforegroundwindow              *
			 * SystemParametersInfo(2) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-systemparametersinfoa                  *
			 * An application cannot force a window to the foreground while the user is working with another window. Instead,                      *
			 * Windows flashes the taskbar button of the window to notify the user.                                                              *
			 * A process that can set the foreground window can enable another process to set the foreground window by calling                      *
			 * the AllowSetForegroundWindow function(3). The process specified by dwProcessId loses the ability to set the foreground              *
			 * window the next time the user generates input, unless the input is directed at that process, or the next time                      *
			 * a process calls AllowSetForegroundWindow, unless that process is specified.                                                          *
			 * AllowSetForegroundWindow function(3) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-allowsetforegroundwindow *
			 * The foreground process can disable calls to SetForegroundWindow by calling the LockSetForegroundWindow(1) function.                  *
			 * LockSetForegroundWindow(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-locksetforegroundwindow              *
			 ******************************************************************************************************************************************/
			::SetForegroundWindow(hNotepad_plus);
			/*
			 * �����������Ҫ�򿪵��ļ�
			 */
			if (params.size() > 0)    //if there are files to open, use the WM_COPYDATA system
			{
				CmdLineParamsDTO dto = CmdLineParamsDTO::FromCmdLineParams(cmdLineParams);

				COPYDATASTRUCT paramData;
				paramData.dwData = COPYDATA_PARAMS;
				paramData.lpData = &dto;
				paramData.cbData = sizeof(dto);

				COPYDATASTRUCT fileNamesData;
				fileNamesData.dwData = COPYDATA_FILENAMES;
				fileNamesData.lpData = (void *)quotFileName.c_str();
				fileNamesData.cbData = long(quotFileName.length() + 1)*(sizeof(TCHAR));

				::SendMessage(hNotepad_plus, WM_COPYDATA, reinterpret_cast<WPARAM>(hInstance), reinterpret_cast<LPARAM>(&paramData));
				::SendMessage(hNotepad_plus, WM_COPYDATA, reinterpret_cast<WPARAM>(hInstance), reinterpret_cast<LPARAM>(&fileNamesData));
			}
			return 0;
		}
	}

	/*
	 * �����ڿ��ƶ���
	 */
	Notepad_plus_Window notepad_plus_plus;

	/*
	 * �����ļ���
	 */
	generic_string updaterDir = pNppParameters->getNppPath();
	updaterDir += TEXT("\\updater\\");

	/*
	 * ������ִ���ļ�
	 */
	generic_string updaterFullPath = updaterDir + TEXT("gup.exe");

	/*
	 * ��������
	 */
	generic_string updaterParams = TEXT("-v");
	updaterParams += VERSION_VALUE;
	/*
	 * ���������ļ�
	 */
	bool isUpExist = nppGui._doesExistUpdater = (::PathFileExists(updaterFullPath.c_str()) == TRUE);

	/*
	 * �ж��Ƿ���Ҫ����
	 */
	if (doUpdateNpp) // check more detail
	{
		Date today(0);

		if (today < nppGui._autoUpdateOpt._nextUpdateDate)
		{
			doUpdateNpp = false;
		}
	}

//    if (doUpdatePluginList)
//    {
		// TODO: detect update frequency
//    }

	// wingup doesn't work with the obsolet security layer (API) under xp since downloadings are secured with SSL on notepad_plus_plus.org
	winVer ver = pNppParameters->getWinVersion();
	bool isGtXP = ver > WV_XP;

	/*
	 * �����ļ���ǩ�����
	 */
	SecurityGard securityGard;
	bool isSignatureOK = securityGard.checkModule(updaterFullPath, nm_gup);

	/*
	 * �����
	 * �׸�����ʵ��
	 * �����ļ�����
	 * ϵͳ�汾����XP
	 * �����ļ�ǩ����ȷ
	 */
	if (TheFirstOne && isUpExist && isGtXP && isSignatureOK)
	{
		if (pNppParameters->isx64())
		{
			updaterParams += TEXT(" -px64");
		}

		/*
		 * ȷ����Ҫ��Ҫ�汾������ִ������
		 */
		if (doUpdateNpp)
		{
			Process updater(updaterFullPath.c_str(), updaterParams.c_str(), updaterDir.c_str());
			updater.run();

			// Update next update date
			if (nppGui._autoUpdateOpt._intervalDays < 0) // Make sure interval days value is positive
			{
				nppGui._autoUpdateOpt._intervalDays = 0 - nppGui._autoUpdateOpt._intervalDays;
			}
			nppGui._autoUpdateOpt._nextUpdateDate = Date(nppGui._autoUpdateOpt._intervalDays);
		}

		// to be removed
		doUpdatePluginList = false;

		/*
		 * ȷ����Ҫ����汾������ִ�в������
		 */
		if (doUpdatePluginList)
		{
			// Update Plugin List
			updaterParams += TEXT(" -upPL");

			// overrided "InfoUrl" in gup.xml
			updaterParams += TEXT(" https://notepad-plus-plus.org/update/pluginListDownloadUrl.php");

			Process updater(updaterFullPath.c_str(), updaterParams.c_str(), updaterDir.c_str());
			updater.run();

			// TODO: Update next update date

		}
	}

	/*
	 * ��Ϣ��
	 */
	MSG msg;
	msg.wParam = 0;
	Win32Exception::installHandler();
	try
	{
		/*
		 * ��ʼ��������
		 */
		notepad_plus_plus.init(hInstance, NULL, quotFileName.c_str(), &cmdLineParams);
		/***********************************************************
		 * ����WM_COPYDATA��Ϣ���������Խ�����������ʵ����������Ϣ *
		 ***********************************************************/
		allowWmCopydataMessages(notepad_plus_plus, pNppParameters, ver);
		/************
		 * ��Ϣѭ�� *
		 ************/
//		bool going = true;
//		while (going)
//		{
//			going = (::GetMessageW(&msg, NULL, 0, 0) != 0);
//			if (going)
//			{
//				// if the message doesn't belong to the notepad_plus_plus's dialog
//				if (!notepad_plus_plus.isDlgsMsg(&msg))
//				{
//					if (::TranslateAccelerator(notepad_plus_plus.getHSelf(), notepad_plus_plus.getAccTable(), &msg) == 0)
//					{
//						::TranslateMessage(&msg);
//						::DispatchMessageW(&msg);
//					}
//				}
//			}
//		}
		/******************************************************************************************************************
		 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-getmessage?redirectedfrom=MSDN    *
		 * function                                                                                                       *
		 * Retrieves a message from the calling thread's message queue. The function dispatches incoming                  *
		 * sent messages until a posted message is available for retrieval.                                               *
		 * Unlike GetMessage, the PeekMessage(1) function does not wait for a message to be posted before                 *
		 * returning.                                                                                                     *
		 * PeekMessage(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-peekmessagea                *
		 * BOOL GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);                               *
		 * lpMsg                                                                                                          *
		 * Type: LPMSG                                                                                                    *
		 * A pointer to an MSG(2) structure that receives message information from the thread's message queue.            *
		 * MSG(2) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-msg                                 *
		 * hWnd                                                                                                           *
		 * Type: HWND                                                                                                     *
		 * A handle to the window whose messages are to be retrieved. The window must belong to the                       *
		 * current thread.                                                                                                *
		 * If hWnd is NULL, GetMessage retrieves messages for any window that belongs to the current                      *
		 * thread, and any messages on the current thread's message queue whose hwnd value is NULL                        *
		 * (see the MSG(3) structure). Therefore if hWnd is NULL, both window messages and thread messages are processed. *
		 * MSG(3) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-msg                                 *
		 * If hWnd is -1, GetMessage retrieves only messages on the current thread's message queue whose                  *
		 * hwnd value is NULL, that is, thread messages as posted by PostMessage(4) (when the hWnd                        *
		 * parameter is NULL) or PostThreadMessage(5).                                                                    *
		 * PostMessage(4) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-postmessagea                *
		 * PostThreadMessage(5) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-postthreadmessagea    *
		 * wMsgFilterMin                                                                                                  *
		 * Type: UINT                                                                                                     *
		 * The integer value of the lowest message value to be retrieved. Use WM_KEYFIRST (0x0100) to                     *
		 * specify the first keyboard message or WM_MOUSEFIRST (0x0200) to specify the first mouse                        *
		 * message.                                                                                                       *
		 * Use WM_INPUT(6) here and in wMsgFilterMax to specify only the WM_INPUT messages.                               *
		 * WM_INPUT(6) : https://docs.microsoft.com/windows/desktop/inputdev/wm-input                                     *
		 * If wMsgFilterMin and wMsgFilterMax are both zero, GetMessage returns all available messages                    *
		 * (that is, no range filtering is performed).                                                                    *
		 * wMsgFilterMax                                                                                                  *
		 * Type: UINT                                                                                                     *
		 * The integer value of the highest message value to be retrieved. Use WM_KEYLAST to specify the                  *
		 * last keyboard message or WM_MOUSELAST to specify the last mouse message.                                       *
		 * Use WM_INPUT(7) here and in wMsgFilterMin to specify only the WM_INPUT messages.                               *
		 * If wMsgFilterMin and wMsgFilterMax are both zero, GetMessage returns all available messages                    *
		 * (that is, no range filtering is performed).                                                                    *
		 * WM_INPUT(7) : https://docs.microsoft.com/windows/desktop/inputdev/wm-input                                     *
		 * Return Value�U                                                                                                 *
		 * Type: Type: BOOL                                                                                               *
		 * If the function retrieves a message other than WM_QUIT(8), the return value is nonzero.                        *
		 * If the function retrieves the WM_QUIT(8) message, the return value is zero.                                    *
		 * WM_QUIT(8) : https://docs.microsoft.com/windows/desktop/winmsg/wm-quit                                         *
		 * If there is an error, the return value is -1. For example, the function fails if hWnd is an invalid            *
		 * window handle or lpMsg is an invalid pointer. To get extended error information, call GetLastError(9).         *
		 * Because the return value can be nonzero, zero, or -1, avoid code like this:                                    *
		 * GetLastError(9) : https://docs.microsoft.com/windows/desktop/api/errhandlingapi/nf-errhandlingapi-getlasterror *
		 * while (GetMessage( lpMsg, hWnd, 0, 0)) ...                                                                     *
		 * The possibility of a -1 return value in the case that hWnd is an invalid parameter (such as referring          *
		 * to a window that has already been destroyed) means that such code can lead to fatal application                *
		 * errors. Instead, use code like this:                                                                           *
		 * BOOL bRet;                                                                                                     *
		 *                                                                                                                *
		 * while( (bRet = GetMessage( &msg, hWnd, 0, 0 )) != 0)                                                           *
		 * {                                                                                                              *
		 *     if (bRet == -1)                                                                                            *
		 *     {                                                                                                          *
		 *         // handle the error and possibly exit                                                                  *
		 *     }                                                                                                          *
		 *     else                                                                                                       *
		 *     {                                                                                                          *
		 *         TranslateMessage(&msg);                                                                                *
		 *         DispatchMessage(&msg);                                                                                 *
		 *     }                                                                                                          *
		 * }                                                                                                              *
		 * ������Ϣ��                                                                                                     *
		 * �����Ϣ����WM_QUIT���ط����ֵ�����򷵻����ֵ                                                                *
		 * �����������-1���磺���ھ����Ч��MSGָ��յȣ��������·���-1                                                *
		 ******************************************************************************************************************/
		while (::GetMessageW(&msg, NULL, 0, 0))
		{
			// if the message doesn't belong to the notepad_plus_plus's dialog
			if (!notepad_plus_plus.isDlgsMsg(&msg))
			{
				/**************************************************************************************************************************
				 * form : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-translateacceleratorw                     *
				 * function                                                                                                               *
				 * Processes accelerator keys for menu commands. The function translates a WM_KEYDOWN(1) or                               *
				 * WM_SYSKEYDOWN(2) message to a WM_COMMAND(3) or WM_SYSCOMMAND(4) message (if there is                                   *
				 * an entry for the key in the specified accelerator table) and then sends the WM_COMMAND or                              *
				 * WM_SYSCOMMAND message directly to the specified window procedure. TranslateAccelerator                                 *
				 * does not return until the window procedure has processed the message.                                                  *
				 * WM_KEYDOWN(1) : https://docs.microsoft.com/windows/desktop/inputdev/wm-keydown                                         *
				 * WM_SYSKEYDOWN(2) : https://docs.microsoft.com/windows/desktop/inputdev/wm-syskeydown                                   *
				 * WM_COMMAND(3) : https://docs.microsoft.com/windows/desktop/menurc/wm-command                                           *
				 * WM_SYSCOMMAND(4) : https://docs.microsoft.com/windows/desktop/menurc/wm-syscommand                                     *
				 * int TranslateAcceleratorW(HWND hWnd, HACCEL hAccTable, LPMSG lpMsg);                                                   *
				 * hWnd                                                                                                                   *
				 * Type: HWND                                                                                                             *
				 * A handle to the window whose messages are to be translated.                                                            *
				 * hAccTable                                                                                                              *
				 * Type: HACCEL                                                                                                           *
				 * A handle to the accelerator table. The accelerator table must have been loaded by a call to the                        *
				 * LoadAccelerators(5) function or created by a call to the CreateAcceleratorTable(6) function.                           *
				 * LoadAccelerators(5) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-loadacceleratorsa              *
				 * CreateAcceleratorTable(6) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-createacceleratortablea  *
				 * lpMsg                                                                                                                  *
				 * Type: LPMSG                                                                                                            *
				 * A pointer to an MSG(7) structure that contains message information retrieved from the calling                          *
				 * thread's message queue using the GetMessage(8) or PeekMessage(9) function.                                             *
				 * MSG(7) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-msg                                         *
				 * GetMessage(8) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getmessage                           *
				 * PeekMessage(9) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-peekmessagea                        *
				 * Return Value�U                                                                                                         *
				 * Type: int                                                                                                              *
				 * If the function succeeds, the return value is nonzero.                                                                 *
				 * If the function fails, the return value is zero. To get extended error information, call GetLastError(10).             *
				 * GetLastError(10) : https://docs.microsoft.com/windows/desktop/api/errhandlingapi/nf-errhandlingapi-getlasterror        *
				 * ����˵���ݼ���Ϣ                                                                                                     *
				 *************************************************************************************************************************/
				if (::TranslateAccelerator(notepad_plus_plus.getHSelf(), notepad_plus_plus.getAccTable(), &msg) == 0)
				{
					/***************************************************************************************************************************
					 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-translatemessage?redirectedfrom=MSDN       *
					 * function                                                                                                                *
					 * Translates virtual-key messages into character messages. The character messages are posted to                           *
					 * the calling thread's message queue, to be read the next time the thread calls the GetMessage(1) or                      *
					 * PeekMessage(2) function.                                                                                                *
					 * GetMessage(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getmessage                            *
					 * PeekMessage(2) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-peekmessagea                         *
					 * ��virtual-key��Ϣת��Ϊ�ַ���Ϣ                                                                                         *
					 * BOOL TranslateMessage(const MSG *lpMsg);                                                                                *
					 * lpMsg                                                                                                                   *
					 * Type: const MSG(3)*                                                                                                     *
					 * A pointer to an MSG structure that contains message information retrieved from the calling                              *
					 * thread's message queue by using the GetMessage(4) or PeekMessage(5) function.                                           *
					 * MSG(3): https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-msg                                           *
					 * GetMessage(4) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getmessage                            *
					 * PeekMessage(5) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-peekmessagea                         *
					 * Return Value�U                                                                                                          *
					 * Type: Type: BOOL                                                                                                        *
					 * If the message is translated (that is, a character message is posted to the thread's message                            *
					 * queue), the return value is nonzero.                                                                                    *
					 * If the message is WM_KEYDOWN(6), WM_KEYUP(7), WM_SYSKEYDOWN(8), or WM_SYSKEYUP(9), the return                           *
					 * value is nonzero, regardless of the translation.                                                                        *
					 * If the message is not translated (that is, a character message is not posted to the thread's                            *
					 * message queue), the return value is zero.                                                                               *
					 * WM_KEYDOWN(6) : https://docs.microsoft.com/windows/desktop/inputdev/wm-keydown                                          *
					 * WM_KEYUP(7) : https://docs.microsoft.com/windows/desktop/inputdev/wm-keyup                                              *
					 * WM_SYSKEYDOWN(8) : https://docs.microsoft.com/windows/desktop/inputdev/wm-syskeydown                                    *
					 * WM_SYSKEYUP(9) : https://docs.microsoft.com/windows/desktop/inputdev/wm-syskeyup                                        *
					 * Remarks�U                                                                                                               *
					 * The TranslateMessage function does not modify the message pointed to by the lpMsg                                       *
					 * parameter.                                                                                                              *
					 * WM_KEYDOWN(10) and WM_KEYUP(11) combinations produce a WM_CHAR(12) or WM_DEADCHAR(13)                                   *
					 * message. WM_SYSKEYDOWN(14) and WM_SYSKEYUP(15) combinations produce a WM_SYSCHAR(16) or                                 *
					 * WM_SYSDEADCHAR(17) message.                                                                                             *
					 * WM_KEYDOWN(10) : https://docs.microsoft.com/windows/desktop/inputdev/wm-keydown                                         *
					 * WM_KEYUP(11) : https://docs.microsoft.com/windows/desktop/inputdev/wm-keyup                                             *
					 * WM_CHAR(12) : https://docs.microsoft.com/windows/desktop/inputdev/wm-char                                               *
					 * WM_DEADCHAR(13) : https://docs.microsoft.com/windows/desktop/inputdev/wm-deadchar                                       *
					 * WM_SYSKEYDOWN(14) : https://docs.microsoft.com/windows/desktop/inputdev/wm-syskeydown                                   *
					 * WM_SYSKEYUP(15) : https://docs.microsoft.com/windows/desktop/inputdev/wm-syskeyup                                       *
					 * WM_SYSCHAR(16) : https://docs.microsoft.com/windows/desktop/menurc/wm-syschar                                           *
					 * WM_SYSDEADCHAR(17) : https://docs.microsoft.com/windows/desktop/inputdev/wm-sysdeadchar                                 *
					 * TranslateMessage produces WM_CHAR(18) messages only for keys that are mapped to ASCII                                   *
					 * characters by the keyboard driver.                                                                                      *
					 * WM_CHAR(18) : https://docs.microsoft.com/windows/desktop/inputdev/wm-char                                               *
					 * If applications process virtual-key messages for some other purpose, they should not call                               *
					 * TranslateMessage. For instance, an application should not call TranslateMessage if the                                  *
					 * TranslateAccelerator(19) function returns a nonzero value. Note that the application is responsible for                 *
					 * retrieving and dispatching input messages to the dialog box. Most applications use the main                             *
					 * message loop for this. However, to permit the user to move to and to select controls by using the                       *
					 * keyboard, the application must call IsDialogMessage(20). For more information, see Dialog Box Keyboard Interface(21).   *
					 * TranslateAccelerator(19) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-translateacceleratora      *
					 * IsDialogMessage(20) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-isdialogmessagea                *
					 * Dialog Box Keyboard Interface(21) : https://docs.microsoft.com/windows/desktop/dlgbox/dlgbox-programming-considerations *
					 * Examples�U                                                                                                              *
					 * For an example, see Creating a Message Loop(22).                                                                        *
					 * Creating a Message Loop(22) : https://docs.microsoft.com/windows/desktop/winmsg/using-messages-and-message-queues       *
					 ***************************************************************************************************************************/
					::TranslateMessage(&msg);
					/***************************************************************************************************************************
					 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-dispatchmessage?redirectedfrom=MSDN        *
					 * function                                                                                                                *
					 * Dispatches a message to a window procedure. It is typically used to dispatch a message retrieved                        *
					 * by the GetMessage(1) function.                                                                                          *
					 * �򴰿ڹ���ת����Ϣ������GetMessage(1)������Ϣ                                                                           *
					 * GetMessage(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getmessage                            *
					 * LRESULT DispatchMessage(const MSG * lpMsg);                                                                             *
					 * lpMsg                                                                                                                   *
					 * Type: const MSG(2)*                                                                                                     *
					 * A pointer to a structure that contains the message.                                                                     *
					 * MSG(2) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-msg                                          *
					 * Return Value�U                                                                                                          *
					 * Type: Type: LRESULT                                                                                                     *
					 * The return value specifies the value returned by the window procedure. Although its meaning                             *
					 * depends on the message being dispatched, the return value generally is ignored.                                         *
					 * Remarks�U                                                                                                               *
					 * The MSG(3) structure must contain valid message values. If the lpmsg parameter points to a                              *
					 * WM_TIMER(4) message and the lParam parameter of the WM_TIMER message is not NULL, lParam                                *
					 * points to a function that is called instead of the window procedure.                                                    *
					 * MSG(3) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-msg                                          *
					 * WM_TIMER(4) : https://docs.microsoft.com/windows/desktop/winmsg/wm-timer                                                *
					 * https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-isdialogmessagea                                      *
					 * Note that the application is responsible for retrieving and dispatching input messages to                               *
					 * the dialog box. Most applications use the main message loop for this. However, to permit the user to                    *
					 * move to and to select controls by using the keyboard, the application must call IsDialogMessage(5).                     *
					 * For more information, see Dialog Box Keyboard Interface(6).                                                             *
					 * IsDialogMessage(5) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-isdialogmessagea                 *
					 * Dialog Box Keyboard Interface(6) : https://docs.microsoft.com/windows/desktop/dlgbox/dlgbox-programming-considerations  *
					 * Examples�U                                                                                                              *
					 * For an example, see Creating a Message Loop(7).                                                                         *
					 * Creating a Message Loop(7) : https://docs.microsoft.com/windows/desktop/winmsg/using-messages-and-message-queues        *
					 ***************************************************************************************************************************/
					::DispatchMessageW(&msg);
				}
			}
		}
//		for (;;)
//		{
//			switch (::GetMessageW(&msg, NULL, 0, 0))
//			{
//				case -1:
//				case 0:
//				{
//					break;
//				}
//				default:
//				{
//					// if the message doesn't belong to the notepad_plus_plus's dialog
//					if (!notepad_plus_plus.isDlgsMsg(&msg))
//					{
//						if (::TranslateAccelerator(notepad_plus_plus.getHSelf(), notepad_plus_plus.getAccTable(), &msg) == 0)
//						{
//							::TranslateMessage(&msg);
//							::DispatchMessageW(&msg);
//						}
//					}
//					continue;
//				}
//			}
//			break;
//		}
	}
	catch (int i)
	{
		TCHAR str[50] = TEXT("God Damned Exception : ");
		TCHAR code[10];
		wsprintf(code, TEXT("%d"), i);
		wcscat_s(str, code);
		::MessageBox(Notepad_plus_Window::gNppHWND, str, TEXT("Int Exception"), MB_OK);
		doException(notepad_plus_plus);
	}
	catch (std::runtime_error & ex)
	{
		::MessageBoxA(Notepad_plus_Window::gNppHWND, ex.what(), "Runtime Exception", MB_OK);
		doException(notepad_plus_plus);
	}
	catch (const Win32Exception & ex)
	{
		//TODO: sane number
		TCHAR message[1024];
		/*****************************************************************************
		 * Win32Exception                                                            *
		 * An exception occured. Notepad++ cannot recover and must be shut down.\r\n *
		 * The exception details are as follows:\r\n                                 *
		 * Code:\t0x%08X\r\n                                                         *
		 * Type:\t%S\r\n                                                             *
		 * Exception address: 0x%p                                                   *
		 *****************************************************************************/
		wsprintf(message, WIN32EXCEPTION_MESSAGE_FORMATTER, ex.code(), ex.what(), ex.where());
		::MessageBox(Notepad_plus_Window::gNppHWND, message, TEXT("Win32Exception"), MB_OK | MB_ICONERROR);
		mdump.writeDump(ex.info());
		doException(notepad_plus_plus);
	}
	catch (std::exception & ex)
	{
		::MessageBoxA(Notepad_plus_Window::gNppHWND, ex.what(), "General Exception", MB_OK);
		doException(notepad_plus_plus);
	}
	catch (...) // this shouldnt ever have to happen
	{
		::MessageBoxA(Notepad_plus_Window::gNppHWND, "An exception that we did not yet found its name is just caught", "Unknown Exception", MB_OK);
		doException(notepad_plus_plus);
	}
	WindowsApi::ReleaseMutex(nppMutex);
	return static_cast<int>(msg.wParam);
}
