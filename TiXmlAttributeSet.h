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
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
#ifndef __REAL_KESIC_LEE_TI_XML_ATTRIBUTE_SET_H__
#define __REAL_KESIC_LEE_TI_XML_ATTRIBUTE_SET_H__ 1

#include "TiXmlAttribute.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/******************************************************************************
 * A class used to manage a group of attributes.                              *
 * It is only used internally, both by the ELEMENT and the DECLARATION.       *
 *                                                                            *
 * The set can be changed transparent to the Element and Declaration          *
 * classes that use it, but NOT transparent to the Attribute                  *
 * which has to implement a next() and previous() method. Which makes         *
 * it a bit problematic and prevents the use of STL.                          *
 *                                                                            *
 * This version is implemented with circular lists because:                   *
 * - I like circular lists                                                    *
 * - it demonstrates some independence from the (typical) doubly linked list. *
 ******************************************************************************/
class TiXmlAttributeSet
{
public:
    TiXmlAttributeSet();
    ~TiXmlAttributeSet();

    void Add(TiXmlAttribute * attribute);
    void Remove(TiXmlAttribute * attribute);

    TiXmlAttribute * First() const;
    TiXmlAttribute * Last() const;
    TiXmlAttribute * Find(const TCHAR * name) const;

private:
    TiXmlAttribute sentinel;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_ATTRIBUTE_SET_H__

