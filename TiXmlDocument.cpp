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
#include "CommonFunctionDefine.h"
#include "TiXmlDocument.h"

/***********************************************
 * Create an empty document, that has no name. *
 ***********************************************/
TiXmlDocument::TiXmlDocument() : TiXmlNode(TiXmlNode::DOCUMENT)
{
    tabsize = 4;
    ClearError();
}

TiXmlDocument::TiXmlDocument(generic_string documentName) : TiXmlNode(TiXmlNode::DOCUMENT)
{
    tabsize = 4;
    value = TIXML_STRING(documentName.c_str());
    ClearError();
}

/********************************************************************************************
 * Create a document with a name. The name of the document is also the filename of the xml. *
 ********************************************************************************************/
TiXmlDocument::TiXmlDocument(const TCHAR * documentName) : TiXmlNode(TiXmlNode::DOCUMENT)
{
    tabsize = 4;
    value = documentName;
    ClearError();
}

TiXmlDocument::~TiXmlDocument()
{
}

/********************************************************
 * Load a file using the current document value.        *
 * Returns true if successful. Will delete any existing    *
 * document data before loading.                        *
 ********************************************************/
bool TiXmlDocument::LoadFile()
{
    // See STL_STRING_BUG below.
    StringToBuffer buf(value);

    if (buf.buffer && LoadFile(buf.buffer))
        return true;

    return false;
}

/*****************************************************************************
 * Save a file using the current document value. Returns true if successful. *
 *****************************************************************************/
bool TiXmlDocument::SaveFile() const
{
    // See STL_STRING_BUG below.
    StringToBuffer buf(value);

    if (buf.buffer && SaveFile(buf.buffer))
        return true;

    return false;
}

/*********************************************************************
 * Load a file using the given filename. Returns true if successful. *
 *********************************************************************/
bool TiXmlDocument::LoadFile(const TCHAR * filename)
{
    // Delete the existing data:
    Clear();
    location.Clear();

    // There was a really terrifying little bug here. The code:
    //        value = filename
    // in the STL case, cause the assignment method of the string to
    // be called. What is strange, is that the string had the same
    // address as it's c_str() method, and so bad things happen. Looks
    // like a bug in the Microsoft STL implementation.
    // See STL_STRING_BUG above.
    // Fixed with the StringToBuffer class.
    value = filename;

    FILE* file = generic_fopen(value.c_str(), TEXT("r"));

    if (file)
    {
        // Get the file size, so we can pre-allocate the generic_string. HUGE speed impact.
        long length = 0;
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Strange case, but good to handle up front.
        if (length == 0)
        {
            fclose(file);
            return false;
        }

        // If we have a file, assume it is all one big XML file, and read it in.
        // The document parser may decide the document ends sooner than the entire file, however.
        TIXML_STRING data;
        data.reserve(length);

        const int BUF_SIZE = 2048;
        TCHAR buf[BUF_SIZE];

        while (generic_fgets(buf, BUF_SIZE, file))
        {
            data += buf;
        }
        fclose(file);

        Parse(data.c_str(), 0);

        if (Error())
            return false;
        else
            return true;
    }
    SetError(TIXML_ERROR_OPENING_FILE, 0, 0);
    return false;
}

/*********************************************************************
 * Save a file using the given filename. Returns true if successful. *
 *********************************************************************/
bool TiXmlDocument::SaveFile(const TCHAR * filename) const
{
    // The old c stuff lives on...
    FILE* fp = generic_fopen(filename, TEXT("w"));
    if (fp)
    {
        Print(fp, 0);
        fclose(fp);
        return true;
    }
    return false;
}

/**************************************************************************
 * Get the root element -- the only top level element -- of the document. *
 * In well formed XML, there should only be one. TinyXml is tolerant of      *
 * multiple elements at the document level.                                  *
 **************************************************************************/
TiXmlElement* TiXmlDocument::RootElement() const
{
    return FirstChildElement();
}

/********************************************************************************************
 * If an error occurs, Error will be set to true. Also,                                        *
 * - The ErrorId() will contain the integer identifier of the error (not generally useful)    *
 * - The ErrorDesc() method will return the name of the error. (very useful)                *
 * - The ErrorRow() and ErrorCol() will return the location of the error (if known)            *
 ********************************************************************************************/
bool TiXmlDocument::Error() const
{
    return error;
}

/************************************************************************
 * Contains a textual (english) description of the error if one occurs. *
 ************************************************************************/
const TCHAR * TiXmlDocument::ErrorDesc() const
{
    return errorDesc.c_str();
}

/*************************************************************************************
 * Generally, you probably want the error generic_string ( ErrorDesc() ). But if you *
 * prefer the ErrorId, this function will fetch it.                                     *
 *************************************************************************************/
const int TiXmlDocument::ErrorId()    const
{
    return errorId;
}

/***************************************************************************************
 * Returns the location (if known) of the error. The first column is column 1,           *
 * and the first row is row 1. A value of 0 means the row and column wasn't applicable *
 * (memory errors, for example, have no row/column) or the parser lost the error. (An  *
 * error in the error reporting, in that case.)                                           *
 *                                                                                        *
 * @sa SetTabSize, Row, Column                                                           *
 ***************************************************************************************/
int TiXmlDocument::ErrorRow()
{
    return errorLocation.row + 1;
}

/********************************************************
 * < The column where the error occured. See ErrorRow() *
 ********************************************************/
int TiXmlDocument::ErrorCol()
{
    return errorLocation.col + 1;
}

/*****************************************************************************
 * By calling this method, with a tab size                                     *
 * greater than 0, the row and column of each node and attribute is stored     *
 * when the file is loaded. Very useful for tracking the DOM back in to         *
 * the source file.                                                             *
 *                                                                              *
 * The tab size is required for calculating the location of nodes. If not     *
 * set, the default of 4 is used. The tabsize is set per document. Setting     *
 * the tabsize to 0 disables row/column tracking.                             *
 *                                                                              *
 * Note that row and column tracking is not supported when using operator>>. *
 *                                                                              *
 * The tab size needs to be enabled before the parse or load. Correct usage: *
 * @verbatim                                                                 *
 * TiXmlDocument doc;                                                         *
 * doc.SetTabSize( 8 );                                                         *
 * doc.Load( "myfile.xml" );                                                 *
 * @endverbatim                                                                 *
 *                                                                              *
 * @sa Row, Column                                                             *
 *****************************************************************************/
void TiXmlDocument::SetTabSize(int _tabsize)
{
    tabsize = _tabsize;
}

int TiXmlDocument::TabSize() const
{
    return tabsize;
}

/****************************************************************************
 * If you have handled the error, it can be reset with this call. The error    *
 * state is automatically cleared if you Parse a new XML block.                *
 ****************************************************************************/
void TiXmlDocument::ClearError()
{
    error = false;
    errorId = 0;
    errorDesc = TEXT("");
    errorLocation.row = errorLocation.col = 0;
    //errorLocation.last = 0; 
}

/**************************************
 * Dump the document to standard out. *
 **************************************/
void TiXmlDocument::Print() const
{
    Print(stdout, 0);
}

/******************
 * [internal use] *
 ******************/
void TiXmlDocument::Print(FILE* cfile, int depth) const
{
    TiXmlNode* node;
    for (node = FirstChild(); node; node = node->NextSibling())
    {
        node->Print(cfile, depth);
        generic_fprintf(cfile, TEXT("\n"));
    }
}

void TiXmlDocument::StreamOut(TIXML_OSTREAM * out) const
{
    TiXmlNode* node;
    for (node = FirstChild(); node; node = node->NextSibling())
    {
        node->StreamOut(out);

        // Special rule for streams: stop after the root element.
        // The stream in code will only read one element, so don't
        // write more than one.
        if (node->ToElement())
            break;
    }
}

/******************
* [internal use] *
******************/
TiXmlNode * TiXmlDocument::Clone() const
{
    TiXmlDocument* clone = new TiXmlDocument();
    if (!clone)
        return 0;

    CopyToClone(clone);
    clone->error = error;
    clone->errorDesc = errorDesc.c_str();

    TiXmlNode* node = 0;
    for (node = firstChild; node; node = node->NextSibling())
    {
        clone->LinkEndChild(node->Clone());
    }
    return clone;
}





























