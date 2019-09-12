/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/

#ifndef __REAL_KESIC_LEE_TINY_XML_BASE_HEADER_H__
#define __REAL_KESIC_LEE_TINY_XML_BASE_HEADER_H__ 1

/***********************重构完成后移除起***************************/

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

#ifdef _MSC_VER
#pragma warning( pop )
#endif

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

#ifdef TIXML_USE_STL
#define TIXML_STRING	generic_string
#define TIXML_ISTREAM	std::basic_istream<TCHAR>
#define TIXML_OSTREAM	std::basic_ostream<TCHAR>
#else
#include "TiXmlString.h"
#define TIXML_STRING	TiXmlString
#define TIXML_OSTREAM	TiXmlOutStream
#endif


#endif // !__REAL_KESIC_LEE_TINY_XML_BASE_HEADER_H__
