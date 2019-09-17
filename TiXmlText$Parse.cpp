/********************************
* 移动自：                     *
* tinyxml.cpp                  *
* tinyxmlparser.cpp            *
* 由不明真相的吃瓜群众移动至此 *
********************************/
#include "CommonFunctionDefine.h"
#include "TiXmlText.h"
#include "TiXmlParsingData.h"
// TiXmlText.cpp

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
