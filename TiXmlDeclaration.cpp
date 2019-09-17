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
#include "TiXmlDeclaration.h"
#include "CommonFunctionDefine.h"

TiXmlDeclaration::TiXmlDeclaration() : TiXmlNode(TiXmlNode::DECLARATION)
{
}

TiXmlDeclaration::TiXmlDeclaration(const TCHAR * _version, const TCHAR * _encoding, const TCHAR * _standalone) : TiXmlNode(TiXmlNode::DECLARATION)
{
    version = _version;
    encoding = _encoding;
    standalone = _standalone;
}

TiXmlDeclaration::~TiXmlDeclaration()
{
}

/*************************************************
* Version. Will return empty if none was found. *
**************************************************/
const TCHAR * TiXmlDeclaration::Version() const
{
    return version.c_str();
}
/**************************************************
* Encoding. Will return empty if none was found. *
***************************************************/
const TCHAR * TiXmlDeclaration::Encoding() const
{
    return encoding.c_str();
}
/**********************************
 * Is this a standalone document? *
 **********************************/
const TCHAR * TiXmlDeclaration::Standalone() const
{
    return standalone.c_str();
}

/*******************************************************
 * [internal use] Creates a new Element and returs it. *
 *******************************************************/
TiXmlNode * TiXmlDeclaration::Clone() const
{
    TiXmlDeclaration* clone = new TiXmlDeclaration();

    if (!clone)
        return 0;

    CopyToClone(clone);
    clone->version = version;
    clone->encoding = encoding;
    clone->standalone = standalone;
    return clone;
}
/******************
 * [internal use] *
 *******************/
void TiXmlDeclaration::Print(FILE * cfile, int /*depth*/) const
{
    generic_fprintf(cfile, TEXT("<?xml "));

    if (!version.empty())
        generic_fprintf(cfile, TEXT("version=\"%s\" "), version.c_str());
    if (!encoding.empty())
        generic_fprintf(cfile, TEXT("encoding=\"%s\" "), encoding.c_str());
    if (!standalone.empty())
        generic_fprintf(cfile, TEXT("standalone=\"%s\" "), standalone.c_str());
    generic_fprintf(cfile, TEXT("?>"));
}

void TiXmlDeclaration::StreamOut(TIXML_OSTREAM * stream) const
{
    (*stream) << TEXT("<?xml ");

    if (!version.empty())
    {
        (*stream) << TEXT("version=\"");
        PutString(version, stream);
        (*stream) << TEXT("\" ");
    }
    if (!encoding.empty())
    {
        (*stream) << TEXT("encoding=\"");
        PutString(encoding, stream);
        (*stream) << TEXT("\" ");
    }
    if (!standalone.empty())
    {
        (*stream) << TEXT("standalone=\"");
        PutString(standalone, stream);
        (*stream) << TEXT("\" ");
    }
    (*stream) << TEXT("?>");
}

























































