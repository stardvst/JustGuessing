#include "burgerBuilder.h"
#include "propertiesBuilder.h"
#include "categoryProperties.h"

#include <vector>

BurgerBuilder::BurgerBuilder()
{
	m_properties = new CategoryProperties;
}

BurgerBuilder::~BurgerBuilder()
{
	delete m_properties;
}

void BurgerBuilder::setPropertyName()
{
	m_properties->m_propertyName = "burger";
}

void BurgerBuilder::setWordsFilePath()
{
	m_properties->m_wordsFilePath = "words/burger.txt";
}

void BurgerBuilder::setIconPaths()
{
	m_properties->m_iconPaths.reserve(6);
	for (auto i = 0; i < 6; ++i)
		m_properties->m_iconPaths.push_back(":/burger/burger_" + QString::number(i));
}
