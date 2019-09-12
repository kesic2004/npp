/********************************
 * 移动自：                     *
 * tinyxml.cpp                  *
 * tinyxmlparser.cpp            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
#include "CommonFunctionDefine.h"
#include "TinyXmlBaseEnum.h"
#include "TiXmlElement.h"
/*************************
 * Construct an element. *
 *************************/
TiXmlElement::TiXmlElement(const TCHAR * _value) : TiXmlNode(TiXmlNode::ELEMENT)
{
	firstChild = lastChild = 0;
	value = _value;
}

#ifdef TIXML_USE_STL
/***********************
 * string constructor. *
 ***********************/
TiXmlElement::TiXmlElement(const generic_string& _value) : TiXmlNode(TiXmlNode::ELEMENT)
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif

TiXmlElement::~TiXmlElement()
{
	while (attributeSet.First())
	{
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove(node);
		delete node;
	}
}

/***********************************************************
 * Given an attribute name, Attribute() returns the value  *
 * for the attribute of that name, or null if none exists. *
 ***********************************************************/
const TCHAR * TiXmlElement::Attribute(const TCHAR * name) const
{
	TiXmlAttribute* node = attributeSet.Find(name);

	if (node)
		return node->Value();

	return 0;
}

/***************************************************************
 * Given an attribute name, Attribute() returns the value	   *
 * for the attribute of that name, or null if none exists.	   *
 * If the attribute exists and can be converted to an integer, *
 * the integer value will be put in the return 'i', if 'i'	   *
 * is non-null.												   *
 ***************************************************************/
const TCHAR * TiXmlElement::Attribute(const TCHAR * name, int* i) const
{
	const TCHAR * s = Attribute(name);
	if (i)
	{
		if (s)
			*i = generic_atoi(s);
		else
			*i = 0;
	}
	return s;
}

/**************************************************************
 * Given an attribute name, Attribute() returns the value	  *
 * for the attribute of that name, or null if none exists.	  *
 * If the attribute exists and can be converted to an double, *
 * the double value will be put in the return 'd', if 'd'	  *
 * is non-null.												  *
 **************************************************************/
const TCHAR * TiXmlElement::Attribute(const TCHAR * name, double * d) const
{
	const TCHAR * s = Attribute(name);
	if (d)
	{
		if (s)
		{
			*d = generic_atof(s);
		}
		else
		{
			*d = 0;
		}
	}
	return s;
}

/**************************************************************************
 * QueryIntAttribute examines the attribute - it is an alternative to the *
 * Attribute() method with richer error checking.						  *
 * If the attribute is an integer, it is stored in 'value' and			  *
 * the call returns TIXML_SUCCESS. If it is not							  *
 * an integer, it returns TIXML_WRONG_TYPE. If the attribute			  *
 * does not exist, then TIXML_NO_ATTRIBUTE is returned.					  *
 **************************************************************************/
int TiXmlElement::QueryIntAttribute(const TCHAR * name, int * ival) const
{
	TiXmlAttribute* node = attributeSet.Find(name);
	if (!node)
		return TIXML_NO_ATTRIBUTE;

	return node->QueryIntValue(ival);
}

/**************************************************************************
 * QueryDoubleAttribute examines the attribute - see QueryIntAttribute(). *
 **************************************************************************/
int TiXmlElement::QueryDoubleAttribute(const TCHAR * name, double * dval) const
{
	TiXmlAttribute* node = attributeSet.Find(name);
	if (!node)
		return TIXML_NO_ATTRIBUTE;

	return node->QueryDoubleValue(dval);
}

/****************************************************************
 * Sets an attribute of name to a given value. The attribute    *
 * will be created if it does not exist, or changed if it does. *
 ****************************************************************/
void TiXmlElement::SetAttribute(const TCHAR * name, const TCHAR * _value)
{
	TiXmlAttribute* node = attributeSet.Find(name);
	if (node)
	{
		node->SetValue(_value);
		return;
	}

	TiXmlAttribute* attrib = new TiXmlAttribute(name, _value);
	if (attrib)
	{
		attributeSet.Add(attrib);
	}
	else
	{
		TiXmlDocument* document = GetDocument();
		if (document) document->SetError(TIXML_ERROR_OUT_OF_MEMORY, 0, 0);
	}
}

#ifdef TIXML_USE_STL
const TCHAR* TiXmlElement::Attribute(const generic_string& name) const
{
	return Attribute(name.c_str());
}

const TCHAR* TiXmlElement::Attribute(const generic_string& name, int* i) const
{
	return Attribute(name.c_str(), i);
}

/********************
 * STL string form. *
 ********************/
void TiXmlElement::SetAttribute(const generic_string& name, const generic_string& _value)
{
	StringToBuffer n(name);
	StringToBuffer v(_value);
	if (n.buffer && v.buffer)
		SetAttribute(n.buffer, v.buffer);
}
/**********************
 * < STL string form. *
 **********************/
void TiXmlElement::SetAttribute(const generic_string& name, int _value)
{
	StringToBuffer n(name);
	if (n.buffer)
		SetAttribute(n.buffer, _value);
}
#endif

/****************************************************************
 * Sets an attribute of name to a given value. The attribute	*
 * will be created if it does not exist, or changed if it does. *
 ****************************************************************/
void TiXmlElement::SetAttribute(const TCHAR * name, int val)
{
	TCHAR buf[64];
	wsprintf(buf, TEXT("%d"), val);
	SetAttribute(name, buf);
}

/*********************************************
 * Deletes an attribute with the given name. *
 *********************************************/
void TiXmlElement::RemoveAttribute(const TCHAR * name)
{
	TiXmlAttribute* node = attributeSet.Find(name);
	if (node)
	{
		attributeSet.Remove(node);
		delete node;
	}
}

#ifdef TIXML_USE_STL
/**********************
 * < STL string form. *
 **********************/
void TiXmlElement::RemoveAttribute(const generic_string& name) 	 
{
	RemoveAttribute(name.c_str());
}
#endif

/*************************************************
 * < Access the first attribute in this element. *
 *************************************************/
TiXmlAttribute* TiXmlElement::FirstAttribute() const { return attributeSet.First(); }

/************************************************
 * < Access the last attribute in this element. *
 ************************************************/
TiXmlAttribute* TiXmlElement::LastAttribute()	const { return attributeSet.Last(); }

/*******************************************************
 * [internal use] Creates a new Element and returs it. *
 *******************************************************/
TiXmlNode * TiXmlElement::Clone() const
{
	TiXmlElement* clone = new TiXmlElement(Value());
	if (!clone)
		return 0;

	CopyToClone(clone);

	// Clone the attributes, then clone the children.
	TiXmlAttribute* attribute = 0;
	for (attribute = attributeSet.First();
		attribute;
		attribute = attribute->Next())
	{
		clone->SetAttribute(attribute->Name(), attribute->Value());
	}

	TiXmlNode* node = 0;
	for (node = firstChild; node; node = node->NextSibling())
	{
		clone->LinkEndChild(node->Clone());
	}
	return clone;
}

/******************
 * [internal use] *
 ******************/
void TiXmlElement::Print(FILE * cfile, int depth) const
{
	int i;
	for (i = 0; i<depth; i++)
	{
		generic_fprintf(cfile, TEXT("    "));
	}

	generic_fprintf(cfile, TEXT("<%s"), value.c_str());

	TiXmlAttribute* attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next())
	{
		generic_fprintf(cfile, TEXT(" "));
		attrib->Print(cfile, depth);
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode* node;
	if (!firstChild)
	{
		generic_fprintf(cfile, TEXT(" />"));
	}
	else if (firstChild == lastChild && firstChild->ToText())
	{
		generic_fprintf(cfile, TEXT(">"));
		firstChild->Print(cfile, depth + 1);
		generic_fprintf(cfile, TEXT("</%s>"), value.c_str());
	}
	else
	{
		generic_fprintf(cfile, TEXT(">"));

		for (node = firstChild; node; node = node->NextSibling())
		{
			if (!node->ToText())
			{
				generic_fprintf(cfile, TEXT("\n"));
			}
			node->Print(cfile, depth + 1);
		}
		generic_fprintf(cfile, TEXT("\n"));
		for (i = 0; i<depth; ++i)
			generic_fprintf(cfile, TEXT("    "));
		generic_fprintf(cfile, TEXT("</%s>"), value.c_str());
	}
}



/************************************
 * Used to be public [internal use] *
 ************************************/
#ifdef TIXML_USE_STL
void TiXmlElement::StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag)
{
	// We're called with some amount of pre-parsing. That is, some of "this"
	// element is in "tag". Go ahead and stream to the closing ">"
	while (in->good())
	{
		int c = in->get();
		(*tag) += static_cast<TCHAR>(c);

		if (c == '>')
			break;
	}

	if (tag->length() < 3) return;

	// Okay...if we are a "/>" tag, then we're done. We've read a complete tag.
	// If not, identify and stream.

	if (tag->at(tag->length() - 1) == '>'
		&& tag->at(tag->length() - 2) == '/')
	{
		// All good!
		return;
	}
	else if (tag->at(tag->length() - 1) == '>')
	{
		// There is more. Could be:
		//		text
		//		closing tag
		//		another node.
		for (;; )
		{
			StreamWhiteSpace(in, tag);

			// Do we have text?
			if (in->good() && in->peek() != '<')
			{
				// Yep, text.
				TiXmlText text(TEXT(""));
				text.StreamIn(in, tag);

				// What follows text is a closing tag or another node.
				// Go around again and figure it out.
				continue;
			}

			// We now have either a closing tag...or another node.
			// We should be at a "<", regardless.
			if (!in->good()) return;
			assert(in->peek() == '<');
			size_t tagIndex = tag->length();

			bool closingTag = false;
			bool firstCharFound = false;

			for (;; )
			{
				if (!in->good())
					return;

				int c = in->peek();

				if (c == '>')
					break;

				*tag += static_cast<TCHAR>(c);
				in->get();

				if (!firstCharFound && c != '<' && !IsWhiteSpace(c))
				{
					firstCharFound = true;
					if (c == '/')
						closingTag = true;
				}
			}
			// If it was a closing tag, then read in the closing '>' to clean up the input stream.
			// If it was not, the streaming will be done by the tag.
			if (closingTag)
			{
				int c = in->get();
				assert(c == '>');
				*tag += static_cast<TCHAR>(c);

				// We are done, once we've found our closing tag.
				return;
			}
			else
			{
				// If not a closing tag, id it, and stream.
				const TCHAR* tagloc = tag->c_str() + tagIndex;
				TiXmlNode* node = Identify(tagloc);
				if (!node)
					return;
				node->StreamIn(in, tag);
				delete node;
				node = 0;

				// No return: go around from the beginning: text, closing tag, or node.
			}
		}
	}
}
#endif

void TiXmlElement::StreamOut(TIXML_OSTREAM * stream) const
{
	(*stream) << TEXT("<") << value;

	TiXmlAttribute* attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next())
	{
		(*stream) << TEXT(" ");
		attrib->StreamOut(stream);
	}

	// If this node has children, give it a closing tag. Else
	// make it an empty tag.
	TiXmlNode* node;
	if (firstChild)
	{
		(*stream) << TEXT(">");

		for (node = firstChild; node; node = node->NextSibling())
		{
			node->StreamOut(stream);
		}
		(*stream) << TEXT("</") << value << TEXT(">");
	}
	else
	{
		(*stream) << TEXT(" />");
	}
}

/*************************************************
 * [internal use]								 *
 * Attribtue parsing starts: next TCHAR past '<' *
 * returns: next TCHAR past '>'                  *
 *************************************************/
const TCHAR * TiXmlElement::Parse(const TCHAR * p, TiXmlParsingData * data)
{
	p = SkipWhiteSpace(p);
	TiXmlDocument* document = GetDocument();

	if (!p || !*p)
	{
		if (document) document->SetError(TIXML_ERROR_PARSING_ELEMENT, 0, 0);
		return 0;
	}

	//	TiXmlParsingData data( p, prevData );
	if (data)
	{
		data->Stamp(p);
		location = data->Cursor();
	}

	if (*p != '<')
	{
		if (document) document->SetError(TIXML_ERROR_PARSING_ELEMENT, p, data);
		return 0;
	}

	p = SkipWhiteSpace(p + 1);

	// Read the name.
	const TCHAR* pErr = p;

	p = ReadName(p, &value);
	if (!p || !*p)
	{
		if (document)	document->SetError(TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data);
		return 0;
	}

	TIXML_STRING endTag(TEXT("</"));
	endTag += value;
	endTag += TEXT(">");

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while (p && *p)
	{
		pErr = p;
		p = SkipWhiteSpace(p);
		if (!p || !*p)
		{
			if (document) document->SetError(TIXML_ERROR_READING_ATTRIBUTES, pErr, data);
			return 0;
		}
		if (*p == '/')
		{
			++p;
			// Empty tag.
			if (*p != '>')
			{
				if (document) document->SetError(TIXML_ERROR_PARSING_EMPTY, p, data);
				return 0;
			}
			return (p + 1);
		}
		else if (*p == '>')
		{
			// Done with attributes (if there were any.)
			// Read the value -- which can include other
			// elements -- read the end tag, and return.
			++p;
			p = ReadValue(p, data);		// Note this is an Element method, and will set the error if one happens.
			if (!p || !*p)
				return 0;

			// We should find the end tag now
			if (StringEqual(p, endTag.c_str(), false))
			{
				p += endTag.length();
				return p;
			}
			else
			{
				if (document) document->SetError(TIXML_ERROR_READING_END_TAG, p, data);
				return 0;
			}
		}
		else
		{
			// Try to read an attribute:
			TiXmlAttribute* attrib = new TiXmlAttribute();
			if (!attrib)
			{
				if (document) document->SetError(TIXML_ERROR_OUT_OF_MEMORY, pErr, data);
				return 0;
			}

			attrib->SetDocument(document);
			const TCHAR* pErr = p;
			p = attrib->Parse(p, data);

			if (!p || !*p)
			{
				if (document) document->SetError(TIXML_ERROR_PARSING_ELEMENT, pErr, data);
				delete attrib;
				return 0;
			}

			// Handle the strange case of double attributes:
			TiXmlAttribute* node = attributeSet.Find(attrib->Name());
			if (node)
			{
				node->SetValue(attrib->Value());
				delete attrib;
				return 0;
			}

			attributeSet.Add(attrib);
		}
	}
	return p;
}

/*****************************************************************
 * [internal use]												 *
 * Reads the "value" of the element -- another element, or text. *
 * This should terminate with the current end tag.				 *
 *****************************************************************/
const TCHAR * TiXmlElement::ReadValue(const TCHAR * p, TiXmlParsingData * data)
{
	TiXmlDocument* document = GetDocument();

	// Read in text and elements in any order.
	p = SkipWhiteSpace(p);
	while (p && *p)
	{
		if (*p != '<')
		{
			// Take what we have, make a text element.
			TiXmlText* textNode = new TiXmlText(TEXT(""));

			if (!textNode)
			{
				if (document) document->SetError(TIXML_ERROR_OUT_OF_MEMORY, 0, 0);
				return 0;
			}

			p = textNode->Parse(p, data);

			if (!textNode->Blank())
				LinkEndChild(textNode);
			else
				delete textNode;
		}
		else
		{
			// We hit a '<'
			// Have we hit a new element or an end tag?
			if (StringEqual(p, TEXT("</"), false))
			{
				return p;
			}
			else
			{
				TiXmlNode* node = Identify(p);
				if (node)
				{
					p = node->Parse(p, data);
					LinkEndChild(node);
				}
				else
				{
					return 0;
				}
			}
		}
		p = SkipWhiteSpace(p);
	}

	if (!p)
	{
		if (document) document->SetError(TIXML_ERROR_READING_ELEMENT_VALUE, 0, 0);
	}
	return p;
}













