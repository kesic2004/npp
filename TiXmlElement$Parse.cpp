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
#include "TinyXmlBaseEnum.h"
#include "TiXmlElement.h"
#include "TiXmlParsingData.h"
#include "TiXmlDocument.h"

 /*************************************************
 * [internal use]                                 *
 * Attribtue parsing starts: next TCHAR past '<' *
 * returns: next TCHAR past '>'                  *
 *************************************************/
const TCHAR * TiXmlElement::Parse(const TCHAR * p, TiXmlParsingData * data)
{
	p = SkipWhiteSpace(p);
	TiXmlDocument* document = GetDocument();

	if (!p || !*p)
	{
		if (document) document->SetError(TIXML_ERROR_PARSING_ELEMENT, 0, 0);
		return 0;
	}

	//    TiXmlParsingData data( p, prevData );
	if (data)
	{
		data->Stamp(p);
		location = data->Cursor();
	}

	if (*p != '<')
	{
		if (document) document->SetError(TIXML_ERROR_PARSING_ELEMENT, p, data);
		return 0;
	}

	p = SkipWhiteSpace(p + 1);

	// Read the name.
	const TCHAR* pErr = p;

	p = ReadName(p, &value);
	if (!p || !*p)
	{
		if (document)    document->SetError(TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data);
		return 0;
	}

	TIXML_STRING endTag(TEXT("</"));
	endTag += value;
	endTag += TEXT(">");

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while (p && *p)
	{
		pErr = p;
		p = SkipWhiteSpace(p);
		if (!p || !*p)
		{
			if (document) document->SetError(TIXML_ERROR_READING_ATTRIBUTES, pErr, data);
			return 0;
		}
		if (*p == '/')
		{
			++p;
			// Empty tag.
			if (*p != '>')
			{
				if (document) document->SetError(TIXML_ERROR_PARSING_EMPTY, p, data);
				return 0;
			}
			return (p + 1);
		}
		else if (*p == '>')
		{
			// Done with attributes (if there were any.)
			// Read the value -- which can include other
			// elements -- read the end tag, and return.
			++p;
			p = ReadValue(p, data);        // Note this is an Element method, and will set the error if one happens.
			if (!p || !*p)
				return 0;

			// We should find the end tag now
			if (StringEqual(p, endTag.c_str(), false))
			{
				p += endTag.length();
				return p;
			}
			else
			{
				if (document) document->SetError(TIXML_ERROR_READING_END_TAG, p, data);
				return 0;
			}
		}
		else
		{
			// Try to read an attribute:
			TiXmlAttribute* attrib = new TiXmlAttribute();
			if (!attrib)
			{
				if (document) document->SetError(TIXML_ERROR_OUT_OF_MEMORY, pErr, data);
				return 0;
			}

			attrib->SetDocument(document);
			const TCHAR* pErr = p;
			p = attrib->Parse(p, data);

			if (!p || !*p)
			{
				if (document) document->SetError(TIXML_ERROR_PARSING_ELEMENT, pErr, data);
				delete attrib;
				return 0;
			}

			// Handle the strange case of double attributes:
			TiXmlAttribute* node = attributeSet.Find(attrib->Name());
			if (node)
			{
				node->SetValue(attrib->Value());
				delete attrib;
				return 0;
			}

			attributeSet.Add(attrib);
		}
	}
	return p;
}
