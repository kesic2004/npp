/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
#ifndef __REAL_KESIC_LEE_TI_XML_NODE_H__
#define __REAL_KESIC_LEE_TI_XML_NODE_H__ 1

#include "TiXmlBase.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/*****************************************************************
 * The parent class for everything in the Document Object Model. *
 * (Except for attributes).										 *
 * Nodes have siblings, a parent, and children. A node can be	 *
 * in a document, or stand on its own. The type of a TiXmlNode	 *
 * can be queried, and it can be cast to its more defined type.	 *
 *****************************************************************/
class TiXmlNode : public TiXmlBase
{
	friend class TiXmlDocument;
	friend class TiXmlElement;
public:
#ifdef TIXML_USE_STL	
	/***********************************************************************
	 * An input stream operator, for every class. Tolerant of newlines and *
	 * formatting, but doesn't expect them.                                *
	 ***********************************************************************/
	friend TIXML_ISTREAM & operator >> (TIXML_ISTREAM & in, TiXmlNode& base);
		
	/************************************************************************
	 * An output stream operator, for every class. Note that this outputs	*
	 * without any newlines or formatting, as opposed to Print(), which		*
	 * includes tabs and new lines.											*
	 * 																		*
	 * The operator<< and operator>> are not completely symmetric. Writing	*
	 * a node to a stream is very well defined. You'll get a nice stream	*
	 * of output, without any extra whitespace or newlines.					*
	 * 																		*
	 * But reading is not as well defined. (As it always is.) If you create	*
	 * a TiXmlElement (for example) and read that from an input stream,		*
	 * the text needs to define an element or junk will result. This is		*
	 * true of all input streams, but it's worth keeping in mind.			*
	 * 																		*
	 * A TiXmlDocument will read nodes until it reads a root element, and	*
	 * all the children of that root element.								*
	 ************************************************************************/
	friend TIXML_OSTREAM & operator<< (TIXML_OSTREAM & out, const TiXmlNode & base);

	/**************************************************
	 * Appends the XML node or attribute to a string. *
	 **************************************************/
	friend generic_string & operator<< (generic_string & out, const TiXmlNode & base);

#else
	/************************************************
	 * Used internally, not part of the public API. *
	 ************************************************/
	friend TIXML_OSTREAM & operator<< (TIXML_OSTREAM & out, const TiXmlNode & base);
#endif

	/*********************************************************
	 * The types of XML nodes supported by TinyXml. (All the *
	 * unsupported types are picked up by UNKNOWN.)          *
	 *********************************************************/
	enum NodeType
	{
		DOCUMENT,
		ELEMENT,
		COMMENT,
		UNKNOWN,
		TEXT,
		DECLARATION,
		TYPECOUNT
	};

	virtual ~TiXmlNode();

	/***********************************************************
	 * The meaning of 'value' changes for the specific type of *
	 * TiXmlNode.											   *
	 * @verbatim											   *
	 * Document:	filename of the xml file				   *
	 * Element:	name of the element							   *
	 * Comment:	the comment text							   *
	 * Unknown:	the tag contents							   *
	 * Text:		the text generic_string					   *
	 * @endverbatim											   *
	 *                                                         *
	 * The subclasses will wrap this function.                 *
	 ***********************************************************/
	const TCHAR * Value() const;

	/**********************************************
	 * Changes the value of the node. Defined as: *
	 * @verbatim								  *
	 * Document:	filename of the xml file	  *
	 * Element:	name of the element				  *
	 * Comment:	the comment text				  *
	 * Unknown:	the tag contents				  *
	 * Text:		the text generic_string		  *
	 * @endverbatim								  *
	 **********************************************/
	void SetValue(const TCHAR * _value);

#ifdef TIXML_USE_STL
	/********************
	 * STL string form. *
	 ********************/
	void SetValue(const generic_string & _value);
#endif

	/*****************************************************************
	 * Delete all the children of this node. Does not affect 'this'. *
	 *****************************************************************/
	void Clear();

	/************************
	 * One step up the DOM. *
	 ************************/
	TiXmlNode * Parent() const;

	/**************************************************************************
	 * < The first child of this node. Will be null if there are no children. *
	 **************************************************************************/
	TiXmlNode * FirstChild()	const;

	/*****************************************************************************************
	 * < The first child of this node with the matching 'value'. Will be null if none found. *
	 *****************************************************************************************/
	TiXmlNode * FirstChild(const TCHAR * value) const;

	/***********************************************************************
	 * The last child of this node. Will be null if there are no children. *
	 ***********************************************************************/
	TiXmlNode * LastChild() const;

	/****************************************************************************************
	 * The last child of this node matching 'value'. Will be null if there are no children. *
	 ****************************************************************************************/
	TiXmlNode * LastChild(const TCHAR * value) const;

#ifdef TIXML_USE_STL
	/***********************************
	 * < STL std::generic_string form. *
	 ***********************************/
	TiXmlNode * FirstChild(const generic_string & _value) const;

	/***********************************
	 * < STL std::generic_string form. *
	 ***********************************/
	TiXmlNode * LastChild(const generic_string & _value) const;
#endif

	/****************************************************************************
	 * An alternate way to walk the children of a node.							*
	 * One way to iterate over nodes is:										*
	 * @verbatim																*
	 * for( child = parent->FirstChild(); child; child = child->NextSibling() )	*
	 * @endverbatim																*
	 * 																			*
	 * IterateChildren does the same thing with the syntax:						*
	 * @verbatim																*
	 * child = 0;																*
	 * while( child = parent->IterateChildren( child ) )						*
	 * @endverbatim																*
	 * 																			*
	 * IterateChildren takes the previous child as input and finds				*
	 * the next one. If the previous child is null, it returns the				*
	 * first. IterateChildren will return null when done.						*
	 ****************************************************************************/
	TiXmlNode* IterateChildren(TiXmlNode * previous) const;						

	/**********************************************************************************
	 * This flavor of IterateChildren searches for children with a particular 'value' *
	 **********************************************************************************/
	TiXmlNode* IterateChildren(const TCHAR * value, TiXmlNode* previous) const;

#ifdef TIXML_USE_STL
	/*********************************
	 * STL std::generic_string form. *
	 *********************************/
	TiXmlNode* IterateChildren(const generic_string& _value, TiXmlNode* previous) const;
#endif

	/********************************************************************
	 * Add a new node related to this. Adds a child past the LastChild. *
	 * Returns a pointer to the new object or NULL if an error occured. *
	 ********************************************************************/
	TiXmlNode* InsertEndChild(const TiXmlNode& addThis);


	/***********************************************************************
	 * Add a new node related to this. Adds a child past the LastChild.	   *
	 * 																	   *
	 * NOTE: the node to be added is passed by pointer, and will be		   *
	 * henceforth owned (and deleted) by tinyXml. This method is efficient *
	 * and avoids an extra copy, but should be used with care as it		   *
	 * uses a different memory model than the other insert functions.	   *
	 * 																	   *
	 * @sa InsertEndChild												   *
	 ***********************************************************************/
	TiXmlNode * LinkEndChild(TiXmlNode* addThis);

	/****************************************************************************
	 * Add a new node related to this. Adds a child before the specified child.	*
	 * Returns a pointer to the new object or NULL if an error occured.			*
	 ****************************************************************************/
	TiXmlNode * InsertBeforeChild(TiXmlNode* beforeThis, const TiXmlNode& addThis);

	/***************************************************************************
	 * Add a new node related to this. Adds a child after the specified child. *
	 * Returns a pointer to the new object or NULL if an error occured.		   *
	 ***************************************************************************/
	TiXmlNode * InsertAfterChild(TiXmlNode* afterThis, const TiXmlNode& addThis);

	/********************************************************************
	 * Replace a child of this node.									*
	 * Returns a pointer to the new object or NULL if an error occured. *
	 ********************************************************************/
	TiXmlNode * ReplaceChild(TiXmlNode* replaceThis, const TiXmlNode& withThis);

	/********************************
	 * Delete a child of this node. *
	 ********************************/
	bool RemoveChild(TiXmlNode* removeThis);

	/*******************************
	 * Navigate to a sibling node. *
	 *******************************/
	TiXmlNode* PreviousSibling() const { return prev; }

	/*******************************
	 * Navigate to a sibling node. *
	 *******************************/
	TiXmlNode* PreviousSibling(const TCHAR *) const;

#ifdef TIXML_USE_STL
	/***********************************
	 * < STL std::generic_string form. *
	 ***********************************/
	TiXmlNode* PreviousSibling(const generic_string& _value) const;

	/***********************************
	 * < STL std::generic_string form. *
	 ***********************************/
	TiXmlNode* NextSibling(const generic_string& _value) const;
#endif

	/********************************
	 *  Navigate to a sibling node. *
	 ********************************/
	TiXmlNode* NextSibling() const { return next; }

	/******************************************************
	 * Navigate to a sibling node with the given 'value'. *
	 ******************************************************/
	TiXmlNode* NextSibling(const TCHAR *) const;

	/**************************************************************
	 * Convenience function to get through elements.			  *
	 * Calls NextSibling and ToElement. Will skip all non-Element *
	 * nodes. Returns 0 if there is not another element.		  *
	 **************************************************************/
	TiXmlElement* NextSiblingElement() const;

	/**************************************************************
	 * Convenience function to get through elements.			  *
	 * Calls NextSibling and ToElement. Will skip all non-Element *
	 * nodes. Returns 0 if there is not another element.		  *
	 **************************************************************/
	TiXmlElement* NextSiblingElement(const TCHAR *) const;

#ifdef TIXML_USE_STL
	/**********************************
	 *< STL std::generic_string form. *
	 **********************************/
	TiXmlElement* NextSiblingElement(const generic_string& _value) const;
#endif

	/*************************************************
	 * Convenience function to get through elements. *
	 *************************************************/
	TiXmlElement* FirstChildElement()	const;

	/*************************************************
	 * Convenience function to get through elements. *
	 *************************************************/
	TiXmlElement* FirstChildElement(const TCHAR * value) const;

#ifdef TIXML_USE_STL
	/***********************************
	 * < STL std::generic_string form. *
	 ***********************************/
	TiXmlElement* FirstChildElement(const generic_string& _value) const;
#endif

	/****************************************************************
	 * Query the type (as an enumerated value, above) of this node.	*
	 * The possible types are: DOCUMENT, ELEMENT, COMMENT,			*
	 * UNKNOWN, TEXT, and DECLARATION.								*
	 ****************************************************************/
	virtual int Type() const;

	/*******************************************************
	 * Return a pointer to the Document this node lives in. *
	 * Returns null if not in a document.				   *
	 *******************************************************/
	TiXmlDocument* GetDocument() const;

	/**********************************************
	 * Returns true if this node has no children. *
	 **********************************************/
	bool NoChildren() const;

	/******************************************************************************
	 * < Cast to a more defined type. Will return null not of the requested type. *
	 ******************************************************************************/
	TiXmlDocument * ToDocument() const;

	/******************************************************************************
	 * < Cast to a more defined type. Will return null not of the requested type. *
	 ******************************************************************************/
	TiXmlElement *  ToElement() const;

	/******************************************************************************
	 * < Cast to a more defined type. Will return null not of the requested type. *
	 ******************************************************************************/
	TiXmlComment *  ToComment() const;

	/******************************************************************************
	 * < Cast to a more defined type. Will return null not of the requested type. *
	 ******************************************************************************/
	TiXmlUnknown *  ToUnknown() const;

	/******************************************************************************
	 * < Cast to a more defined type. Will return null not of the requested type. *
	 ******************************************************************************/
	TiXmlText*	   ToText()    const;

	/******************************************************************************
	 * < Cast to a more defined type. Will return null not of the requested type. *
	 ******************************************************************************/
	TiXmlDeclaration* ToDeclaration() const;

	virtual TiXmlNode* Clone() const = 0;

	void SetUserData(void* user);

	void * GetUserData();

protected:
	TiXmlNode(NodeType type);

#ifdef TIXML_USE_STL
	/****************************************
	 * The real work of the input operator. *
	 ****************************************/
	virtual void StreamIn(TIXML_ISTREAM* in, TIXML_STRING* tag) = 0;
#endif

	/**********************************************************************************
	 * Figure out what is at *p, and parse it. Returns null if it is not an xml node. *
	 **********************************************************************************/
	TiXmlNode* Identify(const TCHAR* start);
	void CopyToClone(TiXmlNode* target) const {
		target->SetValue(value.c_str());
		target->userData = userData;
	}

	/****************************************************
	 * Internal Value function returning a TIXML_STRING *
	 ****************************************************/
	TIXML_STRING SValue() const { return value; }

	TiXmlNode*		parent;
	NodeType		type;

	TiXmlNode*		firstChild;
	TiXmlNode*		lastChild;

	TIXML_STRING	value;

	TiXmlNode*		prev;
	TiXmlNode*		next;
	void*			userData;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
