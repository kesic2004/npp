/********************************
* 移动自：tinyxml.h            *
* 由不明真相的吃瓜群众移动至此 *
********************************/
#ifndef __REAL_KESIC_LEE_TI_XML_CURSOR_H__
#define __REAL_KESIC_LEE_TI_XML_CURSOR_H__ 1

#include "TinyXmlBaseHeader.h"

struct TiXmlCursor;
class TiXmlBase;
class TiXmlNode;
class TiXmlAttribute;
class TiXmlAttributeSet;
class TiXmlElement;
class TiXmlComment;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlUnknown;
class TiXmlDocument;
class TiXmlHandle;
class TiXmlParsingData;


#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/*******************************************************
 * & Internal structure for tracking location of items *
 * in the XML file.                                    *
 *******************************************************/
struct TiXmlCursor // TiXmlCursor.h
{
public:
	TiXmlCursor();
	void Clear();

public:
	int row;	// 0 based.
	int col;	// 0 based.
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_CURSOR_H__
