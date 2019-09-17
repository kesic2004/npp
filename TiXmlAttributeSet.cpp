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
/********************************
 * 移动自：tinyxml.h            *
 * 由不明真相的吃瓜群众移动至此 *
 ********************************/
#include "TiXmlAttributeSet.h"

TiXmlAttributeSet::TiXmlAttributeSet()
{
    sentinel.next = &sentinel;
    sentinel.prev = &sentinel;
}

TiXmlAttributeSet::~TiXmlAttributeSet()
{
    assert(sentinel.next == &sentinel);
    assert(sentinel.prev == &sentinel);
}

void TiXmlAttributeSet::Add(TiXmlAttribute * addMe)
{
	/********************************************
	 * Shouldn't be multiply adding to the set. *
	 ********************************************/
    assert(!Find(addMe->Name()));

    addMe->next = &sentinel;
    addMe->prev = sentinel.prev;

    sentinel.prev->next = addMe;
    sentinel.prev = addMe;
}

void TiXmlAttributeSet::Remove(TiXmlAttribute * removeMe)
{
    TiXmlAttribute* node;

    for (node = sentinel.next; node != &sentinel; node = node->next)
    {
        if (node == removeMe)
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->next = 0;
            node->prev = 0;
            return;
        }
    }

    /**********************************************
	 * we tried to remove a non-linked attribute. *
	 **********************************************/
    assert(0);
}

TiXmlAttribute * TiXmlAttributeSet::First() const
{
    return (sentinel.next == &sentinel) ? 0 : sentinel.next;
}

TiXmlAttribute * TiXmlAttributeSet::Last() const
{
    return (sentinel.prev == &sentinel) ? 0 : sentinel.prev;
}

TiXmlAttribute * TiXmlAttributeSet::Find(const TCHAR * name) const
{
    TiXmlAttribute* node;

    for (node = sentinel.next; node != &sentinel; node = node->next)
    {
        if (node->name == name)
            return node;
    }
    return 0;
}





