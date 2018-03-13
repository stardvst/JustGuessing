#include "carsBuilder.h"
#include "propertiesBuilder.h"
#include "categoryProperties.h"

#include <vector>

CarsBuilder::CarsBuilder()
{
	m_properties = new CategoryProperties;
}

CarsBuilder::~CarsBuilder()
{
	delete m_properties;
}

void CarsBuilder::setPropertyName()
{
	m_properties->m_propertyName = "car";
}

void CarsBuilder::setWordsFilePath()
{
	m_properties->m_wordsFilePath = "words/cars.txt";
}

void CarsBuilder::setIconPaths()
{
	m_properties->m_iconPaths.reserve(6);
	for (auto i = 0; i < 6; ++i)
		m_properties->m_iconPaths.push_back(":/car/tachka_" + QString::number(i));
}
