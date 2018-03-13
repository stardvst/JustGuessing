#ifndef GAME_PAGE_H
#define GAME_PAGE_H

#include <QWidget>
#include <QSet>

// forward declarations
class JustGuessing;
class QPaintEvent;
class QPushButton;
class QLabel;
class QHBoxLayout;
class SettingsPage;
struct CategoryProperties;

class GamePage : public QWidget
{
	Q_OBJECT

public:
	explicit GamePage(JustGuessing *pGame, QWidget *parent = nullptr);

	void getGuessedWords(QVector<QString> &guessedWords) const;
	QString getSecretWord() const;
	void reset();

signals:
	void wordIsGuessed(const QString &secretWord);
	void gameOver();

public slots:
	void init();
	void onLetterIsGuessed();
	void onCategoryChanged();

protected:
	void keyPressEvent(QKeyEvent *event) override;

private slots:
	void hideLetter() const;
	void updateHiddenWord();
	void onUsernameChanged(const QString &newUsername);

private:
	bool loadWords(CategoryProperties *pCtgProps);
	void makeSecretWord();
	void setStylesheets();

	void hide(QPushButton *btn) const;
	void hide(const QChar &letter) const;
	void updateWord(const QChar &btnLetter);

	QPushButton *getLetterButton(const QChar &letter) const;

private:
	JustGuessing * m_pGame;

	bool m_bMuted;

	QLabel *m_pScoreLabel;
	QLabel *m_pGuessesLeftLabel;
	QLabel *m_pUsernameLabel;

	QSet<QString> m_words;
	QString m_secretWord;

	QVector<QPushButton *> m_letterBtns;
	QVector<QLabel *> m_secretWordLetters;

	// heart image labels
	QVector<QLabel *> m_pHeartImages;

	// counters  for correct/wrong letters
	int m_iCorrectLetters { 0 };
	int m_iWrongLetters { 0 };

	// max wrong letter count
	static const auto MAX_WRONG_LETTERS = 6;

	// total lives
	static const auto LIVES = 4;
	int m_iLives { LIVES };

	QLabel *m_pProductImage;
	QHBoxLayout *m_pHiddenLettersLayout;
	QHBoxLayout *m_pFirstRowLettersLayout;
	QHBoxLayout *m_pSecondRowLettersLayout;
	CategoryProperties *m_pCtgProps;

	// list of guessed words
	QVector<QString> m_guessedWords;

	bool m_bInited { false };
};

#endif // !GAME_PAGE_H
