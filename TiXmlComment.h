/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/

#ifndef __REAL_KESIC_LEE_TIXMLCOMMENT_H__
#define __REAL_KESIC_LEE_TIXMLCOMMENT_H__ 1

//TiXmlComment.h
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

#include "TiXmlNode.h"

/*******************
 * An XML comment. *
 *******************/
class TiXmlComment : public TiXmlNode
{
public:
	/********************************
	 * Constructs an empty comment. *
	 ********************************/
	TiXmlComment();

	virtual ~TiXmlComment();

	/*******************************************************
	 * [internal use] Creates a new Element and returs it. *
	 *******************************************************/
	virtual TiXmlNode* Clone() const;

	/******************
	 * [internal use] *
	 ******************/
	virtual void Print(FILE* cfile, int depth) const;
protected:
	/*********************
	 * used to be public *
	 *********************/
#ifdef TIXML_USE_STL
	virtual void StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag);
#endif
	virtual void StreamOut(TIXML_OSTREAM * out) const;
	/*************************************************
	 * [internal use]								 *
	 * Attribtue parsing starts: at the ! of the !-- *
	 * returns: next TCHAR past '>'					 *
	 *************************************************/
	virtual const TCHAR* Parse(const TCHAR* p, TiXmlParsingData* data);
};


#ifdef _MSC_VER
#pragma warning( pop )
#endif


#endif // !__REAL_KESIC_LEE_TIXMLCOMMENT_H__

