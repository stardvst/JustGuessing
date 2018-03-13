#include "compBuilder.h"
#include "propertiesBuilder.h"
#include "categoryProperties.h"

#include <vector>

CompBuilder::CompBuilder()
{
	m_properties = new CategoryProperties;
}

CompBuilder::~CompBuilder()
{
	delete m_properties;
}

void CompBuilder::setPropertyName()
{
	m_properties->m_propertyName = "computer";
}

void CompBuilder::setWordsFilePath()
{
	m_properties->m_wordsFilePath = "words/computer.txt";
}

void CompBuilder::setIconPaths()
{
	m_properties->m_iconPaths.reserve(6);
	for (auto i = 0; i < 6; ++i)
		m_properties->m_iconPaths.push_back(":/computer/computer_" + QString::number(i));
}
