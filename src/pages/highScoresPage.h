#ifndef HIGHSCORESPAGE_H
#define HIGHSCORESPAGE_H

#include "../JustGuessing.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMap>

class HighScoresPage : public QWidget
{
	Q_OBJECT
public:
	//using QVariantMultiMap_t = QMultiMap<QVariant, QString>;

	//friend QDataStream &operator<<(QDataStream &out, const QVariantMultiMap_t &mmap);
	//friend QDataStream &operator>>(QDataStream &in, QVariantMultiMap_t &mmap);

	explicit HighScoresPage(JustGuessing *pGame, QWidget *parent = nullptr);

	int getScore(const QString &username) const;
	void addScore(const QString &username);

private:
	void readSettings();

	void createHighScoreEntries();
	void setStylesheets();

	private slots:
	void writeSettings() const;

private:

	JustGuessing * m_pGame;

	// the number of high score entries to save/display
	static const auto HIGH_SCORE_ENTRIES = 7;

	// username => score map
	QVariantMap m_highScores;

	// layout to hold high score entries
	QVBoxLayout *m_pEntriesLayout;
};

//Q_DECLARE_METATYPE(HighScoresPage::QVariantMultiMap_t)

#endif // !HIGHSCORESPAGE_H