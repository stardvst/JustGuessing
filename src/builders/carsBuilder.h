#ifndef CARS_BUILDER_H
#define CARS_BUILDER_H

#include "propertiesBuilder.h"

class CarsBuilder : public PropertiesBuilder
{
public:
	CarsBuilder();
	~CarsBuilder() override;

	void setPropertyName() override;
	void setWordsFilePath() override;
	void setIconPaths() override;
};

#endif // !CARS_BUILDER_H
