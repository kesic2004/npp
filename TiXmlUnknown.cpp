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

TiXmlUnknown::TiXmlUnknown() : TiXmlNode(TiXmlNode::UNKNOWN)
{
}

TiXmlUnknown::~TiXmlUnknown()
{
}

/******************
 * [internal use] *
 ******************/
TiXmlNode * TiXmlUnknown::Clone() const
{
    TiXmlUnknown* clone = new TiXmlUnknown();

    if (!clone)
    {
        return 0;
    }

    CopyToClone(clone);
    return clone;
}

/******************
 * [internal use] *
 ******************/
void TiXmlUnknown::Print(FILE * cfile, int depth) const
{
    for (int i = 0; i<depth; i++)
        generic_fprintf(cfile, TEXT("    "));
    generic_fprintf(cfile, TEXT("%s"), value.c_str());
}

void TiXmlUnknown::StreamOut(TIXML_OSTREAM * stream) const
{
    (*stream) << TEXT("<") << value << TEXT(">");        // Don't use entities hear! It is unknown.
}
























