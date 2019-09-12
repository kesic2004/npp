/********************************
 * 移动自：tinyxmlparser.cpp    *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/


#ifndef __REAL_KESIC_LEE_TI_XML_PARSING_DATA_H__
#define __REAL_KESIC_LEE_TI_XML_PARSING_DATA_H__ 1

#include <tchar.h>
#include "TiXmlCursor.h"

class TiXmlParsingData
{
	friend class TiXmlDocument;
public:
	/***************************************************************************
	 * TiXmlParsingData( const TCHAR* now, const TiXmlParsingData* prevData ); *
	 ***************************************************************************/
	void Stamp(const TCHAR * now);

	const TiXmlCursor& Cursor();
	//void Update( const TCHAR* now );

private:
	// Only used by the document!
	TiXmlParsingData(const TCHAR * start, int _tabsize, int row, int col);

	TiXmlCursor		cursor;
	const TCHAR *	stamp;
	int				tabsize;
};

#endif // !__REAL_KESIC_LEE_TI_XML_PARSING_DATA_H__


#pragma once
