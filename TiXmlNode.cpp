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
 * 移动自：                     *
 * tinyxml.cpp                  *
 * tinyxmlparser.cpp            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
#include "TiXmlNode.h"

#ifndef TIXML_USE_STL    
/************************************************
 * Used internally, not part of the public API. *
 ************************************************/
TIXML_OSTREAM & operator << (TIXML_OSTREAM & out, const TiXmlNode & base)
{
    base.StreamOut(&out);
    return out;
}
#endif

TiXmlNode::~TiXmlNode()
{
    TiXmlNode* node = firstChild;
    TiXmlNode* temp = 0;

    while (node)
    {
        temp = node;
        node = node->next;
        delete temp;
    }
}


/***********************************************************
 * The meaning of 'value' changes for the specific type of *
 * TiXmlNode.                                              *
 * @verbatim                                               *
 * Document:    filename of the xml file                   *
 * Element:    name of the element                         *
 * Comment:    the comment text                            *
 * Unknown:    the tag contents                            *
 * Text:        the text generic_string                    *
 * @endverbatim                                            *
 *                                                         *
 * The subclasses will wrap this function.                 *
 ***********************************************************/
const TCHAR * TiXmlNode::Value() const 
{
    return value.c_str();
}

/**********************************************
* Changes the value of the node. Defined as: *
* @verbatim                                  *
* Document:    filename of the xml file      *
* Element:    name of the element            *
* Comment:    the comment text               *
* Unknown:    the tag contents               *
* Text:        the text generic_string       *
* @endverbatim                               *
**********************************************/
void TiXmlNode::SetValue(const TCHAR * _value)
{
    value = _value;
}

/*****************************************************************
 * Delete all the children of this node. Does not affect 'this'. *
 *****************************************************************/
void TiXmlNode::Clear()
{
    TiXmlNode* node = firstChild;
    TiXmlNode* temp = 0;

    while (node)
    {
        temp = node;
        node = node->next;
        delete temp;
    }

    firstChild = 0;
    lastChild = 0;
}

/************************
 * One step up the DOM. *
 ************************/
TiXmlNode * TiXmlNode::Parent() const
{
    return parent;
}


/**************************************************************************
 * < The first child of this node. Will be null if there are no children. *
 **************************************************************************/
TiXmlNode * TiXmlNode::FirstChild()    const
{
    return firstChild;
}

/*****************************************************************************************
 * < The first child of this node with the matching 'value'. Will be null if none found. *
 *****************************************************************************************/
TiXmlNode * TiXmlNode::FirstChild(const TCHAR * _value) const
{
    TiXmlNode* node;
    for (node = firstChild; node; node = node->next)
    {
        if (node->SValue() == TIXML_STRING(_value))
            return node;
    }
    return 0;
}

/***********************************************************************
 * The last child of this node. Will be null if there are no children. *
 ***********************************************************************/
TiXmlNode * TiXmlNode::LastChild() const
{
    return lastChild;
}

/****************************************************************************************
 * The last child of this node matching 'value'. Will be null if there are no children. *
 ****************************************************************************************/
TiXmlNode * TiXmlNode::LastChild(const TCHAR * _value) const
{
    TiXmlNode* node;
    for (node = lastChild; node; node = node->prev)
    {
        if (node->SValue() == TIXML_STRING(_value))
            return node;
    }
    return 0;
}

/****************************************************************************
 * An alternate way to walk the children of a node.                         *
 * One way to iterate over nodes is:                                        *
 * @verbatim                                                                *
 * for( child = parent->FirstChild(); child; child = child->NextSibling() ) *
 * @endverbatim                                                             *
 *                                                                          *
 * IterateChildren does the same thing with the syntax:                     *
 * @verbatim                                                                *
 * child = 0;                                                               *
 * while( child = parent->IterateChildren( child ) )                        *
 * @endverbatim                                                             *
 *                                                                          *
 * IterateChildren takes the previous child as input and finds              *
 * the next one. If the previous child is null, it returns the              *
 * first. IterateChildren will return null when done.                       *
 ****************************************************************************/
TiXmlNode * TiXmlNode::IterateChildren(TiXmlNode * previous) const
{
    if (!previous)
    {
        return FirstChild();
    }
    else
    {
        assert(previous->parent == this);
        return previous->NextSibling();
    }
}

/**********************************************************************************
 * This flavor of IterateChildren searches for children with a particular 'value' *
 **********************************************************************************/
TiXmlNode * TiXmlNode::IterateChildren(const TCHAR * val, TiXmlNode * previous) const
{
    if (!previous)
    {
        return FirstChild(val);
    }
    else
    {
        assert(previous->parent == this);
        return previous->NextSibling(val);
    }
}

/********************************************************************
 * Add a new node related to this. Adds a child past the LastChild. *
 * Returns a pointer to the new object or NULL if an error occured. *
 ********************************************************************/
TiXmlNode * TiXmlNode::InsertEndChild(const TiXmlNode & addThis)
{
    TiXmlNode * node = addThis.Clone();
    if (!node)
    {
        return 0;
    }

    return LinkEndChild(node);
}


/***********************************************************************
 * Add a new node related to this. Adds a child past the LastChild.    *
 *                                                                     *
 * NOTE: the node to be added is passed by pointer, and will be        *
 * henceforth owned (and deleted) by tinyXml. This method is efficient *
 * and avoids an extra copy, but should be used with care as it        *
 * uses a different memory model than the other insert functions.      *
 *                                                                     *
 * @sa InsertEndChild                                                  *
 ***********************************************************************/
TiXmlNode * TiXmlNode::LinkEndChild(TiXmlNode * node)
{
    node->parent = this;

    node->prev = lastChild;
    node->next = 0;

    if (lastChild)
    {
        lastChild->next = node;
    }
    /*************************
     * it was an empty list. *
     *************************/
    else
    {
        firstChild = node;
    }

    lastChild = node;
    return node;
}

/****************************************************************************
 * Add a new node related to this. Adds a child before the specified child. *
 * Returns a pointer to the new object or NULL if an error occured.         *
 ****************************************************************************/
TiXmlNode * TiXmlNode::InsertBeforeChild(TiXmlNode* beforeThis, const TiXmlNode& addThis)
{
    if (!beforeThis || beforeThis->parent != this)
        return 0;

    TiXmlNode* node = addThis.Clone();
    if (!node)
        return 0;
    node->parent = this;

    node->next = beforeThis;
    node->prev = beforeThis->prev;
    if (beforeThis->prev)
    {
        beforeThis->prev->next = node;
    }
    else
    {
        assert(firstChild == beforeThis);
        firstChild = node;
    }
    beforeThis->prev = node;
    return node;
}

/***************************************************************************
 * Add a new node related to this. Adds a child after the specified child. *
 * Returns a pointer to the new object or NULL if an error occured.           *
 ***************************************************************************/
TiXmlNode * TiXmlNode::InsertAfterChild(TiXmlNode * afterThis, const TiXmlNode & addThis)
{
    if (!afterThis || afterThis->parent != this)
        return 0;

    TiXmlNode* node = addThis.Clone();
    if (!node)
        return 0;
    node->parent = this;

    node->prev = afterThis;
    node->next = afterThis->next;
    if (afterThis->next)
    {
        afterThis->next->prev = node;
    }
    else
    {
        assert(lastChild == afterThis);
        lastChild = node;
    }
    afterThis->next = node;
    return node;
}

/********************************************************************
 * Replace a child of this node.                                    *
 * Returns a pointer to the new object or NULL if an error occured. *
 ********************************************************************/
TiXmlNode * TiXmlNode::ReplaceChild(TiXmlNode* replaceThis, const TiXmlNode& withThis)
{
    if (replaceThis->parent != this)
    {
        return 0;
    }

    TiXmlNode* node = withThis.Clone();
    if (!node)
    {
        return 0;
    }

    node->next = replaceThis->next;
    node->prev = replaceThis->prev;

    if (replaceThis->next)
    {
        replaceThis->next->prev = node;
    }
    else
    {
        lastChild = node;
    }

    if (replaceThis->prev)
    {
        replaceThis->prev->next = node;
    }
    else
    {
        firstChild = node;
    }

    delete replaceThis;
    node->parent = this;
    return node;
}

/********************************
 * Delete a child of this node. *
 ********************************/
bool TiXmlNode::RemoveChild(TiXmlNode * removeThis)
{
    if (removeThis->parent != this)
    {
        assert(0);
        return false;
    }

    if (removeThis->next)
    {
        removeThis->next->prev = removeThis->prev;
    }
    else
    {
        lastChild = removeThis->prev;
    }

    if (removeThis->prev)
    {
        removeThis->prev->next = removeThis->next;
    }
    else
    {
        firstChild = removeThis->next;
    }

    delete removeThis;
    return true;
}

/*******************************
 * Navigate to a sibling node. *
 *******************************/
TiXmlNode * TiXmlNode::PreviousSibling() const
{
    return prev;
}

/*******************************
 * Navigate to a sibling node. *
 *******************************/
TiXmlNode * TiXmlNode::PreviousSibling(const TCHAR * _value) const
{
    TiXmlNode * node;
    for (node = prev; node; node = node->prev)
    {
        if (node->SValue() == TIXML_STRING(_value))
        {
            return node;
        }
    }
    return 0;
}

TiXmlNode * TiXmlNode::NextSibling() const
{
    return next;
}

/******************************************************
 * Navigate to a sibling node with the given 'value'. *
 ******************************************************/
TiXmlNode * TiXmlNode::NextSibling(const TCHAR * _value) const
{
    TiXmlNode* node;
    for (node = next; node; node = node->next)
    {
        if (node->SValue() == TIXML_STRING(_value))
        {
            return node;
        }
    }
    return 0;
}

/**************************************************************
 * Convenience function to get through elements.              *
 * Calls NextSibling and ToElement. Will skip all non-Element *
 * nodes. Returns 0 if there is not another element.          *
 **************************************************************/
TiXmlElement * TiXmlNode::NextSiblingElement() const
{
    TiXmlNode* node;

    for (node = NextSibling();
        node;
        node = node->NextSibling())
    {
        if (node->ToElement())
            return node->ToElement();
    }
    return 0;
}

/**************************************************************
 * Convenience function to get through elements.              *
 * Calls NextSibling and ToElement. Will skip all non-Element *
 * nodes. Returns 0 if there is not another element.          *
 **************************************************************/
TiXmlElement * TiXmlNode::NextSiblingElement(const TCHAR * _value) const
{
    TiXmlNode* node;

    for (node = NextSibling(_value);
        node;
        node = node->NextSibling(_value))
    {
        if (node->ToElement())
            return node->ToElement();
    }
    return 0;
}

/*************************************************
 * Convenience function to get through elements. *
 *************************************************/
TiXmlElement * TiXmlNode::FirstChildElement() const
{
    TiXmlNode* node;

    for (node = FirstChild(); node; node = node->NextSibling())
    {
        if (node->ToElement())
        {
            return node->ToElement();
        }
    }
    return 0;
}

/*************************************************
 * Convenience function to get through elements. *
 *************************************************/
TiXmlElement * TiXmlNode::FirstChildElement(const TCHAR * _value) const
{
    TiXmlNode* node;

    for (node = FirstChild(_value); node; node = node->NextSibling(_value))
    {
        if (node->ToElement())
        {
            return node->ToElement();
        }
    }
    return 0;
}

/****************************************************************
 * Query the type (as an enumerated value, above) of this node.    *
 * The possible types are: DOCUMENT, ELEMENT, COMMENT,            *
 * UNKNOWN, TEXT, and DECLARATION.                                *
 ****************************************************************/
int TiXmlNode::Type() const { return type; }

/*******************************************************
 * Return a pointer to the Document this node lives in. *
 * Returns null if not in a document.                   *
 *******************************************************/
TiXmlDocument * TiXmlNode::GetDocument() const
{
    const TiXmlNode* node;

    for (node = this; node; node = node->parent)
    {
        if (node->ToDocument())
            return node->ToDocument();
    }
    return 0;
}

/**********************************************
 * Returns true if this node has no children. *
 **********************************************/
bool TiXmlNode::NoChildren() const
{
    return !firstChild;
}

/******************************************************************************
 * < Cast to a more defined type. Will return null not of the requested type. *
 ******************************************************************************/
TiXmlDocument * TiXmlNode::ToDocument()    const
{
    return (this && type == DOCUMENT) ? (TiXmlDocument*) this : 0;
}

/******************************************************************************
 * < Cast to a more defined type. Will return null not of the requested type. *
 ******************************************************************************/
TiXmlElement *  TiXmlNode::ToElement() const
{
    return (this && type == ELEMENT) ? (TiXmlElement*)  this : 0;
}

/******************************************************************************
 * < Cast to a more defined type. Will return null not of the requested type. *
 ******************************************************************************/
TiXmlComment *  TiXmlNode::ToComment() const
{
    return (this && type == COMMENT) ? (TiXmlComment*)  this : 0;
}

/******************************************************************************
 * < Cast to a more defined type. Will return null not of the requested type. *
 ******************************************************************************/
TiXmlUnknown *  TiXmlNode::ToUnknown() const
{
    return (this && type == UNKNOWN) ? (TiXmlUnknown*)  this : 0;
}

/******************************************************************************
 * < Cast to a more defined type. Will return null not of the requested type. *
 ******************************************************************************/
TiXmlText *       TiXmlNode::ToText()    const
{
    return (this && type == TEXT) ? (TiXmlText*)     this : 0;
}

/******************************************************************************
 * < Cast to a more defined type. Will return null not of the requested type. *
 ******************************************************************************/
TiXmlDeclaration * TiXmlNode::ToDeclaration() const
{
    return (this && type == DECLARATION) ? (TiXmlDeclaration*) this : 0;
}

void TiXmlNode::SetUserData(void* user)
{
    userData = user;
}

void * TiXmlNode::GetUserData()
{
    return userData;
}

TiXmlNode::TiXmlNode(NodeType _type)
{
    parent = 0;
    type = _type;
    firstChild = 0;
    lastChild = 0;
    prev = 0;
    next = 0;
    userData = 0;
}


//TiXmlNode::
//TiXmlNode::
//TiXmlNode::
//TiXmlNode::
//TiXmlNode::
//TiXmlNode::
//TiXmlNode::
//TiXmlNode::
//TiXmlNode::


