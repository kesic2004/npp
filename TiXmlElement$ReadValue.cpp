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
#include "TiXmlParsingData.h"
#include "TiXmlText.h"
#include "TiXmlDocument.h"

/*****************************************************************
 * [internal use]                                                *
 * Reads the "value" of the element -- another element, or text. *
 * This should terminate with the current end tag.               *
 *****************************************************************/
const TCHAR * TiXmlElement::ReadValue(const TCHAR * p, TiXmlParsingData * data)
{
	TiXmlDocument* document = GetDocument();

	// Read in text and elements in any order.
	p = SkipWhiteSpace(p);
	while (p && *p)
	{
		if (*p != '<')
		{
			// Take what we have, make a text element.
			TiXmlText* textNode = new TiXmlText(TEXT(""));

			if (!textNode)
			{
				if (document) document->SetError(TIXML_ERROR_OUT_OF_MEMORY, 0, 0);
				return 0;
			}

			p = textNode->Parse(p, data);

			if (!textNode->Blank())
				LinkEndChild(textNode);
			else
				delete textNode;
		}
		else
		{
			// We hit a '<'
			// Have we hit a new element or an end tag?
			if (StringEqual(p, TEXT("</"), false))
			{
				return p;
			}
			else
			{
				TiXmlNode* node = Identify(p);
				if (node)
				{
					p = node->Parse(p, data);
					LinkEndChild(node);
				}
				else
				{
					return 0;
				}
			}
		}
		p = SkipWhiteSpace(p);
	}

	if (!p)
	{
		if (document) document->SetError(TIXML_ERROR_READING_ELEMENT_VALUE, 0, 0);
	}
	return p;
}
