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
#include "TiXmlDeclaration.h"
#include "TiXmlAttribute.h"
#include "TiXmlParsingData.h"
#include "TiXmlDocument.h"


/*************************************************
 * [internal use]                                 *
 * Attribtue parsing starts: next TCHAR past '<' *
 *                  returns: next TCHAR past '>'    *
 *************************************************/
const TCHAR * TiXmlDeclaration::Parse(const TCHAR * p, TiXmlParsingData * data)
{
	p = SkipWhiteSpace(p);
	// Find the beginning, find the end, and look for
	// the stuff in-between.
	TiXmlDocument* document = GetDocument();
	if (!p || !*p || !StringEqual(p, TEXT("<?xml"), true))
	{
		if (document) document->SetError(TIXML_ERROR_PARSING_DECLARATION, 0, 0);
		return 0;
	}
	//    TiXmlParsingData data( p, prevData );
	if (data)
	{
		data->Stamp(p);
		location = data->Cursor();
	}
	p += 5;

	version = TEXT("");
	encoding = TEXT("");
	standalone = TEXT("");

	while (p && *p)
	{
		if (*p == '>')
		{
			++p;
			return p;
		}

		p = SkipWhiteSpace(p);
		if (StringEqual(p, TEXT("version"), true))
		{
			TiXmlAttribute attrib;
			p = attrib.Parse(p, data);
			version = attrib.Value();
		}
		else if (StringEqual(p, TEXT("encoding"), true))
		{
			TiXmlAttribute attrib;
			p = attrib.Parse(p, data);
			encoding = attrib.Value();
		}
		else if (StringEqual(p, TEXT("standalone"), true))
		{
			TiXmlAttribute attrib;
			p = attrib.Parse(p, data);
			standalone = attrib.Value();
		}
		else
		{
			// Read over whatever it is.
			while (p && *p && *p != '>' && !_istspace(*p))
				++p;
		}
	}
	return 0;
}
