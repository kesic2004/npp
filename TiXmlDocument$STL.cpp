/*************************************************************************************************
 * www.sourceforge.net/projects/tinyxml                                                          *
 * Original code (2.0 and earlier )copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com) *
 *                                                                                               *
 * This software is provided 'as-is', without any express or implied                             *
 * warranty. In no event will the authors be held liable for any                                 *
 * damages arising from the use of this software.                                                *
 *                                                                                               *
 * Permission is granted to anyone to use this software for any                                  *
 * purpose, including commercial applications, and to alter it and                               *
 * redistribute it freely, subject to the following restrictions:                                *
 *                                                                                               *
 * 1. The origin of this software must not be misrepresented; you must                           *
 * not claim that you wrote the original software. If you use this                               *
 * software in a product, an acknowledgment in the product documentation                         *
 * would be appreciated but is not required.                                                     *
 *                                                                                               *
 * 2. Altered source versions must be plainly marked as such, and                                *
 * must not be misrepresented as being the original software.                                    *
 *                                                                                               *
 * 3. This notice may not be removed or altered from any source                                  *
 * distribution.                                                                                 *
 *************************************************************************************************/
#include "TiXmlDocument.h"

#ifdef TIXML_USE_STL
#include "CommonFunctionDefine.h"
#endif

#ifdef TIXML_USE_STL
 /****************
 * Constructor. *
 ****************/
TiXmlDocument::TiXmlDocument(const generic_string & documentName) : TiXmlNode(TiXmlNode::DOCUMENT)
{
	tabsize = 4;
	value = documentName;
	error = false;
}
#endif

#ifdef TIXML_USE_STL
/*************************
* < STL string version. *
*************************/
bool TiXmlDocument::LoadFile(const generic_string& filename)            ///< STL string version.
{
	StringToBuffer f(filename);
	return (f.buffer && LoadFile(f.buffer));
}
/*************************
* < STL string version. *
*************************/
bool TiXmlDocument::SaveFile(const generic_string& filename) const        ///< STL string version.
{
	StringToBuffer f(filename);
	return (f.buffer && SaveFile(f.buffer));
}
#endif

#ifdef TIXML_USE_STL

void TiXmlDocument::StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag)
{
	// The basic issue with a document is that we don't know what we're
	// streaming. Read something presumed to be a tag (and hope), then
	// identify it, and call the appropriate stream method on the tag.
	//
	// This "pre-streaming" will never read the closing ">" so the
	// sub-tag can orient itself.

	if (!StreamTo(in, '<', tag))
	{
		SetError(TIXML_ERROR_PARSING_EMPTY, 0, 0);
		return;
	}

	while (in->good())
	{
		size_t tagIndex = tag->length();
		while (in->good() && in->peek() != '>')
		{
			int c = in->get();
			(*tag) += static_cast<TCHAR>(c);
		}

		if (in->good())
		{
			// We now have something we presume to be a node of 
			// some sort. Identify it, and call the node to
			// continue streaming.
			TiXmlNode* node = Identify(tag->c_str() + tagIndex);

			if (node)
			{
				node->StreamIn(in, tag);
				bool isElement = node->ToElement() != 0;
				delete node;
				node = 0;

				// If this is the root element, we're done. Parsing will be
				// done by the >> operator.
				if (isElement)
				{
					return;
				}
			}
			else
			{
				SetError(TIXML_ERROR, 0, 0);
				return;
			}
		}
	}
	// We should have returned sooner.
	SetError(TIXML_ERROR, 0, 0);
}

#endif
