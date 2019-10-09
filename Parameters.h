/*****************************************************************************
 * This file is part of Notepad++ project									 *
 * Copyright (C)2003 Don HO <don.h@free.fr>									 *
 *																			 *
 * This program is free software; you can redistribute it and/or			 *
 * modify it under the terms of the GNU General Public License				 *
 * as published by the Free Software Foundation; either						 *
 * version 2 of the License, or (at your option) any later version.			 *
 *																			 *
 * Note that the GPL places important restrictions on "derived works", yet	 *
 * it does not provide a detailed definition of that term.  To avoid		 *
 * misunderstandings, we consider an application to constitute a			 *
 * "derivative work" for the purpose of this license if it does any of the	 *
 * following:																 *
 * 1. Integrates source code from Notepad++.								 *
 * 2. Integrates/includes/aggregates Notepad++ into a proprietary executable *
 *	  installer, such as those produced by InstallShield.					 *
 * 3. Links to a library or executes a program that does any of the above.	 *
 *																			 *
 * This program is distributed in the hope that it will be useful,			 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of			 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			 *
 * GNU General Public License for more details.								 *
 *																			 *
 * You should have received a copy of the GNU General Public License		 *
 * along with this program; if not, write to the Free Software				 *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.				 *
 *****************************************************************************/

#pragma once

#include "tinyxmlA.h"
#include "tinyxml.h"

#include "Scintilla.h"
#include "ScintillaRef.h"

#include "ToolBar.h"
#include "UserDefineLangReference.h"
#include "colors.h"
#include "shortcut.h"
#include "ContextMenu.h"
#include "dpiManager.h"
#include <assert.h>
#include <tchar.h>

class NativeLangSpeaker;

const bool POS_VERTICAL = true;
const bool POS_HORIZOTAL = false;

// 0000 0001
const int UDD_SHOW   = 1;
// 0000 0010
const int UDD_DOCKED = 2;

// 0 : 0000 0000 hide & undocked
// 1 : 0000 0001 show & undocked
// 2 : 0000 0010 hide & docked
// 3 : 0000 0011 show & docked

const int TAB_DRAWTOPBAR = 1;      //0000 0000 0001
const int TAB_DRAWINACTIVETAB = 2; //0000 0000 0010
const int TAB_DRAGNDROP = 4;       //0000 0000 0100
const int TAB_REDUCE = 8;          //0000 0000 1000
const int TAB_CLOSEBUTTON = 16;    //0000 0001 0000
const int TAB_DBCLK2CLOSE = 32;    //0000 0010 0000
const int TAB_VERTICAL = 64;       //0000 0100 0000
const int TAB_MULTILINE = 128;     //0000 1000 0000
const int TAB_HIDE = 256;          //0001 0000 0000
const int TAB_QUITONEMPTY = 512;   //0010 0000 0000


enum class EolType: std::uint8_t
{
	windows,
	macos,
	unix,

	// special values
	unknown, // can not be the first value for legacy code
	osdefault = windows,
};

/*!
** \brief Convert an int into a FormatType
** \param value An arbitrary int
** \param defvalue The default value to use if an invalid value is provided
*/
EolType convertIntToFormatType(int value, EolType defvalue = EolType::osdefault);


enum UniMode
{
	uni8Bit=0,
	uniUTF8=1,
	uni16BE=2,
	uni16LE=3,
	uniCookie=4,
	uni7Bit=5,
	uni16BE_NoBOM=6,
	uni16LE_NoBOM=7,
	uniEnd
};

enum ChangeDetect
{
	cdDisabled   = 0x0,
	cdEnabledOld = 0x01,
	cdEnabledNew = 0x02,
	cdAutoUpdate = 0x04,
	cdGo2end     = 0x08
};

enum BackupFeature
{
	bak_none = 0,
	bak_simple = 1,
	bak_verbose = 2
};

enum OpenSaveDirSetting
{
	dir_followCurrent = 0,
	dir_last = 1,
	dir_userDef = 2
};

enum MultiInstSetting
{
	monoInst = 0,
	multiInstOnSession = 1,
	multiInst = 2
};


const int LANG_INDEX_INSTR  = 0;
const int LANG_INDEX_INSTR2 = 1;
const int LANG_INDEX_TYPE   = 2;
const int LANG_INDEX_TYPE2  = 3;
const int LANG_INDEX_TYPE3  = 4;
const int LANG_INDEX_TYPE4  = 5;
const int LANG_INDEX_TYPE5  = 6;

const int COPYDATA_PARAMS     = 0;
const int COPYDATA_FILENAMESA = 1;
const int COPYDATA_FILENAMESW = 2;


#define PURE_LC_NONE 0
#define PURE_LC_BOL	 1
#define PURE_LC_WSP	 2


#define DECSEP_DOT   0
#define DECSEP_COMMA 1
#define DECSEP_BOTH	 2


#define DROPBOX_AVAILABLE     1
#define ONEDRIVE_AVAILABLE    2
#define GOOGLEDRIVE_AVAILABLE 4


const TCHAR fontSizeStrs[][3] =
{
	TEXT(""),   TEXT("5"),  TEXT("6"),
	TEXT("7"),  TEXT("8"),  TEXT("9"),
	TEXT("10"), TEXT("11"), TEXT("12"),
	TEXT("14"), TEXT("16"), TEXT("18"),
	TEXT("20"), TEXT("22"), TEXT("24"),
	TEXT("26"), TEXT("28")
};


const TCHAR localConfFile[]          = TEXT("doLocalConf.xml");
const TCHAR notepadStyleFile[]       = TEXT("asNotepad.xml");
const TCHAR pluginsForAllUsersFile[] = TEXT("pluginsForAllUsers.xml");


void cutString(const TCHAR *str2cut, std::vector<generic_string> & patternVect);


struct Position
{
	int _firstVisibleLine = 0;
	int _startPos = 0;
	int _endPos = 0;
	int _xOffset = 0;
	int _selMode = 0;
	int _scrollWidth = 1;
};


struct MapPosition
{
	int32_t _firstVisibleDisplayLine = -1;

	int32_t _firstVisibleDocLine     = -1; // map
	int32_t _lastVisibleDocLine      = -1; // map
	int32_t _nbLine                  = -1; // map
	int32_t _higherPos               = -1; // map
	int32_t _width                   = -1;
	int32_t _height                  = -1;
	int32_t _wrapIndentMode          = -1;

	int64_t _KByteInDoc              = _maxPeekLenInKB;

	bool _isWrap = false;

	bool isValid() const
	{
		return (_firstVisibleDisplayLine != -1);
	}

	/************************************************************************************
	 * _nbCharInDoc < _maxPeekLen : Don't scroll the document for the performance issue *
	 ************************************************************************************/
	bool canScroll() const
	{
		return (_KByteInDoc < _maxPeekLenInKB);
	}

private:
	/**********
	 * 512 KB *
	 **********/
	int64_t _maxPeekLenInKB = 512;
};


struct sessionFileInfo : public Position
{
	sessionFileInfo
	(
		const TCHAR       * fn,
		const TCHAR       * ln,
		int                 encoding,
		bool                userReadOnly,
		const Position    & pos,
		const TCHAR       * backupFilePath,
		FILETIME            originalFileLastModifTimestamp,
		const MapPosition & mapPos
	)
	:
		_isUserReadOnly(userReadOnly),
		_encoding(encoding),
		Position(pos),
		_originalFileLastModifTimestamp(originalFileLastModifTimestamp),
		_mapPos(mapPos)
	{
		if (fn)
		{
			_fileName = fn;
		}
		if (ln)
		{
			_langName = ln;
		}
		if (backupFilePath)
		{
			_backupFilePath = backupFilePath;
		}
	}

	sessionFileInfo(generic_string fn) : _fileName(fn)
	{
	}

	FILETIME _originalFileLastModifTimestamp = {};

	generic_string      _fileName;
	generic_string      _langName;
	std::vector<size_t> _marks;
	std::vector<size_t> _foldStates;
	int                 _encoding       = -1;
	bool                _isUserReadOnly = false;
	bool                _isMonitoring   = false;

	generic_string      _backupFilePath;

	MapPosition _mapPos;
};


struct Session
{
	size_t nbMainFiles() const
	{
		return _mainViewFiles.size();
	}

	size_t nbSubFiles() const
	{
		return _subViewFiles.size();
	}

	size_t _activeView      = 0;
	size_t _activeMainIndex = 0;
	size_t _activeSubIndex  = 0;

	std::vector<sessionFileInfo> _mainViewFiles;
	std::vector<sessionFileInfo> _subViewFiles;
};


/*
 * 存储Shell参数
 */
struct CmdLineParams
{
	/*
	 * 主窗口的坐标
	 */
	POINT _point;

	/*
	 * 所编辑文件的内容所对应的Lang，默认是L_EXTERNAL
	 */
	LangType _langType = L_EXTERNAL;

	/*
	 * 启动后将光标移动到所打开文件的某个具体的行
	 */
	int _line2go   = -1;
	/*
	 * 启动后将光标移动到所打开文件的某个具体的列
	 */
	int _column2go = -1;
	/*
	 * 启动后将光标移动到所打开文件的某个具体的字节的位置(如第3个字节)
	 */
	int _pos2go    = -1;

	/**********************************************************
	 * -1: initial value  1: slow  2: fast  3: speed of light *
	 * 打字速度                                               *
	 **********************************************************/
	int _ghostTypingSpeed = -1;

	/*
	 * 国际化文件的文件名
	 */
	generic_string _localizationPath;
	/*
	 * qt qn qh
	 */
	generic_string _easterEggName;

	unsigned char _quoteType = '\0';

	/*
	 * 是否禁用plugin，true : 禁用，默认false
	 */
	bool _isNoPlugin      = false;
	/*
	 * 是否进入不可编辑模式，true : 进入不可编辑模式，默认false
	 */
	bool _isReadOnly      = false;
	/*
	 * 是否加载上一次的会话，true : 不加载上一次的会话，默认false
	 */
	bool _isNoSession     = false;
	/*
	 * 是否允许制表符，true : 不允许，默认false
	 */
	bool _isNoTab         = false;
	/*
	 * 是否以沙盒模式启动，true : 以沙盒模式启动，默认false
	 */
	bool _isPreLaunch     = false;
	/*
	 * 是否显示启动时间，true : 显示启动时间，默认false
	 */
	bool _showLoadingTime = false;
	/*
	 * 窗口是最前显示，true : 窗口最前显示，默认false
	 */
	bool _alwaysOnTop     = false;

	/*
	 * 窗口的x坐标是否有效，true : 有效，默认false
	 */
	bool _isPointXValid = false;
	/*
	 * 窗口的y坐标是否有效，true : 有效，默认false
	 */
	bool _isPointYValid = false;

	/*
	 * 是否指定会话文件，true : 指定会话文件，默认false
	 */
	bool _isSessionFile = false;

	/*
	 * -r
	 */
	bool _isRecursive = false;
	/*
	 * -openFoldersAsWorkspace
	 */
	bool _openFoldersAsWorkspace = false;


	CmdLineParams()
	{
		_point.x = 0;
		_point.y = 0;
	}

	bool isPointValid() const
	{
		return _isPointXValid && _isPointYValid;
	}
};


/****************************************************************************************************
 * A POD class to send CmdLineParams through WM_COPYDATA and to Notepad_plus::loadCommandlineParams *
 * 具体参见：CmdLineParams                                                                          *
 ****************************************************************************************************/
struct CmdLineParamsDTO
{
	LangType _langType = L_EXTERNAL;

	int _line2go = 0;
	int _column2go = 0;
	int _pos2go = 0;

	bool _isReadOnly             = false;
	bool _isNoSession            = false;
	bool _isSessionFile          = false;
	bool _isRecursive            = false;
	bool _openFoldersAsWorkspace = false;

	static CmdLineParamsDTO FromCmdLineParams(const CmdLineParams & params)
	{
		CmdLineParamsDTO dto;
		dto._isReadOnly             = params._isReadOnly;
		dto._isNoSession            = params._isNoSession;
		dto._isSessionFile          = params._isSessionFile;
		dto._isRecursive            = params._isRecursive;
		dto._openFoldersAsWorkspace = params._openFoldersAsWorkspace;

		dto._line2go   = params._line2go;
		dto._column2go = params._column2go;
		dto._pos2go    = params._pos2go;
		
		dto._langType = params._langType;

		return dto;
	}
};


struct FloatingWindowInfo
{
	int _cont;
	RECT _pos;

	FloatingWindowInfo(int cont, int x, int y, int w, int h) : _cont(cont)
	{
		_pos.left	= x;
		_pos.top	= y;
		_pos.right	= w;
		_pos.bottom = h;
	}
};


struct PluginDlgDockingInfo final
{
	generic_string _name;
	int _internalID = -1;

	int _currContainer = -1;
	int _prevContainer = -1;
	bool _isVisible = false;

	PluginDlgDockingInfo
	(
		const TCHAR * pluginName,
		int           id,
		int           curr,
		int           prev,
		bool          isVis
	)
	:
		_internalID(id),
		_currContainer(curr),
		_prevContainer(prev),
		_isVisible(isVis),
		_name(pluginName)
	{
	}

	bool operator == (const PluginDlgDockingInfo& rhs) const
	{
		return _internalID == rhs._internalID and _name == rhs._name;
	}
};


struct ContainerTabInfo final
{
	int _cont = 0;
	int _activeTab = 0;

	ContainerTabInfo(int cont, int activeTab) : _cont(cont), _activeTab(activeTab)
	{
	}
};


struct DockingManagerData final
{
	int _leftWidth   = 200;
	int _rightWidth  = 200;
	int _topHeight   = 200;
	int _bottomHight = 200;

	std::vector<FloatingWindowInfo>   _flaotingWindowInfo;
	std::vector<PluginDlgDockingInfo> _pluginDockInfo;
	std::vector<ContainerTabInfo>     _containerTabInfo;

	bool getFloatingRCFrom(int floatCont, RECT& rc) const
	{
		for (size_t i = 0, fwiLen = _flaotingWindowInfo.size(); i < fwiLen; ++i)
		{
			if (_flaotingWindowInfo[i]._cont == floatCont)
			{
				rc.left   = _flaotingWindowInfo[i]._pos.left;
				rc.top    = _flaotingWindowInfo[i]._pos.top;
				rc.right  = _flaotingWindowInfo[i]._pos.right;
				rc.bottom = _flaotingWindowInfo[i]._pos.bottom;
				return true;
			}
		}
		return false;
	}
};


const int FONTSTYLE_NONE      = 0;
const int FONTSTYLE_BOLD      = 1;
const int FONTSTYLE_ITALIC    = 2;
const int FONTSTYLE_UNDERLINE = 4;


const int STYLE_NOT_USED = -1;


const int COLORSTYLE_FOREGROUND = 0x01;
const int COLORSTYLE_BACKGROUND = 0x02;
const int COLORSTYLE_ALL        = COLORSTYLE_FOREGROUND | COLORSTYLE_BACKGROUND;


struct Style
{
	int _styleID = -1;

	COLORREF _fgColor = COLORREF(STYLE_NOT_USED);
	COLORREF _bgColor = COLORREF(STYLE_NOT_USED);

	int _colorStyle = COLORSTYLE_ALL;

	int _fontStyle  = FONTSTYLE_NONE;
	int _fontSize   = STYLE_NOT_USED;

	int _nesting    = FONTSTYLE_NONE;

	int _keywordClass = STYLE_NOT_USED;

	generic_string * _keywords = nullptr;

	const TCHAR* _fontName = nullptr;

	const TCHAR * _styleDesc = nullptr;

	Style() = default;

	Style(const Style & style)
	{
		_styleID	  = style._styleID;
		_styleDesc    = style._styleDesc;
		_fgColor	  = style._fgColor;
		_bgColor	  = style._bgColor;
		_colorStyle   = style._colorStyle;
		_fontName     = style._fontName;
		_fontSize     = style._fontSize;
		_fontStyle    = style._fontStyle;
		_keywordClass = style._keywordClass;
		_nesting      = style._nesting;
		_keywords     = (style._keywords) ? new generic_string(*(style._keywords)) : nullptr;
	}

	~Style()
	{
		delete _keywords;
	}

	Style& operator = (const Style & style)
	{
		if (this != &style)
		{
			_styleID      = style._styleID;
			_styleDesc    = style._styleDesc;
			_fgColor	  = style._fgColor;
			_bgColor	  = style._bgColor;
			_colorStyle   = style._colorStyle;
			_fontName	  = style._fontName;
			_fontSize	  = style._fontSize;
			_fontStyle    = style._fontStyle;
			_keywordClass = style._keywordClass;
			_nesting	  = style._nesting;

			if (!(_keywords) && style._keywords)
			{
				_keywords = new generic_string(*(style._keywords));
			}
			else if (_keywords && style._keywords)
			{
				_keywords->assign(*(style._keywords));
			}
			else if (_keywords && !(style._keywords))
			{
				delete (_keywords);
				_keywords = nullptr;
			}
		}
		return *this;
	}

	void setKeywords(const TCHAR *str)
	{
		if (!_keywords)
			_keywords = new generic_string(str);
		else
			*_keywords = str;
	}
};


struct GlobalOverride final
{
	bool enableFg        = false;
	bool enableBg        = false;
	bool enableFont      = false;
	bool enableFontSize  = false;
	bool enableBold      = false;
	bool enableItalic    = false;
	bool enableUnderLine = false;
	bool isEnable() const
	{
		return ( ( enableFg ) || ( enableBg ) || ( enableFont ) || ( enableFontSize ) || ( enableBold ) || ( enableItalic ) || ( enableUnderLine ) );
	}
};


struct StyleArray
{
public:
	StyleArray & operator= ( const StyleArray & sa )
	{
		if (this != &sa)
		{
			this->_nbStyler = sa._nbStyler;
			for (int i = 0 ; i < _nbStyler ; ++i)
			{
				this->_styleArray[i] = sa._styleArray[i];
			}
		}
		return *this;
	}

	int getNbStyler() const
	{
		return _nbStyler;
	}

	void setNbStyler(int nb)
	{
		_nbStyler = nb;
	}

	Style & getStyler(size_t index)
	{
		assert(index < SCE_STYLE_ARRAY_SIZE);
		return _styleArray[index];
	}

	bool hasEnoughSpace()
	{
		return (_nbStyler < SCE_STYLE_ARRAY_SIZE);
	}

	void addStyler(int styleID, TiXmlNode * styleNode);

	void addStyler(int styleID, const TCHAR * styleName)
	{
		_styleArray[styleID]._styleID   = styleID;
		_styleArray[styleID]._styleDesc = styleName;
		_styleArray[styleID]._fgColor   = black;
		_styleArray[styleID]._bgColor   = white;
		++_nbStyler;
	}

	int getStylerIndexByID(int id)
	{
		for (int i = 0 ; i < _nbStyler ; ++i)
		{
			if (_styleArray[i]._styleID == id)
			{
				return i;
			}
		}
		return -1;
	}

	int getStylerIndexByName(const TCHAR *name) const
	{
		if (!name)
			return -1;
		for (int i = 0 ; i < _nbStyler ; ++i)
		{
			if (!lstrcmp(_styleArray[i]._styleDesc, name))
				return i;
		}
		return -1;
	}

protected:
	Style _styleArray[SCE_STYLE_ARRAY_SIZE];
	int   _nbStyler = 0;
};


struct LexerStyler : public StyleArray
{
public:
	LexerStyler & operator= (const LexerStyler & ls)
	{
		if (this != &ls)
		{
			*(static_cast<StyleArray *>(this)) = ls;
			this->_lexerName    = ls._lexerName;
			this->_lexerDesc    = ls._lexerDesc;
			this->_lexerUserExt = ls._lexerUserExt;
		}
		return *this;
	}

	void setLexerName(const TCHAR *lexerName)
	{
		_lexerName = lexerName;
	}

	void setLexerDesc(const TCHAR *lexerDesc)
	{
		_lexerDesc = lexerDesc;
	}

	void setLexerUserExt(const TCHAR *lexerUserExt)
	{
		_lexerUserExt = lexerUserExt;
	}

	const TCHAR * getLexerName() const
	{
		return _lexerName.c_str();
	}

	const TCHAR * getLexerDesc() const
	{
		return _lexerDesc.c_str();
	}

	const TCHAR * getLexerUserExt() const
	{
		return _lexerUserExt.c_str();
	}

private :
	generic_string _lexerName;
	generic_string _lexerDesc;
	generic_string _lexerUserExt;
};


const int MAX_LEXER_STYLE = 100;

struct LexerStylerArray
{
public :
	LexerStylerArray() : _nbLexerStyler(0)
	{
	}

	LexerStylerArray & operator=(const LexerStylerArray & lsa)
	{
		if (this != &lsa)
		{
			this->_nbLexerStyler = lsa._nbLexerStyler;
			for (int i = 0; i < this->_nbLexerStyler; ++i)
			{
				this->_lexerStylerArray[i] = lsa._lexerStylerArray[i];
			}
		}
		return *this;
	}

	int getNbLexer() const
	{
		return _nbLexerStyler;
	}

	LexerStyler & getLexerFromIndex(int index)
	{
		return _lexerStylerArray[index];
	}

	const TCHAR * getLexerNameFromIndex(int index) const
	{
		return _lexerStylerArray[index].getLexerName();
	}

	const TCHAR * getLexerDescFromIndex(int index) const
	{
		return _lexerStylerArray[index].getLexerDesc();
	}

	LexerStyler * getLexerStylerByName(const TCHAR *lexerName)
	{
		if (!lexerName)
		{
			return NULL;
		}
		for (int i = 0 ; i < _nbLexerStyler ; ++i)
		{
			if (!lstrcmp(_lexerStylerArray[i].getLexerName(), lexerName))
			{
				return &(_lexerStylerArray[i]);
			}
		}
		return NULL;
	}
	bool hasEnoughSpace()
	{
		return (_nbLexerStyler < MAX_LEXER_STYLE);
	}

	void addLexerStyler(const TCHAR *lexerName, const TCHAR *lexerDesc, const TCHAR *lexerUserExt, TiXmlNode *lexerNode);

	void eraseAll();
private :
	LexerStyler _lexerStylerArray[MAX_LEXER_STYLE];
	int _nbLexerStyler;
};


struct NewDocDefaultSettings final
{
	// -1 when not using
	int      _codepage       = -1;
	EolType  _format         = EolType::osdefault;
	UniMode  _unicodeMode    = uniCookie;
	LangType _lang           = L_TEXT;
	bool     _openAnsiAsUtf8 = true;
};


struct LangMenuItem final
{
	LangType       _langType;
	int            _cmdID;
	generic_string _langName;

	LangMenuItem
	(
		LangType lt,
		int cmdID = 0,
		const generic_string& langName = TEXT("")
	)
	:
		_langType(lt),
		_cmdID(cmdID),
		_langName(langName)
	{
	}
};


struct PrintSettings final
{
	RECT _marge;

	int _printOption = SC_PRINT_COLOURONWHITE;

	int _headerFontStyle = 0;
	int _headerFontSize = 0;

	int _footerFontStyle = 0;
	int _footerFontSize = 0;

	generic_string _headerLeft;
	generic_string _headerMiddle;
	generic_string _headerRight;
	generic_string _headerFontName;

	generic_string _footerLeft;
	generic_string _footerMiddle;
	generic_string _footerRight;
	generic_string _footerFontName;

	bool _printLineNumber = true;


	PrintSettings()
	{
		_marge.left   = 0;
		_marge.top    = 0;
		_marge.right  = 0;
		_marge.bottom = 0;
	}

	bool isHeaderPresent() const
	{
		return ( ( _headerLeft != TEXT("") ) || ( _headerMiddle != TEXT("") ) || ( _headerRight != TEXT("") ) );
	}

	bool isFooterPresent() const
	{
		return ( ( _footerLeft != TEXT("") ) || ( _footerMiddle != TEXT("") ) || ( _footerRight != TEXT("") ) );
	}

	bool isUserMargePresent() const
	{
		return ( (_marge.left != 0) || (_marge.top != 0) || (_marge.right != 0) || (_marge.bottom != 0) );
	}
};


class Date final
{
public:
	Date() = default;
	Date
	(
		unsigned long year,
		unsigned long month,
		unsigned long day
	)
	:
		_year(year),
		_month(month),
		_day(day)
	{
		// I don't think Notepad++ will last till AD 10000 :)
		assert(year > 0 && year <= 9999);
		assert(month > 0 && month <= 12);
		assert(day > 0 && day <= 31);
		assert(!(month == 2 && day > 29) &&
			   !(month == 4 && day > 30) &&
			   !(month == 6 && day > 30) &&
			   !(month == 9 && day > 30) &&
			   !(month == 11 && day > 30));
	}

	explicit Date(const TCHAR * dateStr);

	/***************************************************************************
	 * The constructor which makes the date of number of days from now         *
	 * nbDaysFromNow could be negative if user want to make a date in the past *
	 * if the value of nbDaysFromNow is 0 then the date will be now            *
	 ***************************************************************************/
	Date(int nbDaysFromNow);

	void now();

	/*******************************************
	 * Return Notepad++ date format : YYYYMMDD *
	 *******************************************/
	generic_string toString() const
	{
		TCHAR dateStr[16];
		wsprintf(dateStr, TEXT("%04u%02u%02u"), _year, _month, _day);
		return dateStr;
	}

	bool operator < (const Date & compare) const
	{
		if (this->_year != compare._year)
		{
			return (this->_year < compare._year);
		}
		if (this->_month != compare._month)
		{
			return (this->_month < compare._month);
		}
		return (this->_day < compare._day);
	}

	bool operator > (const Date & compare) const
	{
		if (this->_year != compare._year)
		{
			return (this->_year > compare._year);
		}
		if (this->_month != compare._month)
		{
			return (this->_month > compare._month);
		}
		return (this->_day > compare._day);
	}

	bool operator == (const Date & compare) const
	{
		if (this->_year != compare._year)
		{
			return false;
		}
		if (this->_month != compare._month)
		{
			return false;
		}
		return (this->_day == compare._day);
	}

	bool operator != (const Date & compare) const
	{
		if (this->_year != compare._year)
		{
			return true;
		}
		if (this->_month != compare._month)
		{
			return true;
		}
		return (this->_day != compare._day);
	}

private:
	unsigned long _year  = 2008;
	unsigned long _month = 4;
	unsigned long _day   = 26;
};


class MatchedPairConf final
{
public:
	bool hasUserDefinedPairs() const
	{
		return _matchedPairs.size() != 0;
	}

	bool hasDefaultPairs() const
	{
		return ( _doParentheses ) || ( _doBrackets ) || ( _doCurlyBrackets ) || ( _doQuotes ) || ( _doDoubleQuotes ) || ( _doHtmlXmlTag );
	}

	bool hasAnyPairsPair() const
	{
		return hasUserDefinedPairs() || hasDefaultPairs();
	}

public:
	std::vector<std::pair<char, char>> _matchedPairs;
	std::vector<std::pair<char, char>> _matchedPairsInit; // used only on init
	bool _doHtmlXmlTag = false;
	bool _doParentheses = false;
	bool _doBrackets = false;
	bool _doCurlyBrackets = false;
	bool _doQuotes = false;
	bool _doDoubleQuotes = false;
};


/********************
 * 界面显示相关参数 *
 ********************/
struct NppGUI final
{
public:
	NppGUI()
	{
		_appPos.left   = 0;
		_appPos.top    = 0;
		_appPos.right  = 1100;
		_appPos.bottom = 700;

		_defaultDir[0]    = 0;
		_defaultDirExp[0] = 0;
	}

public:
	/*
	 * 新文档的默认设置
	 */
	NewDocDefaultSettings _newDocDefaultSettings;

	RECT _appPos;

	size_t _snapshotBackupTiming = 7000;

	enum AutocStatus { autoc_none, autoc_func, autoc_word, autoc_both };
	size_t             _autocFromLen   = 1;
	PrintSettings      _printSettings;
	BackupFeature      _backup         = bak_none;
	DockingManagerData _dockingData;
	GlobalOverride     _globalOverride;
	AutocStatus        _autocStatus    = autoc_both;
	MatchedPairConf    _matchedPairConf;

public:
	/*
	 * 自动更新对象
	 */
	struct AutoUpdateOptions
	{
		/*
		 * 下次更新的日期
		 */
		Date _nextUpdateDate;
		int _intervalDays = 15;
		/*
		 * 是否自动更新
		 */
		bool _doAutoUpdate = true;
		AutoUpdateOptions() : _nextUpdateDate(Date())
		{
		}
	}
	_autoUpdateOpt;

	toolBarStatusType _toolBarStatus = TB_STANDARD;

	std::vector<LangMenuItem> _excludedLangList;

public:
	/*********************************
	 * 显示状态栏的方式              *
	 *********************************/
	/*********************************
	 * 1st bit : draw top bar;       *
	 * 2nd bit : draw inactive tabs  *
	 * 3rd bit : enable drag & drop  *
	 * 4th bit : reduce the height   *
	 * 5th bit : enable vertical     *
	 * 6th bit : enable multiline    *
	 *                               *
	 * 0:don't draw;                 *
	 * 1:draw top bar                *
	 * 2:draw inactive tabs          *
	 * 3:draw both                   *
	 * 7:draw both + drag&drop       *
	 ********************************/
	int _tabStatus = (TAB_DRAWTOPBAR | TAB_DRAWINACTIVETAB | TAB_DRAGNDROP | TAB_REDUCE | TAB_CLOSEBUTTON);

	int _userDefineDlgStatus = UDD_DOCKED;

	/********************************
	 * TAB的长度                    *
	 ********************************/
	int _tabSize             = 4;

	int _fileAutoDetection = cdEnabledNew;

public:
	/****************************
	 * 0 : do nothing           *
	 * 1 : don't draw underline *
	 * 2 : draw underline       *
	 ****************************/
	int _styleURL = 2;

	MultiInstSetting _multiInstSetting = monoInst;

	OpenSaveDirSetting _openSaveDir    = dir_followCurrent;

	enum SearchEngineChoice { se_custom = 0, se_duckDuckGo = 1, se_google = 2, se_bing = 3, se_yahoo = 4, se_stackoverflow = 5 };
	SearchEngineChoice _searchEngineChoice = se_google;

	int _caretBlinkRate = 600;
	int _caretWidth     = 1;

public:
	std::string _customWordChars;

	generic_string _definedSessionExt;
	generic_string _definedWorkspaceExt;

	generic_string _themeName;

	// this option will never be read/written from/to config.xml
	generic_string _cloudPath;

	generic_string _searchEngineCustom;

	generic_string     _backupDir;

public:
	TCHAR _defaultDir[MAX_PATH];

	//expanded environment variables
	TCHAR _defaultDirExp[MAX_PATH];

	// this option will never be read/written from/to config.xml
	unsigned char _availableClouds = '\0';

public:
	char _leftmostDelimiter = '(';
	char _rightmostDelimiter = ')';

public:
	/******************
	* 是否显示工具栏 *
	******************/
	bool _toolbarShow = true;
	/******************
	* 是否显示状态栏 *
	******************/
	bool _statusBarShow = true;
	/******************
	* 是否显示菜单栏 *
	******************/
	bool _menuBarShow = true;

public:
	bool _splitterPos = POS_VERTICAL;

public:
	bool _tabReplacedBySpace = false;

public:
	bool _checkHistoryFiles = false;

public:
	bool _isMaximized       = false;
	bool _isMinimizedToTray = false;

public:
	/*****************************************************************
	 * remember next session boolean will be written in the settings *
	 *****************************************************************/
	bool _rememberLastSession = true;

	/**********************************************************
	 * used for if -nosession is indicated on the launch time *
	 **********************************************************/
	bool _isCmdlineNosessionActivated = false;
	bool _detectEncoding = true;
	bool _doTaskList = true;
	bool _maitainIndent = true;
	bool _enableSmartHilite = true;

public:
	bool _smartHiliteCaseSensitive = false;
	bool _smartHiliteWordOnly = true;
	bool _smartHiliteUseFindSettings = false;
	bool _smartHiliteOnAnotherView = false;

public:
	bool _disableSmartHiliteTmp = false;
	bool _enableTagsMatchHilite = true;
	bool _enableTagAttrsHilite = true;
	bool _enableHiliteNonHTMLZone = false;
	bool _styleMRU = true;

public:
	bool _delimiterSelectionOnEntireDocument = false;
	bool _backSlashIsEscapeCharacterForSql = true;

public:
	bool _isWordCharDefault = true;

public:
	/**************************************
	 * 主菜单中的语言菜单是否分类进行展示 *
	 * 值为真：分类进行展式               *
	**************************************/
	bool _isLangMenuCompact = true;

public:
	bool _useDir = false;

public:
	bool _autocIgnoreNumbers = true;

public:
	bool _funcParams = true;

public:
	bool _doesExistUpdater = false;

public:
	bool _shortTitlebar = false;

public:
	bool _enableMultiSelection = false;

public:
	bool _fileSwitcherWithoutExtColumn = false;

public:
	bool _isSnapshotMode = true;

public:
	bool _useNewStyleSaveDlg = true;

public:
	bool _isFolderDroppedOpenFiles = false;

public:
	bool _isDocPeekOnTab = false;
	bool _isDocPeekOnMap = false;

public:
	bool isSnapshotMode() const
	{
		return _isSnapshotMode && _rememberLastSession && !_isCmdlineNosessionActivated;
	}

	void setTabReplacedBySpace(bool b)
	{
		_tabReplacedBySpace = b;
	}

	const NewDocDefaultSettings & getNewDocDefaultSettings() const
	{
		return _newDocDefaultSettings;
	}

};


struct ScintillaViewParams
{
	//"simple", "arrow", "circle", "box" and "none"
	lineWrapMethod _lineWrapMethod = LINEWRAP_ALIGNED;

	folderStyle  _folderStyle = FOLDER_STYLE_BOX;

	int _edgeMode     = EDGE_NONE;
	int _edgeNbColumn = 80;
	int _zoom         = 0;
	int _zoom2        = 0;

	int _borderWidth = 2;

	bool _lineNumberMarginShow     = true;
	bool _bookMarkMarginShow       = true;
	bool _foldMarginShow           = true;
	bool _indentGuideLineShow      = true;
	bool _currentLineHilitingShow  = true;
	bool _wrapSymbolShow           = false;
	bool _doWrap                   = false;
	bool _whiteSpaceShow           = false;
	bool _eolShow                  = false;
	bool _scrollBeyondLastLine     = false;
	bool _disableAdvancedScrolling = false;
	bool _doSmoothFont             = false;
	bool _showBorderEdge           = true;
};


const int NB_LIST              = 20;
const int NB_MAX_LRF_FILE      = 30;
const int NB_MAX_USER_LANG     = 30;
const int NB_MAX_EXTERNAL_LANG = 30;
const int NB_MAX_IMPORTED_UDL  = 50;

const int NB_MAX_FINDHISTORY_FIND    = 30;
const int NB_MAX_FINDHISTORY_REPLACE = 30;
const int NB_MAX_FINDHISTORY_PATH    = 30;
const int NB_MAX_FINDHISTORY_FILTER  = 20;


const int MASK_ReplaceBySpc = 0x80;
const int MASK_TabSize      = 0x7F;


struct Lang final
{
	LangType         _langID                   = L_TEXT;

	int _tabSize                               = -1;

	generic_string   _langName;
	const TCHAR    * _defaultExtList           = nullptr;
	const TCHAR    * _langKeyWordList[NB_LIST];
	const TCHAR    * _pCommentLineSymbol       = nullptr;
	const TCHAR    * _pCommentStart            = nullptr;
	const TCHAR    * _pCommentEnd              = nullptr;

	bool             _isTabReplacedBySpace     = false;

	Lang()
	{
		for (int i = 0 ; i < NB_LIST ; _langKeyWordList[i] = NULL, ++i);
	}

	Lang(LangType langID, const TCHAR *name) : _langID(langID), _langName(name ? name : TEXT(""))
	{
		for (int i = 0 ; i < NB_LIST ; _langKeyWordList[i] = NULL, ++i);
	}

	~Lang() = default;

	void setDefaultExtList(const TCHAR *extLst)
	{
		_defaultExtList = extLst;
	}

	void setCommentLineSymbol(const TCHAR *commentLine)
	{
		_pCommentLineSymbol = commentLine;
	}

	void setCommentStart(const TCHAR *commentStart)
	{
		_pCommentStart = commentStart;
	}

	void setCommentEnd(const TCHAR *commentEnd)
	{
		_pCommentEnd = commentEnd;
	}

	void setTabInfo(int tabInfo)
	{
		if (tabInfo != -1 && tabInfo & MASK_TabSize)
		{
			_isTabReplacedBySpace = (tabInfo & MASK_ReplaceBySpc) != 0;
			_tabSize = tabInfo & MASK_TabSize;
		}
	}

	const TCHAR * getDefaultExtList() const
	{
		return _defaultExtList;
	}

	void setWords(const TCHAR * words, int index)
	{
		_langKeyWordList[index] = words;
	}

	const TCHAR * getWords(int index) const
	{
		return _langKeyWordList[index];
	}

	LangType getLangID() const
	{
		return _langID;
	}

	const TCHAR * getLangName() const
	{
		return _langName.c_str();
	}

	int getTabInfo() const
	{
		if (_tabSize == -1) return -1;
		return (_isTabReplacedBySpace?0x80:0x00) | _tabSize;
	}
};


class UserLangContainer final
{
public:
	UserLangContainer() :_name(TEXT("new user define")), _ext(TEXT("")), _udlVersion(TEXT(""))
	{
		init();
	}

	UserLangContainer(const TCHAR *name, const TCHAR *ext, const TCHAR *udlVer) : _name(name), _ext(ext), _udlVersion(udlVer)
	{
		init();
	}

	UserLangContainer & operator = (const UserLangContainer & ulc)
	{
		if (this != &ulc)
		{
			this->_name                = ulc._name;
			this->_ext                 = ulc._ext;
			this->_udlVersion          = ulc._udlVersion;
			this->_isCaseIgnored       = ulc._isCaseIgnored;
			this->_styleArray          = ulc._styleArray;
			this->_allowFoldOfComments = ulc._allowFoldOfComments;
			this->_forcePureLC         = ulc._forcePureLC;
			this->_decimalSeparator    = ulc._decimalSeparator;
			this->_foldCompact         = ulc._foldCompact;

			int nbStyler               = this->_styleArray.getNbStyler();

			for (int i = 0 ; i < nbStyler ; ++i)
			{
				Style & st = this->_styleArray.getStyler(i);
				if (st._bgColor == COLORREF(-1))
				{
					st._bgColor = white;
				}
				if (st._fgColor == COLORREF(-1))
				{
					st._fgColor = black;
				}
			}

			for (int i = 0; i < SCE_USER_KWLIST_TOTAL; ++i)
			{
				wcscpy_s(this->_keywordLists[i], ulc._keywordLists[i]);
			}

			for (int i = 0; i < SCE_USER_TOTAL_KEYWORD_GROUPS; ++i)
			{
				_isPrefix[i] = ulc._isPrefix[i];
			}
		}
		return *this;
	}

	const TCHAR * getName()
	{
		return _name.c_str();
	}

	const TCHAR * getExtention()
	{
		return _ext.c_str();
	}

	const TCHAR * getUdlVersion()
	{
		return _udlVersion.c_str();
	}

private:
	StyleArray _styleArray;
	generic_string _name;
	generic_string _ext;
	generic_string _udlVersion;

	TCHAR _keywordLists[SCE_USER_KWLIST_TOTAL][max_char];
	bool _isPrefix[SCE_USER_TOTAL_KEYWORD_GROUPS];

	bool _isCaseIgnored;
	bool _allowFoldOfComments;
	int  _forcePureLC;
	int _decimalSeparator;
	bool _foldCompact;

	// nakama zone
	friend class Notepad_plus;
	friend class ScintillaEditView;
	friend class NppParameters;

	friend class SharedParametersDialog;
	friend class FolderStyleDialog;
	friend class KeyWordsStyleDialog;
	friend class CommentStyleDialog;
	friend class SymbolsStyleDialog;
	friend class UserDefineDialog;
	friend class StylerDlg;

	void init()
	{
		_forcePureLC         = PURE_LC_NONE;
		_decimalSeparator    = DECSEP_DOT;
		_foldCompact         = false;
		_isCaseIgnored       = false;
		_allowFoldOfComments = false;

		for (int i = 0; i < SCE_USER_KWLIST_TOTAL; ++i)
		{
			*_keywordLists[i] = '\0';
		}

		for (int i = 0; i < SCE_USER_TOTAL_KEYWORD_GROUPS; ++i)
		{
			_isPrefix[i] = false;
		}
	}
};

#define MAX_EXTERNAL_LEXER_NAME_LEN 16
#define MAX_EXTERNAL_LEXER_DESC_LEN 32


class ExternalLangContainer final
{
public:
	TCHAR _name[MAX_EXTERNAL_LEXER_NAME_LEN];
	TCHAR _desc[MAX_EXTERNAL_LEXER_DESC_LEN];

	ExternalLangContainer(const TCHAR* name, const TCHAR* desc)
	{
		generic_strncpy(_name, name, MAX_EXTERNAL_LEXER_NAME_LEN);
		generic_strncpy(_desc, desc, MAX_EXTERNAL_LEXER_DESC_LEN);
	}
};


struct FindHistory final
{
	enum searchMode
	{
		normal,
		extended,
		regExpr
	};
	enum transparencyMode
	{
		none,
		onLossingFocus,
		persistant
	};

	searchMode       _searchMode       = normal;
	transparencyMode _transparencyMode = onLossingFocus;
	int              _transparency     = 150;

	int _nbMaxFindHistoryPath    = 10;
	int _nbMaxFindHistoryFilter  = 10;
	int _nbMaxFindHistoryFind    = 10;
	int _nbMaxFindHistoryReplace = 10;

	std::vector<generic_string> _findHistoryPaths;
	std::vector<generic_string> _findHistoryFilters;
	std::vector<generic_string> _findHistoryFinds;
	std::vector<generic_string> _findHistoryReplaces;

	bool _isSearch2ButtonsMode = false;

	bool _isMatchWord       = false;
	bool _isMatchCase       = false;
	bool _isWrap            = true;
	bool _isDirectionDown   = true;
	bool _dotMatchesNewline = false;

	bool _isFifRecuisive      = true;
	bool _isFifInHiddenFolder = false;

	bool _isDlgAlwaysVisible = false;
	bool _isFilterFollowDoc = false;
	bool _isFolderFollowDoc = false;
};



class LocalizationSwitcher final
{
friend class NppParameters;
public:
	struct LocalizationDefinition
	{
		const wchar_t *_langName;
		const wchar_t *_xmlFileName;
	};

	bool addLanguageFromXml(const std::wstring & xmlFullPath);

	std::wstring getLangFromXmlFileName(const wchar_t * fn) const;

	std::wstring getXmlFilePathFromLangName(const wchar_t * langName) const;

	bool switchToLang(const wchar_t * lang2switch) const;

	size_t size() const
	{
		return _localizationList.size();
	}

	std::pair<std::wstring, std::wstring> getElementFromIndex(size_t index) const
	{
		if (index >= _localizationList.size())
			return std::pair<std::wstring, std::wstring>(std::wstring(), std::wstring());
		return _localizationList[index];
	}

	void setFileName(const char *fn)
	{
		if (fn)
			_fileName = fn;
	}

	std::string getFileName() const
	{
		return _fileName;
	}

private:
	std::vector< std::pair< std::wstring, std::wstring > > _localizationList;
	std::wstring _nativeLangPath;
	std::string _fileName;
};


class ThemeSwitcher final
{
friend class NppParameters;

public:
	void addThemeFromXml(const generic_string& xmlFullPath)
	{
		_themeList.push_back(std::pair<generic_string, generic_string>(getThemeFromXmlFileName(xmlFullPath.c_str()), xmlFullPath));
	}

	void addDefaultThemeFromXml(const generic_string & xmlFullPath)
	{
		_themeList.push_back(std::pair<generic_string, generic_string>(TEXT("Default (stylers.xml)"), xmlFullPath));
	}

	generic_string getThemeFromXmlFileName(const TCHAR * fn) const;

	generic_string getXmlFilePathFromThemeName(const TCHAR * themeName) const
	{
		if ( ( !themeName ) || ( themeName[0] ) )
		{
			return generic_string();
		}
		generic_string themePath = _stylesXmlPath;
		return themePath;
	}

	bool themeNameExists(const TCHAR *themeName)
	{
		for (size_t i = 0; i < _themeList.size(); ++i )
		{
			if (!(getElementFromIndex(i)).first.compare(themeName))
			{
				return true;
			}
		}
		return false;
	}

	size_t size() const
	{
		return _themeList.size();
	}


	std::pair<generic_string, generic_string> & getElementFromIndex(size_t index)
	{
		assert(index < _themeList.size());
		return _themeList[index];
	}

private:
	std::vector<std::pair<generic_string, generic_string>> _themeList;
	generic_string _stylesXmlPath;
};


class PluginList final
{
public :
	void add(generic_string fn, bool isInBL)
	{
		_list.push_back(std::pair<generic_string, bool>(fn, isInBL));
	}

private:
	std::vector<std::pair<generic_string, bool>>_list;
};


struct UdlXmlFileState final
{
	TiXmlDocument                           * _udlXmlDoc = nullptr;
	std::pair<unsigned char, unsigned char>   _indexRange;
	bool                                      _isDirty   = false;

	UdlXmlFileState(TiXmlDocument* doc, bool isDirty, std::pair<unsigned char, unsigned char> range) : _udlXmlDoc(doc), _isDirty(isDirty), _indexRange(range) {}
};

const int  NB_LANG = 100;
const bool DUP     = true;
const bool FREE     = false;

const int RECENTFILES_SHOWFULLPATH     = -1;
const int RECENTFILES_SHOWONLYFILENAME = 0;

/*************************************************************************
 * final 修饰符该类不允许有派生类，该类是单例模式，用于存储NPP的相关参数 *
 *************************************************************************/
class NppParameters final
{
public:
	/**********************
	 * 返回单例实例的地址 *
	 **********************/
	static NppParameters * getInstance()
	{
		static NppParameters instance;
		return &instance;
	}

	static LangType getLangIDFromStr(const TCHAR * langName);

	static generic_string getLocPathFromStr(const generic_string & localizationCode);

	/*******************
	 * 加载XML配置文件 *
	 *******************/
	bool load();

	bool reloadLang();

	bool reloadStylers(TCHAR *stylePath = nullptr);

	void destroyInstance();

	generic_string getSettingsFolder();

	/*
	 * @member
	 */
	int L_END;

	/*
	 * @member
	 */
	bool _isTaskListRBUTTONUP_Active = false;

	const NppGUI & getNppGUI() const
	{
		return _nppGUI;
	}

	const TCHAR * getWordList(LangType langID, int typeIndex) const
	{
		Lang *pLang = getLangFromID(langID);
		if (!pLang)
		{
			return nullptr;
		}

		return pLang->getWords(typeIndex);
	}


	Lang * getLangFromID(LangType langID) const
	{
		for (int i = 0 ; i < _nbLang ; ++i)
		{
			if (_langList[i] && _langList[i]->_langID == langID)
			{
				return _langList[i];
			}
		}
		return nullptr;
	}

	Lang * getLangFromIndex(size_t i) const {
		return (i < size_t(_nbLang)) ? _langList[i] : nullptr;
	}

	int getNbLang() const
	{
		return _nbLang;
	}

	LangType getLangFromExt(const TCHAR *ext);

	const TCHAR * getLangExtFromName(const TCHAR *langName) const
	{
		for (int i = 0 ; i < _nbLang ; ++i)
		{
			if (_langList[i]->_langName == langName)
			{
				return _langList[i]->_defaultExtList;
			}
		}
		return nullptr;
	}

	const TCHAR * getLangExtFromLangType(LangType langType) const
	{
		for (int i = 0 ; i < _nbLang ; ++i)
		{
			if (_langList[i]->_langID == langType)
			{
				return _langList[i]->_defaultExtList;
			}
		}
		return nullptr;
	}

	int getNbLRFile() const
	{
		return _nbRecentFile;
	}

	generic_string * getLRFile(int index) const
	{
		return _LRFileList[index];
	}

	void setNbMaxRecentFile(int nb)
	{
		_nbMaxRecentFile = nb;
	}

	int getNbMaxRecentFile() const
	{
		return _nbMaxRecentFile;
	}

	void setPutRecentFileInSubMenu(bool doSubmenu)
	{
		_putRecentFileInSubMenu = doSubmenu;
	}

	bool putRecentFileInSubMenu() const
	{
		return _putRecentFileInSubMenu;
	}

	void setRecentFileCustomLength(int len)
	{
		_recentFileCustomLength = len;
	}

	int getRecentFileCustomLength() const
	{
		return _recentFileCustomLength;
	}

	const ScintillaViewParams& getSVP() const
	{
		return _svp;
	}

	bool writeRecentFileHistorySettings(int nbMaxFile = -1) const;

	bool writeHistory(const TCHAR *fullpath);

	/***********************************************************************************************************************/

	bool writeProjectPanelsSettings() const;

	bool writeFileBrowserSettings(const std::vector<generic_string> & rootPath, const generic_string & latestSelectedItemPath) const;

	/***********************************************************************************************************************/

	TiXmlNode* getChildElementByAttribut(TiXmlNode *pere, const TCHAR *childName, const TCHAR *attributName, const TCHAR *attributVal) const;

	bool writeScintillaParams();

	void createXmlTreeFromGUIParams();

	void writeStyles(LexerStylerArray & lexersStylers, StyleArray & globalStylers);

	bool insertTabInfo(const TCHAR *langName, int tabInfo);

	LexerStylerArray & getLStylerArray()
	{
		return _lexerStylerArray;
	}

	StyleArray & getGlobalStylers()
	{
		return _widgetStyleArray;
	}

	StyleArray & getMiscStylerArray()
	{
		return _widgetStyleArray;
	}

	GlobalOverride & getGlobalOverrideStyle()
	{
		return _nppGUI._globalOverride;
	}

	COLORREF getCurLineHilitingColour();

	void setCurLineHilitingColour(COLORREF colour2Set);

	void setFontList(HWND hWnd);

	bool isInFontList(const generic_string& fontName2Search) const;

	const std::vector<generic_string>& getFontList() const
	{
		return _fontlist;
	}

	int getNbUserLang() const
	{
		return _nbUserLang;
	}

	UserLangContainer & getULCFromIndex(size_t i)
	{
		return *_userLangArray[i];
	}

	UserLangContainer * getULCFromName(const TCHAR *userLangName);

	int getNbExternalLang() const
	{
		return _nbExternalLang;
	}

	int getExternalLangIndexFromName(const TCHAR *externalLangName) const;

	ExternalLangContainer & getELCFromIndex(int i)
	{
		return *_externalLangArray[i];
	}

	bool ExternalLangHasRoom() const
	{
		return _nbExternalLang < NB_MAX_EXTERNAL_LANG;
	}

	void getExternalLexerFromXmlTree(TiXmlDocument *doc);

	std::vector<TiXmlDocument *> * getExternalLexerDoc()
	{
		return &_pXmlExternalLexerDoc;
	}

	void writeDefaultUDL();

	void writeNonDefaultUDL();

	void writeNeed2SaveUDL();

	void writeShortcuts();

	void writeSession(const Session & session, const TCHAR *fileName = NULL);

	bool writeFindHistory();

	bool isExistingUserLangName(const TCHAR *newName) const
	{
		if ((!newName) || (!newName[0]))
		{
			return true;
		}

		for (int i = 0 ; i < _nbUserLang ; ++i)
		{
			if (!lstrcmp(_userLangArray[i]->_name.c_str(), newName))
			{
				return true;
			}
		}
		return false;
	}

	const TCHAR * getUserDefinedLangNameFromExt(TCHAR *ext, TCHAR *fullName) const;

	int addUserLangToEnd(const UserLangContainer & userLang, const TCHAR *newName);

	void removeUserLang(size_t index);

	bool isExistingExternalLangName(const TCHAR *newName) const;

	int addExternalLangToEnd(ExternalLangContainer * externalLang);

	TiXmlDocumentA * getNativeLangA() const
	{
		return _pXmlNativeLangDocA;
	}

	TiXmlDocument * getToolIcons() const
	{
		return _pXmlToolIconsDoc;
	}

	bool isTransparentAvailable() const
	{
		return (_transparentFuncAddr != NULL);
	}

	/**********************************
	 * 0 <= percent < 256             *
	 * if (percent == 255) then opacq *
	 **********************************/
	void SetTransparent(HWND hwnd, int percent);

	void removeTransparent(HWND hwnd);

	void setCmdlineParam(const CmdLineParamsDTO & cmdLineParams)
	{
		_cmdLineParams = cmdLineParams;
	}

	const CmdLineParamsDTO & getCmdLineParams() const
	{
		return _cmdLineParams;
	}

	void setFileSaveDlgFilterIndex(int ln)
	{
		_fileSaveDlgFilterIndex = ln;
	}

	int getFileSaveDlgFilterIndex() const
	{
		return _fileSaveDlgFilterIndex;
	}

	bool isRemappingShortcut() const
	{
		return _shortcuts.size() != 0;
	}

	std::vector<CommandShortcut> & getUserShortcuts()
	{
		return _shortcuts;
	}

	std::vector<size_t> & getUserModifiedShortcuts()
	{
		return _customizedShortcuts;
	}

	void addUserModifiedIndex(size_t index);

	std::vector<MacroShortcut> & getMacroList()
	{
		return _macros;
	}

	std::vector<UserCommand> & getUserCommandList()
	{
		return _userCommands;
	}

	std::vector<PluginCmdShortcut> & getPluginCommandList()
	{
		return _pluginCommands;
	}

	std::vector<size_t> & getPluginModifiedKeyIndices()
	{
		return _pluginCustomizedCmds;
	}

	void addPluginModifiedIndex(size_t index);

	std::vector<ScintillaKeyMap> & getScintillaKeyList()
	{
		return _scintillaKeyCommands;
	}

	std::vector<int> & getScintillaModifiedKeyIndices()
	{
		return _scintillaModifiedKeyIndices;
	}

	void addScintillaModifiedIndex(int index);

	std::vector<MenuItemUnit> & getContextMenuItems()
	{
		return _contextMenuItems;
	}

	const Session & getSession() const
	{
		return _session;
	}

	bool hasCustomContextMenu() const
	{
		return !_contextMenuItems.empty();
	}

	/***********************************************************************************************************************/

	void setAccelerator(Accelerator *pAccel)
	{
		_pAccelerator = pAccel;
	}

	Accelerator * getAccelerator()
	{
		return _pAccelerator;
	}

	void setScintillaAccelerator(ScintillaAccelerator *pScintAccel)
	{
		_pScintAccelerator = pScintAccel;
	}

	ScintillaAccelerator * getScintillaAccelerator()
	{
		return _pScintAccelerator;
	}

	/***********************************************************************************************************************/

	generic_string getNppPath() const
	{
		return _nppPath;
	}

	generic_string getContextMenuPath() const
	{
		return _contextMenuPath;
	}

	const TCHAR * getAppDataNppDir() const
	{
		return _appdataNppDir.c_str();
	}

	const TCHAR * getPluginRootDir() const
	{
		return _pluginRootDir.c_str();
	}

	const TCHAR * getPluginConfDir() const
	{
		return _pluginConfDir.c_str();
	}
	const TCHAR * getUserPluginConfDir() const
	{
		return _userPluginConfDir.c_str();
	}

	const TCHAR * getWorkingDir() const
	{
		return _currentDirectory.c_str();
	}
	const TCHAR * getWorkSpaceFilePath(int i) const
	{
		if (i < 0 || i > 2) return nullptr;
		return _workSpaceFilePathes[i].c_str();
	}

	/***********************************************************************************************************************/

	const std::vector<generic_string> getFileBrowserRoots() const
	{
		return _fileBrowserRoot;
	}

	void setWorkSpaceFilePath(int i, const TCHAR * wsFile);

	/***********************************************************************************************************************/

	void setWorkingDir(const TCHAR * newPath);

	void setStartWithLocFileName(const generic_string& locPath)
	{
		_startWithLocFileName = locPath;
	}

	void setFunctionListExportBoolean(bool doIt)
	{
		_doFunctionListExport = doIt;
	}

	bool doFunctionListExport() const
	{
		return _doFunctionListExport;
	}

	/***********************************************************************************************************************/

	void setPrintAndExitBoolean(bool doIt)
	{
		_doPrintAndExit = doIt;
	}

	bool doPrintAndExit() const
	{
		return _doPrintAndExit;
	}

	/***********************************************************************************************************************/

	bool loadSession(Session & session, const TCHAR *sessionFileName);

	int langTypeToCommandID(LangType lt) const;

	WNDPROC getEnableThemeDlgTexture() const
	{
		return _enableThemeDialogTextureFuncAddr;
	}

	struct FindDlgTabTitiles final
	{
		generic_string _find;
		generic_string _replace;
		generic_string _findInFiles;
		generic_string _mark;
	};

	FindDlgTabTitiles & getFindDlgTabTitiles()
	{
		return _findDlgTabTitiles;
	}

	bool asNotepadStyle() const
	{
		return _asNotepadStyle;
	}

	bool reloadPluginCmds()
	{
		return getPluginCmdsFromXmlTree();
	}

	/***********************************************************************************************************************/

	bool getContextMenuFromXmlTree(HMENU mainMenuHadle, HMENU pluginsMenu);

	bool reloadContextMenuFromXmlTree(HMENU mainMenuHadle, HMENU pluginsMenu);

	winVer getWinVersion() const
	{
		return _winVersion;
	}

	generic_string getWinVersionStr() const;

	generic_string getWinVerBitStr() const;

	FindHistory & getFindHistory()
	{
		return _findHistory;
	}

	/*
	 * @member
	 */
	// an on the fly variable for find/replace functions
	bool _isFindReplacing = false;

	void safeWow64EnableWow64FsRedirection(BOOL Wow64FsEnableRedirection);

	/***********************************************************************************************************************/

	LocalizationSwitcher & getLocalizationSwitcher()
	{
		return _localizationSwitcher;
	}

	ThemeSwitcher & getThemeSwitcher()
	{
		return _themeSwitcher;
	}

	/***********************************************************************************************************************/

	std::vector<generic_string> & getBlackList()
	{
		return _blacklist;
	}

	bool isInBlackList(TCHAR *fn) const
	{
		for (auto& element: _blacklist)
		{
			if (element == fn)
				return true;
		}
		return false;
	}

	/***********************************************************************************************************************/

	PluginList & getPluginList()
	{
		return _pluginList;
	}

	bool importUDLFromFile(const generic_string& sourceFile);

	bool exportUDLToFile(size_t langIndex2export, const generic_string& fileName2save);

	NativeLangSpeaker* getNativeLangSpeaker()
	{
		return _pNativeLangSpeaker;
	}
	void setNativeLangSpeaker(NativeLangSpeaker * nls)
	{
		_pNativeLangSpeaker = nls;
	}

	bool isLocal() const
	{
		return _isLocal;
	}

	void saveConfig_xml();

	generic_string getUserPath() const
	{
		return _userPath;
	}

	/***********************************************************************************************************************/

	bool writeSettingsFilesOnCloudForThe1stTime(const generic_string & cloudSettingsPath);

	void setCloudChoice(const TCHAR *pathChoice);

	void removeCloudChoice();

	bool isCloudPathChanged() const;

	bool isx64() const
	{
		return _isx64;
	}

	/***********************************************************************************************************************/

	COLORREF getCurrentDefaultBgColor() const
	{
		return _currentDefaultBgColor;
	}

	COLORREF getCurrentDefaultFgColor() const
	{
		return _currentDefaultFgColor;
	}

	void setCurrentDefaultBgColor(COLORREF c)
	{
		_currentDefaultBgColor = c;
	}

	void setCurrentDefaultFgColor(COLORREF c)
	{
		_currentDefaultFgColor = c;
	}

	bool useNewStyleSaveDlg() const
	{
		return _nppGUI._useNewStyleSaveDlg;
	}

	void setUseNewStyleSaveDlg(bool v)
	{
		_nppGUI._useNewStyleSaveDlg = v;
	}

	/*
	 * @member
	 */
	DPIManager _dpiManager;

	/***********************************************************************************************************************/

	generic_string static getSpecialFolderLocation(int folderKind);

	/***********************************************************************************************************************/

	void setUdlXmlDirtyFromIndex(size_t i);

	void setUdlXmlDirtyFromXmlDoc(const TiXmlDocument* xmlDoc);

	void removeIndexFromXmlUdls(size_t i);

private:
	NppParameters();
	~NppParameters();

	/***********************************************************************************************************************/

	/********************************************
	 * 为了实现单例，禁止了复制构造和赋值运算符 *
	 ********************************************/
	// No copy ctor and assignment
	NppParameters(const NppParameters &) = delete;

	NppParameters & operator = (const NppParameters &) = delete;

	/***********************************************************************************************************************/

	/***************************************************************
	* 为了实现单例，禁止了移动计算的复制构造和移动计算的赋值运算符 *
	****************************************************************/
	// No move ctor and assignment
	NppParameters(NppParameters &&) = delete;

	NppParameters & operator =(NppParameters &&) = delete;

	/***********************************************************************************************************************/

	/*
	* @member
	*/

	/*
	 * %NPP_HOME%\\lang.xml的Document对象
	 */
	TiXmlDocument                * _pXmlDoc           = nullptr;

	/*
	 * %NPP_HOME%\\config.xml的Document对象
	 */
	TiXmlDocument                * _pXmlUserDoc       = nullptr;

	/*
	 * %NPP_HOME%\\stylers.xml的Document对象
	 */
	TiXmlDocument                * _pXmlUserStylerDoc = nullptr;

	/*
	 * %NPP_HOME%\\userDefineLang.xml的Document对象
	 */
	TiXmlDocument                * _pXmlUserLangDoc   = nullptr;
	/*
	 * %NPP_HOME%\\userDefineLang.xml的Document对象
	 */
	std::vector<UdlXmlFileState>   _pXmlUserLangsDoc;

	/*
	 * %NPP_HOME%\\toolbarIcons.xml的Document对象
	 */
	TiXmlDocument                * _pXmlToolIconsDoc = nullptr;

	/*
	 * %NPP_HOME%\\shortcuts.xml的Document对象
	 */
	TiXmlDocument                * _pXmlShortcutDoc  = nullptr;

	/*
	 * %NPP_HOME%\\session.xml的Document对象
	 */
	TiXmlDocument                * _pXmlSessionDoc   = nullptr;
	TiXmlDocument                * _pXmlBlacklistDoc = nullptr;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	 /*
	  * %NPP_HOME%\\nativeLang.xml的Document对象
	  */
	TiXmlDocumentA * _pXmlNativeLangDocA  = nullptr;

	/*
	 * %NPP_HOME%\\contextMenu.xml的Document对象
	 */
	TiXmlDocumentA * _pXmlContextMenuDocA = nullptr;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	std::vector<TiXmlDocument *> _pXmlExternalLexerDoc;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	/*
	 */
	NppGUI               _nppGUI;
	ScintillaViewParams  _svp;
	Lang               * _langList[NB_LANG];
	int                  _nbLang = 0;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	// Recent File History
	generic_string * _LRFileList[NB_MAX_LRF_FILE];
	int              _nbRecentFile                 = 0;
	int              _nbMaxRecentFile              = 10;
	bool             _putRecentFileInSubMenu       = false;

	/***************************
	 * <0: Full File Path Name *
	 * =0: Only File Name      *
	 * >0: Custom Entry Length *
	 ***************************/
	int _recentFileCustomLength = RECENTFILES_SHOWFULLPATH;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	FindHistory _findHistory;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	UserLangContainer     * _userLangArray[NB_MAX_USER_LANG];

	// won't be exceeded to 255;
	unsigned char          _nbUserLang                              = 0;

	/******************************************************
	 * %NPP_HOME%\\userDefineLang.xml文件(用户定义的lang) *
	 ******************************************************/
	generic_string         _userDefineLangPath;
	ExternalLangContainer *_externalLangArray[NB_MAX_EXTERNAL_LANG];
	int                    _nbExternalLang                          = 0;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	CmdLineParamsDTO _cmdLineParams;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	int _fileSaveDlgFilterIndex = -1;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	// All Styles (colours & fonts)
	LexerStylerArray _lexerStylerArray;
	StyleArray       _widgetStyleArray;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	std::vector<generic_string> _fontlist;
	std::vector<generic_string> _blacklist;
	PluginList                  _pluginList;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	/*********************************
	 * 用于加载动态链接库uxtheme.dll *
	 *********************************/
	HMODULE _hUXTheme = nullptr;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	/***********************************************************************************
	 * typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);                *
	 * WINUSERAPI BOOL WINAPI SetLayeredWindowAttributes(HWND, COLORREF, BYTE, DWORD); *
	 ***********************************************************************************/
	WNDPROC _transparentFuncAddr              = nullptr;

	/********************************************************************
	 * typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM); *
	 * THEMEAPI EnableThemeDialogTexture(HWND, DWORD);                  *
	 ********************************************************************/
	WNDPROC _enableThemeDialogTextureFuncAddr = nullptr;

	/*************************************************
	 * %NPP_HOME%\\doLocalConf.xml是否存在           *
	 *************************************************/
	bool                                      _isLocal;

	/********************************************
	 * 当前可执行程序是否是64位的当前可执行程序 *
	 * 默认值：false，即32位当前可执行程序      *
	 * 其他值：true，即64位当前可执行程序       *
	 * by default 32-bit                        *
	 ********************************************/
	bool                                      _isx64 = false;

	/***********************************************************************************************************************/

public:
	void setShortcutDirty()
	{
		_isAnyShortcutModified = true;
	}
private:

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	bool                           _isAnyShortcutModified = false;

	//main menu shortuts. Static size
	/****************
	 * 主菜单快捷键 *
	 ****************/
	std::vector<CommandShortcut>   _shortcuts;

	//altered main menu shortcuts. Indices static. Needed when saving alterations
	std::vector<size_t>            _customizedShortcuts;

	//macro shortcuts, dynamic size, defined on loading macros and adding/deleting them
	std::vector<MacroShortcut>     _macros;

	//run shortcuts, dynamic size, defined on loading run commands and adding/deleting them
	std::vector<UserCommand>       _userCommands;

	//plugin commands, dynamic size, defined on loading plugins
	std::vector<PluginCmdShortcut> _pluginCommands;

	//plugincommands that have been altered. Indices determined after loading ALL plugins. Needed when saving alterations
	std::vector<size_t>            _pluginCustomizedCmds;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	//scintilla keycommands. Static size
	/*******************
	 * Scintilla快捷键 *
	 *******************/
	std::vector<ScintillaKeyMap> _scintillaKeyCommands;
	//modified scintilla keys. Indices static, determined by searching for commandId. Needed when saving alterations
	std::vector<int>             _scintillaModifiedKeyIndices;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	LocalizationSwitcher _localizationSwitcher;
	generic_string       _startWithLocFileName;
	/*
	 * 是否功能导出同时退出，true : 功能导出同时退出，默认false
	 */
	bool                 _doFunctionListExport = false;
	/*
	 * 是否打印后同时退出，true : 打印后同时退出，默认false
	 */
	bool                 _doPrintAndExit       = false;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	ThemeSwitcher _themeSwitcher;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	//vector<generic_string> _noMenuCmdNames;
	/*
	 * 主窗口中的上下文菜单
	 */
	std::vector<MenuItemUnit> _contextMenuItems;
	Session                   _session;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	 /****************************************************
	  * %NPP_HOME%\\plugins\\shortcuts.xml文件的绝对路径 *
	  ****************************************************/
	generic_string _shortcutsPath;
	 /******************************************************
	  * %NPP_HOME%\\plugins\\contextMenu.xml文件的绝对路径 *
	  ******************************************************/
	generic_string _contextMenuPath;
	/*****************************************
	 * %NPP_HOME%即NPP可执行文件所在的文件夹 *
	 *****************************************/
	generic_string _sessionPath;
	/*******************************************
	 * %NPP_HOME%\\blacklist.xml文件的绝对路径 *
	 *******************************************/
	generic_string _blacklistPath;
	/*****************************************
	 * %NPP_HOME%即NPP可执行文件所在的文件夹 *
	 *****************************************/
	generic_string _nppPath;
	/*****************************************
	 * 用户指定的默认文件夹，默认指向：      *
	 * %NPP_HOME%即NPP可执行文件所在的文件夹 *
	 *****************************************/
	generic_string _userPath;
	/*****************************************
	 * %NPP_HOME%\\stylers.xml文件的绝对路径 *
	 *****************************************/
	generic_string _stylerPath; // stylers
	// sentinel of the absence of "doLocalConf.xml" : (_appdataNppDir == TEXT(""))?"doLocalConf.xml present":"doLocalConf.xml absent"
	generic_string _appdataNppDir;
	// plugins root where all the plugins are installed
	/***************************************
	 * %NPP_HOME%\\plugins文件夹的绝对路径 *
	 ***************************************/
	generic_string _pluginRootDir;

	// plugins config dir where the plugin list is installed
	/**************************************************************
	* %NPP_HOME%\\plugins\\Config文件夹的绝对路径，这个是非用户的 *
	***************************************************************/
	generic_string _pluginConfDir;

	// plugins config dir for per user where the plugin parameters are saved / loaded
	/*************************************************************
	 * %NPP_HOME%\\plugins\\Config文件夹的绝对路径，这个是用户的 *
	 * 就是用户的plugins\\Config文件夹的绝对路径                 *
	 * 也有可能是<user name>\Application Data\\plugins\\Config   *
	 *************************************************************/
	generic_string _userPluginConfDir;

	/****************************
	 * 调用可执行文件的绝对路径 *
	 ****************************/
	generic_string _currentDirectory;
	generic_string _workSpaceFilePathes[3];

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	std::vector<generic_string> _fileBrowserRoot;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	Accelerator *_pAccelerator;
	ScintillaAccelerator * _pScintAccelerator;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	FindDlgTabTitiles _findDlgTabTitiles;

	/*************************************
	 * %NPP_HOME%\\asNotepad.xml是否存在 *
	 *************************************/
	bool              _asNotepadStyle    = false;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	/***************************************************
	 * 当前的操作系统 @See winVer(Notepad_plus_msgs.h) *
	 ***************************************************/
	winVer   _winVersion;
	Platform _platForm;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	NativeLangSpeaker *_pNativeLangSpeaker = nullptr;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	COLORREF _currentDefaultBgColor;
	COLORREF _currentDefaultFgColor;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	generic_string _initialCloudChoice;

	/***********************************************************************************************************************/

	/*
	 * @member
	 */
	generic_string _wingupFullPath;
	generic_string _wingupParams;
	generic_string _wingupDir;
	bool           _isElevationRequired = false;

	/***********************************************************************************************************************/

public:
	/***********************************************************************************************************************/

	generic_string getWingupFullPath() const
	{
		return _wingupFullPath;
	}

	generic_string getWingupParams() const
	{
		return _wingupParams;
	}

	generic_string getWingupDir() const
	{
		return _wingupDir;
	}

	bool shouldDoUAC() const
	{
		return _isElevationRequired;
	}

	void setWingupFullPath(const generic_string& val2set)
	{
		_wingupFullPath = val2set;
	}

	void setWingupParams(const generic_string& val2set)
	{
		_wingupParams = val2set;
	}

	void setWingupDir(const generic_string& val2set)
	{
		_wingupDir = val2set;
	}

	void setElevationRequired(bool val2set)
	{
		_isElevationRequired = val2set;
	}

	/***********************************************************************************************************************/

private:
	void getLangKeywordsFromXmlTree();

	bool getUserParametersFromXmlTree();

	bool getUserStylersFromXmlTree();

	std::pair<unsigned char, unsigned char> addUserDefineLangsFromXmlTree(TiXmlDocument *tixmldoc);

	/***********************************************************************************************************************/

	bool getShortcutsFromXmlTree();

	/***********************************************************************************************************************/

	bool getMacrosFromXmlTree();

	bool getUserCmdsFromXmlTree();

	bool getPluginCmdsFromXmlTree();

	bool getScintKeysFromXmlTree();

	bool getSessionFromXmlTree(TiXmlDocument *pSessionDoc = NULL, Session *session = NULL);

	bool getBlackListFromXmlTree();

	/***********************************************************************************************************************/

	void feedGUIParameters(TiXmlNode *node);

	void feedKeyWordsParameters(TiXmlNode *node);

	void feedFileListParameters(TiXmlNode *node);

	void feedScintillaParam(TiXmlNode *node);

	void feedDockingManager(TiXmlNode *node);

	void feedFindHistoryParameters(TiXmlNode *node);

	void feedProjectPanelsParameters(TiXmlNode *node);

	void feedFileBrowserParameters(TiXmlNode *node);

	bool feedStylerArray(TiXmlNode *node);

	std::pair<unsigned char, unsigned char> feedUserLang(TiXmlNode *node);

	void feedUserStyles(TiXmlNode *node);

	void feedUserKeywordList(TiXmlNode *node);

	void feedUserSettings(TiXmlNode *node);

	void feedShortcut(TiXmlNode *node);

	void feedMacros(TiXmlNode *node);

	void feedUserCmds(TiXmlNode *node);

	void feedPluginCustomizedCmds(TiXmlNode *node);

	void feedScintKeys(TiXmlNode *node);

	bool feedBlacklist(TiXmlNode *node);

	/***********************************************************************************************************************/

	void getActions(TiXmlNode *node, Macro & macro);

	bool getShortcuts(TiXmlNode *node, Shortcut & sc);

	/***********************************************************************************************************************/

	void writeStyle2Element(Style & style2Write, Style & style2Sync, TiXmlElement *element);

	void insertUserLang2Tree(TiXmlNode *node, UserLangContainer *userLang);

	void insertCmd(TiXmlNode *cmdRoot, const CommandShortcut & cmd);

	void insertMacro(TiXmlNode *macrosRoot, const MacroShortcut & macro);

	void insertUserCmd(TiXmlNode *userCmdRoot, const UserCommand & userCmd);

	void insertScintKey(TiXmlNode *scintKeyRoot, const ScintillaKeyMap & scintKeyMap);

	void insertPluginCmd(TiXmlNode *pluginCmdRoot, const PluginCmdShortcut & pluginCmd);

	void stylerStrOp(bool op);

	TiXmlElement * insertGUIConfigBoolNode(TiXmlNode *r2w, const TCHAR *name, bool bVal);

	void insertDockingParamNode(TiXmlNode *GUIRoot);

	void writeExcludedLangList(TiXmlElement *element);

	void writePrintSetting(TiXmlElement *element);

	//initialise menu keys and scintilla keys. Other keys are initialized on their own
	/******************
	 * 初始化菜单热键 *
	 ******************/
	void initMenuKeys();

	//these functions have to be called first before any modifications are loaded
	/*************************
	 * 初始化Scintilla快捷键 *
	 *************************/
	void initScintillaKeys();

	// return -1 if not found
	int getCmdIdFromMenuEntryItemName(HMENU mainMenuHadle, const generic_string& menuEntryName, const generic_string& menuItemName);

	// return -1 if not found
	int getPluginCmdIdFromMenuEntryItemName(HMENU pluginsMenu, const generic_string& pluginName, const generic_string& pluginCmdName);

	winVer getWindowsVersion();

};




























































