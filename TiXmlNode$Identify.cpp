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
/*************
 * _istalpha *
 *************/
#include <tchar.h>
#include "TiXmlNode.h"
#include "TiXmlDeclaration.h"
#include "TiXmlDocument.h"
#include "TiXmlComment.h"
#include "TiXmlElement.h"
#include "TiXmlUnknown.h"

 /**********************************************************************************
 * Figure out what is at *p, and parse it. Returns null if it is not an xml node. *
 **********************************************************************************/
TiXmlNode * TiXmlNode::Identify(const TCHAR * p)
{
	TiXmlNode* returnNode = 0;

	p = SkipWhiteSpace(p);
	if (!p || !*p || *p != '<')
	{
		return 0;
	}

	TiXmlDocument* doc = GetDocument();
	p = SkipWhiteSpace(p);

	if (!p || !*p)
	{
		return 0;
	}

	// What is this thing? 
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	//

	const TCHAR* xmlHeader = { TEXT("<?xml") };
	const TCHAR* commentHeader = { TEXT("<!--") };

	if (StringEqual(p, xmlHeader, true))
	{
#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Declaration\n");
#endif
		returnNode = new TiXmlDeclaration();
	}
	else if (_istalpha(*(p + 1))
		|| *(p + 1) == '_')
	{
#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Element\n");
#endif
		returnNode = new TiXmlElement(TEXT(""));
	}
	else if (StringEqual(p, commentHeader, false))
	{
#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Comment\n");
#endif
		returnNode = new TiXmlComment();
	}
	else
	{
#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Unknown\n");
#endif
		returnNode = new TiXmlUnknown();
	}

	if (returnNode)
	{
		// Set the parent, so it can report errors
		returnNode->parent = this;
	}
	else
	{
		if (doc)
			doc->SetError(TIXML_ERROR_OUT_OF_MEMORY, 0, 0);
	}
	return returnNode;
}
