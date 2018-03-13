#ifndef COMP_BUILDER_H
#define COMP_BUILDER_H

#include "propertiesBuilder.h"

class CompBuilder : public PropertiesBuilder
{
public:
	CompBuilder();
	~CompBuilder() override;

	void setPropertyName() override;
	void setWordsFilePath() override;
	void setIconPaths() override;
};

#endif // !COMP_BUILDER_H
