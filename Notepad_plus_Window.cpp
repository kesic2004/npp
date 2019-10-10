// This file is part of Notepad++ project
// Copyright (C)2003 Don HO <don.h@free.fr>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// Note that the GPL places important restrictions on "derived works", yet
// it does not provide a detailed definition of that term.  To avoid
// misunderstandings, we consider an application to constitute a
// "derivative work" for the purpose of this license if it does any of the
// following:
// 1. Integrates source code from Notepad++.
// 2. Integrates/includes/aggregates Notepad++ into a proprietary executable
//    installer, such as those produced by InstallShield.
// 3. Links to a library or executes a program that does any of the above.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include <time.h>
#include <shlwapi.h>
#include "Notepad_plus_Window.h"

/*********************************************
 * 主窗口标题和主窗口类的名称                *
 * TEXT("已经被光杆儿司令员篡改了(哈哈哈)"); *
 *********************************************/
const TCHAR Notepad_plus_Window::_className[32] = TEXT("Notepad++");
const TCHAR Notepad_plus_Window::_titleName[32] = TEXT("Notepad++");
HWND Notepad_plus_Window::gNppHWND = NULL;


/*
 *匿名namespace
 */
namespace // anonymous
{

	struct PaintLocker final
	{
		explicit PaintLocker(HWND handle) : handle(handle)
		{
			// disallow drawing on the window
			/*********************************************************************************************************************
			 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-lockwindowupdate?redirectedfrom=MSDN *
			 * function                                                                                                          *
			 * The LockWindowUpdate function disables or enables drawing in the specified window. Only one                       *
			 * window can be locked at a time.                                                                                   *
			 * 禁止或允许绘制指定的窗口，如果参数为空则绘制指定的窗口将被允许                                                    *
			 * BOOL LockWindowUpdate(HWND hWndLock);                                                                             *
			 * hWndLock                                                                                                          *
			 * The window in which drawing will be disabled. If this parameter is NULL, drawing in the locked                    *
			 * window is enabled.                                                                                                *
			 * Return Value                                                                                                    *
			 * If the function succeeds, the return value is nonzero.                                                            *
			 * If the function fails, the return value is zero, indicating that an error occurred or another window              *
			 * was already locked.                                                                                               *
			 * Remarks                                                                                                         *
			 * The purpose of the LockWindowUpdate function is to permit drag/drop feedback to be drawn                          *
			 * over a window without interference from the window itself. The intent is that the window is locked                *
			 * when feedback is drawn and unlocked when feedback is complete. LockWindowUpdate is not                            *
			 * intended for general-purpose suppression of window redraw. Use the WM_SETREDRAW(1) message                        *
			 * to disable redrawing of a particular window.                                                                      *
			 * WM_SETREDRAW(1) : https://docs.microsoft.com/windows/desktop/gdi/wm-setredraw                                     *
			 * If an application with a locked window (or any locked child windows) calls the GetDC(2), GetDCEx(3), or           *
			 * BeginPaint(4) function, the called function returns a device context with a visible region that is                *
			 * empty. This will occur until the application unlocks the window by calling LockWindowUpdate,                      *
			 * specifying a value of NULL for hWndLock.                                                                          *
			 * GetDC(2) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getdc                                *
			 * GetDCEx(3) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getdcex                            *
			 * BeginPaint(4) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-beginpaint                      *
			 * If an application attempts to draw within a locked window, the system records the extent of the                   *
			 * attempted operation in a bounding rectangle. When the window is unlocked, the system                              *
			 * invalidates the area within this bounding rectangle, forcing an eventual WM_PAINT(5) message to be                *
			 * sent to the previously locked window and its child windows. If no drawing has occurred while the                  *
			 * window updates were locked, no area is invalidated.                                                               *
			 * WM_PAINT(5) : https://docs.microsoft.com/windows/desktop/gdi/wm-paint                                             *
			 * LockWindowUpdate does not make the specified window invisible and does not clear                                  *
			 * the WS_VISIBLE style bit.                                                                                         *
			 * A locked window cannot be moved.                                                                                  *
			 *********************************************************************************************************************/
			::LockWindowUpdate(handle);
		}

		~PaintLocker()
		{
			// re-allow drawing for the window
			::LockWindowUpdate(NULL);

			// force re-draw
			::InvalidateRect(handle, nullptr, TRUE);
			::RedrawWindow(handle, nullptr, NULL, RDW_ERASE | RDW_ALLCHILDREN | RDW_FRAME | RDW_INVALIDATE);
		}

		HWND handle;
	};

} // anonymous namespace




/*
 * 主窗口初始化
 */
void Notepad_plus_Window::init(HINSTANCE hInst, HWND parent, const TCHAR * cmdLine, CmdLineParams * cmdLineParams)
{
	/*
	 * 记录启动过程所用的时间
	 */
	time_t timestampBegin = 0;
	if (cmdLineParams->_showLoadingTime)
	{
		timestampBegin = time(NULL);
	}

	/****************************************
	 * 设置当前实例句柄和父窗口(实际是空值) *
	 ****************************************/
	Window::init(hInst, parent);

	/*
	 * 主窗口类
	 */
	WNDCLASS nppClass;

	nppClass.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;

	/****************************************
	 * 主窗口的系统回调，通过窗口回调函数， *
	 * 实现主窗口中的所有子窗口的初始化，   *
	 * 当前窗口句柄和当前对象地址的绑定     *
	 ****************************************/
	nppClass.lpfnWndProc   = Notepad_plus_Window::Notepad_plus_Proc;
	nppClass.cbClsExtra    = 0;
	nppClass.cbWndExtra    = 0;
	nppClass.hInstance     = _hInst;
	/******************************************************************************************************************
	 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-loadiconw                         *
	 * function                                                                                                       *
	 * Loads the specified icon resource from the executable (.exe) file associated with an application               *
	 * instance.                                                                                                      *
	 * 从指定的可执行文件加载图标资源                                                                                 *
	 * Note  This function has been superseded by the LoadImage function.                                             *
	 * 该函数已经被函数LoadImage所取代                                                                                *
	 * HICON LoadIconW(HINSTANCE hInstance, LPCWSTR lpIconName);                                                      *
	 * hInstance                                                                                                      *
	 * Type: HINSTANCE                                                                                                *
	 * A handle to an instance of the module whose executable file contains the icon to be loaded. This               *
	 * parameter must be NULL when a standard icon is being loaded.                                                   *
	 * lpIconName                                                                                                     *
	 * Type: LPCTSTR                                                                                                  *
	 * The name of the icon resource to be loaded. Alternatively, this parameter can contain the resource             *
	 * identifier in the low-order word and zero in the high-order word. Use the MAKEINTRESOURCE(1)                   *
	 * macro to create this value.                                                                                    *
	 * MAKEINTRESOURCE(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-makeintresourcea        *
	 * To use one of the predefined icons, set the hInstance parameter to NULL and the lpIconName                     *
	 * parameter to one of the following values.                                                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | Value                  | Meaning                                      |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_APPLICATION        | Default application icon.                    |                                      *
	 * | MAKEINTRESOURCE(32512) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_ASTERISK           | Asterisk icon. Same as IDI_INFORMATION.      |                                      *
	 * | MAKEINTRESOURCE(32516) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_ERROR              | Hand-shaped icon.                            |                                      *
	 * | MAKEINTRESOURCE(32513) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_EXCLAMATION        | Exclamation point icon. Same as IDI_WARNING. |                                      *
	 * | MAKEINTRESOURCE(32515) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_HAND               | Hand-shaped icon. Same as IDI_ERROR.         |                                      *
	 * | MAKEINTRESOURCE(32513) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_INFORMATION        | Asterisk icon.                               |                                      *
	 * | MAKEINTRESOURCE(32516) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_QUESTION           | Question mark icon.                          |                                      *
	 * | MAKEINTRESOURCE(32514) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_SHIELD             | Security Shield icon.                        |                                      *
	 * | MAKEINTRESOURCE(32518) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_WARNING            | Exclamation point icon.                      |                                      *
	 * | MAKEINTRESOURCE(32515) |                                              |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * | IDI_WINLOGO            | Default application icon.                    |                                      *
	 * | MAKEINTRESOURCE(32517) | Windows 2000:  Windows logo icon.            |                                      *
	 * +------------------------+----------------------------------------------+                                      *
	 * Return Value                                                                                                 *
	 * Type: HICON                                                                                                    *
	 * If the function succeeds, the return value is a handle to the newly loaded icon.                               *
	 * If the function fails, the return value is NULL. To get extended error information, call GetLastError(2).      *
	 * GetLastError(2) : https://docs.microsoft.com/windows/desktop/api/errhandlingapi/nf-errhandlingapi-getlasterror *
	 * Remarks                                                                                                      *
	 * LoadIcon loads the icon resource only if it has not been loaded; otherwise, it retrieves a handle to           *
	 * the existing resource. The function searches the icon resource for the icon most appropriate for the           *
	 * current display. The icon resource can be a color or monochrome bitmap.                                        *
	 *                                                                                                                *
	 * LoadIcon can only load an icon whose size conforms to the SM_CXICON and SM_CYICON system                       *
	 * metric values. Use the LoadImage(3) function to load icons of other sizes.                                     *
	 * LoadImage(3) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-loadimagea                    *
	 ******************************************************************************************************************/
	nppClass.hIcon         = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_M30ICON));
	/************************************************************************************************************
	 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-loadcursorw                 *
	 * function                                                                                                 *
	 * Loads the specified cursor resource from the executable (.EXE) file associated with an application       *
	 * instance.                                                                                                *
	 * Note  This function has been superseded by the LoadImage function.                                       *
	 * 从指定的可执行文件中获取指定的光标，该函数已经被LoadImage所取代                                          * 
	 * HCURSOR LoadCursorW(HINSTANCE hInstance, LPCWSTR lpCursorName);                                          *
	 * hInstance                                                                                                *
	 * Type: HINSTANCE                                                                                          *
	 * A handle to an instance of the module whose executable file contains the cursor to be loaded.            *
	 * lpCursorName                                                                                             *
	 * Type: LPCTSTR                                                                                            *
	 * The name of the cursor resource to be loaded. Alternatively, this parameter can consist of the           *
	 * resource identifier in the low-order word and zero in the high-order word. The MAKEINTRESOURCE           *
	 * macro can also be used to create this value. To use one of the predefined cursors, the                   *
	 * application must set the hInstance parameter to NULL and the lpCursorName parameter to one the           *
	 * following values.                                                                                        *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | Value           | Value Detail           | Meaning                                                  |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_APPSTARTING | MAKEINTRESOURCE(32650) | Standard arrow and small hourglass                       |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_ARROW       | MAKEINTRESOURCE(32512) | Standard arrow                                           |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_CROSS       | MAKEINTRESOURCE(32515) | Crosshair                                                |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_HAND        | MAKEINTRESOURCE(32649) | Hand                                                     |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_HELP        | MAKEINTRESOURCE(32651) | Arrow and question mark                                  |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_IBEAM       | MAKEINTRESOURCE(32513) | I-beam                                                   |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_ICON        | MAKEINTRESOURCE(32641) | Obsolete for applications marked version 4.0 or later.   |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_NO          | MAKEINTRESOURCE(32648) | Slashed circle                                           |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_SIZE        | MAKEINTRESOURCE(32640) | Obsolete for applications marked version 4.0 or later.   |  *
	 * |                 |                        | Use IDC_SIZEALL.                                         |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_SIZEALL     | MAKEINTRESOURCE(32646) | Four-pointed arrow pointing north, south, east, and west |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_SIZENESW    | MAKEINTRESOURCE(32643) | Double-pointed arrow pointing northeast and southwest    |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_SIZENS      | MAKEINTRESOURCE(32645) | Double-pointed arrow pointing north and south            |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_SIZENWSE    | MAKEINTRESOURCE(32642) | Double-pointed arrow pointing northwest and southeast    |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_SIZEWE      | MAKEINTRESOURCE(32644) | Double-pointed arrow pointing west and east              |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_UPARROW     | MAKEINTRESOURCE(32516) | Vertical arrow                                           |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * | IDC_WAIT        | MAKEINTRESOURCE(32514) | Hourglass                                                |  *
	 * +-----------------+------------------------+----------------------------------------------------------+  *
	 * Return Value                                                                                           *
	 * Type: HCURSOR                                                                                            *
	 * If the function succeeds, the return value is the handle to the newly loaded cursor.                     *
	 * If the function fails, the return value is NULL. To get extended error information, call GetLastError.   *
	 * Remarks                                                                                                *
	 * The LoadCursor function loads the cursor resource only if it has not been loaded; otherwise, it          *
	 * retrieves the handle to the existing resource. This function returns a valid cursor handle only if the   *
	 * lpCursorName parameter is a pointer to a cursor resource. If lpCursorName is a pointer to any type of    *
	 * resource other than a cursor (such as an icon), the return value is not NULL, even though it is not a    *
	 * valid cursor handle.                                                                                     *
	 * The LoadCursor function searches the cursor resource most appropriate for the cursor for the current     *
	 * display device. The cursor resource can be a color or monochrome bitmap.                                 *
	 * DPI Virtualization                                                                                     *
	 * This API does not participate in DPI virtualization. The output returned is not affected by the DPI of   *
	 * the calling thread.                                                                                      *
	 * Examples                                                                                               *
	 * For an example, see Creating a Cursor(1).                                                                *
	 * Creating a Cursor(1) : https://docs.microsoft.com/windows/desktop/menurc/using-cursors                   *
	 ************************************************************************************************************/
	nppClass.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
	/***************************************************************************************************************************
	 * from https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-createsolidbrush?redirectedfrom=MSDN           *
	 * function                                                                                                                *
	 * The CreateSolidBrush function creates a logical brush that has the specified solid color.                               *
	 * 新建一个指定颜色的画刷，如果执行成功返回画刷的唯一标识，失败返回空                                                      *
	 * HBRUSH CreateSolidBrush(COLORREF color);                                                                                *
	 * color                                                                                                                   *
	 * The color of the brush. To create a COLORREF(1) color value, use the RGB(2) macro.                                      *
	 * COLORREF(1) : https://docs.microsoft.com/windows/desktop/gdi/colorref                                                   *
	 * RGB(2) ： https://docs.microsoft.com/windows/desktop/api/wingdi/nf-wingdi-rgb                                           *
	 * Return Value                                                                                                          *
	 * If the function succeeds, the return value identifies a logical brush.                                                  *
	 * If the function fails, the return value is NULL.                                                                        *
	 * Remarks                                                                                                               *
	 * When you no longer need the HBRUSH object, call the DeleteObject(3) function to delete it.                              *
	 * DeleteObject(3) : https://docs.microsoft.com/windows/desktop/api/wingdi/nf-wingdi-deleteobject                          *
	 * A solid brush is a bitmap that the system uses to paint the interiors of filled shapes.                                 *
	 *                                                                                                                         *
	 * After an application creates a brush by calling CreateSolidBrush, it can select that brush into any                     *
	 * device context by calling the SelectObject(4) function.                                                                 *
	 * SelectObject(4) : https://docs.microsoft.com/windows/desktop/api/wingdi/nf-wingdi-selectobject                          *
	 * To paint with a system color brush, an application should use GetSysColorBrush (nIndex) instead                         *
	 * of CreateSolidBrush(GetSysColor(nIndex)), because GetSysColorBrush(5) returns a cached brush instead                    *
	 * of allocating a new one.                                                                                                *
	 * GetSysColorBrush(5) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getsyscolorbrush                *
	 * ICM: No color management is done at brush creation. However, color management is performed when the                     *
	 * brush is selected into an ICM-enabled device context.                                                                   *
	 * Examples                                                                                                              *
	 * For an example, see Creating Colored Pens and Brushes(6).                                                               *
	 * Creating Colored Pens and Brushes(6) : https://docs.microsoft.com/windows/desktop/gdi/creating-colored-pens-and-brushes *
	 ***************************************************************************************************************************/
	nppClass.hbrBackground = ::GetSysColorBrush(COLOR_MENU);/*::CreateSolidBrush(::GetSysColor(COLOR_MENU));*/
	/***************************************************************************************************
	 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-makeintresourcew   *
	 * macro                                                                                           *
	 * Converts an integer value to a resource type compatible with the resource-management functions. *
	 * This macro is used in place of a string containing the name of the resource.                    *
	 * void MAKEINTRESOURCEW(i);                                                                       *
	 * i                                                                                               *
	 * The integer value to be converted.                                                              *
	 * Return Value                                                                                  *
	 * None                                                                                            *
	 * Remarks                                                                                       *
	 * The return value should be passed only to functions which explicitly indicate that they accept  *
	 * MAKEINTRESOURCE as a parameter. For example, the resource management functions allow            *
	 * the return value of MAKEINTRESOURCE to be passed as the lpType or lpName parameters.            *
	 * 把一个整数的资源转换为兼容的资源的                                                              *
	 ***************************************************************************************************/
	nppClass.lpszMenuName  = MAKEINTRESOURCE(IDR_M30_MENU);
	nppClass.lpszClassName = _className;

	this->_isPrelaunch = cmdLineParams->_isPreLaunch;

	/*************************************************************************************************************************************
	 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-registerclassw                                       *
	 * function                                                                                                                          *
	 * Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function.                              *
	 *     Note                                                                                                                          *
	 *        The RegisterClass function has been superseded by the RegisterClassEx function.                                            *
	 *        You can still use RegisterClass, however, if you do not need to set the class small icon.                                  *
	 * ATOM RegisterClassW(const WNDCLASSW *lpWndClass);                                                                                 *
	 * lpWndClass                                                                                                                        *
	 * Type: const WNDCLASS*                                                                                                             *
	 * A pointer to a WNDCLASS structure. You must fill the structure with the appropriate class attributes                              *
	 * before passing it to the function.                                                                                                *
	 * Return Value                                                                                                                    *
	 * Type: Type: ATOM                                                                                                                  *
	 * If the function succeeds, the return value is a class atom that uniquely identifies the class being                               *
	 * registered. This atom can only be used by the CreateWindow, CreateWindowEx, GetClassInfo,                                         *
	 * GetClassInfoEx, FindWindow, FindWindowEx, and UnregisterClass functions and the                                                   *
	 * IActiveIMMap::FilterClientWindows method.                                                                                         *
	 * If the function fails, the return value is zero. To get extended error information, call GetLastError.                            *
	 * Remarks                                                                                                                         *
	 * If you register the window class by using RegisterClassA, the application tells the system that the                               *
	 * windows of the created class expect messages with text or character parameters to use the ANSI                                    *
	 * character set; if you register it by using RegisterClassW, the application requests that the system                               *
	 * pass text parameters of messages as Unicode. The IsWindowUnicode(1) function enables applications to                              *
	 * query the nature of each window. For more information on ANSI and Unicode functions, see                                          *
	 * Conventions for Function Prototypes(2).                                                                                           *
	 * IsWindowUnicode(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-iswindowunicode                            *
	 * Conventions for Function Prototypes(2) : https://docs.microsoft.com/windows/desktop/Intl/conventions-for-function-prototypes      *
	 * All window classes that an application registers are unregistered when it terminates.                                             *
	 * No window classes registered by a DLL are unregistered when the DLL is unloaded. A DLL must                                       *
	 * explicitly unregister its classes when it is unloaded.                                                                            *
	 * Examples                                                                                                                        *
	 * For an example, see Associating a Window Procedure with a Window Class.                                                           *
	 * Associating a Window Procedure with a Window Class(3) : https://docs.microsoft.com/windows/desktop/winmsg/using-window-procedures *
	 * 注册窗口，返回指向窗口类的唯一标识                                                                                                *
	 *************************************************************************************************************************************/
	if (!::RegisterClass(&nppClass))
	{
		throw std::runtime_error("Notepad_plus_Window::init : RegisterClass() function failed");
	}

	/*
	 * 全局参数
	 */
	NppParameters * pNppParams = NppParameters::getInstance();
	/*
	 * 全局图形展示参数
	 */
	NppGUI        & nppGUI     = const_cast<NppGUI &>(pNppParams->getNppGUI());

	/*
	 * 参数是否禁用插件
	 */
	if (cmdLineParams->_isNoPlugin)
	{
		this->_notepad_plus_plus_core._pluginsManager.disable();
	}

	/****************************************************************
	 * 初始化当前窗口实例句柄，并把当前对象的指针传递给窗口类的对象 *
	 ****************************************************************/
	/********************************************************************************************************************************
	 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-createwindowexw                                 *
	 * function                                                                                                                     *
	 * Creates an overlapped, pop-up, or child window with an extended window style; otherwise, this                                *
	 * function is identical to the CreateWindow(1) function. For more information about creating a                                 *
	 * window and for full descriptions of the other parameters of CreateWindowEx, see                                              *
	 * CreateWindow.                                                                                                                *
	 * CreateWindow(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-createwindowa                            *
	 * HWND CreateWindowEx                                                                                                          *
	 * (                                                                                                                            *
	 *     DWORD     dwExStyle,                                                                                                     *
	 *     LPCWSTR   lpClassName,                                                                                                   *
	 *     LPCWSTR   lpWindowName,                                                                                                  *
	 *     DWORD     dwStyle,                                                                                                       *
	 *     int       X,                                                                                                             *
	 *     int       Y,                                                                                                             *
	 *     int       nWidth,                                                                                                        *
	 *     int       nHeight,                                                                                                       *
	 *     HWND      hWndParent,                                                                                                    *
	 *     HMENU     hMenu,                                                                                                         *
	 *     HINSTANCE hInstance,                                                                                                     *
	 *     LPVOID    lpParam                                                                                                        *
	 * );                                                                                                                           *
	 * dwExStyle                                                                                                                    *
	 * Type: DWORD                                                                                                                  *
	 * The extended window style of the window being created. For a list of possible values,see                                     *
	 * Extended Window Styles(2).                                                                                                   *
	 * Extended Window Styles(2) : https://docs.microsoft.com/windows/desktop/winmsg/extended-window-styles                         *
	 * lpClassName                                                                                                                  *
	 * Type: LPCTSTR                                                                                                                *
	 * A null-terminated string or a class atom created by a previous call to the RegisterClass(3) or                               *
	 * RegisterClassEx(4) function. The atom must be in the low-order word of lpClassName; the high-order                           *
	 * word must be zero. If lpClassName is a string, it specifies the window class name. The class name                            *
	 * can be any name registered with RegisterClass or RegisterClassEx, provided that the module                                   *
	 * that registers the class is also the module that creates the window. The class name can also be any                          *
	 * of the predefined system class(5) names.                                                                                     *
	 * RegisterClass(3) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-registerclassa                          *
	 * RegisterClassEx(4) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-registerclassexa                      *
	 * system class(5) : https://docs.microsoft.com/windows/desktop/winmsg/about-window-classes                                     *
	 * lpWindowName                                                                                                                 *
	 * Type: LPCTSTR                                                                                                                *
	 * The window name. If the window style specifies a title bar, the window title pointed to by                                   *
	 * lpWindowName is displayed in the title bar. When using CreateWindow(6) to create controls, such as                           *
	 * buttons, check boxes, and static controls, use lpWindowName to specify the text of the control.                              *
	 * When creating a static control with the SS_ICON style, use lpWindowName to specify the icon                                  *
	 * name or identifier. To specify an identifier, use the syntax "#num".                                                         *
	 * CreateWindow(6) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-createwindowa                            *
	 * dwStyle                                                                                                                      *
	 * Type: DWORD                                                                                                                  *
	 * The style of the window being created. This parameter can be a combination of the window style                               *
	 * values(7), plus the control styles indicated in the Remarks section.                                                         *
	 * window style values(7) : https://docs.microsoft.com/windows/desktop/winmsg/window-styles                                     *
	 * X                                                                                                                            *
	 * Type: int                                                                                                                    *
	 * The initial horizontal position of the window. For an overlapped or pop-up window, the x                                     *
	 * parameter is the initial x-coordinate of the window's upper-left corner, in screen coordinates. For                          *
	 * a child window, x is the x-coordinate of the upper-left corner of the window relative to the upper-left                      *
	 * corner of the parent window's client area. If x is set to CW_USEDEFAULT, the system selects                                  *
	 * the default position for the window's upper-left corner and ignores the y parameter.                                         *
	 * CW_USEDEFAULT is valid only for overlapped windows; if it is specified for a pop-up or child window,                         *
	 * the x and y parameters are set to zero.                                                                                      *
	 * Y                                                                                                                            *
	 * Type: int                                                                                                                    *
	 * The initial vertical position of the window. For an overlapped or pop-up window, the y parameter                             *
	 * is the initial y-coordinate of the window's upper-left corner, in screen coordinates. For a child                            *
	 * window, y is the initial y-coordinate of the upper-left corner of the child window relative to the                           *
	 * upper-left corner of the parent window's client area. For a list box y is the initial y-coordinate of                        *
	 * the upper-left corner of the list box's client area relative to the upper-left corner of the parent                          *
	 * window's client area.                                                                                                        *
	 * If an overlapped window is created with the WS_VISIBLE style bit set and the x parameter is set to                           *
	 * CW_USEDEFAULT, then the y parameter determines how the window is shown. If the y parameter                                   *
	 * is CW_USEDEFAULT, then the window manager calls ShowWindow(8) with the SW_SHOW flag after                                    *
	 * the window has been created. If the y parameter is some other value, then the window manager                                 *
	 * calls ShowWindow with that value as the nCmdShow parameter.                                                                  *
	 * ShowWindow(8) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-showwindow                                 *
	 * nWidth                                                                                                                       *
	 * Type: int                                                                                                                    *
	 * The width, in device units, of the window. For overlapped windows, nWidth is the window's width,                             *
	 * in screen coordinates, or CW_USEDEFAULT. If nWidth is CW_USEDEFAULT, the system selects a                                    *
	 * default width and height for the window; the default width extends from the initial x-coordinates                            *
	 * to the right edge of the screen; the default height extends from the initial y-coordinate to the top                         *
	 * of the icon area. CW_USEDEFAULT is valid only for overlapped windows; if CW_USEDEFAULT is                                    *
	 * specified for a pop-up or child window, the nWidth and nHeight parameter are set to zero.                                    *
	 * nHeight                                                                                                                      *
	 * Type: int                                                                                                                    *
	 * The height, in device units, of the window. For overlapped windows, nHeight is the window's                                  *
	 * height, in screen coordinates. If the nWidth parameter is set to CW_USEDEFAULT, the system                                   *
	 * ignores nHeight.                                                                                                             *
	 * hWndParent                                                                                                                   *
	 * Type: HWND                                                                                                                   *
	 * A handle to the parent or owner window of the window being created. To create a child window or                              *
	 * an owned window, supply a valid window handle. This parameter is optional for pop-up                                         *
	 * windows.                                                                                                                     *
	 * To create a message-only window(9), supply HWND_MESSAGE or a handle to an existing message-only                              *
	 * window.                                                                                                                      *
	 * message-only window(9) : https://docs.microsoft.com/windows/desktop/winmsg/window-features                                   *
	 * hMenu                                                                                                                        *
	 * Type: HMENU                                                                                                                  *
	 * A handle to a menu, or specifies a child-window identifier, depending on the window style. For an                            *
	 * overlapped or pop-up window, hMenu identifies the menu to be used with the window; it can be                                 *
	 * NULL if the class menu is to be used. For a child window, hMenu specifies the child-window                                   *
	 * identifier, an integer value used by a dialog box control to notify its parent about events. The                             *
	 * application determines the child-window identifier; it must be unique for all child windows with                             *
	 * the same parent window.                                                                                                      *
	 * hInstance                                                                                                                    *
	 * Type: HINSTANCE                                                                                                              *
	 * A handle to the instance of the module to be associated with the window.                                                     *
	 * lpParam                                                                                                                      *
	 * Type: LPVOID                                                                                                                 *
	 * Pointer to a value to be passed to the window through the CREATESTRUCT(10) structure                                         *
	 * (lpCreateParams member) pointed to by the lParam param of the WM_CREATE message. This                                        *
	 * message is sent to the created window by this function before it returns.                                                    *
	 * CREATESTRUCT(10) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-createstructa                           *
	 * If an application calls CreateWindow(11) to create a MDI client window, lpParam should point to a                            *
	 * CLIENTCREATESTRUCT(12) structure. If an MDI client window calls CreateWindow to create an MDI                                *
	 * child window, lpParam should point to a MDICREATESTRUCT(13) structure. lpParam may be NULL if                                *
	 * no additional data is needed.                                                                                                *
	 * CreateWindow(11) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-createwindowa                           *
	 * CLIENTCREATESTRUCT(12) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-clientcreatestruct                *
	 * MDICREATESTRUCT(13) : https://docs.microsoft.com/windows/desktop/api/winuser/ns-winuser-mdicreatestructa                     *
	 * Return Value                                                                                                               *  
	 * Type: Type: HWND                                                                                                             *
	 * If the function succeeds, the return value is a handle to the new window.                                                    *
	 * If the function fails, the return value is NULL. To get extended error information, call GetLastError(14).                   *
	 * This function typically fails for one of the following reasons:                                                              *
	 * GetLastError(14) : https://docs.microsoft.com/windows/desktop/api/errhandlingapi/nf-errhandlingapi-getlasterror              *
	 *     * an invalid parameter value                                                                                             *
	 *     * the system class was registered by a different module                                                                  *
	 *     * The WH_CBT hook is installed and returns a failure code                                                                *
	 *     * if one of the controls in the dialog template is not registered, or its window window                                  *
	 *     procedure fails WM_CREATE(15) or WM_NCCREATE(16)                                                                         *
	 *     WM_CREATE(15) : https://docs.microsoft.com/windows/desktop/winmsg/wm-create                                              *
	 *     WM_NCCREATE(16) : https://docs.microsoft.com/windows/desktop/winmsg/wm-nccreate                                          *
	 * Remarks                                                                                                                      *
	 * The CreateWindowEx function sends WM_NCCREATE(17), WM_NCCALCSIZE(18), and WM_CREATE(19)                                      *
	 * messages to the window being created.                                                                                        *
	 * WM_NCCREATE(17) : https://docs.microsoft.com/windows/desktop/winmsg/wm-nccreate                                              *
	 * WM_NCCALCSIZE(18) : https://docs.microsoft.com/windows/desktop/winmsg/wm-nccalcsize                                          *
	 * WM_CREATE(19) : https://docs.microsoft.com/windows/desktop/winmsg/wm-create                                                  *
	 * If the created window is a child window, its default position is at the bottom of the Z-order. If the                        *
	 * created window is a top-level window, its default position is at the top of the Z-order (but beneath                         *
	 * all topmost windows unless the created window is itself topmost).                                                            *
	 * For information on controlling whether the Taskbar displays a button for the created window, see                             *
	 * Managing Taskbar Buttons(20).                                                                                                *
	 * Managing Taskbar Buttons(20) : https://docs.microsoft.com/windows/desktop/shell/taskbar                                      *
	 * For information on removing a window, see the DestroyWindow(21) function.                                                    *
	 * DestroyWindow(21) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-destroywindow                          *
	 * The following predefined control classes can be specified in the lpClassName parameter. Note the                             *
	 * corresponding control styles you can use in the dwStyle parameter.                                                           *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | Class          | Meaning                                                                                               |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | BUTTON         | Designates a small rectangular child window that represents a button the user can                     |   *
	 * |                | click to turn it on or off. Button controls can be used alone or in groups, and they                  |   *
	 * |                | can either be labeled or appear without text. Button controls typically change                        |   *
	 * |                | appearance when the user clicks them. For more information, see Buttons(22).                          |   *
	 * |                | For a table of the button styles you can specify in the dwStyle parameter, see Button Styles(23).     |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | COMBOBOX       | Designates a control consisting of a list box and a selection field similar to an edit                |   *
	 * |                | control. When using this style, an application should either display the list box at all              |   *
	 * |                | times or enable a drop-down list box. If the list box is visible, typing characters into              |   *
	 * |                | the selection field highlights the first list box entry that matches the characters typed.            |   *
	 * |                | Conversely, selecting an item in the list box displays the selected text in the selection             |   *
	 * |                | field. For more information, see Combo Boxes(24).                                                     |   *
	 * |                | For a table of the combo box styles you can specify in the dwStyle parameter, see                     |   *
	 * |                | Combo Box Styles(25).                                                                                 |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | EDIT           | Designates a rectangular child window into which the user can type text from the keyboard.            |   *
	 * |                | The user selects the control and gives it the keyboard focus by clicking it or moving to              |   *
	 * |                | it by pressing the TAB key. The user can type text when the edit control displays a                   |   *
	 * |                | flashing caret; use the mouse to move the cursor, select characters to be replaced,                   |   *
	 * |                | or position the cursor for inserting characters; or use the key to delete characters.                 |   *
	 * |                | For more information, see Edit Controls(26).                                                          |   *
	 * |                | For a table of the edit control styles you can specify in the dwStyle parameter, see                  |   *
	 * |                | Edit Control Styles(27).                                                                              |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | LISTBOX        | Designates a list of character strings. Specify this control whenever an application                  |   *
	 * |                | must present a list of names, such as filenames, from which the user can choose. The                  |   *
	 * |                | user can select a string by clicking it. A selected string is highlighted, and a                      |   *
	 * |                | notification message is passed to the parent window. For more information, see                        |   *
	 * |                | List Boxes(28).                                                                                       |   *
	 * |                | For a table of the list box styles you can specify in the dwStyle parameter, see List Box Styles(29). |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | MDICLIENT      | Designates an MDI client window. This window receives messages that control the MDI                   |   *
	 * |                | application's child windows. The recommended style bits are                                           |   *
	 * |                | WS_CLIPCHILDREN and WS_CHILD. Specify the WS_HSCROLL and WS_VSCROLL styles                            |   *
	 * |                | to create an MDI client window that allows the user to scroll MDI child windows into view.            |   *
	 * |                | For more information, see Multiple Document Interface(30).                                            |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | RichEdit       | Designates a Microsoft Rich Edit 1.0 control. This window lets the user view and edit text            |   *
	 * |                | with character and paragraph formatting, and can include embedded                                     |   *
	 * |                | Component Object Model (COM) objects. For more information, see Rich Edit Controls(31).               |   *
	 * |                | For a table of the rich edit control styles you can specify in the dwStyle parameter, see             |   *
	 * |                | Rich Edit Control Styles(32).                                                                         |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | RICHEDIT_CLASS | Designates a Microsoft Rich Edit 2.0 control. This controls let the user view and edit text           |   *
	 * |                | with character and paragraph formatting, and can include embedded COM objects.                        |   *
	 * |                | For more information, see Rich Edit Controls(31).                                                     |   *
	 * |                | For a table of the rich edit control styles you can specify in the dwStyle parameter, see             |   *
	 * |                | Rich Edit Control Styles(32).                                                                         |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | SCROLLBAR      | Designates a rectangle that contains a scroll box and has direction arrows at both ends.              |   *
	 * |                | The scroll bar sends a notification message to its parent window whenever the                         |   *
	 * |                | user clicks the control. The parent window is responsible for updating the position                   |   *
	 * |                | of the scroll box, if necessary. For more information, see Scroll Bars(33).                           |   *
	 * |                | For a table of the scroll bar control styles you can specify in the dwStyle parameter, see            |   *
	 * |                | Scroll Bar Control Styles(34).                                                                        |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * | STATIC         | Designates a simple text field, box, or rectangle used to label, box, or separate other               |   *
	 * |                | controls. Static controls take no input and provide no output. For more information,                  |   *
	 * |                | see Static Controls(35).                                                                              |   *
	 * |                | For a table of the static control styles you can specify in the dwStyle parameter, see                |   *
	 * |                | Static Control Styles(36).                                                                            |   *
	 * +----------------+-------------------------------------------------------------------------------------------------------+   *
	 * Buttons(22) : https://docs.microsoft.com/windows/desktop/Tapi/buttons                                                        *
	 * Button Styles(23) : https://docs.microsoft.com/windows/desktop/Controls/button-styles                                        *
	 * Combo Boxes(24) : https://docs.microsoft.com/windows/desktop/Controls/combo-boxes                                            *
	 * Combo Box Styles(25) : https://docs.microsoft.com/windows/desktop/Controls/combo-box-styles                                  *
	 * Edit Controls(26) : https://docs.microsoft.com/windows/desktop/Controls/edit-controls                                        *
	 * Edit Control Styles(27) : https://docs.microsoft.com/windows/desktop/Controls/edit-control-styles                            *
	 * List Boxes(28) : https://docs.microsoft.com/windows/desktop/uxguide/ctrl-list-boxes                                          *
	 * List Box Styles(29) : https://docs.microsoft.com/windows/desktop/Controls/list-box-styles                                    *
	 * Multiple Document Interface(30) : https://docs.microsoft.com/windows/desktop/winmsg/multiple-document-interface              *
	 * Rich Edit Controls(31) : https://docs.microsoft.com/windows/desktop/Controls/rich-edit-controls                              *
	 * Rich Edit Control Styles(32) : https://docs.microsoft.com/windows/desktop/Controls/rich-edit-control-styles                  *
	 * Scroll Bars(33) : https://docs.microsoft.com/windows/desktop/Controls/scroll-bars                                            *
	 * Scroll Bar Control Styles(34) : https://docs.microsoft.com/windows/desktop/Controls/scroll-bar-control-styles                *
	 * Static Controls(35) : https://docs.microsoft.com/windows/desktop/Controls/static-controls                                    *
	 * Static Control Styles(36) : https://docs.microsoft.com/windows/desktop/Controls/static-control-styles                        *
	 * The WS_EX_NOACTIVATE value for dwExStyle prevents foreground activation by the system. To                                    *
	 * prevent queue activation when the user clicks on the window, you must process the                                            *
	 * WM_MOUSEACTIVATE(37) message appropriately. To bring the window to the foreground or to                                      *
	 * activate it programmatically, use SetForegroundWindow(38) or SetActiveWindow(39). Returning FALSE to                         *
	 * WM_NCACTIVATE(40) prevents the window from losing queue activation. However, the return value is ignored at activation time. *
	 * WM_MOUSEACTIVATE(37) : https://docs.microsoft.com/windows/desktop/inputdev/wm-mouseactivate                                  *
	 * SetForegroundWindow(38) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-setforegroundwindow              *
	 * SetActiveWindow(39) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-setactivewindow                      *
	 * WM_NCACTIVATE(40) : https://docs.microsoft.com/windows/desktop/winmsg/wm-ncactivate                                          *
	 * With WS_EX_COMPOSITED set, all descendants of a window get bottom-to-top painting order                                      *
	 * using double-buffering. Bottom-to-top painting order allows a descendent window to have                                      *
	 * translucency (alpha) and transparency (color-key) effects, but only if the descendent window also                            *
	 * has the WS_EX_TRANSPARENT bit set. Double-buffering allows the window and its descendents                                    *
	 * to be painted without flicker.                                                                                               *
	 ********************************************************************************************************************************/
	this->_hSelf = ::CreateWindowEx
	(
		WS_EX_ACCEPTFILES | (this->_notepad_plus_plus_core._nativeLangSpeaker.isRTL()?WS_EX_LAYOUTRTL:0),
		Notepad_plus_Window::_className,
		/*默认窗口的标题*/
		Notepad_plus_Window::_titleName,/*TEXT("Notepad++")*/
		(WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN),
		/********************************************************
		 * CreateWindowEx bug : set all 0 to walk around the pb *
		 ********************************************************/
		0, 0, 0, 0,
		this->_hParent,
		nullptr,
		this->_hInst,
		/**********************************************
		 * pass the ptr of this instantiated object	  *
		 * for retrieve it in Notepad_plus_Proc from  *
		 * the CREATESTRUCT.lpCreateParams afterward. *
		 **********************************************/
		(LPVOID) this
	);

	if (NULL == this->_hSelf)
	{
		throw std::runtime_error("Notepad_plus_Window::init : CreateWindowEx() function return null");
	}


	PaintLocker paintLocker{this->_hSelf};

	_notepad_plus_plus_core.staticCheckMenuAndTB();

	/********************************
	 * 类静态成员变量记录当前的窗口 *
	 ********************************/
	Notepad_plus_Window::gNppHWND = this->_hSelf;

	/*
	 * 如果来自Shell的坐标有效，移动窗口至指定的位置，窗口的大小从配置文件config.xml中获取
	 */
	if (cmdLineParams->isPointValid())
	{
		::MoveWindow(this->_hSelf, cmdLineParams->_point.x, cmdLineParams->_point.y, nppGUI._appPos.right, nppGUI._appPos.bottom, TRUE);
	}
	else
	{
		WINDOWPLACEMENT posInfo;
	    posInfo.length = sizeof(WINDOWPLACEMENT);
		posInfo.flags = 0;
		if (this->_isPrelaunch)
		{
			posInfo.showCmd = SW_HIDE;
		}
		else
		{
			posInfo.showCmd = nppGUI._isMaximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
		}

		posInfo.ptMinPosition.x = (LONG)-1;
		posInfo.ptMinPosition.y = (LONG)-1;
		posInfo.ptMaxPosition.x = (LONG)-1;
		posInfo.ptMaxPosition.y = (LONG)-1;
		posInfo.rcNormalPosition.left   = nppGUI._appPos.left;
		posInfo.rcNormalPosition.top    = nppGUI._appPos.top;
		posInfo.rcNormalPosition.bottom = nppGUI._appPos.top + nppGUI._appPos.bottom;
		posInfo.rcNormalPosition.right  = nppGUI._appPos.left + nppGUI._appPos.right;

		//SetWindowPlacement will take care of situations, where saved position was in no longer available monitor
		::SetWindowPlacement(this->_hSelf,&posInfo);
	}

	if ((nppGUI._tabStatus & TAB_MULTILINE) != 0)
	{
		::SendMessage(this->_hSelf, WM_COMMAND, IDM_VIEW_DRAWTABBAR_MULTILINE, 0);
	}

	if (!nppGUI._menuBarShow)
	{
		::SetMenu(this->_hSelf, NULL);
	}

	if (cmdLineParams->_isNoTab || (nppGUI._tabStatus & TAB_HIDE))
	{
		const int tabStatusOld = nppGUI._tabStatus;
		::SendMessage(this->_hSelf, NPPM_HIDETABBAR, 0, TRUE);
		if (cmdLineParams->_isNoTab)
		{
			// Restore old settings when tab bar has been hidden from tab bar.
			nppGUI._tabStatus = tabStatusOld;
		}
	}

	if (cmdLineParams->_alwaysOnTop)
	{
		::SendMessage(this->_hSelf, WM_COMMAND, IDM_VIEW_ALWAYSONTOP, 0);
	}

	nppGUI._isCmdlineNosessionActivated = cmdLineParams->_isNoSession;
	if (nppGUI._rememberLastSession && !cmdLineParams->_isNoSession)
	{
		this->_notepad_plus_plus_core.loadLastSession();
	}

	if (not cmdLineParams->_isPreLaunch)
	{
		if (cmdLineParams->isPointValid())
			::ShowWindow(this->_hSelf, SW_SHOW);
		else
			::ShowWindow(this->_hSelf, nppGUI._isMaximized ? SW_MAXIMIZE : SW_SHOW);
	}
	else
	{
		_notepad_plus_plus_core._pTrayIco = new trayIconControler(this->_hSelf, IDI_M30ICON, IDC_MINIMIZED_TRAY, ::LoadIcon(_hInst, MAKEINTRESOURCE(IDI_M30ICON)), TEXT(""));
		_notepad_plus_plus_core._pTrayIco->doTrayIcon(ADD);
	}

	std::vector<generic_string> fns;
	if (cmdLine)
	{
		fns = _notepad_plus_plus_core.loadCommandlineParams(cmdLine, cmdLineParams);
	}

	std::vector<generic_string> fileNames;
	std::vector<generic_string> patterns;
	patterns.push_back(TEXT("*.xml"));

	generic_string nppDir = pNppParams->getNppPath();

	LocalizationSwitcher & localizationSwitcher = pNppParams->getLocalizationSwitcher();
	std::wstring localizationDir = nppDir;
	PathAppend(localizationDir, TEXT("localization\\"));

	_notepad_plus_plus_core.getMatchedFileNames(localizationDir.c_str(), patterns, fileNames, false, false);
	for (size_t i = 0, len = fileNames.size(); i < len; ++i)
	{
		localizationSwitcher.addLanguageFromXml(fileNames[i].c_str());
	}

	fileNames.clear();
	ThemeSwitcher & themeSwitcher = pNppParams->getThemeSwitcher();

	//  Get themes from both npp install themes dir and app data themes dir with the per user
	//  overriding default themes of the same name.

	generic_string themeDir;
    if (pNppParams->getAppDataNppDir() && pNppParams->getAppDataNppDir()[0])
    {
        themeDir = pNppParams->getAppDataNppDir();
	    PathAppend(themeDir, TEXT("themes\\"));
	    _notepad_plus_plus_core.getMatchedFileNames(themeDir.c_str(), patterns, fileNames, false, false);
	    for (size_t i = 0, len = fileNames.size() ; i < len ; ++i)
	    {
		    themeSwitcher.addThemeFromXml(fileNames[i].c_str());
	    }
    }

	fileNames.clear();
	themeDir.clear();
	themeDir = nppDir.c_str(); // <- should use the pointer to avoid the constructor of copy
	PathAppend(themeDir, TEXT("themes\\"));
	_notepad_plus_plus_core.getMatchedFileNames(themeDir.c_str(), patterns, fileNames, false, false);
	for (size_t i = 0, len = fileNames.size(); i < len ; ++i)
	{
		generic_string themeName( themeSwitcher.getThemeFromXmlFileName(fileNames[i].c_str()) );
		if (! themeSwitcher.themeNameExists(themeName.c_str()) )
		{
			themeSwitcher.addThemeFromXml(fileNames[i].c_str());
		}
	}

	// Restore all dockable panels from the last session
	for (size_t i = 0, len = _notepad_plus_plus_core._internalFuncIDs.size(); i < len; ++i)
	{
		::SendMessage(_hSelf, WM_COMMAND, _notepad_plus_plus_core._internalFuncIDs[i], 0);
	}

	// Launch folder as workspace after all this dockable panel being restored from the last session
	// To avoid dockable panel toggle problem.
	if (cmdLineParams->_openFoldersAsWorkspace)
	{
		_notepad_plus_plus_core.launchFileBrowser(fns, true);
	}

	// Notify plugins that Notepad++ is ready
	SCNotification scnN;
	scnN.nmhdr.code = NPPN_READY;
	scnN.nmhdr.hwndFrom = _hSelf;
	scnN.nmhdr.idFrom = 0;
	_notepad_plus_plus_core._pluginsManager.notify(&scnN);

	if (!cmdLineParams->_easterEggName.empty())
	{
		if (cmdLineParams->_quoteType == 0) // Easter Egg Name
		{
			int iQuote = _notepad_plus_plus_core.getQuoteIndexFrom(cmdLineParams->_easterEggName.c_str());
			if (iQuote != -1)
			{
				_notepad_plus_plus_core.showQuoteFromIndex(iQuote);
			}
		}
		else if (cmdLineParams->_quoteType == 1) // command line quote
		{
			_userQuote = cmdLineParams->_easterEggName;
			_quoteParams.reset();
			_quoteParams._quote = _userQuote.c_str();
			_quoteParams._quoter = TEXT("Anonymous #999");
			_quoteParams._shouldBeTrolling = false;
			_quoteParams._lang = cmdLineParams->_langType;
			if (cmdLineParams->_ghostTypingSpeed == 1)
				_quoteParams._speed = QuoteParams::slow;
			else if (cmdLineParams->_ghostTypingSpeed == 2)
				_quoteParams._speed = QuoteParams::rapid;
			else if (cmdLineParams->_ghostTypingSpeed == 3)
				_quoteParams._speed = QuoteParams::speedOfLight;

			_notepad_plus_plus_core.showQuote(&_quoteParams);
		}
		else if (cmdLineParams->_quoteType == 2) // content drom file
		{
			if (::PathFileExists(cmdLineParams->_easterEggName.c_str()))
			{
				std::string content = getFileContent(cmdLineParams->_easterEggName.c_str());
				WcharMbcsConvertor& wmc = WcharMbcsConvertor::getInstance();
				_userQuote = wmc.char2wchar(content.c_str(), SC_CP_UTF8);
				if (!_userQuote.empty())
				{
					_quoteParams.reset();
					_quoteParams._quote = _userQuote.c_str();
					_quoteParams._quoter = TEXT("Anonymous #999");
					_quoteParams._shouldBeTrolling = false;
					_quoteParams._lang = cmdLineParams->_langType;
					if (cmdLineParams->_ghostTypingSpeed == 1)
						_quoteParams._speed = QuoteParams::slow;
					else if (cmdLineParams->_ghostTypingSpeed == 2)
						_quoteParams._speed = QuoteParams::rapid;
					else if (cmdLineParams->_ghostTypingSpeed == 3)
						_quoteParams._speed = QuoteParams::speedOfLight;

					_notepad_plus_plus_core.showQuote(&_quoteParams);
				}
			}
		}
	}

	if (cmdLineParams->_showLoadingTime)
	{
		time_t timestampEnd = time(NULL);
		double loadTime = difftime(timestampEnd, timestampBegin);

		char dest[256];
		sprintf(dest, "Loading time : %.0lf seconds", loadTime);
		::MessageBoxA(NULL, dest, "", MB_OK);
	}

	bool isSnapshotMode = nppGUI.isSnapshotMode();
	if (isSnapshotMode)
	{
		_notepad_plus_plus_core.checkModifiedDocument(false);
		// Lauch backup task
		_notepad_plus_plus_core.launchDocumentBackupTask();
	}

	// Make this call later to take effect
	::SendMessage(_hSelf, NPPM_INTERNAL_SETWORDCHARS, 0, 0);

	if (pNppParams->doFunctionListExport())
		::SendMessage(_hSelf, NPPM_INTERNAL_EXPORTFUNCLISTANDQUIT, 0, 0);

	if (pNppParams->doPrintAndExit())
		::SendMessage(_hSelf, NPPM_INTERNAL_PRNTANDQUIT, 0, 0);
}


bool Notepad_plus_Window::isDlgsMsg(MSG *msg) const
{
	for (size_t i = 0, len = _notepad_plus_plus_core._hModelessDlgs.size(); i < len; ++i)
	{
		if (_notepad_plus_plus_core.processIncrFindAccel(msg))
			return true;

		if (_notepad_plus_plus_core.processFindAccel(msg))
			return true;

		if (::IsDialogMessageW(_notepad_plus_plus_core._hModelessDlgs[i], msg))
			return true;
	}
	return false;
}
