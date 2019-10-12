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


#pragma once

#include "Parameters.h"
#include <deque>

/******************************
 * 单个最近打开的文件的结构体 *
 ******************************/
struct RecentItem
{
	/**************************
	 * 文件在菜单中的顺序编号 *
	 **************************/
	int _id = 0;
	/*
	 * 文件的绝对路径
	 */
	generic_string _name;

	explicit RecentItem(const TCHAR * name) : _name(name)
	{
	}
};

/** typedef std::deque<RecentItem> recentList; */

/************************
 * 最近打开文件的列表类 *
 ************************/
class LastRecentFileList
{
public:
	/*
	 * 唯一无参构造函数
	 */
	LastRecentFileList()
	{
		LastRecentFileList::_userMax = (NppParameters::getInstance())->getNbMaxRecentFile();
		for (int i = 0; i < NB_MAX_LRF_FILE; i++)
		{
			LastRecentFileList::_idFreeArray[i] = false;
		}
	}

	/*
	 * 把当前对象与菜单进行对接
	 * 
	 */
	void initMenu(HMENU hMenu, int idBase, int posBase, Accelerator * accelerator, bool doSubMenu = false);

	void switchMode();

	void updateMenu();

	void add(const TCHAR *fn);
	void remove(const TCHAR *fn);
	void remove(size_t index);
	void clear();

	int getSize()
	{
		return _size;
	}


	int getMaxNbLRF() const
	{
		return NB_MAX_LRF_FILE;
	}

	int getUserMaxNbLRF() const
	{
		return _userMax;
	}

	//use menu id
	generic_string & getItem(int id);

	//use menu id
	generic_string & getIndex(int index);

	generic_string getFirstItem() const
	{
		if (_lrfl.size() == 0)
		{
			return TEXT("");
		}
		return _lrfl.front()._name;
	}

	void setUserMaxNbLRF(int size);

	void saveLRFL();

	/*
	 * 加锁或解锁
	 */
	void setLock(bool lock)
	{
		_locked = lock;
	}

	/*
	 * 设置语言编码
	 */
	void setLangEncoding(int nativeLangEncoding)
	{
		_nativeLangEncoding = nativeLangEncoding;
	}

	/*
	 * 是否是子菜单模式
	 */
	bool isSubMenuMode() const
	{
		return (_hParentMenu != NULL);
	}

private:
	/**************************
	 * 最近打开的文件列表队列 *
	 **************************/
	std::deque<RecentItem> _lrfl;

	Accelerator * _pAccelerator = nullptr;
	/*
	 * 当前允许的最大历史记录数
	 */
	int _userMax = 0;
	/*
	 * 当前数量
	 */
	int _size = 0;
	/*
	 */
	int _nativeLangEncoding = -1;

	// For the menu
	/****************************************
	 * 是否以子菜单的方式展式最近打开的文件 *
	 ****************************************/
	HMENU _hParentMenu = nullptr;

	/*
	 * 目标菜单
	 */
	HMENU _hMenu       = nullptr;
	/*
	 *菜单展示的位置
	 */
	int _posBase       = -1;
	/*
	 * 消息序号
	 */
	int _idBase        = -1;

	/*
	 * 相应的数组位置是否被占用，false已占用
	 */
	bool _idFreeArray[NB_MAX_LRF_FILE];

	/*
	 *是否向菜单中加上了分隔符，true表示已经加上，默认和初始值均为：false
	 */
	bool _hasSeparators = false;

	/*
	 * 是否更新的锁，true表示已加锁，默认和初始值均为：false
	 */
	bool _locked        = false;

	int find(const TCHAR *fn);

	int popFirstAvailableID();

	void setAvailable(int id);
};
