/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/

#ifndef __REAL_KESIC_LEE_TIXMLATTRIBUTE_H__
#define __REAL_KESIC_LEE_TIXMLATTRIBUTE_H__ 1

#include <tchar.h>
#include "TiXmlBase.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/*****************************************************************
 * An attribute is a name-value pair. Elements have an arbitrary *
 * number of attributes, each with a unique name.				  *
 * 															  *
 * @note The attributes are not TiXmlNodes, since they are not	  *
 * part of the tinyXML document object model. There are other	  *
 * suggested ways to look at this problem.						  *
 ******************************************************************/
class TiXmlAttribute : public TiXmlBase
{
	friend class TiXmlAttributeSet;

public:
	/*********************************
	 * Construct an empty attribute. *
	 *********************************/
	TiXmlAttribute();

#ifdef TIXML_USE_STL
	/***********************
	 * string constructor. *
	 ***********************/
	TiXmlAttribute(const generic_string& _name, const generic_string& _value);
#endif

	/*************************************************
	 * Construct an attribute with a name and value. *
	 *************************************************/
	TiXmlAttribute(const TCHAR * _name, const TCHAR * _value);

	/****************************************
	 * < Return the name of this attribute. *
	 ****************************************/
	const TCHAR*		Name()  const;

	/*****************************************
	 * < Return the value of this attribute. *
	 *****************************************/
	const TCHAR*		Value() const;
	
	/******************************************************************
	 * < Return the value of this attribute, converted to an integer. *
	 ******************************************************************/
	const int       IntValue() const;
	
	/****************************************************************
	 * < Return the value of this attribute, converted to a double. *
	 ****************************************************************/
	const double	DoubleValue() const;

	/********************************************************************************
	 * QueryIntValue examines the value generic_string. It is an alternative to the	*
	 * IntValue() method with richer error checking.								*
	 * If the value is an integer, it is stored in 'value' and						*
	 * the call returns TIXML_SUCCESS. If it is not									*
	 * an integer, it returns TIXML_WRONG_TYPE.										*
	 * 																				*
	 * A specialized but useful call. Note that for success it returns 0,			*
	 * which is the opposite of almost all other TinyXml calls.						*
	 ********************************************************************************/
	int QueryIntValue(int* value) const;
	
	/****************************************************************************
	 * QueryDoubleValue examines the value generic_string. See QueryIntValue(). *
	 ****************************************************************************/
	int QueryDoubleValue(double* value) const;

	/*************************************
	 * < Set the name of this attribute. *
	 *************************************/
	void SetName(const TCHAR* _name);
	
	/********************
	 * < Set the value. *
	 ********************/
	void SetValue(const TCHAR* _value);

	/************************************
	 * < Set the value from an integer. *
	 ************************************/
	void SetIntValue(int value);
	
	/**********************************
	 * < Set the value from a double. *
	 **********************************/
	void SetDoubleValue(double value);

#ifdef TIXML_USE_STL
	/********************
	 * STL string form. *
	 ********************/
	void SetName(const generic_string& _name);

	/********************
	 * STL string form.	*
	 ********************/
	void SetValue(const generic_string& _value);
#endif

	/*******************************************************************
	 * Get the next sibling attribute in the DOM. Returns null at end. *
	 *******************************************************************/
	TiXmlAttribute * Next() const;
	
	/*****************************************************************************
	 * Get the previous sibling attribute in the DOM. Returns null at beginning. *
	 *****************************************************************************/
	TiXmlAttribute* Previous() const;

	bool operator==(const TiXmlAttribute& rhs) const;
	bool operator<(const TiXmlAttribute& rhs)	 const;
	bool operator>(const TiXmlAttribute& rhs)  const;

	/******************************************************
	 * [internal use]                                     *
	 * Attribtue parsing starts: first letter of the name *
	 * returns: the next TCHAR after the value end quote  *
	 ******************************************************/
	virtual const TCHAR* Parse(const TCHAR* p, TiXmlParsingData* data);

	/******************
	 * [internal use] *
	 ******************/
	virtual void Print(FILE* cfile, int depth) const;

	virtual void StreamOut(TIXML_OSTREAM * out) const;
	/****************************************************************
	 * [internal use]                                               *
	 * Set the document pointer so the attribute can report errors. *
	 ****************************************************************/
	void SetDocument(TiXmlDocument* doc);

private:
	/******************************************************
	 * A pointer back to a document, for error reporting. *
	 ******************************************************/
	TiXmlDocument*	document;
	TIXML_STRING name;
	TIXML_STRING value;
	TiXmlAttribute *	prev;
	TiXmlAttribute *	next;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
