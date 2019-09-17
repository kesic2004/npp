/********************************
 * 移动自：tinystrA.h           *
 * 由不明真相的吃瓜群众移动至此 *
 *******************************/
#ifndef __REAL_KESIC_LEE_TI_XML_STRINGA_H__
#define __REAL_KESIC_LEE_TI_XML_STRINGA_H__

#include <Windows.h>
#include <cassert>

#ifdef TIXMLA_USE_STL
#define TIXMLA_STRING	std::string
#define TIXMLA_ISTREAM	std::istream
#define TIXMLA_OSTREAM	std::ostream
#else
#define TIXMLA_STRING	TiXmlStringA
#define TIXMLA_OSTREAM	TiXmlOutStreamA
#endif

#ifndef TIXMLA_USE_STL

#ifndef _INCLUDED
#define TIXMLA_STRING_INCLUDED
#pragma warning( disable : 4514 )
 /*
 TiXmlStringA is an emulation of the std::string template.
 Its purpose is to allow compiling TinyXML on compilers with no or poor STL support.
 Only the member functions relevant to the TinyXML project have been implemented.
 The buffer allocation is made by a simplistic power of 2 like mechanism : if we increase
 a string and there's no more room, we allocate a buffer twice as big as we need.
 */
class TiXmlStringA
{
public:
	// TiXmlStringA constructor, based on a string
	TiXmlStringA(const char * instring);

	// TiXmlStringA empty constructor
	TiXmlStringA();

	// TiXmlStringA copy constructor
	TiXmlStringA(const TiXmlStringA& copy);

	// TiXmlStringA destructor
	~TiXmlStringA();

	// Convert a TiXmlStringA into a classical char *
	const char * c_str() const;

	// Return the length of a TiXmlStringA
	unsigned length() const;

	// TiXmlStringA = operator
	void operator = (const char * content);

	// = operator
	void operator = (const TiXmlStringA & copy);

	// += operator. Maps to append
	TiXmlStringA& operator += (const char * suffix);

	// += operator. Maps to append
	TiXmlStringA& operator += (char single);

	// += operator. Maps to append
	TiXmlStringA& operator += (TiXmlStringA & suffix);

	bool operator == (const TiXmlStringA & compare) const;

	bool operator < (const TiXmlStringA & compare) const;

	bool operator > (const TiXmlStringA & compare) const;

	// Checks if a TiXmlStringA is empty
	bool empty() const;

	// Checks if a TiXmlStringA contains only whitespace (same rules as isspace)
	// Not actually used in tinyxml. Conflicts with a C macro, "isblank",
	// which is a problem. Commenting out. -lee
	//    bool isblank () const;

	// single char extraction
	const char & at(unsigned index) const;

	// find a char in a string. Return TiXmlStringA::notfound if not found
	unsigned find(char lookup) const;

	// find a char in a string from an offset. Return TiXmlStringA::notfound if not found
	unsigned find(char tofind, unsigned offset) const;

	/*	Function to reserve a big amount of data when we know we'll need it. Be aware that this
	function clears the content of the TiXmlStringA if any exists.
	*/
	void reserve(unsigned size);

	// [] operator 
	char & operator [] (unsigned index) const;

	// Error value for find primitive 
	enum {
		notfound = 0xffffffff,
		npos = notfound
	};

	void append(const char * str, int len);

protected:

	// The base string
	char * cstring;
	// Number of chars allocated
	unsigned allocated;
	// Current string size
	unsigned current_length;

	// New size computation. It is simplistic right now : it returns twice the amount
	// we need
	unsigned assign_new_size(unsigned minimum_to_allocate);

	// Internal function that clears the content of a TiXmlStringA
	void empty_it();

	void append(const char *suffix);

	// append function for another TiXmlStringA
	void append(const TiXmlStringA & suffix);

	// append for a single char. This could be improved a lot if needed
	void append(char single);

};
#endif	// TIXMLA_STRING_INCLUDED
#endif	// TIXMLA_USE_STL

#endif // !__REAL_KESIC_LEE_TI_XML_STRINGA_H__


