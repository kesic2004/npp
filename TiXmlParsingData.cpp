/********************************
* 移动自：tinyxmlparser.cpp    *
* 由不明真相的吃瓜群众移动至此 *
********************************/

#include "TiXmlParsingData.h"

/***************************************************************************
 * TiXmlParsingData( const TCHAR* now, const TiXmlParsingData* prevData ); *
 ***************************************************************************/
void TiXmlParsingData::Stamp(const TCHAR* now)
{
	assert(now);

	// Do nothing if the tabsize is 0.
	if (tabsize < 1)
	{
		return;
	}

	// Get the current row, column.
	int row = cursor.row;
	int col = cursor.col;
	const TCHAR* p = stamp;
	assert(p);

	while (p < now)
	{
		// Code contributed by Fletcher Dunn: (modified by lee)
		switch (*p) {
		case 0:
			// We *should* never get here, but in case we do, don't
			// advance past the terminating null character, ever
			return;

		case '\r':
			// bump down to the next line
			++row;
			col = 0;
			// Eat the character
			++p;

			// Check for \r\n sequence, and treat this as a single character
			if (*p == '\n') {
				++p;
			}
			break;

		case '\n':
			// bump down to the next line
			++row;
			col = 0;

			// Eat the character
			++p;

			// Check for \n\r sequence, and treat this as a single
			// character.  (Yes, this bizarre thing does occur still
			// on some arcane platforms...)
			if (*p == '\r') {
				++p;
			}
			break;

		case '\t':
			// Eat the character
			++p;

			// Skip to next tab stop
			col = (col / tabsize + 1) * tabsize;
			break;

		default:
			// Eat the character
			++p;

			// Normal TCHAR - just advance one column
			++col;
			break;
		}
	}
	cursor.row = row;
	cursor.col = col;
	assert(cursor.row >= -1);
	assert(cursor.col >= -1);
	stamp = p;
	assert(stamp);
}

const TiXmlCursor& TiXmlParsingData::Cursor()
{
	return TiXmlParsingData::cursor;
}

// Only used by the document!
TiXmlParsingData(const TCHAR * start, int _tabsize, int row, int col)
{
	assert(start);
	stamp = start;
	tabsize = _tabsize;
	cursor.row = row;
	cursor.col = col;
}
