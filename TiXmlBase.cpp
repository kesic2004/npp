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

#include "CommonFunctionDefine.h"
#include "TiXmlBase.h"
#include <tchar.h>


TiXmlBase::TiXmlBase()
{
}

TiXmlBase::~TiXmlBase()
{
}

/******************************************************************************
 * The world does not agree on whether white space should be kept or          *
 * not. In order to make everyone happy, these global, static functions          *
 * are provided to set whether or not TinyXml will condense all white space      *
 * into a single space or not. The default is to condense. Note changing this *
 * values is not thread safe.                                                 *
 ******************************************************************************/
void TiXmlBase::SetCondenseWhiteSpace(bool condense)
{
    condenseWhiteSpace = condense;
}

/*******************************************
 * Return the current white space setting. *
 *******************************************/
bool TiXmlBase::IsWhiteSpaceCondensed()
{
    return condenseWhiteSpace;
}

/**************************************************************************************
 * Return the position, in the original source file, of this node or attribute.          *
 * The row and column are 1-based. (That is the first row and first column is          *
 * 1,1). If the returns values are 0 or less, then the parser does not have              *
 * a row and column value.                                                              *
 *                                                                                       *
 * Generally, the row and column value will be set when the TiXmlDocument::Load(),      *
 * TiXmlDocument::LoadFile(), or any TiXmlNode::Parse() is called. It will NOT be set *
 * when the DOM was created from operator>>.                                          *
 *                                                                                       *
 * The values reflect the initial load. Once the DOM is modified programmatically      *
 * (by adding or changing nodes and attributes) the new values will NOT update to      *
 * reflect changes in the document.                                                      *
 *                                                                                       *
 * There is a minor performance cost to computing the row and column. Computation      *
 * can be disabled if TiXmlDocument::SetTabSize() is called with 0 as the value.      *
 *                                                                                       *
 * @sa TiXmlDocument::SetTabSize()                                                      *
 **************************************************************************************/
int TiXmlBase::Row() const
{
    return location.row + 1;
}

/**************
 * @See Row() *
 **************/
int TiXmlBase::Column() const
{
    return location.col + 1;
}

/**********************************************************
 * Strange class for a bug fix. Search for STL_STRING_BUG *
 **********************************************************/
TiXmlBase::StringToBuffer::StringToBuffer(const TIXML_STRING & str)
{
    const size_t strLen = str.length() + 1;
    buffer = new TCHAR[strLen];
    if (buffer)
    {
        wcscpy_s(buffer, strLen, str.c_str());
    }
}

TiXmlBase::StringToBuffer::~StringToBuffer()
{
    delete[] buffer;
}

const TCHAR * TiXmlBase::SkipWhiteSpace(const TCHAR * p)
{
    if (!p || !*p)
    {
        return 0;
    }
    while (p && *p)
    {
        // Still using old rules for white space.
        if (_istspace(*p) || *p == '\n' || *p == '\r')
        {
            ++p;
        }
        else
        {
            break;
        }
    }

    return p;
}

bool TiXmlBase::IsWhiteSpace(int c)
{
    return (_istspace(static_cast<TCHAR>(c)) || c == '\n' || c == '\r');
}

/***************************************************************
 * Reads an XML name into the generic_string provided. Returns *
 * a pointer just past the last character of the name,         *
 * or 0 if the function has an error.                          *
 ***************************************************************/
const TCHAR * TiXmlBase::ReadName(const TCHAR * p, TIXML_STRING * name)
{
    *name = TEXT("");
    assert(p);

    /*************************************************************
     * Names start with letters or underscores.                     *
     * After that, they can be letters, underscores, numbers,     *
     * hyphens, or colons. (Colons are valid ony for namespaces, *
     * but tinyxml can't tell namespaces from names.)            *
     *************************************************************/
    if ( (p) && (*p) && ( ( _istalpha(*p) ) || ( *p == '_') ) )
    {
        while ( (p) && (*p) && ( ( _istalnum(*p) ) || (*p == '_') || (*p == '-') || (*p == '.') || (*p == ':') ) )
        {
            (*name) += *p;
            ++p;
        }
        return p;
    }
    return 0;
}

const TCHAR* TiXmlBase::ReadText(
    const TCHAR  * p,                // where to start
    TIXML_STRING * text,            // the generic_string read
    bool           trimWhiteSpace,    // whether to keep the white space
    const TCHAR  * endTag,            // what ends this text
    bool           caseInsensitive    // whether to ignore case in the end tag
)
{
    *text = TEXT("");
    /*****************************************************************
     * @param trimWhiteSpace     certain tags always keep whitespace *
     * @param condenseWhiteSpace if true, whitespace is always kept     *
     *****************************************************************/
    if ( (!trimWhiteSpace) || (!condenseWhiteSpace) )    
    {
        /*****************************
         * Keep all the white space. *
         *****************************/
        while ( (p) && (*p) && ( !StringEqual(p, endTag, caseInsensitive) ) )
        {
            TCHAR c;
            p = GetChar(p, &c);
            (*text) += c;
        }
    }
    else
    {
        bool whitespace = false;

        /*******************************
         * Remove leading white space: *
        ********************************/
        p = SkipWhiteSpace(p);

        while ( (p) && (*p) && ( !StringEqual(p, endTag, caseInsensitive) ) )
        {
            if ( (*p == '\r') || (*p == '\n') )
            {
                whitespace = true;
                ++p;
            }
            else if (_istspace(*p))
            {
                whitespace = true;
                ++p;
            }
            else
            {
                /*******************************************************
                 * If we've found whitespace, add it before the        *
                 * new character. Any whitespace just becomes a space. *
                 *******************************************************/
                if (whitespace)
                {
                    (*text) += ' ';
                    whitespace = false;
                }
                TCHAR c;
                p = GetChar(p, &c);
                (*text) += c;
            }
        }
    }
    return p + lstrlen(endTag);
}

/***************************************************************
 * If an entity has been found, transform it into a character. *
 ***************************************************************/
const TCHAR* TiXmlBase::GetEntity(const TCHAR* p, TCHAR* value)
{
    /***************************************
     * Presume an entity, and pull it out. *
     ***************************************/
    int i;

    /****************************
     * Handle the &#x entities. *
     ****************************/
    if (generic_strncmp(TEXT("&#x"), p, 3) == 0)
    {
        const TCHAR * end = generic_strchr(p + 3, TEXT(';'));
        if ( (end) && ( end - p <= 7 /*3 + 4*/ ) )
        {
            TCHAR * hexend;
            auto val = generic_strtol(p + 3, &hexend, 16);
            if (hexend == end)
            {
                *value = static_cast<TCHAR>(val);
                return end + 1;
            }
        }
    }

    /************************
     * Now try to match it. *
     ************************/
    for (i = 0; i<NUM_ENTITY; ++i)
    {
        if (generic_strncmp(entity[i].str, p, entity[i].strLength) == 0)
        {
            assert(static_cast<unsigned int>(lstrlen(entity[i].str)) == entity[i].strLength);
            *value = entity[i].chr;
            return (p + entity[i].strLength);
        }
    }

    /*********************************************************************
     * So it wasn't an entity, its unrecognized, or something like that. *
     * Don't put back the last one, since we return it!                  *
     *********************************************************************/
    *value = *p;
    return p + 1;
}

/*************************************************
 * Get a character, while interpreting entities. *
 *************************************************/
const TCHAR * TiXmlBase::GetChar(const TCHAR* p, TCHAR* _value)
{
    assert(p);
    if ( *p == '&' )
    {
        return GetEntity(p, _value);
    }
    else
    {
        *_value = *p;
        return p + 1;
    }
}




/**********************************************************************************************
* Puts a generic_string to a stream, expanding entities as it goes.                          *
* Note this should not contian the '<', '>', etc, or they will be transformed into entities! *
**********************************************************************************************/
void TiXmlBase::PutString(const TIXML_STRING & str, TIXML_OSTREAM * stream)
{
    TIXML_STRING buffer;
    PutString(str, &buffer);
    (*stream) << buffer;
}

void TiXmlBase::PutString(const TIXML_STRING& str, TIXML_STRING* outString)
{
    size_t i = 0;

    while (i < str.length())
    {
        int c = str[i];

        if ( (c == '&') && ( i < (str.length() - 2) ) && ( str[i + 1] == '#' ) && ( str[i + 2] == 'x' ) )
        {
            /**********************************************
             * Hexadecimal character reference.              *
             * Pass through unchanged.                      *
             * &#xA9;    -- copyright symbol, for example. *
             **********************************************/
            while (i < str.length())
            {
                outString->append(str.c_str() + i, 1);
                ++i;
                if (str[i] == ';')
                    break;
            }
        }
        else if (c == '&')
        {
            outString->append(entity[0].str, entity[0].strLength);
            ++i;
        }
        else if (c == '<')
        {
            outString->append(entity[1].str, entity[1].strLength);
            ++i;
        }
        else if (c == '>')
        {
            outString->append(entity[2].str, entity[2].strLength);
            ++i;
        }
        else if (c == '\"')
        {
            outString->append(entity[3].str, entity[3].strLength);
            ++i;
        }
        else if (c == '\'')
        {
            outString->append(entity[4].str, entity[4].strLength);
            ++i;
        }
        else if (c < 32 || c > 126)
        {
            /************************************************
             * Easy pass at non-alpha/numeric/symbol        *
             * 127 is the delete key. Below 32 is symbolic.    *
             ************************************************/
            TCHAR buf[32];
            wsprintf(buf, TEXT("&#x%04X;"), static_cast<unsigned int>(c & 0xffff));
            outString->append(buf, lstrlen(buf));
            ++i;
        }
        else
        {
            TCHAR realc = static_cast<TCHAR>(c);
            outString->append(&realc, 1);
            ++i;
        }
    }
}

bool TiXmlBase::condenseWhiteSpace = true;

/**********************************************************
 * Note tha "PutString" hardcodes the same list. This      *
 * is less flexible than it appears. Changing the entries *
 * or order will break putstring.                          *
 **********************************************************/
TiXmlBase::Entity TiXmlBase::entity[NUM_ENTITY] =
{
    { TEXT("&amp;"),  5, '&'  },
    { TEXT("&lt;"),   4, '<'  },
    { TEXT("&gt;"),   4, '>'  },
    { TEXT("&quot;"), 6, '\"' },
    { TEXT("&apos;"), 6, '\'' }
};

/*************************************************************************************
 * Return true if the next characters in the stream are any of the endTag sequences. *
 *************************************************************************************/
bool TiXmlBase::StringEqual(const TCHAR * p, const TCHAR * tag, bool ignoreCase)
{
    assert(p);
    if (!p || !*p)
    {
        assert(0);
        return false;
    }

    if (_totlower(*p) == _totlower(*tag))
    {
        const TCHAR* q = p;

        if (ignoreCase)
        {
            while (*q && *tag && *q == *tag)
            {
                ++q;
                ++tag;
            }

            if (*tag == 0)        // Have we found the end of the tag, and everything equal?
            {
                return true;
            }
        }
        else
        {
            while (*q && *tag && _totlower(*q) == _totlower(*tag))
            {
                ++q;
                ++tag;
            }

            if (*tag == 0)
            {
                return true;
            }
        }
    }
    return false;
}

const TCHAR * TiXmlBase::errorString[TIXML_ERROR_STRING_COUNT] =
{
	TEXT("No error"),
	TEXT("Error"),
	TEXT("Failed to open file"),
	TEXT("Memory allocation failed."),
	TEXT("Error parsing Element."),
	TEXT("Failed to read Element name"),
	TEXT("Error reading Element value."),
	TEXT("Error reading Attributes."),
	TEXT("Error: empty tag."),
	TEXT("Error reading end tag."),
	TEXT("Error parsing Unknown."),
	TEXT("Error parsing Comment."),
	TEXT("Error parsing Declaration."),
	TEXT("Error document empty.")
};
