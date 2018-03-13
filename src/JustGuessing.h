#ifndef JustGuessing_H
#define JustGuessing_H

#include <QLabel>

#include <string_view>

// forward declarations
class QStackedWidget;
class UsernameLineEdit;
class GamePage;
class CategoriesPage;
class ChooseUsernamePage;
class CorrectGuessPage;
class HighScoresPage;

class JustGuessing : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString currentUsername READ getCurrentUsername WRITE setCurrentUsername)

public:

	// static getter for the one game object
	static JustGuessing &getInstance();

	// methods to access pages
	QStackedWidget *getPagesWidget() const;
	CategoriesPage *getCategoriesPage() const;
	GamePage *getGamePage() const;
	HighScoresPage *getHighScoresPage() const;

	// returns the settings file path
	const QString &getSettingsFilePath() const;

	// setter/getter for username
	void setCurrentUsername(const QString &username);
	QString getCurrentUsername() const;

	// adds 1 point to current user's score
	void addPoint() const;

	public slots:
	void showHomePage() const;
	void showGamePage() const;
	void showCategoriesPage() const;
	void showPlayerNamePage() const;
	void showHighScoresPage() const;
	void showCorrectGuessPage() const;
	void showHelpPage() const;
	void showGameOverPage() const;

	void onGameOver() const;

signals:
	void pageAboutToShow() const;
	void usernameChanged(const QString &newUsername) const;
	void appIsClosing() const;

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	explicit JustGuessing(QWidget *parent = nullptr);

	QWidget *createHomePage();
	QWidget *createHelpPage();
	QWidget *createGameOverPage();

	ChooseUsernamePage *getChooseUsernamePage() const;
	CorrectGuessPage *getCorrectGuessPage() const;

	void setStylesheets();

private:

	// the one game object
	static JustGuessing *m_pGameInstance;

	// the pages container
	QStackedWidget *m_pPagesWidget;

	// the current player's username
	QString m_sCurrentUsername;

	// labels for game over page
	QLabel *m_pGameOverStrLabel;
	QLabel *m_pLstOfWordLabel;
	QLabel *m_pLstOfWords;

	// settings file path
	QString m_sSettingsFile;
};

#endif // !JustGuessing_H
