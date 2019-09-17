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
#include "TiXmlDocument.h"

 /****************************************************************
 * Sets an attribute of name to a given value. The attribute    *
 * will be created if it does not exist, or changed if it does. *
 ****************************************************************/
void TiXmlElement::SetAttribute(const TCHAR * name, const TCHAR * _value)
{
	TiXmlAttribute* node = attributeSet.Find(name);
	if (node)
	{
		node->SetValue(_value);
		return;
	}

	TiXmlAttribute* attrib = new TiXmlAttribute(name, _value);
	if (attrib)
	{
		attributeSet.Add(attrib);
	}
	else
	{
		TiXmlDocument* document = GetDocument();
		if (document) document->SetError(TIXML_ERROR_OUT_OF_MEMORY, 0, 0);
	}
}

