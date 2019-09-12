/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
#ifndef __REAL_KESIC_LEE_TI_XML_TEXT_H__
#define __REAL_KESIC_LEE_TI_XML_TEXT_H__ 1

#include <tchar.h>
#include "TiXmlNode.h"

// TiXmlText.h

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/**************************************
 * XML text. Contained in an element. *
 **************************************/
class TiXmlText : public TiXmlNode
{
	friend class TiXmlElement;
public:
	/****************
	 * Constructor. *
	 ****************/
	TiXmlText(const TCHAR * initValue);

	virtual ~TiXmlText();

#ifdef TIXML_USE_STL
	/****************
	 * Constructor. *
	 ****************/
	TiXmlText(const generic_string& initValue);
#endif

	/******************
	 * [internal use] *
	 ******************/
	virtual void Print(FILE* cfile, int depth) const;

protected:
	/********************************************************
	 * [internal use] Creates a new Element and returns it. *
	 ********************************************************/
	virtual TiXmlNode* Clone() const;

	virtual void StreamOut(TIXML_OSTREAM * out) const;

	/*************************************************
	 * [internal use]                                *
	 * returns true if all white space and new lines *
	 *************************************************/
	bool Blank() const;

	/*****************************************************
	 * internal use]									 *
	 * Attribtue parsing starts: First TCHAR of the text *
	 * returns: next TCHAR past '>'                      *
	 *****************************************************/
	virtual const TCHAR* Parse(const TCHAR* p, TiXmlParsingData* data);

	/******************
	 * [internal use] *
	 ******************/
#ifdef TIXML_USE_STL
	virtual void StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag);
#endif
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_TEXT_H__



