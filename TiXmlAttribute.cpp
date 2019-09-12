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

#ifdef TIXML_USE_STL
/***********************
 * string constructor. *
 ***********************/
TiXmlAttribute::TiXmlAttribute(const generic_string& _name, const generic_string& _value)
{
	name = _name;
	value = _value;
	document = 0;
	prev = next = 0;
}
#endif

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
 * QueryIntValue examines the value generic_string. It is an alternative to the	*
 * IntValue() method with richer error checking.								*
 * If the value is an integer, it is stored in 'value' and						*
 * the call returns TIXML_SUCCESS. If it is not									*
 * an integer, it returns TIXML_WRONG_TYPE.										*
 * 																				*
 * A specialized but useful call. Note that for success it returns 0,			*
 * which is the opposite of almost all other TinyXml calls.						*
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


#ifdef TIXML_USE_STL
/********************
 * STL string form. *
 ********************/
void TiXmlAttribute::SetName(const generic_string& _name)
{
	StringToBuffer buf(_name);
	SetName(buf.buffer ? buf.buffer : TEXT("error"));
}

/********************
 * STL string form.	*
 ********************/
void TiXmlAttribute::SetValue(const generic_string& _value)
{
	StringToBuffer buf(_value);
	SetValue(buf.buffer ? buf.buffer : TEXT("error"));
}
#endif

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

/******************************************************
 * [internal use]                                     *
 * Attribtue parsing starts: first letter of the name *
 * returns: the next TCHAR after the value end quote  *
 ******************************************************/
const TCHAR * TiXmlAttribute::Parse(const TCHAR * p, TiXmlParsingData* data)
{
	p = SkipWhiteSpace(p);
	if (!p || !*p) return 0;

	int tabsize = 4;
	if (document)
		tabsize = document->TabSize();

	//	TiXmlParsingData data( p, prevData );
	if (data)
	{
		data->Stamp(p);
		location = data->Cursor();
	}
	// Read the name, the '=' and the value.
	const TCHAR* pErr = p;
	p = ReadName(p, &name);
	if (!p || !*p)
	{
		if (document) document->SetError(TIXML_ERROR_READING_ATTRIBUTES, pErr, data);
		return 0;
	}
	p = SkipWhiteSpace(p);
	if (!p || !*p || *p != '=')
	{
		if (document) document->SetError(TIXML_ERROR_READING_ATTRIBUTES, p, data);
		return 0;
	}

	++p;	// skip '='
	p = SkipWhiteSpace(p);
	if (!p || !*p)
	{
		if (document) document->SetError(TIXML_ERROR_READING_ATTRIBUTES, p, data);
		return 0;
	}

	const TCHAR* end;

	if (*p == '\'')
	{
		++p;
		end = TEXT("\'");
		p = ReadText(p, &value, false, end, false);
	}
	else if (*p == '"')
	{
		++p;
		end = TEXT("\"");
		p = ReadText(p, &value, false, end, false);
	}
	else
	{
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		value = TEXT("");
		while (p && *p										// existence
			&& !_istspace(*p) && *p != '\n' && *p != '\r'	// whitespace
			&& *p != '/' && *p != '>')						// tag end
		{
			value += *p;
			++p;
		}
	}
	return p;
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
