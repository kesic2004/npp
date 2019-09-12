/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
#ifndef __REAL_KESIC_LEE_TIXMLATTRIBUTESET_H__
#define __REAL_KESIC_LEE_TIXMLATTRIBUTESET_H__ 1

#include "TiXmlAttribute.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/******************************************************************************
 * A class used to manage a group of attributes.							   *
 * It is only used internally, both by the ELEMENT and the DECLARATION.	   *
 * 																		   *
 * The set can be changed transparent to the Element and Declaration		   *
 * classes that use it, but NOT transparent to the Attribute				   *
 * which has to implement a next() and previous() method. Which makes		   *
 * it a bit problematic and prevents the use of STL.						   *
 * 																		   *
 * This version is implemented with circular lists because:				   *
 * - I like circular lists													   *
 * - it demonstrates some independence from the (typical) doubly linked list. *
 ******************************************************************************/
class TiXmlAttributeSet
{
public:
	TiXmlAttributeSet();
	~TiXmlAttributeSet();

	void Add(TiXmlAttribute* attribute);
	void Remove(TiXmlAttribute* attribute);

	TiXmlAttribute* First() const;
	TiXmlAttribute* Last()  const;
	TiXmlAttribute*	Find(const TCHAR * name) const;

private:
	TiXmlAttribute sentinel;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif


#endif
