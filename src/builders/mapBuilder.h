#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H

#include "propertiesBuilder.h"

class MapBuilder : public PropertiesBuilder
{
public:
	MapBuilder();
	~MapBuilder() override;

	void setPropertyName() override;
	void setWordsFilePath() override;
	void setIconPaths() override;
};

#endif // !MAP_BUILDER_H
