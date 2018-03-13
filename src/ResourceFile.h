#ifndef RESOURCEFILE_H
#define RESOURCEFILE_H

#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QDir>

#define FILES_DIR "resources"

class ResourceFile : public QFile
{
public:
	ResourceFile(const QString &respath, QObject * parent = 0)
	{
		QFileInfo resource(respath);
		QDir files = QDir::current();

		if (!files.cd(FILES_DIR))
		{
			files.mkdir(FILES_DIR);
			files.cd(FILES_DIR);
		}

		setFileName(files.filePath(resource.fileName()));
		if (!exists())
		{
			QFile resfile(respath);
			if (resfile.open(QIODevice::ReadOnly))
			{
				if (open(QIODevice::ReadWrite))
				{
					write(resfile.readAll());
					close();
				}
				resfile.close();
			}
		}
	}
};

#undef FILES_DIR

#endif // RESOURCEFILE_H