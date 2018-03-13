#ifndef PROPERTY_BUILDER_H
#define PROPERTY_BUILDER_H

#include "categoryProperties.h"

class PropertiesBuilder
{
public:
	virtual ~PropertiesBuilder() = 0;

	virtual void setPropertyName() = 0;
	virtual void setWordsFilePath() = 0;
	virtual void setIconPaths() = 0;

	CategoryProperties *getPropertiesPtr() const { return m_properties; }

protected:
	CategoryProperties * m_properties;
};

inline PropertiesBuilder::~PropertiesBuilder() {}

#endif // !PROPERTY_BUILDER_H
