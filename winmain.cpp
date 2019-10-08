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
 * 因为有的库不能被正常链接，所以使用动态库进行链接 *
 ****************************************************/
#include "Real_Kesic_Lee_Dynamic_Link_Library_Load_And_Proc_Address.h"

#define WindowsApi

// typedef std::vector<generic_string> ParamVector;



namespace
{

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
                    typedef BOOL (WINAPI *MESSAGEFILTERFUNC)(UINT message,DWORD dwFlag);

                    MESSAGEFILTERFUNC func = (MESSAGEFILTERFUNC)::GetProcAddress( hDll, "ChangeWindowMessageFilter" );

                    if (func)
                    {
                        func(WM_COPYDATA, MSGFLT_ADD);
                    }
                }
                else
                {
                    typedef BOOL (WINAPI *MESSAGEFILTERFUNCEX)(HWND hWnd,UINT message,DWORD action,VOID* pChangeFilterStruct);

                    MESSAGEFILTERFUNCEX func = (MESSAGEFILTERFUNCEX)::GetProcAddress( hDll, "ChangeWindowMessageFilterEx" );

                    if (func)
                    {
                        func(notepad_plus_plus.getHSelf(), WM_COPYDATA, MSGFLT_ALLOW, NULL);
                    }
                }
            }
        }
    }

    /*************************************************************
     * commandLine should contain path to n++ executable running *
	 * 把控制台或Shell传递的参数进行拆解，具体可参见argc和argv   *
     *************************************************************/
    std::vector<generic_string> parseCommandLine(const TCHAR* commandLine)
    {
		/*
		 * 存放参数的容器
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
        if (WindowsApi::_tcsnicmp(TEXT("/p"), pCmdLine, 2) == 0 )
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
            if ( *PathFindExtension(str.c_str()) == '\0' )
            {
                // If joined path has no extension, Notepad adds a .txt extension
                str.append(TEXT(".txt"));
            }
            params.push_back(std::move(str));
        }
        return params;
    }

	/********************************************************************************************************
	 * 判断参数列表中是否存在指定的参数，如果存在，具需要从列表中移除该参数时，从列表中移除该参数，返回true *
	 * @param token2Find 指定的参数                                                                         *
	 * @param params     参数列表                                                                           *
	 * @param eraseArg   是否移除指定的参数(true : 表示需要从列表中移除指定的参数，参数重复只移除第一个)    *                          *
	 ********************************************************************************************************/
    bool isInList(const TCHAR * token2Find, std::vector<generic_string> & params, bool eraseArg = true)
    {
        for (auto it = params.begin(); it != params.end(); ++it)
        {
            if (WindowsApi::lstrcmp(token2Find, it->c_str()) == 0)
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

	/*
	 * 查找指定参数
	 * 如果找到指定的参数，那么获取指定参数的值，同时从参数列表中移除对应的参数
	 */
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

	/*
	 * 查找参数中以指定参数str开头的字符串，如果找到，从参数列表中移除指定的字符串，然后返回true
	 */
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
	 * 查找参数中的语言，如果没有找到返回L_EXTERNAL
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
	 * 国际化语言配置文件的文件名
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
	 * 从参数列表中获取数值型的参数，如果参数列表中没有值，返回-1
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
        return generic_atoi(numStr.c_str());
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
	 * 从参数中获取打字速度
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
	 * 解析用的参数名
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
	 * 异常处理
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
            ::MessageBox(Notepad_plus_Window::gNppHWND, displayText.c_str(), TEXT("Recovery success"), MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            ::MessageBox(Notepad_plus_Window::gNppHWND, TEXT("Unfortunatly, Notepad++ was not able to save your work. We are sorry for any lost data."), TEXT("Recovery failure"), MB_OK | MB_ICONERROR);
        }
    }

    PWSTR advanceCmdLine(PWSTR pCmdLine, const generic_string & string)
    {
        const size_t len = string.length();
        while (true)
        {
            PWSTR ignoredString = WindowsApi::wcsstr(pCmdLine, string.c_str());
            if (ignoredString == nullptr)
            {
                // Should never happen - tokenized parameters contain string somewhere, so it HAS to match
                // This is there just in case
                break;
            }
    
            // Match the substring only if it matched an entire substring        
            if ( (ignoredString == pCmdLine || WindowsApi::iswspace(*(ignoredString-1)) ) && // Check start
                 (WindowsApi::iswspace(*(ignoredString+len)) || *(ignoredString+len) == '\0') )
            {
                ignoredString += len;

                // Advance to the first non-whitespace and not quotation mark character
                while (WindowsApi::iswspace( *ignoredString ) || *ignoredString == L'"' )
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
    std::vector<generic_string> params                 = parseCommandLine(pCmdLine);
    PWSTR                       pCmdLineWithoutIgnores = stripIgnoredParams(params, pCmdLine);

    //for debugging purposes.
    MiniDumper mdump;
    /******************************
     * 初始化动态链接库的函数指针 *
     ******************************/
    const RealKesicLeeDynamicLinkLibraryLoad & obj = getRealKesicLeeDynamicLinkLibraryLoad();
    initialRealKesicLeeDynamicLinkLibraryLoad(obj);

	/*
	 * 该实例是否是单独的实例
	 * true  : 是单独的实例
	 */
    bool TheFirstOne = true;
	WindowsApi::SetLastError(NO_ERROR);
	WindowsApi::CreateMutex(NULL, false, TEXT("nppInstance"));
    if (WindowsApi::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        TheFirstOne = false;
    }

    // Convert commandline to notepad-compatible format, if applicable
    if ( isInList(FLAG_NOTEPAD_COMPATIBILITY, params) )
    {
        params = convertParamsToNotepadStyle(pCmdLineWithoutIgnores);
    }

    bool isParamePresent;
    bool showHelp             = isInList(FLAG_HELP, params);
    bool isMultiInst          = isInList(FLAG_MULTI_INSTANCE, params);
    bool doFunctionListExport = isInList(FLAG_FUNCLSTEXPORT, params);
    bool doPrintAndQuit       = isInList(FLAG_PRINTANDQUIT, params);

    CmdLineParams cmdLineParams;
    cmdLineParams._isNoTab                = isInList(FLAG_NOTABBAR, params);
    cmdLineParams._isNoPlugin             = isInList(FLAG_NO_PLUGIN, params);
    cmdLineParams._isReadOnly             = isInList(FLAG_READONLY, params);
    cmdLineParams._isNoSession            = isInList(FLAG_NOSESSION, params);
    cmdLineParams._isPreLaunch            = isInList(FLAG_SYSTRAY, params);
    cmdLineParams._alwaysOnTop            = isInList(FLAG_ALWAYS_ON_TOP, params);
    cmdLineParams._showLoadingTime        = isInList(FLAG_LOADINGTIME, params);
    cmdLineParams._isSessionFile          = isInList(FLAG_OPENSESSIONFILE, params);
    cmdLineParams._isRecursive            = isInList(FLAG_RECURSIVE, params);
    cmdLineParams._openFoldersAsWorkspace = isInList(FLAG_OPEN_FOLDERS_AS_WORKSPACE, params);
    cmdLineParams._langType               = getLangTypeFromParam(params);
    cmdLineParams._localizationPath       = getLocalizationPathFromParam(params);
    cmdLineParams._easterEggName          = getEasterEggNameFromParam(params, cmdLineParams._quoteType);
    cmdLineParams._ghostTypingSpeed       = getGhostTypingSpeedFromParam(params);

    // getNumberFromParam should be run at the end, to not consuming the other params
    cmdLineParams._line2go   = getNumberFromParam('n', params, isParamePresent);
    cmdLineParams._column2go = getNumberFromParam('c', params, isParamePresent);
    cmdLineParams._pos2go    = getNumberFromParam('p', params, isParamePresent);
    cmdLineParams._point.x   = getNumberFromParam('x', params, cmdLineParams._isPointXValid);
    cmdLineParams._point.y   = getNumberFromParam('y', params, cmdLineParams._isPointYValid);

    /****************
     * 显示帮助信息 *
     ****************/
    if (showHelp)
    {
        ::MessageBox(NULL, COMMAND_ARG_HELP, TEXT("Notepad++ Command Argument Help"), MB_OK);
    }

	/*******************************
	 * Notepad ++ 启动时需要的参数 *
	 *******************************/
    NppParameters * pNppParameters = NppParameters::getInstance();
    NppGUI & nppGui = const_cast<NppGUI &>(pNppParameters->getNppGUI());
    bool doUpdateNpp = nppGui._autoUpdateOpt._doAutoUpdate;
    bool doUpdatePluginList = nppGui._autoUpdateOpt._doAutoUpdate;

    // export functionlist feature will serialize fuctionlist on the disk, then exit Notepad++. So it's important to not launch into existing instance, and keep it silent.
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
	 * Notepad ++ 加载启动时需要的参数 *
	 ***********************************/
	pNppParameters->load();

    pNppParameters->setFunctionListExportBoolean(doFunctionListExport);
    pNppParameters->setPrintAndExitBoolean(doPrintAndQuit);

    // override the settings if notepad style is present
    if (pNppParameters->asNotepadStyle())
    {
        isMultiInst = true;
        cmdLineParams._isNoTab = true;
        cmdLineParams._isNoSession = true;
    }

    // override the settings if multiInst is choosen by user in the preference dialog
    const NppGUI & nppGUI = pNppParameters->getNppGUI();
    if (nppGUI._multiInstSetting == multiInst)
    {
        isMultiInst = true;
        // Only the first launch remembers the session
        if (!TheFirstOne)
        {
            cmdLineParams._isNoSession = true;
        }
    }

    generic_string quotFileName = TEXT("");
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

    if ((!isMultiInst) && (!TheFirstOne))
    {
        HWND hNotepad_plus = ::FindWindow(Notepad_plus_Window::getClassName(), NULL);
        for (int i = 0 ;!hNotepad_plus && i < 5 ; ++i)
        {
            Sleep(100);
            hNotepad_plus = ::FindWindow(Notepad_plus_Window::getClassName(), NULL);
        }

        if (hNotepad_plus)
        {
            // First of all, destroy static object NppParameters
            pNppParameters->destroyInstance();

            int sw = 0;

            if (::IsZoomed(hNotepad_plus))
            {
                sw = SW_MAXIMIZE;
            }
            else if (::IsIconic(hNotepad_plus))
            {
                sw = SW_RESTORE;
            }

            if (sw != 0)
            {
                ::ShowWindow(hNotepad_plus, sw);
            }

            ::SetForegroundWindow(hNotepad_plus);

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

    Notepad_plus_Window notepad_plus_plus;

    generic_string updaterDir = pNppParameters->getNppPath();
    updaterDir += TEXT("\\updater\\");

    generic_string updaterFullPath = updaterDir + TEXT("gup.exe");

    generic_string updaterParams = TEXT("-v");
    updaterParams += VERSION_VALUE;

    bool isUpExist = nppGui._doesExistUpdater = (::PathFileExists(updaterFullPath.c_str()) == TRUE);

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

    SecurityGard securityGard;
    bool isSignatureOK = securityGard.checkModule(updaterFullPath, nm_gup);

    if (TheFirstOne && isUpExist && isGtXP && isSignatureOK)
    {
        if (pNppParameters->isx64())
        {
            updaterParams += TEXT(" -px64");
        }

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

    MSG msg;
    msg.wParam = 0;
    Win32Exception::installHandler();
    try
    {
        notepad_plus_plus.init(hInstance, NULL, quotFileName.c_str(), &cmdLineParams);
        allowWmCopydataMessages(notepad_plus_plus, pNppParameters, ver);
        bool going = true;
		/************
		 * 消息循环 *
		 ************/
        while (going)
        {
            going = (::GetMessageW(&msg, NULL, 0, 0) != 0);
            if (going)
            {
                // if the message doesn't belong to the notepad_plus_plus's dialog
                if (!notepad_plus_plus.isDlgsMsg(&msg))
                {
                    if (::TranslateAccelerator(notepad_plus_plus.getHSelf(), notepad_plus_plus.getAccTable(), &msg) == 0)
                    {
                        ::TranslateMessage(&msg);
                        ::DispatchMessageW(&msg);
                    }
                }
            }
        }
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
        TCHAR message[1024];    //TODO: sane number
        wsprintf(message, TEXT("An exception occured. Notepad++ cannot recover and must be shut down.\r\nThe exception details are as follows:\r\n")
            TEXT("Code:\t0x%08X\r\nType:\t%S\r\nException address: 0x%p"), ex.code(), ex.what(), ex.where());
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

    return static_cast<int>(msg.wParam);
}
