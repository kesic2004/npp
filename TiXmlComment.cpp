/********************************
 * 移动自：                     *
 * tinyxml.cpp                  *
 * tinyxmlparser.cpp            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/

#include <sstream>
#include "CommonFunctionDefine.h"
#include "TiXmlComment.h"

/********************************
 * Constructs an empty comment. *
 ********************************/
TiXmlComment::TiXmlComment() : TiXmlNode(TiXmlNode::COMMENT)
{
}

TiXmlComment::~TiXmlComment()
{
}

/*******************************************************
 * [internal use] Creates a new Element and returs it. *
 *******************************************************/
TiXmlNode * TiXmlComment::Clone() const
{
	TiXmlComment* clone = new TiXmlComment();

	if (!clone)
		return 0;

	CopyToClone(clone);
	return clone;
}


/******************
 * [internal use] *
 ******************/
void TiXmlComment::Print(FILE * cfile, int depth) const
{
	for (int i = 0; i<depth; i++)
	{
		fputs("    ", cfile);
	}
	generic_fprintf(cfile, TEXT("<!--%s-->"), value.c_str());
}


#ifdef TIXML_USE_STL
void TiXmlComment::StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag)
{
	while (in->good())
	{
		int c = in->get();
		(*tag) += static_cast<TCHAR>(c);

		if (c == '>'
			&& tag->at(tag->length() - 2) == '-'
			&& tag->at(tag->length() - 3) == '-')
		{
			// All is well.
			return;
		}
	}
}

#endif

void TiXmlComment::StreamOut(TIXML_OSTREAM * stream) const
{
	(*stream) << TEXT("<!--");
	PutString(value, stream);
	(*stream) << TEXT("-->");
}


/*************************************************
 * [internal use]								 *
 * Attribtue parsing starts: at the ! of the !-- *
 * returns: next TCHAR past '>'					 *
 *************************************************/
const TCHAR* TiXmlComment::Parse(const TCHAR* p, TiXmlParsingData* data)
{
	TiXmlDocument* document = GetDocument();
	value = TEXT("");

	p = SkipWhiteSpace(p);

	//	TiXmlParsingData data( p, prevData );
	if (data)
	{
		data->Stamp(p);
		location = data->Cursor();
	}
	const TCHAR* startTag = TEXT("<!--");
	const TCHAR* endTag = TEXT("-->");

	if (!StringEqual(p, startTag, false))
	{
		document->SetError(TIXML_ERROR_PARSING_COMMENT, p, data);
		return 0;
	}
	p += lstrlen(startTag);
	p = ReadText(p, &value, false, endTag, false);
	return p;
}
