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
#include "CommonFunctionDefine.h"
#include "TiXmlDocument.h"
#include "TiXmlParsingData.h"

/******************************************************
 * Parse the given null terminated block of xml data. *
 ******************************************************/
const TCHAR * TiXmlDocument::Parse(const TCHAR * p, TiXmlParsingData * prevData)
{
	ClearError();

	// Parse away, at the document level. Since a document
	// contains nothing but other tags, most of what happens
	// here is skipping white space.
	if (!p || !*p)
	{
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0, 0);
		return 0;
	}

	// Note that, for a document, this needs to come
	// before the while space skip, so that parsing
	// starts from the pointer we are given.
	location.Clear();
	if (prevData)
	{
		location.row = prevData->cursor.row;
		location.col = prevData->cursor.col;
	}
	else
	{
		location.row = 0;
		location.col = 0;
	}
	TiXmlParsingData data(p, TabSize(), location.row, location.col);
	location = data.Cursor();

	p = SkipWhiteSpace(p);
	if (!p)
	{
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0, 0);
		return 0;
	}

	while (p && *p)
	{
		TiXmlNode* node = Identify(p);
		if (node)
		{
			p = node->Parse(p, &data);
			LinkEndChild(node);
		}
		else
		{
			break;
		}
		p = SkipWhiteSpace(p);
	}

	// All is well.
	return p;
}
