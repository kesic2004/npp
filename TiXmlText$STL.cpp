/********************************
 * �ƶ��ԣ�                     *
 * tinyxml.cpp                  *
 * tinyxmlparser.cpp            *
 * �ɲ�������ĳԹ�Ⱥ���ƶ����� *
 ********************************/
#include "CommonFunctionDefine.h"
#include "TiXmlText.h"
// TiXmlText.cpp
#ifdef TIXML_USE_STL
#endif

#ifdef TIXML_USE_STL
/****************
 * Constructor. *
 ****************/
TiXmlText::TiXmlText(const generic_string & initValue) : TiXmlNode(TiXmlNode::TEXT)
{
	SetValue(initValue);
}
#endif

#ifdef TIXML_USE_STL
/******************
 * [internal use] *
 ******************/
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
