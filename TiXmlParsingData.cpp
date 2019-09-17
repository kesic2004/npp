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
 * 移动自：tinyxmlparser.cpp    *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/

#include "TiXmlParsingData.h"

/***************************************************************************
 * TiXmlParsingData( const TCHAR* now, const TiXmlParsingData* prevData ); *
 ***************************************************************************/
void TiXmlParsingData::Stamp(const TCHAR * now)
{
    assert(now);

    // Do nothing if the tabsize is 0.
    if (tabsize < 1)
    {
        return;
    }

    // Get the current row, column.
    int row = cursor.row;
    int col = cursor.col;
    const TCHAR * p = stamp;
    assert(p);

    while (p < now)
    {
        // Code contributed by Fletcher Dunn: (modified by lee)
        switch (*p) {
        case 0:
            // We *should* never get here, but in case we do, don't
            // advance past the terminating null character, ever
            return;

        case '\r':
            // bump down to the next line
            ++row;
            col = 0;
            // Eat the character
            ++p;

            // Check for \r\n sequence, and treat this as a single character
            if (*p == '\n') {
                ++p;
            }
            break;

        case '\n':
            // bump down to the next line
            ++row;
            col = 0;

            // Eat the character
            ++p;

            // Check for \n\r sequence, and treat this as a single
            // character.  (Yes, this bizarre thing does occur still
            // on some arcane platforms...)
            if (*p == '\r') {
                ++p;
            }
            break;

        case '\t':
            // Eat the character
            ++p;

            // Skip to next tab stop
            col = (col / tabsize + 1) * tabsize;
            break;

        default:
            // Eat the character
            ++p;

            // Normal TCHAR - just advance one column
            ++col;
            break;
        }
    }
    cursor.row = row;
    cursor.col = col;
    assert(cursor.row >= -1);
    assert(cursor.col >= -1);
    stamp = p;
    assert(stamp);
}

const TiXmlCursor& TiXmlParsingData::Cursor()
{
    return TiXmlParsingData::cursor;
}

// Only used by the document!
TiXmlParsingData::TiXmlParsingData(const TCHAR * start, int _tabsize, int row, int col)
{
    assert(start);
    stamp = start;
    tabsize = _tabsize;
    cursor.row = row;
    cursor.col = col;
}
