#ifndef BURGER_BUILDER_H
#define BURGER_BUILDER_H

#include "propertiesBuilder.h"

class BurgerBuilder : public PropertiesBuilder
{
public:
	BurgerBuilder();
	~BurgerBuilder() override;

	void setPropertyName() override;
	void setWordsFilePath() override;
	void setIconPaths() override;
};

#endif // !BURGER_BUILDER_H
