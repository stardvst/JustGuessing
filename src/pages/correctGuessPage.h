#ifndef CORRECTGUESSPAGE_H
#define CORRECTGUESSPAGE_H

#include <QWidget>

class JustGuessing;
class QToolButton;
class QLabel;

class CorrectGuessPage : public QWidget
{
	Q_OBJECT
public:
	explicit CorrectGuessPage(JustGuessing *pGame, QWidget *parent = nullptr);

	private slots:
	void onWordIsGuessed(const QString &secretWord);
	void onWikiPageRequested() const;

private:
	void setStylesheets();

private:

	JustGuessing * m_pGame;

	// the question mark button
	QToolButton *m_pWikiPageBtn;

	// the score label
	QLabel *m_pWordLabel;

	// the score label
	QLabel *m_pScoreLabel;

	// the correct word
	QString m_sGuessedWord;
};

#endif // !CORRECTGUESSPAGE_H