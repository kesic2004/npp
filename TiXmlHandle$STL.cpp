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
#include "TiXmlHandle.h"

#ifdef TIXML_USE_STL
TiXmlHandle TiXmlHandle::FirstChild(const generic_string& _value) const
{
	return FirstChild(_value.c_str());
}

TiXmlHandle TiXmlHandle::FirstChildElement(const generic_string& _value) const
{
	return FirstChildElement(_value.c_str());
}

TiXmlHandle TiXmlHandle::Child(const generic_string& _value, int index) const
{
	return Child(_value.c_str(), index);
}

TiXmlHandle TiXmlHandle::ChildElement(const generic_string& _value, int index) const
{
	return ChildElement(_value.c_str(), index);
}
#endif
