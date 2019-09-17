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
#ifndef __REAL_KESIC_LEE_TI_XML_DECLARATION_H__
#define __REAL_KESIC_LEE_TI_XML_DECLARATION_H__ 1
// TiXmlDeclaration.h
#include "TiXmlNode.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif


/*********************************************************************
 * In correct XML the declaration is the first entry in the file.    *
 * @verbatim                                                         *
 * <?xml version="1.0" standalone="yes"?>                            *
 * @endverbatim                                                      *
 *                                                                   *
 * TinyXml will happily read or write files without a declaration,   *
 * however. There are 3 possible attributes to the declaration:      *
 * version, encoding, and standalone.                                *
 *                                                                   *
 * Note: In this version of the code, the attributes are             *
 * handled as special cases, not generic attributes, simply          *
 * because there can only be at most 3 and they are always the same. *
 *********************************************************************/
class TiXmlDeclaration : public TiXmlNode
{
public:
    /***********************************
     * Construct an empty declaration. *
     ***********************************/
    TiXmlDeclaration();

#ifdef TIXML_USE_STL
    /****************
     * Constructor. *
     ****************/
    TiXmlDeclaration(const generic_string & _version, const generic_string & _encoding, const generic_string & _standalone);
#endif

    /**************
     * Construct. *
     **************/
    TiXmlDeclaration(const TCHAR * _version, const TCHAR * _encoding, const TCHAR * _standalone);

	virtual ~TiXmlDeclaration();

    /*************************************************
     * Version. Will return empty if none was found. *
     *************************************************/
    const TCHAR * Version() const;
    /**************************************************
     * Encoding. Will return empty if none was found. *
     **************************************************/
    const TCHAR * Encoding() const;
    /**********************************
     * Is this a standalone document? *
     **********************************/
    const TCHAR * Standalone() const;

    /*******************************************************
     * [internal use] Creates a new Element and returs it. *
     *******************************************************/
    virtual TiXmlNode* Clone() const;
    /******************
     * [internal use] *
     ******************/
    virtual void Print(FILE * cfile, int depth) const;

protected:
#ifdef TIXML_USE_STL
    /*********************
     * used to be public *
     *********************/
    virtual void StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag);
#endif
    virtual void StreamOut(TIXML_OSTREAM * out) const;

    /*************************************************
     * [internal use]                                *
     * Attribtue parsing starts: next TCHAR past '<' *
     *                  returns: next TCHAR past '>' *
     *************************************************/
    virtual const TCHAR * Parse(const TCHAR * p, TiXmlParsingData * data);

private:
    TIXML_STRING version;
    TIXML_STRING encoding;
    TIXML_STRING standalone;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_DECLARATION_H__


