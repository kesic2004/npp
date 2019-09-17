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
#ifdef TIXML_USE_STL 
#include <sstream>
#endif

#ifdef TIXML_USE_STL    
/***********************************************************************
 * An input stream operator, for every class. Tolerant of newlines and *
 * formatting, but doesn't expect them.                                *
 ***********************************************************************/
TIXML_ISTREAM & operator >> (TIXML_ISTREAM & in, TiXmlNode & base)
{
	TIXML_STRING tag;
	tag.reserve(8 * 1000);
	base.StreamIn(&in, &tag);

	base.Parse(tag.c_str(), 0);
	return in;
}

/************************************************************************
 * An output stream operator, for every class. Note that this outputs   *
 * without any newlines or formatting, as opposed to Print(), which     *
 * includes tabs and new lines.                                         *
 *                                                                      *
 * The operator<< and operator>> are not completely symmetric. Writing  *
 * a node to a stream is very well defined. You'll get a nice stream    *
 * of output, without any extra whitespace or newlines.                 *
 *                                                                      *
 * But reading is not as well defined. (As it always is.) If you create *
 * a TiXmlElement (for example) and read that from an input stream,     *
 * the text needs to define an element or junk will result. This is     *
 * true of all input streams, but it's worth keeping in mind.           *
 *                                                                      *
 * A TiXmlDocument will read nodes until it reads a root element, and   *
 * all the children of that root element.                               *
 ************************************************************************/
TIXML_OSTREAM & operator << (TIXML_OSTREAM & out, const TiXmlNode & base)
{
	base.StreamOut(&out);
	return out;
}

/**************************************************
 * Appends the XML node or attribute to a string. *
 **************************************************/
generic_string & operator<< (generic_string & out, const TiXmlNode & base)
{
	/************************************************************
	 * std::ostringstream os_stream( std::ostringstream::out ); *
	 ************************************************************/
	std::basic_ostringstream<TCHAR> os_stream(std::ostringstream::out);
	base.StreamOut(&os_stream);

	out.append(os_stream.str());
	return out;
}
#endif

#ifdef TIXML_USE_STL
/********************
* STL string form. *
********************/
void TiXmlNode::SetValue(const generic_string & _value)
{
	StringToBuffer buf(_value);
	SetValue(buf.buffer ? buf.buffer : TEXT(""));
}
#endif

#ifdef TIXML_USE_STL
/***********************************
* < STL std::generic_string form. *
***********************************/
TiXmlNode * TiXmlNode::FirstChild(const generic_string& _value) const
{
	return FirstChild(_value.c_str());
}

/***********************************
* < STL std::generic_string form. *
***********************************/
TiXmlNode * TiXmlNode::LastChild(const generic_string& _value) const
{
	return LastChild(_value.c_str());
}
#endif

#ifdef TIXML_USE_STL
/*********************************
* STL std::generic_string form. *
*********************************/
TiXmlNode * TiXmlNode::IterateChildren(const generic_string & _value, TiXmlNode * previous) const
{
	return IterateChildren(_value.c_str(), previous);
}
#endif

#ifdef TIXML_USE_STL
/***********************************
* < STL std::generic_string form. *
***********************************/
TiXmlNode * TiXmlNode::PreviousSibling(const generic_string& _value) const
{
	return PreviousSibling(_value.c_str());
}

/***********************************
* < STL std::generic_string form. *
***********************************/
TiXmlNode * TiXmlNode::NextSibling(const generic_string& _value) const
{
	return NextSibling(_value.c_str());
}
#endif

#ifdef TIXML_USE_STL
/**********************************
*< STL std::generic_string form. *
**********************************/
TiXmlElement * TiXmlNode::NextSiblingElement(const generic_string & _value) const
{
	return NextSiblingElement(_value.c_str());
}
#endif

#ifdef TIXML_USE_STL
/***********************************
* < STL std::generic_string form. *
***********************************/
TiXmlElement * TiXmlNode::FirstChildElement(const generic_string & _value) const
{
	return FirstChildElement(_value.c_str());
}
#endif
