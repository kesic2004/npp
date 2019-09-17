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
/***********************************************************
 * 移动自：tinyxml.h                                       *
 * 由不明真相的吃瓜群众移动至此                            *
 ***********************************************************/
// TiXmlDocument.h
#ifndef __REAL_KESIC_LEE_TI_XML_DOCUMENT_H__
#define __REAL_KESIC_LEE_TI_XML_DOCUMENT_H__ 1

#include "CommonGenericString.h"
#include "TiXmlNode.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/*******************************************************************
 * Always the top level node. A document binds together all the    *
 * XML pieces. It can be saved, loaded, and printed to the screen. *
 * The 'value' of a document node is the xml file name.            *
 *******************************************************************/
class TiXmlDocument : public TiXmlNode
{
public:
    /***********************************************
     * Create an empty document, that has no name. *
     ***********************************************/
    TiXmlDocument();
    TiXmlDocument(generic_string documentName);
    /********************************************************************************************
     * Create a document with a name. The name of the document is also the filename of the xml. *
     ********************************************************************************************/
    TiXmlDocument(const TCHAR * documentName);

#ifdef TIXML_USE_STL
    /****************
     * Constructor. *
     ****************/
    TiXmlDocument(const generic_string & documentName);
#endif

    virtual ~TiXmlDocument();

    /********************************************************
     * Load a file using the current document value.        *
     * Returns true if successful. Will delete any existing *
     * document data before loading.                        *
     ********************************************************/
    bool LoadFile();

    /*****************************************************************************
     * Save a file using the current document value. Returns true if successful. *
     *****************************************************************************/
    bool SaveFile() const;

    /*********************************************************************
     * Load a file using the given filename. Returns true if successful. *
     *********************************************************************/

    bool LoadFile(const TCHAR * filename);

    /*********************************************************************
     * Save a file using the given filename. Returns true if successful. *
     *********************************************************************/
    bool SaveFile(const TCHAR * filename) const;

#ifdef TIXML_USE_STL
    /*************************
     * < STL string version. *
     *************************/
    bool LoadFile(const generic_string& filename);
    /*************************
     * < STL string version. *
     *************************/
    bool SaveFile(const generic_string& filename) const;
#endif

    /******************************************************
     * Parse the given null terminated block of xml data. *
     ******************************************************/
    virtual const TCHAR* Parse(const TCHAR* p, TiXmlParsingData* data = 0);

    /**************************************************************************
     * Get the root element -- the only top level element -- of the document. *
     * In well formed XML, there should only be one. TinyXml is tolerant of   *
     * multiple elements at the document level.                               *
     **************************************************************************/
    TiXmlElement* RootElement() const;

    /********************************************************************************************
     * If an error occurs, Error will be set to true. Also,                                     *
     * - The ErrorId() will contain the integer identifier of the error (not generally useful)  *
     * - The ErrorDesc() method will return the name of the error. (very useful)                *
     * - The ErrorRow() and ErrorCol() will return the location of the error (if known)         *
     ********************************************************************************************/
    bool Error() const;

    /************************************************************************
     * Contains a textual (english) description of the error if one occurs. *
     ************************************************************************/
    const TCHAR * ErrorDesc() const;

    /*************************************************************************************
     * Generally, you probably want the error generic_string ( ErrorDesc() ). But if you *
     * prefer the ErrorId, this function will fetch it.                                  *
     *************************************************************************************/
    const int ErrorId()    const;

    /***************************************************************************************
     * Returns the location (if known) of the error. The first column is column 1,         *
     * and the first row is row 1. A value of 0 means the row and column wasn't applicable *
     * (memory errors, for example, have no row/column) or the parser lost the error. (An  *
     * error in the error reporting, in that case.)                                        *
     *                                                                                     *
     * @sa SetTabSize, Row, Column                                                         *
     ***************************************************************************************/
    int ErrorRow();

    /********************************************************
     * < The column where the error occured. See ErrorRow() *
     ********************************************************/
    int ErrorCol();

    /*****************************************************************************
     * By calling this method, with a tab size                                   *
     * greater than 0, the row and column of each node and attribute is stored   *
     * when the file is loaded. Very useful for tracking the DOM back in to      *
     * the source file.                                                          *
     *                                                                           *
     * The tab size is required for calculating the location of nodes. If not    *
     * set, the default of 4 is used. The tabsize is set per document. Setting   *
     * the tabsize to 0 disables row/column tracking.                            *
     *                                                                           *
     * Note that row and column tracking is not supported when using operator>>. *
     *                                                                           *
     * The tab size needs to be enabled before the parse or load. Correct usage: *
     * @verbatim                                                                 *
     * TiXmlDocument doc;                                                        *
     * doc.SetTabSize( 8 );                                                      *
     * doc.Load( "myfile.xml" );                                                 *
     * @endverbatim                                                              *
     *                                                                           *
     * @sa Row, Column                                                           *
     *****************************************************************************/
    void SetTabSize(int _tabsize);

    int TabSize() const;

    /****************************************************************************
     * If you have handled the error, it can be reset with this call. The error *
     * state is automatically cleared if you Parse a new XML block.             *
     ****************************************************************************/
    void ClearError();

    /**************************************
     * Dump the document to standard out. *
     **************************************/
    void Print() const;

    /******************
     * [internal use] *
     ******************/
    virtual void Print(FILE* cfile, int depth = 0) const;

    /******************
     * [internal use] *
     ******************/
    void SetError(int err, const TCHAR* errorLocation, TiXmlParsingData* prevData);

protected:
    virtual void StreamOut(TIXML_OSTREAM * out) const;
    /******************
     * [internal use] *
     ******************/
    virtual TiXmlNode* Clone() const;
#ifdef TIXML_USE_STL
    virtual void StreamIn(TIXML_ISTREAM * in, TIXML_STRING * tag);
#endif

private:
    bool error;
    int  errorId;
    TIXML_STRING errorDesc;
    int tabsize;
    TiXmlCursor errorLocation;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_DOCUMENT_H__

