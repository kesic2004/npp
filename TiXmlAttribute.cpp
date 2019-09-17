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
#include "TinyXmlBaseEnum.h"
#include "TiXmlAttribute.h"

/*********************************
 * Construct an empty attribute. *
 *********************************/
TiXmlAttribute::TiXmlAttribute()
{
    document = 0;
    prev = next = 0;
}

/*************************************************
 * Construct an attribute with a name and value. *
 *************************************************/
TiXmlAttribute::TiXmlAttribute(const TCHAR * _name, const TCHAR * _value)
{
    name = _name;
    value = _value;
    document = 0;
    prev = next = 0;
}

/****************************************
 * < Return the name of this attribute. *
 ****************************************/
const TCHAR * TiXmlAttribute::Name()  const
{
    return name.c_str();
}

/*****************************************
 * < Return the value of this attribute. *
 *****************************************/
const TCHAR * TiXmlAttribute::Value() const
{
    return value.c_str();
}

/******************************************************************
 * < Return the value of this attribute, converted to an integer. *
 ******************************************************************/
const int TiXmlAttribute::IntValue() const
{
    return generic_atoi(value.c_str());
}


/****************************************************************
 * < Return the value of this attribute, converted to a double. *
 ****************************************************************/
const double TiXmlAttribute::DoubleValue() const
{
    return generic_atof(value.c_str());
}


/********************************************************************************
 * QueryIntValue examines the value generic_string. It is an alternative to the *
 * IntValue() method with richer error checking.                                *
 * If the value is an integer, it is stored in 'value' and                      *
 * the call returns TIXML_SUCCESS. If it is not                                 *
 * an integer, it returns TIXML_WRONG_TYPE.                                     *
 *                                                                              *
 * A specialized but useful call. Note that for success it returns 0,           *
 * which is the opposite of almost all other TinyXml calls.                     *
 ********************************************************************************/
int TiXmlAttribute::QueryIntValue(int* ival) const
{
    if (generic_sscanf(value.c_str(), TEXT("%d"), ival) == 1)
        return TIXML_SUCCESS;
    return TIXML_WRONG_TYPE;
}

/****************************************************************************
 * QueryDoubleValue examines the value generic_string. See QueryIntValue(). *
 ****************************************************************************/
int TiXmlAttribute::QueryDoubleValue(double* dval) const
{
    if (generic_sscanf(value.c_str(), TEXT("%lf"), dval) == 1)
        return TIXML_SUCCESS;
    return TIXML_WRONG_TYPE;
}

/*************************************
 * < Set the name of this attribute. *
 *************************************/
void TiXmlAttribute::SetName(const TCHAR* _name)
{
    name = _name;
}

/********************
 * < Set the value. *
 ********************/
void TiXmlAttribute::SetValue(const TCHAR* _value)
{
    value = _value;
}

/************************************
 * < Set the value from an integer. *
 ************************************/
void TiXmlAttribute::SetIntValue(int _value)
{
    TCHAR buf[64];
    wsprintf(buf, TEXT("%d"), _value);
    SetValue(buf);
}


/**********************************
 * < Set the value from a double. *
 **********************************/
void TiXmlAttribute::SetDoubleValue(double _value)
{
    TCHAR buf[64];
    wsprintf(buf, TEXT("%lf"), _value);
    SetValue(buf);
}


/*******************************************************************
 * Get the next sibling attribute in the DOM. Returns null at end. *
 *******************************************************************/
TiXmlAttribute* TiXmlAttribute::Next() const
{
    // We are using knowledge of the sentinel. The sentinel
    // have a value or name.
    if (next->value.empty() && next->name.empty())
        return 0;
    return next;
}

/*****************************************************************************
 * Get the previous sibling attribute in the DOM. Returns null at beginning. *
 *****************************************************************************/
TiXmlAttribute* TiXmlAttribute::Previous() const
{
    // We are using knowledge of the sentinel. The sentinel
    // have a value or name.
    if (prev->value.empty() && prev->name.empty())
        return 0;
    return prev;
}


bool TiXmlAttribute::operator==(const TiXmlAttribute& rhs) const
{
    return rhs.name == name;
}

bool TiXmlAttribute::operator<(const TiXmlAttribute& rhs) const
{
    return name < rhs.name;
}

bool TiXmlAttribute::operator>(const TiXmlAttribute& rhs) const
{
    return name > rhs.name;
}



/******************
 * [internal use] *
 ******************/
void TiXmlAttribute::Print(FILE * cfile, int /*depth*/) const
{
    TIXML_STRING n, v;

    PutString(Name(), &n);
    PutString(Value(), &v);

    if (value.find('\"') == TIXML_STRING::npos)
        generic_fprintf(cfile, TEXT("%s=\"%s\""), n.c_str(), v.c_str());
    else
        generic_fprintf(cfile, TEXT("%s='%s'"), n.c_str(), v.c_str());
}

void TiXmlAttribute::StreamOut(TIXML_OSTREAM * stream) const
{
    if (value.find('\"') != TIXML_STRING::npos)
    {
        PutString(name, stream);
        (*stream) << TEXT("=") << TEXT("'");
        PutString(value, stream);
        (*stream) << TEXT("'");
    }
    else
    {
        PutString(name, stream);
        (*stream) << TEXT("=") << TEXT("\"");
        PutString(value, stream);
        (*stream) << TEXT("\"");
    }
}


/****************************************************************
 * [internal use]                                               *
 * Set the document pointer so the attribute can report errors. *
 ****************************************************************/
void TiXmlAttribute::SetDocument(TiXmlDocument* doc)
{
    document = doc;
}
