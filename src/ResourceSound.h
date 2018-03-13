#ifndef RESOURCESOUND_H
#define RESOURCESOUND_H

#include <QSound>
#include "ResourceFile.h"

class ResourceSound : private ResourceFile, public QSound
{
public:
	ResourceSound(const QString & respath, QObject * parent = 0)
		: ResourceFile(respath, parent)
		, QSound(ResourceFile::fileName(), parent)
	{
	}
};

#endif