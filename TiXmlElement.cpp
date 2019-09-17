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
#include "CommonFunctionDefine.h"
#include "TinyXmlBaseEnum.h"
#include "TiXmlElement.h"
/*************************
 * Construct an element. *
 *************************/
TiXmlElement::TiXmlElement(const TCHAR * _value) : TiXmlNode(TiXmlNode::ELEMENT)
{
    firstChild = lastChild = 0;
    value = _value;
}

TiXmlElement::~TiXmlElement()
{
    while (attributeSet.First())
    {
        TiXmlAttribute* node = attributeSet.First();
        attributeSet.Remove(node);
        delete node;
    }
}

/***********************************************************
 * Given an attribute name, Attribute() returns the value  *
 * for the attribute of that name, or null if none exists. *
 ***********************************************************/
const TCHAR * TiXmlElement::Attribute(const TCHAR * name) const
{
    TiXmlAttribute* node = attributeSet.Find(name);

    if (node)
        return node->Value();

    return 0;
}

/***************************************************************
 * Given an attribute name, Attribute() returns the value      *
 * for the attribute of that name, or null if none exists.     *
 * If the attribute exists and can be converted to an integer, *
 * the integer value will be put in the return 'i', if 'i'     *
 * is non-null.                                                *
 ***************************************************************/
const TCHAR * TiXmlElement::Attribute(const TCHAR * name, int* i) const
{
    const TCHAR * s = Attribute(name);
    if (i)
    {
        if (s)
            *i = generic_atoi(s);
        else
            *i = 0;
    }
    return s;
}

/**************************************************************
 * Given an attribute name, Attribute() returns the value      *
 * for the attribute of that name, or null if none exists.      *
 * If the attribute exists and can be converted to an double, *
 * the double value will be put in the return 'd', if 'd'      *
 * is non-null.                                                  *
 **************************************************************/
const TCHAR * TiXmlElement::Attribute(const TCHAR * name, double * d) const
{
    const TCHAR * s = Attribute(name);
    if (d)
    {
        if (s)
        {
            *d = generic_atof(s);
        }
        else
        {
            *d = 0;
        }
    }
    return s;
}

/**************************************************************************
 * QueryIntAttribute examines the attribute - it is an alternative to the *
 * Attribute() method with richer error checking.                          *
 * If the attribute is an integer, it is stored in 'value' and              *
 * the call returns TIXML_SUCCESS. If it is not                              *
 * an integer, it returns TIXML_WRONG_TYPE. If the attribute              *
 * does not exist, then TIXML_NO_ATTRIBUTE is returned.                      *
 **************************************************************************/
int TiXmlElement::QueryIntAttribute(const TCHAR * name, int * ival) const
{
    TiXmlAttribute* node = attributeSet.Find(name);
    if (!node)
        return TIXML_NO_ATTRIBUTE;

    return node->QueryIntValue(ival);
}

/**************************************************************************
 * QueryDoubleAttribute examines the attribute - see QueryIntAttribute(). *
 **************************************************************************/
int TiXmlElement::QueryDoubleAttribute(const TCHAR * name, double * dval) const
{
    TiXmlAttribute* node = attributeSet.Find(name);
    if (!node)
        return TIXML_NO_ATTRIBUTE;

    return node->QueryDoubleValue(dval);
}

/****************************************************************
 * Sets an attribute of name to a given value. The attribute    *
 * will be created if it does not exist, or changed if it does. *
 ****************************************************************/
void TiXmlElement::SetAttribute(const TCHAR * name, int val)
{
    TCHAR buf[64];
    wsprintf(buf, TEXT("%d"), val);
    SetAttribute(name, buf);
}

/*********************************************
 * Deletes an attribute with the given name. *
 *********************************************/
void TiXmlElement::RemoveAttribute(const TCHAR * name)
{
    TiXmlAttribute* node = attributeSet.Find(name);
    if (node)
    {
        attributeSet.Remove(node);
        delete node;
    }
}

/*************************************************
 * < Access the first attribute in this element. *
 *************************************************/
TiXmlAttribute* TiXmlElement::FirstAttribute() const { return attributeSet.First(); }

/************************************************
 * < Access the last attribute in this element. *
 ************************************************/
TiXmlAttribute* TiXmlElement::LastAttribute()    const { return attributeSet.Last(); }

/*******************************************************
 * [internal use] Creates a new Element and returs it. *
 *******************************************************/
TiXmlNode * TiXmlElement::Clone() const
{
    TiXmlElement* clone = new TiXmlElement(Value());
    if (!clone)
        return 0;

    CopyToClone(clone);

    // Clone the attributes, then clone the children.
    TiXmlAttribute* attribute = 0;
    for (attribute = attributeSet.First();
        attribute;
        attribute = attribute->Next())
    {
        clone->SetAttribute(attribute->Name(), attribute->Value());
    }

    TiXmlNode* node = 0;
    for (node = firstChild; node; node = node->NextSibling())
    {
        clone->LinkEndChild(node->Clone());
    }
    return clone;
}

/******************
 * [internal use] *
 ******************/
void TiXmlElement::Print(FILE * cfile, int depth) const
{
    int i;
    for (i = 0; i<depth; i++)
    {
        generic_fprintf(cfile, TEXT("    "));
    }

    generic_fprintf(cfile, TEXT("<%s"), value.c_str());

    TiXmlAttribute* attrib;
    for (attrib = attributeSet.First(); attrib; attrib = attrib->Next())
    {
        generic_fprintf(cfile, TEXT(" "));
        attrib->Print(cfile, depth);
    }

    // There are 3 different formatting approaches:
    // 1) An element without children is printed as a <foo /> node
    // 2) An element with only a text child is printed as <foo> text </foo>
    // 3) An element with children is printed on multiple lines.
    TiXmlNode* node;
    if (!firstChild)
    {
        generic_fprintf(cfile, TEXT(" />"));
    }
    else if (firstChild == lastChild && firstChild->ToText())
    {
        generic_fprintf(cfile, TEXT(">"));
        firstChild->Print(cfile, depth + 1);
        generic_fprintf(cfile, TEXT("</%s>"), value.c_str());
    }
    else
    {
        generic_fprintf(cfile, TEXT(">"));

        for (node = firstChild; node; node = node->NextSibling())
        {
            if (!node->ToText())
            {
                generic_fprintf(cfile, TEXT("\n"));
            }
            node->Print(cfile, depth + 1);
        }
        generic_fprintf(cfile, TEXT("\n"));
        for (i = 0; i<depth; ++i)
            generic_fprintf(cfile, TEXT("    "));
        generic_fprintf(cfile, TEXT("</%s>"), value.c_str());
    }
}



void TiXmlElement::StreamOut(TIXML_OSTREAM * stream) const
{
    (*stream) << TEXT("<") << value;

    TiXmlAttribute* attrib;
    for (attrib = attributeSet.First(); attrib; attrib = attrib->Next())
    {
        (*stream) << TEXT(" ");
        attrib->StreamOut(stream);
    }

    // If this node has children, give it a closing tag. Else
    // make it an empty tag.
    TiXmlNode* node;
    if (firstChild)
    {
        (*stream) << TEXT(">");

        for (node = firstChild; node; node = node->NextSibling())
        {
            node->StreamOut(stream);
        }
        (*stream) << TEXT("</") << value << TEXT(">");
    }
    else
    {
        (*stream) << TEXT(" />");
    }
}














