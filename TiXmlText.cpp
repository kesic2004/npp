/********************************
 * 移动自：                     *
 * tinyxml.cpp                  *
 * tinyxmlparser.cpp            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
#include "CommonFunctionDefine.h"
#include "TiXmlText.h"
// TiXmlText.cpp

/****************
 * Constructor. *
 ****************/
TiXmlText::TiXmlText(const TCHAR * initValue) : TiXmlNode(TiXmlNode::TEXT)
{
	SetValue(initValue);
}
TiXmlText::~TiXmlText()
{
}

#ifdef TIXML_USE_STL
/****************
 * Constructor. *
 ****************/
TiXmlText(const generic_string& initValue) : TiXmlNode(TiXmlNode::TEXT)
{
	SetValue(initValue);
}
#endif

/******************
 * [internal use] *
 ******************/
void TiXmlText::Print(FILE* cfile, int /*depth*/) const
{
	TIXML_STRING buffer;
	PutString(value, &buffer);
	generic_fprintf(cfile, TEXT("%s"), buffer.c_str());
}

/********************************************************
 * [internal use] Creates a new Element and returns it. *
 ********************************************************/
TiXmlNode * TiXmlText::Clone() const
{
	TiXmlText* clone = 0;
	clone = new TiXmlText(TEXT(""));

	if (!clone)
		return 0;

	CopyToClone(clone);
	return clone;
}

void TiXmlText::StreamOut(TIXML_OSTREAM * stream) const
{
	PutString(value, stream);
}

/*************************************************
 * [internal use]                                *
 * returns true if all white space and new lines *
 *************************************************/
bool TiXmlText::Blank() const
{
	for (size_t i = 0, len = value.length(); i < len; i++)
		if (!_istspace(value[i]))
			return false;
	return true;
}

/*****************************************************
 * internal use]									 *
 * Attribtue parsing starts: First TCHAR of the text *
 * returns: next TCHAR past '>'                      *
 *****************************************************/
const TCHAR * TiXmlText::Parse(const TCHAR * p, TiXmlParsingData * data)
{
	value = TEXT("");
	//	TiXmlParsingData data( p, prevData );
	if (data)
	{
		data->Stamp(p);
		location = data->Cursor();
	}
	bool ignoreWhite = true;

	const TCHAR* end = TEXT("<");
	p = ReadText(p, &value, ignoreWhite, end, false);
	if (p)
		return p - 1;	// don't truncate the '<'
	return 0;
}

/******************
 * [internal use] *
 ******************/
#ifdef TIXML_USE_STL
void TiXmlText::StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag)
{
	while (in->good())
	{
		int c = in->peek();
		if (c == '<')
			return;

		(*tag) += static_cast<TCHAR>(c);
		in->get();
	}
}
#endif





