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

void TiXmlAttributeSet::Add(TiXmlAttribute* addMe)
{
	assert(!Find(addMe->Name()));	// Shouldn't be multiply adding to the set.

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

	// we tried to remove a non-linked attribute.
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





