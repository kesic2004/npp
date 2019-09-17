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
// TiXmlHandle.h
#ifndef __REAL_KESIC_LEE_TI_XML_HANDLE_H__
#define __REAL_KESIC_LEE_TI_XML_HANDLE_H__ 1

#include "TiXmlNode.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4458 ) // declaration of 'parameter' hides class member
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

/************************************************************************************************************************
 * A TiXmlHandle is a class that wraps a node pointer with null checks; this is                                         *
 * an incredibly useful thing. Note that TiXmlHandle is not part of the TinyXml                                         *
 * DOM structure. It is a separate utility class.                                                                       *
 *                                                                                                                      *
 * Take an example:                                                                                                     *
 * @verbatim                                                                                                            *
 * <Document>                                                                                                           *
 * <Element attributeA = TEXT("valueA")>                                                                                *
 * <Child attributeB = TEXT("value1") />                                                                                *
 * <Child attributeB = TEXT("value2") />                                                                                *
 * </Element>                                                                                                           *
 * <Document>                                                                                                           *
 * @endverbatim                                                                                                         *
 *                                                                                                                      *
 * Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very                                    *
 * easy to write a *lot* of code that looks like:                                                                       *
 *                                                                                                                      *
 * @verbatim                                                                                                            *
 * TiXmlElement* root = document.FirstChildElement( "Document" );                                                       *
 * if ( root )                                                                                                          *
 * {                                                                                                                    *
 * TiXmlElement* element = root->FirstChildElement( "Element" );                                                        *
 * if ( element )                                                                                                       *
 * {                                                                                                                    *
 *      TiXmlElement* child = element->FirstChildElement( "Child" );                                                    *
 * if ( child )                                                                                                         *
 * {                                                                                                                    *
 *      TiXmlElement* child2 = child->NextSiblingElement( "Child" );                                                    *
 *      if ( child2 )                                                                                                   *
 *      {                                                                                                               *
 * // Finally do something useful.                                                                                      *
 * @endverbatim                                                                                                         *
 *                                                                                                                      *
 * And that doesn't even cover "else" cases. TiXmlHandle addresses the verbosity                                        *
 * of such code. A TiXmlHandle checks for null    pointers so it is perfectly safe                                      *
 * and correct to use:                                                                                                  *
 *                                                                                                                      *
 * @verbatim                                                                                                            *
 * TiXmlHandle docHandle( &document );                                                                                  *
 * TiXmlElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).Element();     *
 * if ( child2 )                                                                                                        *
 * {                                                                                                                    *
 *      // do something useful                                                                                          *
 * @endverbatim                                                                                                         *
 *                                                                                                                      *
 * Which is MUCH more concise and useful.                                                                               *
 *                                                                                                                      *
 * It is also safe to copy handles - internally they are nothing more than node pointers.                               *
 * @verbatim                                                                                                            *
 * TiXmlHandle handleCopy = handle;                                                                                     *
 * @endverbatim                                                                                                         *
 *                                                                                                                      *
 * What they should not be used for is iteration:                                                                       *
 *                                                                                                                      *
 * @verbatim                                                                                                            *
 * int i=0;                                                                                                             *
 * while ( true )                                                                                                       *
 * {                                                                                                                    *
 *      TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).Element(); *
 *      if ( !child )                                                                                                   *
 *         break;                                                                                                       *
 *      // do something                                                                                                 *
 *      ++i;                                                                                                            *
 * }                                                                                                                    *
 * @endverbatim                                                                                                         *
 *                                                                                                                      *
 * It seems reasonable, but it is in fact two embedded while loops. The Child method is                                 *
 * a linear walk to find the element, so this code would iterate much more than it needs                                *
 * to. Instead, prefer:                                                                                                 *
 *                                                                                                                      *
 * @verbatim                                                                                                            *
 * TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).Element();    *
 *                                                                                                                      *
 * for( child; child; child=child->NextSiblingElement() )                                                               *
 * {                                                                                                                    *
 *      // do something                                                                                                 *
 * }                                                                                                                    *
 * @endverbatim                                                                                                         *
 ************************************************************************************************************************/
class TiXmlHandle
{
public:
    /*****************************************************************************************
     * Create a handle from any node (at any depth of the tree.) This can be a null pointer. *
     *****************************************************************************************/
    TiXmlHandle(TiXmlNode* node);

    /********************
     * Copy constructor *
     ********************/
    TiXmlHandle(const TiXmlHandle& ref);

    /********************************************
     * Return a handle to the first child node. *
     ********************************************/
    TiXmlHandle FirstChild() const;

    /****************************************************************
     * Return a handle to the first child node with the given name. *
     ****************************************************************/
    TiXmlHandle FirstChild(const TCHAR * value) const;

    /***********************************************
     * Return a handle to the first child element. *
     ***********************************************/
    TiXmlHandle FirstChildElement() const;

    /*******************************************************************
     * Return a handle to the first child element with the given name. *
     *******************************************************************/
    TiXmlHandle FirstChildElement(const TCHAR * value) const;

    /*************************************************************
     * Return a handle to the "index" child with the given name. *
     * The first child is 0, the second 1, etc.                     *
     *************************************************************/
    TiXmlHandle Child(const TCHAR* value, int index) const;

    /********************************************
     * Return a handle to the "index" child.    *
     * The first child is 0, the second 1, etc.    *
     ********************************************/
    TiXmlHandle Child(int index) const;

    /*********************************************************************************
     * Return a handle to the "index" child element with the given name.             *
     * The first child element is 0, the second 1, etc. Note that only TiXmlElements *
     * are indexed: other types are not counted.                                     *
     *********************************************************************************/
    TiXmlHandle ChildElement(const TCHAR* value, int index) const;

    /*********************************************************************************
     * Return a handle to the "index" child element.                                 *
     * The first child element is 0, the second 1, etc. Note that only TiXmlElements *
     * are indexed: other types are not counted.                                     *
     *********************************************************************************/
    TiXmlHandle ChildElement(int index) const;

#ifdef TIXML_USE_STL
    TiXmlHandle FirstChild(const generic_string& _value) const;
    TiXmlHandle FirstChildElement(const generic_string& _value) const;

    TiXmlHandle Child(const generic_string& _value, int index) const;
    TiXmlHandle ChildElement(const generic_string& _value, int index) const;
#endif

    /***********************************************************
     * Return the handle as a TiXmlNode. This may return null. *
     ***********************************************************/
    TiXmlNode * Node() const;

    /**************************************************************
     * Return the handle as a TiXmlElement. This may return null. *
     **************************************************************/
    TiXmlElement * Element() const;

    /***********************************************************
     * Return the handle as a TiXmlText. This may return null. *
     ***********************************************************/
    TiXmlText * Text() const;

private:
    TiXmlNode * node;
};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // !__REAL_KESIC_LEE_TI_XML_HANDLE_H__

