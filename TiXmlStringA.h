#ifndef __REAL_KESIC_LEE_TI_XML_STRING_A_H__
#define __REAL_KESIC_LEE_TI_XML_STRING_A_H__ 1

#ifndef TIXMLA_USE_STL

/********************************************************************************************
 * TiXmlStringA is an emulation of the std::string template.                                *
 * Its purpose is to allow compiling TinyXML on compilers with no or poor STL support.      *
 * Only the member functions relevant to the TinyXML project have been implemented.         *
 * The buffer allocation is made by a simplistic power of 2 like mechanism : if we increase *
 * a string and there's no more room, we allocate a buffer twice as big as we need.         *
 ********************************************************************************************/
class TiXmlStringA
{
public:
	// TiXmlStringA constructor, based on a string
	/*explicit */TiXmlStringA(const char * instring);

	// TiXmlStringA empty constructor
	TiXmlStringA()
	{
		allocated = 0;
		cstring = NULL;
		current_length = 0;
	}

	// TiXmlStringA copy constructor
	TiXmlStringA(const TiXmlStringA & copy);
	TiXmlStringA(const TiXmlStringA && copy) = delete;

	// TiXmlStringA destructor
	~TiXmlStringA()
	{
		empty_it();
	}

	// Convert a TiXmlStringA into a classical char *
	const char * c_str() const
	{
		if (allocated)
			return cstring;
		return "";
	}

	// Return the length of a TiXmlStringA
	unsigned length() const
	{
		return (allocated) ? current_length : 0;
	}

	// TiXmlStringA = operator
	void operator = (const char * content);

	// = operator
	void operator = (const TiXmlStringA & copy);
	void operator = (const TiXmlStringA && copy) = delete;

	// += operator. Maps to append
	TiXmlStringA& operator += (const char * suffix)
	{
		append(suffix);
		return *this;
	}

	// += operator. Maps to append
	TiXmlStringA& operator += (char single)
	{
		append(single);
		return *this;
	}

	// += operator. Maps to append
	TiXmlStringA& operator += (TiXmlStringA & suffix)
	{
		append(suffix);
		return *this;
	}
	TiXmlStringA& operator += (TiXmlStringA && suffix) = delete;

	bool operator == (const TiXmlStringA & compare) const;
	bool operator < (const TiXmlStringA & compare) const;
	bool operator > (const TiXmlStringA & compare) const;

	// Checks if a TiXmlStringA is empty
	bool empty() const
	{
		return length() ? false : true;
	}

	// Checks if a TiXmlStringA contains only whitespace (same rules as isspace)
	// Not actually used in tinyxml. Conflicts with a C macro, "isblank",
	// which is a problem. Commenting out. -lee
	//    bool isblank () const;

	// single char extraction
	const char & at(unsigned index) const
	{
		assert(index < length());
		return cstring[index];
	}

	// find a char in a string. Return TiXmlStringA::notfound if not found
	unsigned find(char lookup) const
	{
		return find(lookup, 0);
	}

	// find a char in a string from an offset. Return TiXmlStringA::notfound if not found
	unsigned find(char tofind, unsigned offset) const;

	/*	Function to reserve a big amount of data when we know we'll need it. Be aware that this
	function clears the content of the TiXmlStringA if any exists.
	*/
	void reserve(unsigned size)
	{
		empty_it();
		if (size)
		{
			allocated = size;
//			TiXmlStringA cstring = new char[size];
			TiXmlStringA::cstring = new char[size];
			cstring[0] = 0;
			current_length = 0;
		}
	}

	// [] operator 
	char& operator [] (unsigned index) const
	{
		assert(index < length());
		return cstring[index];
	}

	// Error value for find primitive 
	enum {
		notfound = 0xffffffff,
		npos = notfound
	};

	void append(const char *str, int len);

protected:

	// The base string
	char * cstring;
	// Number of chars allocated
	unsigned allocated;
	// Current string size
	unsigned current_length;

	// New size computation. It is simplistic right now : it returns twice the amount
	// we need
	unsigned assign_new_size(unsigned minimum_to_allocate)
	{
		return minimum_to_allocate * 2;
	}

	// Internal function that clears the content of a TiXmlStringA
	void empty_it()
	{
		if (cstring)
			delete[] cstring;
		cstring = NULL;
		allocated = 0;
		current_length = 0;
	}

	void append(const char *suffix);

	// append function for another TiXmlStringA
	void append(const TiXmlStringA & suffix)
	{
		append(suffix.c_str());
	}

	// append for a single char. This could be improved a lot if needed
	void append(char single)
	{
		char smallstr[2];
		smallstr[0] = single;
		smallstr[1] = 0;
		append(smallstr);
	}
private:
	//static void memcpy(void * _Dst, void const * _Src, size_t _Size);
	static void memcpy(char * _Dst, const char * _Src, size_t _Size);
	static void lichepi(char * _Dst);
};

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

#endif	// TIXMLA_USE_STL

#endif // !__REAL_KESIC_LEE_TI_XML_STRING_A_H__
