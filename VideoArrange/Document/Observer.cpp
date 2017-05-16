#include "Observer.h"
#include "Document.h"

QDomElement Observer::data(const QStringList& path)
{
	return Document::instance()->data(path);
}
