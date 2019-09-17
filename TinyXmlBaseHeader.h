/*************************************************************************************************
 * www.sourceforge.net/projects/tinyxml                                                          *
 * Original code (2.0 and earlier )copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com) *
 *                                                                                               *
 * This software is provided 'as-is', without any express or implied                             *
 * warranty. In no event will the authors be held liable for any                                 *
 * damages arising from the use of this software.                                                *
 *                                                                                               *
 * Permission is granted to anyone to use this software for any                                  *
 * purpose, including commercial applications, and to alter it and                               *
 * redistribute it freely, subject to the following restrictions:                                *
 *                                                                                               *
 * 1. The origin of this software must not be misrepresented; you must                           *
 * not claim that you wrote the original software. If you use this                               *
 * software in a product, an acknowledgment in the product documentation                         *
 * would be appreciated but is not required.                                                     *
 *                                                                                               *
 * 2. Altered source versions must be plainly marked as such, and                                *
 * must not be misrepresented as being the original software.                                    *
 *                                                                                               *
 * 3. This notice may not be removed or altered from any source                                  *
 * distribution.                                                                                 *
 *************************************************************************************************/
/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/

#ifndef __REAL_KESIC_LEE_TINY_XML_BASE_HEADER_H__
#define __REAL_KESIC_LEE_TINY_XML_BASE_HEADER_H__ 1

/***********************重构完成后移除起***************************/

//#ifdef _MSC_VER
//#pragma warning( push )
//#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
//#pragma warning( disable : 4530 )
//#pragma warning( disable : 4786 )
//#endif

//#ifdef _MSC_VER
//#pragma warning( pop )
//#endif

/***********************重构完成后移除止***************************/

// Help out windows:
#if defined( _DEBUG ) && !defined( DEBUG )
#define DEBUG
#endif

#if defined( DEBUG ) && defined( _MSC_VER )
#define TIXML_LOG OutputDebugString
#else
#define TIXML_LOG printf
#endif

// #define TIXML_USE_STL 1
#ifdef TIXML_USE_STL
#include <cassert>
#include <Windows.h>
#include "CommonGenericString.h"
#define TIXML_STRING    generic_string
#define TIXML_ISTREAM    std::basic_istream<TCHAR>
#define TIXML_OSTREAM    std::basic_ostream<TCHAR>
#else
#include "TiXmlString.h"
#define TIXML_STRING    TiXmlString
#define TIXML_OSTREAM    TiXmlOutStream
#endif

#endif // !__REAL_KESIC_LEE_TINY_XML_BASE_HEADER_H__

