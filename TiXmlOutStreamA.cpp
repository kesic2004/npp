#include "TiXmlOutStreamA.h"

#ifndef TIXMLA_USE_STL

#ifndef _INCLUDED
#define TIXMLA_STRING_INCLUDED
#pragma warning( disable : 4514 )

TiXmlOutStreamA::TiXmlOutStreamA() : TiXmlStringA()
{
}

// TiXmlOutStreamA << operator. Maps to TiXmlStringA::append
TiXmlOutStreamA & TiXmlOutStreamA::operator << (const char * in)
{
	append(in);
	return (*this);
}

// TiXmlOutStreamA << operator. Maps to TiXmlStringA::append
TiXmlOutStreamA & TiXmlOutStreamA::operator << (const TiXmlStringA & in)
{
	append(in.c_str());
	return (*this);
}

#endif	// TIXMLA_STRING_INCLUDED
#endif	// TIXMLA_USE_STL



