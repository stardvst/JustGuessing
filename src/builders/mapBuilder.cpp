#include "mapBuilder.h"
#include "propertiesBuilder.h"
#include "categoryProperties.h"

#include <vector>

MapBuilder::MapBuilder()
{
	m_properties = new CategoryProperties;
}

MapBuilder::~MapBuilder()
{
	delete m_properties;
}

void MapBuilder::setPropertyName()
{
	m_properties->m_propertyName = "map";
}

void MapBuilder::setWordsFilePath()
{
	m_properties->m_wordsFilePath = "words/geography.txt";
}

void MapBuilder::setIconPaths()
{
	m_properties->m_iconPaths.reserve(6);
	for (auto i = 0; i < 6; ++i)
		m_properties->m_iconPaths.push_back(":/map/map_" + QString::number(i));
}
