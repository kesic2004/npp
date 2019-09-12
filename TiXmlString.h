/********************************
 * 移动自：tinystr.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
/********************************
 * TiXmlString.h                *
 ********************************/

#ifndef __REAL_KESIC_LEE_TI_XML_STRING_H__
#define __REAL_KESIC_LEE_TI_XML_STRING_H__ 1

#include <cassert>
#include <Windows.h>

#ifndef TIXML_USE_STL

#pragma warning( disable : 4514 )


/********************************************************************************************
 * TiXmlString is an emulation of the string template.									    *
 * Its purpose is to allow compiling TinyXML on compilers with no or poor STL support.	    *
 * Only the member functions relevant to the TinyXML project have been implemented.		    *
 * The buffer allocation is made by a simplistic power of 2 like mechanism : if we increase *
 * a generic_string and there's no more room, we allocate a buffer twice as big as we need. *
 ********************************************************************************************/
class TiXmlString
{
public:
	/******************************************************
	 * TiXmlString constructor, based on a generic_string *
	 ******************************************************/
	TiXmlString(const TCHAR * instring);

	/*********************************
	 * TiXmlString empty constructor *
	 *********************************/
		TiXmlString();

	/********************************
	 * TiXmlString copy constructor *
	 ********************************/
	TiXmlString(const TiXmlString& copy);

	/**************************
	 * TiXmlString destructor *
	 **************************/
	~TiXmlString();

	/************************************************
	 * Convert a TiXmlString into a classical TCHAR *
	 ************************************************/
	const TCHAR * c_str() const;

	/**************************************
	 * Return the length of a TiXmlString *
	 **************************************/
	unsigned length() const;

	/***************************
	 *  TiXmlString = operator *
	 ***************************/
	void operator = (const TCHAR * content);

	/**************
	 * = operator *
	 **************/
	void operator = (const TiXmlString & copy);

	/*******************************
	 * += operator. Maps to append *
	 *******************************/
	TiXmlString& operator += (const TCHAR * suffix);

	/*******************************
	 * += operator. Maps to append *
	 *******************************/
	TiXmlString& operator += (TCHAR single);

	/*******************************
	 * += operator. Maps to append *
	 *******************************/
	TiXmlString& operator += (TiXmlString & suffix);

	bool operator == (const TiXmlString & compare) const;
	bool operator < (const TiXmlString & compare) const;
	bool operator > (const TiXmlString & compare) const;

	/************************************
	 * Checks if a TiXmlString is empty *
	 ************************************/
	bool empty() const;

	/****************************************************************************
	 * Checks if a TiXmlString contains only whitespace (same rules as isspace) *
	 * Not actually used in tinyxml. Conflicts with a C macro, TEXT("isblank"), *
	 * which is a problem. Commenting out. -lee                                 *
	*****************************************************************************/
	/* bool isblank () const; */

	/***************************
	 * single TCHAR extraction *
	 ***************************/
	const TCHAR& at(unsigned index) const;

	/*******************************************************************************
	 * find a TCHAR in a generic_string. Return TiXmlString::notfound if not found *
	 *******************************************************************************/
	unsigned find(TCHAR lookup) const;

	/**********************************************************************************************
	 * find a TCHAR in a generic_string from an offset. Return TiXmlString::notfound if not found *
	 **********************************************************************************************/
	unsigned find(TCHAR tofind, unsigned offset) const;

	/*******************************************************************************************
	 * Function to reserve a big amount of data when we know we'll need it. Be aware that this *
	 * function clears the content of the TiXmlString if any exists.                           *
	 *******************************************************************************************/
	void reserve(unsigned size);

	/***************
	 * [] operator * 
	 ***************/
	TCHAR& operator [] (unsigned index) const;

	/**********************************
	 * Error value for find primitive *
	 **********************************/
	enum
	{
		notfound = 0xffffffff,
		npos = notfound
	};

	/*****************************************************
	 * append a const TCHAR * to an existing TiXmlString *
	 *****************************************************/
	void append(const TCHAR *str, int len);

protected:

	/***************************
	 * The base generic_string *
	 ***************************/
	TCHAR * cstring;

	/*****************************
	 * Number of chars allocated *
	 *****************************/
	unsigned allocated;

	/*******************************
	 * Current generic_string size *
	 *******************************/
	unsigned current_length;

	/**********************************************************************************
	 * New size computation. It is simplistic right now : it returns twice the amount *
	 * we need                                                                        *
	 **********************************************************************************/
	unsigned assign_new_size(unsigned minimum_to_allocate);

	/**************************************************************
	 * Internal function that clears the content of a TiXmlString *
	 **************************************************************/
	void empty_it();

	/*****************************************************
	 * append a const TCHAR * to an existing TiXmlString *
	 *****************************************************/
	void append(const TCHAR *suffix);

	/*******************************************
	 * append function for another TiXmlString *
	 *******************************************/
	void append(const TiXmlString & suffix);

	// append for a single TCHAR. This could be improved a lot if needed
	void append(TCHAR single);

};

/*******************************************************************************
 * TiXmlOutStream is an emulation of std::ostream. It is based on TiXmlString. *
 * Only the operators that we need for TinyXML have been developped.           *
 *******************************************************************************/
class TiXmlOutStream : public TiXmlString
{
public:
	TiXmlOutStream();

	/***********************************************************
	 * TiXmlOutStream << operator. Maps to TiXmlString::append *
	 ***********************************************************/
	TiXmlOutStream & operator << (const TCHAR * in);

	/***********************************************************
	 * TiXmlOutStream << operator. Maps to TiXmlString::append *
	 ***********************************************************/
	TiXmlOutStream & operator << (const TiXmlString & in);
};

#endif	// TIXML_USE_STL

#endif // __REAL_KESIC_LEE_TI_XML_STRING_H__
