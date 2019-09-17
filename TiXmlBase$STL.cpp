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
/*************************************************************************************************
 * 移动自：                                                                                      *
 * tinyxml.h                                                                                     *
 * tinyxml.cpp                                                                                   *
 * tinyxmlparser.cpp                                                                             *
 * 由不明真相的吃瓜群众移动至此                                                                  *
 *************************************************************************************************/

#include "TiXmlBase.h"
#ifdef TIXML_USE_STL
#include "CommonFunctionDefine.h"
#include <tchar.h>
#endif


#ifdef TIXML_USE_STL
bool TiXmlBase::StreamWhiteSpace(TIXML_ISTREAM * in, TIXML_STRING * tag)
{
	for (;; )
	{
		if (!in->good()) return false;

		int c = in->peek();
		if (!IsWhiteSpace(c))
			return true;
		*tag += static_cast<TCHAR>(in->get());
	}
}

bool TiXmlBase::StreamTo(TIXML_ISTREAM * in, int character, TIXML_STRING * tag)
{
	while (in->good())
	{
		int c = in->peek();
		if (c == character)
			return true;

		in->get();
		*tag += static_cast<TCHAR>(c);
	}
	return false;
}
#endif
