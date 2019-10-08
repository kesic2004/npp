#include <cassert>
#include <windows.h>
#include "CommonGenericStdFunctionDefine.h"
#include "TiXmlStringA.h"





#ifndef TIXMLA_USE_STL

#define MAX_SIZE 1048576 // 1024 * 1024

// TiXmlStringA constructor, based on a C string
TiXmlStringA::TiXmlStringA(const char* instring)
{
	unsigned newlen;
	char * newstring = nullptr;

	if (!instring)
	{
		allocated = 0;
		cstring = NULL;
		current_length = 0;
		return;
	}
	newlen = strlen(instring) + 1;
	newstring = new char[newlen];

//	memset(newstring, 0, newlen);
	memcpy(newstring, instring, newlen);
	// strcpy (newstring, instring);
	allocated = newlen;
	cstring = newstring;
	current_length = newlen - 1;
}

// TiXmlStringA copy constructor
TiXmlStringA::TiXmlStringA(const TiXmlStringA& copy)
{
	unsigned newlen;
	char * newstring = nullptr;

	// Prevent copy to self!
	if (&copy == this)
		return;

	if (!copy.allocated)
	{
		allocated = 0;
		cstring = NULL;
		current_length = 0;
		return;
	}
	newlen = copy.length() + 1;
	newstring = new char[newlen];
//	memset(newstring, 0, newlen);
	// strcpy (newstring, copy . cstring);
	memcpy(newstring, copy.cstring, newlen);
	allocated = newlen;
	cstring = newstring;
	current_length = newlen - 1;
}

// TiXmlStringA = operator. Safe when assign own content
void TiXmlStringA ::operator = (const char * content)
{
	unsigned newlen;
	char * newstring = nullptr;

	if (!content)
	{
		empty_it();
		return;
	}
	newlen = strlen(content) + 1;
	newstring = new char[newlen];
//	memset(newstring, 0, newlen);
	// strcpy (newstring, content);
	memcpy(newstring, content, newlen);
	empty_it();
	allocated = newlen;
	cstring = newstring;
	current_length = newlen - 1;
}

// = operator. Safe when assign own content
void TiXmlStringA ::operator = (const TiXmlStringA & copy)
{
	unsigned newlen;
	char * newstring = nullptr;

	if (!copy.length())
	{
		empty_it();
		return;
	}
	newlen = copy.length() + 1;
	newstring = new char[newlen];
//	memset(newstring, 0, newlen);
	// strcpy (newstring, copy . c_str ());
	memcpy(newstring, copy.c_str(), newlen);
	empty_it();
	allocated = newlen;
	cstring = newstring;
	current_length = newlen - 1;
}


//// Checks if a TiXmlStringA contains only whitespace (same rules as isspace)
//bool TiXmlStringA::isblank () const
//{
//    char * lookup;
//    for (lookup = cstring; * lookup; lookup++)
//        if (! isspace (* lookup))
//            return false;
//    return true;
//}

// append a const char * to an existing TiXmlStringA
void TiXmlStringA::append(const char* str, int len)
{
	char * new_string = nullptr;
	unsigned new_alloc, new_size, size_suffix;

	size_suffix = strlen(str);
	if (len < (int)size_suffix)
		size_suffix = len;
	if (!size_suffix)
		return;

	new_size = length() + size_suffix + 1;
	// check if we need to expand
	if (new_size > allocated)
	{
		// compute new size
		new_alloc = assign_new_size(new_size);

		// allocate new buffer
		new_string = new char[new_alloc];
//		memset(new_string, 0, new_alloc);
		new_string[0] = 0;

		// copy the previous allocated buffer into this one
		if (allocated && cstring)
			// strcpy (new_string, cstring);
			memcpy(new_string, cstring, length());

		// append the suffix. It does exist, otherwize we wouldn't be expanding 
		// strncat (new_string, str, len);
		memcpy(new_string + length(),
			str,
			size_suffix);

		// return previsously allocated buffer if any
		if (allocated && cstring)
			delete[] cstring;

		// update member variables
		cstring = new_string;
		allocated = new_alloc;
	}
	else
	{
		// we know we can safely append the new string
		// strncat (cstring, str, len);
		memcpy(cstring + length(),
			str,
			size_suffix);
	}
	current_length = new_size - 1;
	cstring[current_length] = 0;
}


// append a const char * to an existing TiXmlStringA
void TiXmlStringA::append(const char * suffix)
{
	char * new_string = nullptr;
	unsigned new_alloc, new_size;

	new_size = length() + strlen(suffix) + 1;
	// check if we need to expand
	if (new_size > allocated)
	{
		// compute new size
		new_alloc = assign_new_size(new_size);

		// allocate new buffer
		new_string = new char[new_alloc];
//		memset(new_string, 0, new_alloc);
		new_string[0] = 0;

		// copy the previous allocated buffer into this one
		if (allocated && cstring)
			memcpy(new_string, cstring, 1 + length());
		// strcpy (new_string, cstring);

		// append the suffix. It does exist, otherwize we wouldn't be expanding 
		memcpy(new_string + length(),
			suffix,
			strlen(suffix) + 1);

		// return previsously allocated buffer if any
		if (allocated && cstring)
			delete[] cstring;

		// update member variables
		cstring = new_string;
		allocated = new_alloc;
	}
	else
	{
		//if (cstring == nullptr)
		//{
		//	cstring = new char[max(current_length, length() * 2) + 1];
		//}
		// we know we can safely append the new string
		memcpy(cstring + length(),
			suffix,
			strlen(suffix) + 1);
	}
	current_length = new_size - 1;
}

// Check for TiXmlStringA equuivalence
//bool TiXmlStringA::operator == (const TiXmlStringA & compare) const
//{
//    return (! strcmp (c_str (), compare . c_str ()));
//}

//unsigned TiXmlStringA::length () const
//{
//    if (allocated)
//        // return strlen (cstring);
//        return current_length;
//    return 0;
//}


unsigned TiXmlStringA::find(char tofind, unsigned offset) const
{
	char * lookup;

	if (offset >= length())
		return (unsigned)notfound;
	for (lookup = cstring + offset; *lookup; lookup++)
		if (*lookup == tofind)
			return lookup - cstring;
	return (unsigned)notfound;
}


bool TiXmlStringA::operator == (const TiXmlStringA & compare) const
{
	if (allocated && compare.allocated)
	{
		assert(cstring);
		assert(compare.cstring);
		return (strcmp(cstring, compare.cstring) == 0);
	}
	return false;
}


bool TiXmlStringA::operator < (const TiXmlStringA & compare) const
{
	if (allocated && compare.allocated)
	{
		assert(cstring);
		assert(compare.cstring);
		return (strcmp(cstring, compare.cstring) > 0);
	}
	return false;
}


bool TiXmlStringA::operator > (const TiXmlStringA & compare) const
{
	if (allocated && compare.allocated)
	{
		assert(cstring);
		assert(compare.cstring);
		return (strcmp(cstring, compare.cstring) < 0);
	}
	return false;
}

//void TiXmlStringA::memcpy(void * _Dst, void const * _Src, size_t _Size)
//{
//	if (!_Dst || !_Src)
//	{
//		::MessageBox(nullptr, TEXT("李扯火了"), TEXT("空指针"), MB_OK | MB_ICONERROR);
//		//		throw std::runtime_error("李扯火了");
//		return;
//	}
//	for (size_t i = 0; i < _Size; ++i)
//	{
//		((unsigned char *)_Dst)[i] = ((unsigned char *)_Src)[i];
//	}
//	if (((unsigned char *)_Dst)[_Size - 1])
//	{
//		((unsigned char *)_Dst)[_Size - 1] = '\0';
//	}
//}

void TiXmlStringA::memcpy(char * _Dst, const char * _Src, size_t _Size)
{
	if (!_Dst || !_Src)
	{
		::MessageBox(nullptr, TEXT("李扯淡了"), TEXT("空指针"), MB_OK | MB_ICONERROR);
		//		throw std::runtime_error("李扯火了");
		return;
	}
	for (size_t i = 0; i < _Size; ++i)
	{
		_Dst[i] = _Src[i];
	}
	//if (_Dst[_Size - 1])
	//{
		_Dst[_Size - 1] = '\0';
	//}
}

void TiXmlStringA::lichepi(char * _Dst)
{
	::MessageBox(nullptr, TEXT("李扯皮了"), TEXT("空指针"), MB_OK | MB_ICONERROR);
}


#endif	// TIXMLA_USE_STL
