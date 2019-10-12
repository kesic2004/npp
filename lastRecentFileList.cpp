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



#include "lastRecentFileList.h"
#include "menuCmdID.h"
#include "localization.h"

/*
 * @param hMenu        对接到的菜单句柄
 * @param idBase       菜单消息序号
 * @param pAccelerator 快捷键相关
 * @param doSubMenu    是否子菜单展示(true : 以子菜单展示)
 */
void LastRecentFileList::initMenu(HMENU hMenu, int idBase, int posBase, Accelerator *pAccelerator, bool doSubMenu)
{
	if (doSubMenu)
	{
		_hParentMenu = hMenu;
		_hMenu = ::CreatePopupMenu();
	}
	else
	{
		_hParentMenu = NULL;
		_hMenu = hMenu;
	}

	_idBase             = idBase;
	_posBase            = posBase;
	_pAccelerator       = pAccelerator;
	_nativeLangEncoding = NPP_CP_WIN_1252;

	/*
	 * 把数组占位设置为未占位
	 */
	for (int i = 0; i < sizeof(_idFreeArray); ++i)
	{
		_idFreeArray[i] = true;
	}
}

/*
 * 子菜单模式和非子菜单模式之间的转换，只清空菜单，和更新菜单数据
 */
void LastRecentFileList::switchMode()
{
	// Remove all menu items
	/***************************************************************************************************************
	 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-removemenu?redirectedfrom=MSDN *
	 * Deletes a menu item or detaches a submenu from the specified menu. If the menu item opens a drop-down menu  *
	 * or submenu, RemoveMenu does not destroy the menu or its handle, allowing the menu to be reused. Before this *
	 * function is called, the GetSubMenu(1) function should retrieve a handle to the drop-down menu or submenu.   *
	 * 从指定下拉式菜单中移除指定的子菜单，移除的子菜单不会被销毁，还可以再被重新使用，在使用之前请先调用          *
	 * GetSubMenu(1)函数                                                                                           *
	 * GetSubMenu(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-getsubmenu                *
	 * BOOL RemoveMenu(HMENU hMenu, UINT uPosition, UINT uFlags);                                                  *
	 * hMenu                                                                                                       *
	 * Type: HMENU                                                                                                 *
	 * A handle to the menu to be changed.                                                                         *
	 * uPosition                                                                                                   *
	 * Type: UINT                                                                                                  *
	 * The menu item to be deleted, as determined by the uFlags parameter.                                         *
	 * uFlags                                                                                                      *
	 * Type: UINT                                                                                                  *
	 * Indicates how the uPosition parameter is interpreted. This parameter must be one of the following values.   *
	 * +---------------+--------------------------------------------------------------------+                      *
	 * | Value         | Meaning                                                            |                      *
	 * | MF_BYCOMMAND  | Indicates that uPosition gives the identifier of the menu item. If |                      *
	 * | 0x00000000L   | neither the MF_BYCOMMAND nor MF_BYPOSITION flag is specified,      |                      *
	 * |               | the MF_BYCOMMAND flag is the default flag.                         |                      *
	 * +---------------+--------------------------------------------------------------------+                      *
	 * | MF_BYPOSITION | Indicates that uPosition gives the zero-based relative position    |                      *
	 * | 0x00000400L   | of the menu item.                                                  |                      *
	 * +---------------+--------------------------------------------------------------------+                      *
	 * Return Value                                                                                                *
	 * Type: BOOL                                                                                                  *
	 * If the function succeeds, the return value is nonzero.                                                      *
	 * If the function fails, the return value is zero. To get extended error information, call GetLastError.      *
	 * Remarks                                                                                                     *
	 * The application must call the DrawMenuBar function whenever a menu changes, whether the menu                *
	 * is in a displayed window.                                                                                   *
	 ***************************************************************************************************************/
	::RemoveMenu(_hMenu, IDM_FILE_RESTORELASTCLOSEDFILE, MF_BYCOMMAND);
	::RemoveMenu(_hMenu, IDM_OPEN_ALL_RECENT_FILE, MF_BYCOMMAND);
	::RemoveMenu(_hMenu, IDM_CLEAN_RECENT_FILE_LIST, MF_BYCOMMAND);

	for (int i = 0; i < _size; ++i)
	{
		::RemoveMenu(_hMenu, _lrfl.at(i)._id, MF_BYCOMMAND);
	}

	// mode main menu
	if (_hParentMenu == NULL)
	{
		if (_size > 0)
		{
			::RemoveMenu(_hMenu, _posBase, MF_BYPOSITION);
			::RemoveMenu(_hMenu, _posBase, MF_BYPOSITION);
		}
		// switch to sub-menu mode
		_hParentMenu = _hMenu;
		/**********************************************************************************************************************
		 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-createpopupmenu?redirectedfrom=MSDN   *
		 * function                                                                                                           *
		 * Creates a drop-down menu, submenu, or shortcut menu. The menu is initially empty. You can insert or append menu    *
		 * items by using the InsertMenuItem(1) function. You can also use the InsertMenu function to insert menu items       *
		 * and the AppendMenu(2) function to append menu items.                                                               *
		 * InsertMenuItem(1) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-insertmenuitema              *
		 * AppendMenu(2) : https://docs.microsoft.com/windows/desktop/menurc/u                                                *
		 * HMENU CreatePopupMenu();                                                                                           *
		 * 新建一个下接式菜单、子菜单、快捷菜单                                                                               *
		 * Return Value                                                                                                     *
		 * Type: HMENU                                                                                                        *
		 * If the function succeeds, the return value is a handle to the newly created menu.                                  *
		 * If the function fails, the return value is NULL. To get extended error information, call GetLastError.             *
		 * Remarks                                                                                                          *
		 * The application can add the new menu to an existing menu, or it can display a shortcut menu by calling the         *
		 * TrackPopupMenuEx(3) or TrackPopupMenu(4) functions.                                                                *
		 * TrackPopupMenuEx(3) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-trackpopupmenuex           *
		 * TrackPopupMenu(4) : https://docs.microsoft.com/windows/desktop/api/winuser/nf-winuser-trackpopupmenu               *
		 * Resources associated with a menu that is assigned to a window are freed automatically. If the menu is not          *
		 * assigned to a window, an application must free system resources associated with the menu before closing.           *
		 * An application frees menu resources by calling the DestroyMenu function.                                           *
		 * Examples                                                                                                         *
		 * For an example, see Adding Lines and Graphs to a Menu(5).                                                          *
		 * Adding Lines and Graphs to a Menu(5) : https://docs.microsoft.com/windows/desktop/menurc/using-menus               *
		 **********************************************************************************************************************/
		_hMenu = ::CreatePopupMenu();
		::RemoveMenu(_hMenu, _posBase+1, MF_BYPOSITION);
	}
	else // mode sub-menu
	{
		if (_size > 0)
		{
			::RemoveMenu(_hParentMenu, _posBase, MF_BYPOSITION);
			::RemoveMenu(_hParentMenu, _posBase, MF_BYPOSITION);
		}
		// switch to main menu mode
		/****************************************************************************************************************
		 * from : https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-destroymenu?redirectedfrom=MSDN *
		 * function                                                                                                     *
		 * Destroys the specified menu and frees any memory that the menu occupies.                                     *
		 * 销毁指定的下拉菜单同时释放该菜单占用的内存空间                                                               *
		 * BOOL DestroyMenu(HMENU hMenu);                                                                               *
		 * hMenu                                                                                                        *
		 * Type: HMENU                                                                                                  *
		 * A handle to the menu to be destroyed.                                                                        *
		 * Return Value                                                                                               *
		 * Type: BOOL                                                                                                   *
		 * If the function succeeds, the return value is nonzero.                                                       *
		 * If the function fails, the return value is zero. To get extended error information, call GetLastError.       *
		 * Remarks                                                                                                    *
		 * Before closing, an application must use the DestroyMenu function to destroy a menu not assigned to a         *
		 * window. A menu that is assigned to a window is automatically destroyed when the application closes.          *
		 * DestroyMenu is recursive, that is, it will destroy the menu and all its submenus.                            *
		 * Examples                                                                                                   *
		 * For an example, see Displaying a Shortcut Menu(1).                                                           *
		 * Displaying a Shortcut Menu(1) : https://docs.microsoft.com/windows/desktop/menurc/using-menus                *
		 ****************************************************************************************************************/
		::DestroyMenu(_hMenu);
		_hMenu = _hParentMenu;
		_hParentMenu = NULL;
	}
	_hasSeparators = false;
};

void LastRecentFileList::updateMenu()
{
	NppParameters *pNppParam = NppParameters::getInstance();

	if (!_hasSeparators && _size > 0) 
	{	
		//add separators
		NativeLangSpeaker *pNativeLangSpeaker = pNppParam->getNativeLangSpeaker();

		generic_string recentFileList = pNativeLangSpeaker->getSpecialMenuEntryName("RecentFiles"); /*  */

		/***************************************************************
		 * #define    IDM                              40000           *
		 * #define    IDM_FILE                         (IDM + 1000)    *
		 * #define    IDM_FILE_RESTORELASTCLOSEDFILE   (IDM_FILE + 21) *
		 ***************************************************************/
		generic_string openRecentClosedFile = pNativeLangSpeaker->getNativeLangMenuString(IDM_FILE_RESTORELASTCLOSEDFILE); /* 恢复最近关闭的文件 */

		/*******************************************************************
		 * #define    IDM                                  40000           *
		 * #define    IDM_EDIT                             (IDM + 2000)    *
		 * #define    IDM_OPEN_ALL_RECENT_FILE             (IDM_EDIT + 40) *
		 *******************************************************************/
		generic_string openAllFiles = pNativeLangSpeaker->getNativeLangMenuString(IDM_OPEN_ALL_RECENT_FILE);/* 打开文件列表 */

		/*******************************************************************
		 * #define    IDM                                  40000           *
		 * #define    IDM_EDIT                             (IDM + 2000)    *
		 * #define    IDM_CLEAN_RECENT_FILE_LIST           (IDM_EDIT + 41) *
		 *******************************************************************/
		generic_string cleanFileList = pNativeLangSpeaker->getNativeLangMenuString(IDM_CLEAN_RECENT_FILE_LIST);/* 清除文件列表 */

		if (recentFileList == TEXT(""))
		{
			recentFileList = TEXT("&Recent Files");
		}
		if (openRecentClosedFile == TEXT(""))
		{
			openRecentClosedFile = TEXT("Restore Recent Closed File");
		}
		if (openAllFiles == TEXT(""))
		{
			openAllFiles = TEXT("Open All Recent Files");
		}
		if (cleanFileList == TEXT(""))
		{
			cleanFileList = TEXT("Empty Recent Files List");
		}

		if (!isSubMenuMode())
		{
			::InsertMenu(_hMenu, _posBase + 0, MF_BYPOSITION, static_cast<UINT_PTR>(-1), 0); /*菜单分隔符*/
		}

		/***************************************************************
		 * #define    IDM                              40000           *
		 * #define    IDM_FILE                         (IDM + 1000)    *
		 * #define    IDM_FILE_RESTORELASTCLOSEDFILE   (IDM_FILE + 21) *
		 ***************************************************************/
		::InsertMenu(_hMenu, _posBase + 1, MF_BYPOSITION, IDM_FILE_RESTORELASTCLOSEDFILE, openRecentClosedFile.c_str());

		/*******************************************************************
		 * #define    IDM                                  40000           *
		 * #define    IDM_EDIT                             (IDM + 2000)    *
		 * #define    IDM_OPEN_ALL_RECENT_FILE             (IDM_EDIT + 40) *
		 *******************************************************************/
		::InsertMenu(_hMenu, _posBase + 2, MF_BYPOSITION, IDM_OPEN_ALL_RECENT_FILE, openAllFiles.c_str());

		/*******************************************************************
		 * #define    IDM                                  40000           *
		 * #define    IDM_EDIT                             (IDM + 2000)    *
		 * #define    IDM_CLEAN_RECENT_FILE_LIST           (IDM_EDIT + 41) *
		 *******************************************************************/
		::InsertMenu(_hMenu, _posBase + 3, MF_BYPOSITION, IDM_CLEAN_RECENT_FILE_LIST, cleanFileList.c_str());

		/****************************************
		 * static_cast<UINT_PTR>(-1) == 2^64 -1 *
		 ****************************************/
		::InsertMenu(_hMenu, _posBase + 4, MF_BYPOSITION, static_cast<UINT_PTR>(-1), 0); /*菜单分隔符*/
		/*
		 * 设置为已经加上了分隔符
		 */
		_hasSeparators = true;

		if (isSubMenuMode())
		{
			::InsertMenu(_hParentMenu, _posBase + 0, MF_BYPOSITION | MF_POPUP, reinterpret_cast<UINT_PTR>(_hMenu), (LPCTSTR)recentFileList.c_str());
			::InsertMenu(_hParentMenu, _posBase + 1, MF_BYPOSITION, static_cast<UINT_PTR>(-1), 0);
		}
	}
	else if (_hasSeparators && _size == 0) 	//remove separators
	{
		::RemoveMenu(_hMenu, _posBase + 4,                   MF_BYPOSITION);
		::RemoveMenu(_hMenu, IDM_CLEAN_RECENT_FILE_LIST,     MF_BYCOMMAND);
		::RemoveMenu(_hMenu, IDM_OPEN_ALL_RECENT_FILE,       MF_BYCOMMAND);
		::RemoveMenu(_hMenu, IDM_FILE_RESTORELASTCLOSEDFILE, MF_BYCOMMAND);
		::RemoveMenu(_hMenu, _posBase + 0,                   MF_BYPOSITION);
		_hasSeparators = false;

		if (isSubMenuMode())
		{
			// Remove "Recent Files" Entry and the separator from the main menu
			::RemoveMenu(_hParentMenu, _posBase + 1, MF_BYPOSITION);
			::RemoveMenu(_hParentMenu, _posBase + 0, MF_BYPOSITION);

			// Remove the last left separator from the submenu
			::RemoveMenu(_hMenu, 0, MF_BYPOSITION);
		}
	}

	_pAccelerator->updateFullMenu();

	//Remove all menu items
	for (int i = 0; i < _size; ++i) 
	{
		::RemoveMenu(_hMenu, _lrfl.at(i)._id, MF_BYCOMMAND);
	}

	//Then readd them, so everything stays in sync
	for (int j = 0; j < _size; ++j)
	{
		generic_string strBuffer(BuildMenuFileName(pNppParam->getRecentFileCustomLength(), j, _lrfl.at(j)._name));
		::InsertMenu(_hMenu, _posBase + j, MF_BYPOSITION, _lrfl.at(j)._id, strBuffer.c_str());
	}
	
}

/*
 * 把fn新加到队列中(如果发现重复的字符串，先移除该项)，同时更亲菜单
 */
void LastRecentFileList::add(const TCHAR *fn) 
{
	if (_userMax == 0 || _locked)
	{
		return;
	}

	RecentItem itemToAdd(fn);

	int index = find(fn);

	//already in list, bump upwards
	if (index != -1)
	{
		remove(index);
	}

	if (_size == _userMax)
	{
		itemToAdd._id = _lrfl.back()._id;
		//remove oldest
		/************************************************************************************************************
		 * from : https://docs.microsoft.com/en-us/previous-versions/0x0e3528(v=vs.140)?redirectedfrom=MSDN         *
		 * Deletes the element at the end of the deque.                                                             *
		 * void pop_back();                                                                                         *
		 * Remarks                                                                                                *
		 * The last element must not be empty. pop_back never throws an exception.                                  *
		 * Example                                                                                                  *
		 * +------------------------------------------------------------------------------------------------------+ *
		 * | // deque_pop_back.cpp                                                                                | *
		 * | // compile with: /EHsc                                                                               | *
		 * | #include <deque>                                                                                     | *
		 * | #include <iostream>                                                                                  | *
		 * |                                                                                                      | *
		 * | int main( )                                                                                          | *
		 * | {                                                                                                    | *
		 * |     using namespace std;                                                                             | *
		 * |     deque <int> c1;                                                                                  | *
		 * |                                                                                                      | *
		 * |     c1.push_back( 1 );                                                                               | *
		 * |     c1.push_back( 2 );                                                                               | *
		 * |     cout << "The first element is: " << c1.front( ) << endl;                                         | *
		 * |     cout << "The last element is: " << c1.back( ) << endl;                                           | *
		 * |                                                                                                      | *
		 * |     c1.pop_back( );                                                                                  | *
		 * |     cout << "After deleting the element at the end of the deque, the "                               | *
		 * |             "last element is: " << c1.back( ) << endl;                                               | *
		 * | }                                                                                                    | *
		 * +------------------------------------------------------------------------------------------------------+ *
		 * | The first element is: 1                                                                              | *
		 * | The last element is: 2                                                                               | *
		 * | After deleting the element at the end of the deque, the last element is: 1                           | *
		 * +------------------------------------------------------------------------------------------------------+ *
		 * 移除队列中最后的那个元素                                                                                 *
		 ************************************************************************************************************/
		_lrfl.pop_back();
	}
	else
	{
		itemToAdd._id = popFirstAvailableID();
		++_size;
	}
	_lrfl.push_front(itemToAdd);
	updateMenu();
}

void LastRecentFileList::remove(const TCHAR *fn) 
{ 
	int index = find(fn);
	if (index != -1)
	{
		remove(index);
	}
}

/*
 * 从除列中移除相应的字符串，从菜单中移除相应的菜单，当前数量减1，同时跟新菜单
 */
void LastRecentFileList::remove(size_t index) 
{
	if (_size == 0 || _locked)
	{
		return;
	}
	if (index < _lrfl.size())
	{
		::RemoveMenu(_hMenu, _lrfl.at(index)._id, MF_BYCOMMAND);
		setAvailable(_lrfl.at(index)._id);
		_lrfl.erase(_lrfl.begin() + index);
		--_size;
		updateMenu();
	}
}


void LastRecentFileList::clear() 
{
	if (_size == 0)
	{
		return;
	}

	for (int i = (_size-1); i >= 0; i--) 
	{
		::RemoveMenu(_hMenu, _lrfl.at(i)._id, MF_BYCOMMAND);
		setAvailable(_lrfl.at(i)._id);
		_lrfl.erase(_lrfl.begin() + i);
	}
	_size = 0;
	updateMenu();
}


generic_string & LastRecentFileList::getItem(int id) 
{
	int i = 0;
	for (; i < _size; ++i)
	{
		if (_lrfl.at(i)._id == id)
			break;
	}
	if (i == _size)
	{
		i = 0;
	}
	return _lrfl.at(i)._name;	//if not found, return first
};

generic_string & LastRecentFileList::getIndex(int index)
{
	return _lrfl.at(index)._name;	//if not found, return first
}


/*
 * 设置当前的最大容量
 * 如果当前容量已经超过设置的最大容量
 *     从队列的最后移除多出的部分，清空占位，同时更新菜单
 * 否则
 *     只更新相应的值
 */
void LastRecentFileList::setUserMaxNbLRF(int size)
{
	_userMax = size;
	if (_size > _userMax) 
	{	//start popping items
		int toPop = _size-_userMax;
		while (toPop > 0) 
		{
			::RemoveMenu(_hMenu, _lrfl.back()._id, MF_BYCOMMAND);
			setAvailable(_lrfl.back()._id);
			_lrfl.pop_back();
			toPop--;
			_size--;
		}
		updateMenu();
		_size = _userMax;
	}
}



void LastRecentFileList::saveLRFL()
{
	NppParameters *pNppParams = NppParameters::getInstance();
	if (pNppParams->writeRecentFileHistorySettings(_userMax))
	{
		//reverse order: so loading goes in correct order
		for (int i = _size - 1; i >= 0; i--)
		{
			pNppParams->writeHistory(_lrfl.at(i)._name.c_str());
		}
	}
}


/*
 * 查找指定的定符串在队列的位置
 */
int LastRecentFileList::find(const TCHAR *fn)
{
	for (int i = 0; i < _size; ++i)
	{
		if (OrdinalIgnoreCaseCompareStrings(_lrfl.at(i)._name.c_str(), fn) == 0)
		{
			return i;
		}
	}
	return -1;
}

/*
 * 获取占位的序号
 */
int LastRecentFileList::popFirstAvailableID() 
{
	for (int i = 0 ; i < NB_MAX_LRF_FILE ; ++i)
	{
		if (_idFreeArray[i])
		{
			_idFreeArray[i] = false;
			return i + _idBase;
		}
	}
	return 0;
}

/*
 * 清空相应的文件占位
 */
void LastRecentFileList::setAvailable(int id)
{
	int index = id - _idBase;
	_idFreeArray[index] = true;
}
