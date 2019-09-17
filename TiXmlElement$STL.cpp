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
#include "TiXmlElement.h"

#ifdef TIXML_USE_STL
#include "TinyXmlBaseEnum.h"
#include "TiXmlText.h"
#endif

#ifdef TIXML_USE_STL
/***********************
* string constructor. *
***********************/
TiXmlElement::TiXmlElement(const generic_string& _value) : TiXmlNode(TiXmlNode::ELEMENT)
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif

#ifdef TIXML_USE_STL
const TCHAR* TiXmlElement::Attribute(const generic_string& name) const
{
	return Attribute(name.c_str());
}

const TCHAR* TiXmlElement::Attribute(const generic_string& name, int* i) const
{
	return Attribute(name.c_str(), i);
}

/********************
* STL string form. *
********************/
void TiXmlElement::SetAttribute(const generic_string& name, const generic_string& _value)
{
	StringToBuffer n(name);
	StringToBuffer v(_value);
	if (n.buffer && v.buffer)
		SetAttribute(n.buffer, v.buffer);
}
/**********************
* < STL string form. *
**********************/
void TiXmlElement::SetAttribute(const generic_string& name, int _value)
{
	StringToBuffer n(name);
	if (n.buffer)
		SetAttribute(n.buffer, _value);
}
#endif

#ifdef TIXML_USE_STL
/**********************
* < STL string form. *
**********************/
void TiXmlElement::RemoveAttribute(const generic_string& name)
{
	RemoveAttribute(name.c_str());
}
#endif

/************************************
* Used to be public [internal use] *
************************************/
#ifdef TIXML_USE_STL
void TiXmlElement::StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag)
{
	// We're called with some amount of pre-parsing. That is, some of "this"
	// element is in "tag". Go ahead and stream to the closing ">"
	while (in->good())
	{
		int c = in->get();
		(*tag) += static_cast<TCHAR>(c);

		if (c == '>')
			break;
	}

	if (tag->length() < 3) return;

	// Okay...if we are a "/>" tag, then we're done. We've read a complete tag.
	// If not, identify and stream.

	if (tag->at(tag->length() - 1) == '>'
		&& tag->at(tag->length() - 2) == '/')
	{
		// All good!
		return;
	}
	else if (tag->at(tag->length() - 1) == '>')
	{
		// There is more. Could be:
		//        text
		//        closing tag
		//        another node.
		for (;; )
		{
			StreamWhiteSpace(in, tag);

			// Do we have text?
			if (in->good() && in->peek() != '<')
			{
				// Yep, text.
				TiXmlText text(TEXT(""));
				text.StreamIn(in, tag);

				// What follows text is a closing tag or another node.
				// Go around again and figure it out.
				continue;
			}

			// We now have either a closing tag...or another node.
			// We should be at a "<", regardless.
			if (!in->good()) return;
			assert(in->peek() == '<');
			size_t tagIndex = tag->length();

			bool closingTag = false;
			bool firstCharFound = false;

			for (;; )
			{
				if (!in->good())
					return;

				int c = in->peek();

				if (c == '>')
					break;

				*tag += static_cast<TCHAR>(c);
				in->get();

				if (!firstCharFound && c != '<' && !IsWhiteSpace(c))
				{
					firstCharFound = true;
					if (c == '/')
						closingTag = true;
				}
			}
			// If it was a closing tag, then read in the closing '>' to clean up the input stream.
			// If it was not, the streaming will be done by the tag.
			if (closingTag)
			{
				int c = in->get();
				assert(c == '>');
				*tag += static_cast<TCHAR>(c);

				// We are done, once we've found our closing tag.
				return;
			}
			else
			{
				// If not a closing tag, id it, and stream.
				const TCHAR* tagloc = tag->c_str() + tagIndex;
				TiXmlNode* node = Identify(tagloc);
				if (!node)
					return;
				node->StreamIn(in, tag);
				delete node;
				node = 0;

				// No return: go around from the beginning: text, closing tag, or node.
			}
		}
	}
}
#endif
