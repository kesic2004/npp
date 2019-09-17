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
#include "TiXmlHandle.h"


/*****************************************************************************************
 * Create a handle from any node (at any depth of the tree.) This can be a null pointer. *
 *****************************************************************************************/
TiXmlHandle::TiXmlHandle(TiXmlNode* node)
{
    this->node = node;
}

/********************
 * Copy constructor *
 ********************/
TiXmlHandle::TiXmlHandle(const TiXmlHandle & ref)
{
    this->node = ref.node;
}

/********************************************
 * Return a handle to the first child node. *
 ********************************************/
TiXmlHandle TiXmlHandle::FirstChild() const
{
    if (node)
    {
        TiXmlNode* child = node->FirstChild();
        if (child)
            return TiXmlHandle(child);
    }
    return TiXmlHandle(0);
}

/****************************************************************
 * Return a handle to the first child node with the given name. *
 ****************************************************************/
TiXmlHandle TiXmlHandle::FirstChild(const TCHAR * value) const
{
    if (node)
    {
        TiXmlNode* child = node->FirstChild(value);
        if (child)
            return TiXmlHandle(child);
    }
    return TiXmlHandle(0);
}

/***********************************************
 * Return a handle to the first child element. *
 ***********************************************/
TiXmlHandle TiXmlHandle::FirstChildElement() const
{
    if (node)
    {
        TiXmlElement * child = node->FirstChildElement();
        if (child)
            return /*TiXmlHandle(child);*/TiXmlHandle((TiXmlNode *)child);/*自动向上强转*/
    }
    return TiXmlHandle(0);
}

/*******************************************************************
 * Return a handle to the first child element with the given name. *
 *******************************************************************/
TiXmlHandle TiXmlHandle::FirstChildElement(const TCHAR * value) const
{
    if (node)
    {
        TiXmlElement* child = node->FirstChildElement(value);
        if (child)
            return /*TiXmlHandle(child);*/TiXmlHandle((TiXmlNode *)child);/*自动向上强转*/
    }
    return TiXmlHandle(0);
}

/*************************************************************
 * Return a handle to the "index" child with the given name. *
 * The first child is 0, the second 1, etc.                  *
 *************************************************************/
TiXmlHandle TiXmlHandle::Child(const TCHAR* value, int count) const
{
    if (node)
    {
        int i;
        TiXmlNode* child = node->FirstChild(value);
        for (i = 0;
            child && i<count;
            child = child->NextSibling(value), ++i)
        {
            // nothing
        }
        if (child)
            return TiXmlHandle(child);
    }
    return TiXmlHandle(0);
}

/********************************************
 * Return a handle to the "index" child.    *
 * The first child is 0, the second 1, etc. *
 ********************************************/
TiXmlHandle TiXmlHandle::Child(int count) const
{
    if (node)
    {
        int i;
        TiXmlNode* child = node->FirstChild();
        for (i = 0;
            child && i<count;
            child = child->NextSibling(), ++i)
        {
            // nothing
        }
        if (child)
            return TiXmlHandle(child);
    }
    return TiXmlHandle(0);
}

/*********************************************************************************
 * Return a handle to the "index" child element with the given name.             *
 * The first child element is 0, the second 1, etc. Note that only TiXmlElements *
 * are indexed: other types are not counted.                                     *
 *********************************************************************************/
TiXmlHandle TiXmlHandle::ChildElement(const TCHAR * value, int count) const
{
    if (node)
    {
        int i;
        TiXmlElement* child = node->FirstChildElement(value);
        for (i = 0;
            child && i<count;
            child = /*child->NextSiblingElement(value)*/((TiXmlNode *)child)->NextSiblingElement(value), ++i)/*自动向上强转*/
        {
            // nothing
        }
        if (child)
            return /*TiXmlHandle(child);*/TiXmlHandle((TiXmlNode *)child);/*自动向上强转*/
    }
    return TiXmlHandle(0);
}

/*********************************************************************************
 * Return a handle to the "index" child element.                                 *
 * The first child element is 0, the second 1, etc. Note that only TiXmlElements *
 * are indexed: other types are not counted.                                     *
 *********************************************************************************/
TiXmlHandle TiXmlHandle::ChildElement(int count) const
{
    if (node)
    {
        int i;
        TiXmlElement* child = node->FirstChildElement();
        for (i = 0;
            child && i<count;
            child = /*child->NextSiblingElement()*/((TiXmlNode *)child)->NextSiblingElement(), ++i)/*自动向上强转*/
        {
            // nothing
        }
        if (child)
            return /*TiXmlHandle(child);*/TiXmlHandle((TiXmlNode *)child);/*自动向上强转*/
    }
    return TiXmlHandle(0);
}

/***********************************************************
 * Return the handle as a TiXmlNode. This may return null. *
 ***********************************************************/
TiXmlNode* TiXmlHandle::Node() const
{
    return node;
}

/**************************************************************
 * Return the handle as a TiXmlElement. This may return null. *
 **************************************************************/
TiXmlElement * TiXmlHandle::Element() const
{
    return ((node && node->ToElement()) ? node->ToElement() : 0);
}

/***********************************************************
 * Return the handle as a TiXmlText. This may return null. *
 ***********************************************************/
TiXmlText * TiXmlHandle::Text() const
{
    return ((node && node->ToText()) ? node->ToText() : 0);
}

































