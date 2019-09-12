/********************************
 * �ƶ��ԣ�tinyxml.h            *
 * �ɲ�������ĳԹ�Ⱥ���ƶ����� *
 ********************************/

#ifndef __REAL_KESIC_LEE_TINY_XML_BASE_ENUM_H__
#define __REAL_KESIC_LEE_TINY_XML_BASE_ENUM_H__ 1

#include "TinyXmlBaseHeader.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/*******************************************
 * Only used by Attribute::Query functions *
 *******************************************/
enum
{
	TIXML_SUCCESS,
	TIXML_NO_ATTRIBUTE,
	TIXML_WRONG_TYPE
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TINY_XML_BASE_ENUM_H__

