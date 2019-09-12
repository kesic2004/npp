/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/

#ifndef __REAL_KESIC_LEE_TI_XML_BASE_H__
#define __REAL_KESIC_LEE_TI_XML_BASE_H__ 1

#include "TiXmlParsingData.h"

/***********************************************************
 * TiXmlBase is a base class for every class in TinyXml.   *
 * It does little except to establish that TinyXml classes *
 * can be printed and provide some utility functions.	   *
 * 														   *
 * In XML, the document and elements can contain		   *
 * other elements and other types of nodes.				   *
 * 														   *
 * @verbatim											   *
 * A Document can contain:	Element	(container or leaf)	   *
 * Comment (leaf)										   *
 * Unknown (leaf)										   *
 * Declaration( leaf )									   *
 * 														   *
 * An Element can contain:	Element (container or leaf)	   *
 * Text	(leaf)											   *
 * Attributes (not on tree)								   *
 * Comment (leaf)										   *
 * Unknown (leaf)										   *
 * 														   *
 * A Decleration contains: Attributes (not on tree)		   *
 * @endverbatim											   *
 ***********************************************************/

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

class TiXmlBase
{
	friend class TiXmlNode;
	friend class TiXmlElement;
	friend class TiXmlDocument;

public:
	TiXmlBase();
	virtual ~TiXmlBase();

	/*****************************************************************
	 * All TinyXml classes can print themselves to a filestream.     *
	 * This is a formatted print, and will insert tabs and newlines. *
	 *                                                               *
	 * (For an unformatted stream, use the << operator.)             *
	 */
	virtual void Print(FILE* cfile, int depth) const = 0;

	/******************************************************************************
	 * The world does not agree on whether white space should be kept or		  *
	 * not. In order to make everyone happy, these global, static functions		  *
	 * are provided to set whether or not TinyXml will condense all white space	  *
	 * into a single space or not. The default is to condense. Note changing this *
	 * values is not thread safe.                                                 *
	 ******************************************************************************/
	static void SetCondenseWhiteSpace(bool condense);

	/*******************************************
	 * Return the current white space setting. *
	 *******************************************/
	static bool IsWhiteSpaceCondensed();

	/**************************************************************************************
	 * Return the position, in the original source file, of this node or attribute.		  *
	 * The row and column are 1-based. (That is the first row and first column is		  *
	 * 1,1). If the returns values are 0 or less, then the parser does not have			  *
	 * a row and column value.															  *
	 * 																					  *
	 * Generally, the row and column value will be set when the TiXmlDocument::Load(),	  *
	 * TiXmlDocument::LoadFile(), or any TiXmlNode::Parse() is called. It will NOT be set *
	 * when the DOM was created from operator>>.										  *
	 * 																					  *
	 * The values reflect the initial load. Once the DOM is modified programmatically	  *
	 * (by adding or changing nodes and attributes) the new values will NOT update to	  *
	 * reflect changes in the document.													  *
	 * 																					  *
	 * There is a minor performance cost to computing the row and column. Computation	  *
	 * can be disabled if TiXmlDocument::SetTabSize() is called with 0 as the value.	  *
	 * 																					  *
	 * @sa TiXmlDocument::SetTabSize()													  *
	 **************************************************************************************/
	int Row() const;

	/**************
	 * @See Row() *
	 **************/
	int Column() const;

protected:
	// See STL_STRING_BUG
	// Utility class to overcome a bug.
	/**********************************************************
	 * Strange class for a bug fix. Search for STL_STRING_BUG *
	 **********************************************************/
	class StringToBuffer
	{
	public:
		StringToBuffer(const TIXML_STRING& str);
		~StringToBuffer();
		TCHAR * buffer;
	};

	static const TCHAR *	SkipWhiteSpace(const TCHAR*);

	/*inline*/ static bool	IsWhiteSpace(int c);

	virtual void StreamOut(TIXML_OSTREAM *) const = 0;

#ifdef TIXML_USE_STL
	static bool	StreamWhiteSpace(TIXML_ISTREAM * in, TIXML_STRING * tag);
	static bool StreamTo(TIXML_ISTREAM * in, int character, TIXML_STRING * tag);
#endif

	/***************************************************************
	 * Reads an XML name into the generic_string provided. Returns *
	 * a pointer just past the last character of the name,         *
	 * or 0 if the function has an error.                          *
	 ***************************************************************/
	static const TCHAR* ReadName(const TCHAR * p, TIXML_STRING * name);

	/*****************************************************************************
	 * Reads text. Returns a pointer past the given end tag.					 *
	 * Wickedly complex options, but it keeps the (sensitive) code in one place. *
	 *****************************************************************************/
	static const TCHAR* ReadText(
		const TCHAR  * in,					// where to start
		TIXML_STRING * text,				// the generic_string read
		bool           ignoreWhiteSpace,	// whether to keep the white space
		const TCHAR  * endTag,				// what ends this text
		bool           ignoreCase);			// whether to ignore case in the end tag

	virtual const TCHAR* Parse(const TCHAR * p, TiXmlParsingData * data) = 0;

	/***************************************************************
	 * If an entity has been found, transform it into a character. *
	 ***************************************************************/
	static const TCHAR* GetEntity(const TCHAR* in, TCHAR* value);

	/*************************************************
	 * Get a character, while interpreting entities. *
	 *************************************************/
	/*inline*/ static const TCHAR* GetChar(const TCHAR* p, TCHAR* _value);

	/**********************************************************************************************
	 * Puts a generic_string to a stream, expanding entities as it goes.                          *
	 * Note this should not contian the '<', '>', etc, or they will be transformed into entities! *
	 **********************************************************************************************/
	static void PutString(const TIXML_STRING & str, TIXML_OSTREAM * out);

	static void PutString(const TIXML_STRING & str, TIXML_STRING * out);

	// Return true if the next characters in the stream are any of the endTag sequences.
	static bool StringEqual(const TCHAR* p,
		const TCHAR* endTag,
		bool ignoreCase);


	enum
	{
		TIXML_NO_ERROR = 0,
		TIXML_ERROR,
		TIXML_ERROR_OPENING_FILE,
		TIXML_ERROR_OUT_OF_MEMORY,
		TIXML_ERROR_PARSING_ELEMENT,
		TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TIXML_ERROR_READING_ELEMENT_VALUE,
		TIXML_ERROR_READING_ATTRIBUTES,
		TIXML_ERROR_PARSING_EMPTY,
		TIXML_ERROR_READING_END_TAG,
		TIXML_ERROR_PARSING_UNKNOWN,
		TIXML_ERROR_PARSING_COMMENT,
		TIXML_ERROR_PARSING_DECLARATION,
		TIXML_ERROR_DOCUMENT_EMPTY,

		TIXML_ERROR_STRING_COUNT
	};
	static const TCHAR* errorString[TIXML_ERROR_STRING_COUNT];

	TiXmlCursor location;

private:
	struct Entity
	{
		const TCHAR*     str;
		unsigned int	strLength;
		TCHAR		    chr;
	};
	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6

	};
	static Entity entity[NUM_ENTITY];
	static bool condenseWhiteSpace;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_BASE_H__
