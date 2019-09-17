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
#include "TiXmlUnknown.h"
#include "TiXmlParsingData.h"
#include "TiXmlDocument.h"

 /*****************************************************
 * [internal use]                                     *
 * Attribute parsing starts: First TCHAR of the text *
 * returns: next TCHAR past '>'                         *
 *****************************************************/
const TCHAR * TiXmlUnknown::Parse(const TCHAR * p, TiXmlParsingData * data)
{
	TiXmlDocument* document = GetDocument();
	p = SkipWhiteSpace(p);

	//    TiXmlParsingData data( p, prevData );
	if (data)
	{
		data->Stamp(p);
		location = data->Cursor();
	}
	if (!p || !*p || *p != '<')
	{
		if (document) document->SetError(TIXML_ERROR_PARSING_UNKNOWN, p, data);
		return 0;
	}
	++p;
	value = TEXT("");

	while (p && *p && *p != '>')
	{
		value += *p;
		++p;
	}

	if (!p)
	{
		if (document)    document->SetError(TIXML_ERROR_PARSING_UNKNOWN, 0, 0);
	}
	if (*p == '>')
	{
		return p + 1;
	}
	return p;
}
