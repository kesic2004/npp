/*************************************************************************
 * www.sourceforge.net/projects/tinyxml                                  *
 * Original file by Yves Berquin.                                        *
 *                                                                       *
 * This software is provided 'as-is', without any express or implied     *
 * warranty. In no event will the authors be held liable for any         *
 * damages arising from the use of this software.                        *
 *                                                                       *
 * Permission is granted to anyone to use this software for any          *
 * purpose, including commercial applications, and to alter it and       *
 * redistribute it freely, subject to the following restrictions:        *
 *                                                                       *
 * 1. The origin of this software must not be misrepresented; you must   *
 * not claim that you wrote the original software. If you use this       *
 * software in a product, an acknowledgment in the product documentation *
 * would be appreciated but is not required.                             *
 *                                                                       *
 * 2. Altered source versions must be plainly marked as such, and        *
 * must not be misrepresented as being the original software.            *
 *                                                                       *
 * 3. This notice may not be removed or altered from any source          *
 * distribution.                                                         *
 *************************************************************************/
#include "TiXmlStringA.h"

#ifndef TIXMLA_USE_STL

#ifndef _INCLUDED
#define TIXMLA_STRING_INCLUDED

#pragma warning( disable : 4514 )

 /*************************************************
 * TiXmlStringA constructor, based on a C string *
 *************************************************/
TiXmlStringA::TiXmlStringA(const char* instring)
{
    unsigned newlen;
    char * newstring;

    if (!instring)
    {
        allocated = 0;
        cstring = NULL;
        current_length = 0;
        return;
    }
    newlen = strlen(instring) + 1;
    newstring = new char[newlen];
    memcpy(newstring, instring, newlen);
    // strcpy (newstring, instring);
    allocated = newlen;
    cstring = newstring;
    current_length = newlen - 1;
}

// TiXmlStringA empty constructor
TiXmlStringA::TiXmlStringA()
{
    allocated = 0;
    cstring = NULL;
    current_length = 0;
}

/*********************************
 * TiXmlStringA copy constructor *
 *********************************/
TiXmlStringA::TiXmlStringA(const TiXmlStringA& copy)
{
    unsigned newlen;
    char * newstring;

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
    // strcpy (newstring, copy . cstring);
    memcpy(newstring, copy.cstring, newlen);
    allocated = newlen;
    cstring = newstring;
    current_length = newlen - 1;
}

// TiXmlStringA destructor
TiXmlStringA::~TiXmlStringA()
{
    TiXmlStringA::empty_it();
}

// Convert a TiXmlStringA into a classical char *
const char * TiXmlStringA::c_str() const
{
    if (allocated)
        return cstring;
    return "";
}

// Return the length of a TiXmlStringA
unsigned TiXmlStringA::length() const
{
    return (allocated) ? current_length : 0;
}

// TiXmlStringA = operator. Safe when assign own content

void TiXmlStringA ::operator = (const char * content)
{
    unsigned newlen;
    char * newstring;

    if (!content)
    {
        empty_it();
        return;
    }
    newlen = strlen(content) + 1;
    newstring = new char[newlen];
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
    char * newstring;

    if (!copy.length())
    {
        empty_it();
        return;
    }
    newlen = copy.length() + 1;
    newstring = new char[newlen];
    // strcpy (newstring, copy . c_str ());
    memcpy(newstring, copy.c_str(), newlen);
    empty_it();
    allocated = newlen;
    cstring = newstring;
    current_length = newlen - 1;
}


// += operator. Maps to append
TiXmlStringA& TiXmlStringA::operator += (const char * suffix)
{
    append(suffix);
    return *this;
}

// += operator. Maps to append
TiXmlStringA& TiXmlStringA::operator += (char single)
{
    append(single);
    return *this;
}

// += operator. Maps to append
TiXmlStringA& TiXmlStringA::operator += (TiXmlStringA & suffix)
{
    append(suffix);
    return *this;
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


// Checks if a TiXmlStringA is empty
bool TiXmlStringA::empty() const
{
    return length() ? false : true;
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


// single char extraction
const char & TiXmlStringA::at(unsigned index) const
{
    assert(index < length());
    return cstring[index];
}

// find a char in a string. Return TiXmlStringA::notfound if not found
unsigned TiXmlStringA::find(char lookup) const
{
    return find(lookup, 0);
}


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


/*    Function to reserve a big amount of data when we know we'll need it. Be aware that this
function clears the content of the TiXmlStringA if any exists.
*/
void TiXmlStringA::reserve(unsigned size)
{
    empty_it();
    if (size)
    {
        allocated = size;
        TIXMLA_STRING cstring = new char[size];
        cstring[0] = 0;
        current_length = 0;
    }
}

// [] operator 
char & TiXmlStringA::operator [] (unsigned index) const
{
    assert(index < length());
    return cstring[index];
}

// append a const char * to an existing TiXmlStringA

void TiXmlStringA::append(const char* str, int len)
{
    char * new_string;
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


// New size computation. It is simplistic right now : it returns twice the amount
// we need
unsigned TiXmlStringA::assign_new_size(unsigned minimum_to_allocate)
{
    return minimum_to_allocate * 2;
}

// Internal function that clears the content of a TiXmlStringA
void TiXmlStringA::empty_it()
{
    if (cstring)
        delete[] cstring;
    cstring = NULL;
    allocated = 0;
    current_length = 0;
}

// append a const char * to an existing TiXmlStringA

void TiXmlStringA::append(const char * suffix)
{
    char * new_string;
    unsigned new_alloc, new_size;

    new_size = length() + strlen(suffix) + 1;
    // check if we need to expand
    if (new_size > allocated)
    {
        // compute new size
        new_alloc = assign_new_size(new_size);

        // allocate new buffer
        new_string = new char[new_alloc];
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
        // we know we can safely append the new string
        memcpy(cstring + length(),
            suffix,
            strlen(suffix) + 1);
    }
    current_length = new_size - 1;
}


// append function for another TiXmlStringA
void TiXmlStringA::append(const TiXmlStringA & suffix)
{
    TiXmlStringA::append(suffix.c_str());
}

// append for a single char. This could be improved a lot if needed
void TiXmlStringA::append(char single)
{
    char smallstr[2];
    smallstr[0] = single;
    smallstr[1] = 0;
    append(smallstr);
}

#endif	// TIXMLA_STRING_INCLUDED
#endif	// TIXMLA_USE_STL



