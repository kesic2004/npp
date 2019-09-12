/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/

#ifndef __REAL_KESIC_LEE_TIXMLELEMENT_H__
#define __REAL_KESIC_LEE_TIXMLELEMENT_H__ 1

#include <tchar.h>
#include "TiXmlNode.h"
#include "TiXmlAttributeSet.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/***********************************************************************
 * The element is a container class. It has a value, the element name, *
 * and can contain other elements, text, comments, and unknowns.	   *
 * Elements also contain an arbitrary number of attributes.		       *
 ***********************************************************************/
class TiXmlElement : public TiXmlNode
{
public:
	/*************************
	 * Construct an element. *
	 *************************/
	TiXmlElement(const TCHAR * in_value);

#ifdef TIXML_USE_STL
	/***********************
	 * string constructor. *
	 ***********************/
	TiXmlElement(const generic_string& _value);
#endif

	virtual ~TiXmlElement();

	/***********************************************************
	 * Given an attribute name, Attribute() returns the value  *
	 * for the attribute of that name, or null if none exists. *
	 ***********************************************************/
	const TCHAR* Attribute(const TCHAR* name) const;

	/***************************************************************
	 * Given an attribute name, Attribute() returns the value	   *
	 * for the attribute of that name, or null if none exists.	   *
	 * If the attribute exists and can be converted to an integer, *
	 * the integer value will be put in the return 'i', if 'i'	   *
	 * is non-null.												   *
	 ***************************************************************/
	const TCHAR* Attribute(const TCHAR* name, int* i) const;

	/**************************************************************
	 * Given an attribute name, Attribute() returns the value	  *
	 * for the attribute of that name, or null if none exists.	  *
	 * If the attribute exists and can be converted to an double, *
	 * the double value will be put in the return 'd', if 'd'	  *
	 * is non-null.												  *
	 **************************************************************/
	const TCHAR* Attribute(const TCHAR* name, double* d) const;

	/**************************************************************************
	 * QueryIntAttribute examines the attribute - it is an alternative to the *
	 * Attribute() method with richer error checking.						  *
	 * If the attribute is an integer, it is stored in 'value' and			  *
	 * the call returns TIXML_SUCCESS. If it is not							  *
	 * an integer, it returns TIXML_WRONG_TYPE. If the attribute			  *
	 * does not exist, then TIXML_NO_ATTRIBUTE is returned.					  *
	 **************************************************************************/
	int QueryIntAttribute(const TCHAR* name, int* value) const;

	/**************************************************************************
	 * QueryDoubleAttribute examines the attribute - see QueryIntAttribute(). *
	 **************************************************************************/
	int QueryDoubleAttribute(const TCHAR* name, double* value) const;

	/****************************************************************
	 * Sets an attribute of name to a given value. The attribute    *
	 * will be created if it does not exist, or changed if it does. *
	 ****************************************************************/
	void SetAttribute(const TCHAR* name, const TCHAR * value);

#ifdef TIXML_USE_STL
	const TCHAR* Attribute(const generic_string& name) const;
	const TCHAR* Attribute(const generic_string& name, int* i) const;

	/********************
	 * STL string form. *
	 ********************/
	void SetAttribute(const generic_string& name, const generic_string& _value);
	/**********************
	 * < STL string form. *
	 **********************/
	void SetAttribute(const generic_string& name, int _value);
#endif

	/****************************************************************
	 * Sets an attribute of name to a given value. The attribute	*
	 * will be created if it does not exist, or changed if it does. *
	 ****************************************************************/
	void SetAttribute(const TCHAR * name, int value);

	/*********************************************
	 * Deletes an attribute with the given name. *
	 *********************************************/
	void RemoveAttribute(const TCHAR * name);

#ifdef TIXML_USE_STL
	/**********************
	 * < STL string form. *
	 **********************/
	void RemoveAttribute(const generic_string& name);
#endif

	/*************************************************
	 * < Access the first attribute in this element. *
	 *************************************************/
	TiXmlAttribute* FirstAttribute() const;

	/************************************************
	 * < Access the last attribute in this element. *
	 ************************************************/
	TiXmlAttribute* LastAttribute()	const;

	/*******************************************************
	 * [internal use] Creates a new Element and returs it. *
	 *******************************************************/
	virtual TiXmlNode* Clone() const;
	/******************
	 * [internal use] *
	 ******************/

	virtual void Print(FILE* cfile, int depth) const;

protected:

	/************************************
	 * Used to be public [internal use] *
	 ************************************/
#ifdef TIXML_USE_STL
	virtual void StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag);
#endif
	virtual void StreamOut(TIXML_OSTREAM * out) const;

	/*************************************************
	 * [internal use]								 *
	 * Attribtue parsing starts: next TCHAR past '<' *
	 * returns: next TCHAR past '>'                  *
	 *************************************************/
	virtual const TCHAR* Parse(const TCHAR* p, TiXmlParsingData* data);

	/*****************************************************************
	 * [internal use]												 *
	 * Reads the "value" of the element -- another element, or text. *
	 * This should terminate with the current end tag.				 *
	 *****************************************************************/
	const TCHAR* ReadValue(const TCHAR* in, TiXmlParsingData* prevData);

private:
	TiXmlAttributeSet attributeSet;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TIXMLELEMENT_H__


