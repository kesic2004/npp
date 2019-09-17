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
/****************************************************************************
 * �ƶ��ԣ�tinyxmlparser.cpp                                                *
 * �ɲ�������ĳԹ�Ⱥ���ƶ�����                                             *
 ****************************************************************************/


#ifndef __REAL_KESIC_LEE_TI_XML_PARSING_DATA_H__
#define __REAL_KESIC_LEE_TI_XML_PARSING_DATA_H__ 1

#include "TiXmlCursor.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

class TiXmlParsingData
{
    friend class TiXmlDocument;
public:
    /****************************************************************************
     * TiXmlParsingData( const TCHAR* now, const TiXmlParsingData * prevData ); *
     ****************************************************************************/
    void Stamp(const TCHAR * now);

    const TiXmlCursor& Cursor();
    //void Update( const TCHAR* now );

private:
    /****************************************************************************
     * Only used by the document!                                               *
     ****************************************************************************/
    TiXmlParsingData(const TCHAR * start, int _tabsize, int row, int col);

    TiXmlCursor        cursor;
    const TCHAR *    stamp;
    int                tabsize;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_PARSING_DATA_H__


