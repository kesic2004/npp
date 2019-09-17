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
/***********************************************************
 * 移动自：tinyxml.h                                       *
 * 由不明真相的吃瓜群众移动至此                            *
 ***********************************************************/
//TiXmlUnknown.h
#ifndef __REAL_KESIC_LEE_TI_XML_UNKNOWN_H__
#define __REAL_KESIC_LEE_TI_XML_UNKNOWN_H__ 1

#include "TiXmlNode.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/****************************************************************
 * Any tag that tinyXml doesn't recognize is saved as an        *
 * unknown. It is a tag of text, but should not be modified.    *
 * It will be written back to the XML, unchanged, when the file *
 * is saved.                                                    *
 ****************************************************************/
class TiXmlUnknown : public TiXmlNode
{
public:
    TiXmlUnknown();
    virtual ~TiXmlUnknown();

    /******************
     * [internal use] *
     ******************/
    virtual TiXmlNode* Clone() const;
    /******************
     * [internal use] *
     ******************/
    virtual void Print(FILE* cfile, int depth) const;
protected:
#ifdef TIXML_USE_STL
    virtual void StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag);
#endif
    virtual void StreamOut(TIXML_OSTREAM * out) const;
    /*****************************************************
     * [internal use]                                    *
     * Attribute parsing starts: First TCHAR of the text *
     * returns: next TCHAR past '>'                      *
     *****************************************************/
    virtual const TCHAR* Parse(const TCHAR * p, TiXmlParsingData * data);
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_UNKNOWN_H__
