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

#include "TiXmlComment.h"
#include "TiXmlParsingData.h"
#include "TiXmlDocument.h"

 /*************************************************
  * [internal use]                                *
  * Attribtue parsing starts: at the ! of the !-- *
  * returns: next TCHAR past '>'                  *
  *************************************************/
const TCHAR* TiXmlComment::Parse(const TCHAR* p, TiXmlParsingData* data)
{
	TiXmlDocument* document = GetDocument();
	value = TEXT("");

	p = SkipWhiteSpace(p);

	//    TiXmlParsingData data( p, prevData );
	if (data)
	{
		data->Stamp(p);
		location = data->Cursor();
	}
	const TCHAR* startTag = TEXT("<!--");
	const TCHAR* endTag = TEXT("-->");

	if (!StringEqual(p, startTag, false))
	{
		document->SetError(TIXML_ERROR_PARSING_COMMENT, p, data);
		return 0;
	}
	p += lstrlen(startTag);
	p = ReadText(p, &value, false, endTag, false);
	return p;
}
