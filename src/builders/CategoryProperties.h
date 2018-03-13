#ifndef CATEGORYPROPERTIES_H
#define CATEGORYPROPERTIES_H

#include <QVector>

struct CategoryProperties
{
	QString m_propertyName;
	QString m_wordsFilePath;
	QVector<QString> m_iconPaths;
};

#endif // !CATEGORYPROPERTIES_H
