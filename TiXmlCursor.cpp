/********************************
* 移动自：tinyxml.h            *
* 由不明真相的吃瓜群众移动至此 *
********************************/

#include "TiXmlCursor.h"

TiXmlCursor::TiXmlCursor()
{
	Clear();
}

void TiXmlCursor::Clear()
{
	row = col = -1;
}
