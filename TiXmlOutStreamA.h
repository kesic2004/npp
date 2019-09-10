/********************************
* 移动自：tinystrA.h           *
* 由不明真相的吃瓜群众移动至此 *
*******************************/
#ifndef __REAL_KESIC_LEE_TI_XML_OUTSTREAMA_H__
#define __REAL_KESIC_LEE_TI_XML_OUTSTREAMA_H__

#include "TiXmlStringA.h"

/*
TiXmlOutStreamA is an emulation of std::ostream. It is based on TiXmlStringA.
Only the operators that we need for TinyXML have been developped.
*/
class TiXmlOutStreamA : public TiXmlStringA
{
public:
	TiXmlOutStreamA() : TiXmlStringA() {}

	// TiXmlOutStreamA << operator. Maps to TiXmlStringA::append
	TiXmlOutStreamA & operator << (const char * in)
	{
		append(in);
		return (*this);
	}

	// TiXmlOutStreamA << operator. Maps to TiXmlStringA::append
	TiXmlOutStreamA & operator << (const TiXmlStringA & in)
	{
		append(in.c_str());
		return (*this);
	}
};


#endif
