/********************************
* �ƶ��ԣ�tinyxml.h            *
* �ɲ�������ĳԹ�Ⱥ���ƶ����� *
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
