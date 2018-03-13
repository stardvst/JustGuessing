#include "JustGuessing.h"
#include "strings.h"

// the pages
#include "pages/categoriesPage.h"
#include "pages/chooseUsernamePage.h"
#include "pages/highScoresPage.h"
#include "pages/correctGuessPage.h"
#include "pages/gamePage.h"

// qt includes
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QIcon>
#include <QColor>
#include <QLinearGradient>
#include <QApplication>
#include <QPushButton>
#include <QSpacerItem>
#include <QFontDatabase>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QSettings>
#include <QHelpEvent>

// define the static instance
JustGuessing *JustGuessing::m_pGameInstance = nullptr;

JustGuessing &JustGuessing::getInstance()
{
	if (!m_pGameInstance)
		m_pGameInstance = new JustGuessing;
	return *m_pGameInstance;
}

JustGuessing::JustGuessing(QWidget *parent)
	: QWidget(parent)
	, m_pGameOverStrLabel(nullptr)
	, m_pLstOfWordLabel(nullptr)
	, m_pLstOfWords(nullptr)
{
	// set the settings file path
	m_sSettingsFile = QApplication::applicationDirPath() + "/settings.ini";

	// create the pages
	m_pPagesWidget = new QStackedWidget { this };
	m_pPagesWidget->addWidget(createHomePage());
	m_pPagesWidget->addWidget(new HighScoresPage { this, this });
	m_pPagesWidget->addWidget(new CategoriesPage { this, this });
	m_pPagesWidget->addWidget(new ChooseUsernamePage { this, this });
	m_pPagesWidget->addWidget(new GamePage { this, this });
	m_pPagesWidget->addWidget(new CorrectGuessPage { this, this });
	m_pPagesWidget->addWidget(createHelpPage());
	m_pPagesWidget->addWidget(createGameOverPage());

	const auto layout = new QVBoxLayout;
	layout->addWidget(m_pPagesWidget);
	setLayout(layout);

	QLinearGradient gradient { 25, 100, 150, 175 };
	gradient.setColorAt(0.0, QColor { 211, 50, 195 });
	gradient.setColorAt(0.2, QColor { 163, 21, 211 });
	gradient.setColorAt(1.0, QColor { 66, 33, 211 });

	auto p = palette();
	p.setBrush(QPalette::Background, QBrush { gradient });
	setPalette(p);

	setFixedSize(900, 700);
	setWindowIcon(QIcon { ":/app_logo" });
	setWindowTitle(sAppName);

	const auto pChooseUsernamePage = getChooseUsernamePage();
	connect(pChooseUsernamePage, &ChooseUsernamePage::usernameChosen, this, &JustGuessing::showCategoriesPage);
	connect(pChooseUsernamePage, &ChooseUsernamePage::newUsernameRegistered, this, &JustGuessing::setCurrentUsername);

	const auto pGamePage = getGamePage();
	connect(pGamePage, &GamePage::wordIsGuessed, this, &JustGuessing::showCorrectGuessPage);
	connect(pGamePage, &GamePage::gameOver, this, &JustGuessing::onGameOver);

	setStylesheets();
}

QStackedWidget *JustGuessing::getPagesWidget() const
{
	return m_pPagesWidget;
}

void JustGuessing::addPoint() const
{
	getHighScoresPage()->addScore(m_sCurrentUsername);
}

QWidget *JustGuessing::createHomePage()
{

	// create the title label
	const auto titleLabel = new QLabel { sAppName + '!', this };
	titleLabel->setMinimumWidth(650);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont { "Kristen ITC", 35 });
	titleLabel->setStyleSheet("color: white;");

	// Start button
	const auto startBtn = new QPushButton { sStart };
	startBtn->setMinimumSize(170, 40);
	startBtn->setCursor(Qt::PointingHandCursor);
	startBtn->setAccessibleName("hover_btn");
	connect(startBtn, &QAbstractButton::clicked, this, &JustGuessing::showPlayerNamePage);

	// High Scores button
	const auto highScoresBtn = new QPushButton { sHighScores };
	highScoresBtn->setMinimumSize(170, 40);
	highScoresBtn->setCursor(Qt::PointingHandCursor);
	highScoresBtn->setAccessibleName("hover_btn");
	connect(highScoresBtn, &QAbstractButton::clicked, this, &JustGuessing::showHighScoresPage);

	//// Settings button
	//const auto settingsBtn = new QPushButton { sSettings };
	//settingsBtn->setMinimumSize(170, 40);
	//settingsBtn->setCursor(Qt::PointingHandCursor);
	//settingsBtn->setAccessibleName("hover_btn");
	//connect(settingsBtn, &QAbstractButton::clicked, this, &JustGuessing::showSettingsPage);

	// Help button
	const auto helpBtn = new QPushButton { sHelp };
	helpBtn->setMinimumSize(170, 40);
	helpBtn->setCursor(Qt::PointingHandCursor);
	helpBtn->setAccessibleName("hover_btn");
	connect(helpBtn, &QAbstractButton::clicked, this, &JustGuessing::showHelpPage);

	// Quit button
	const auto quitBtn = new QPushButton { sQuit };
	quitBtn->setMinimumSize(170, 40);
	quitBtn->setCursor(Qt::PointingHandCursor);
	quitBtn->setAccessibleName("hover_btn");
	connect(quitBtn, &QAbstractButton::clicked, qApp, &QApplication::quit);

	// create the left-side layout
	const auto buttonsLayout = new QVBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(startBtn, 0, Qt::AlignHCenter);
	buttonsLayout->addWidget(highScoresBtn, 0, Qt::AlignHCenter);
	//buttonsLayout->addWidget(settingsBtn, 0, Qt::AlignHCenter);
	buttonsLayout->addWidget(helpBtn, 0, Qt::AlignHCenter);
	buttonsLayout->addWidget(quitBtn, 0, Qt::AlignHCenter);
	buttonsLayout->addStretch();

	// create the logo icon
	const auto iconSection = new QLabel { this };
	iconSection->setPixmap(QPixmap { ":/app_logo" });

	// create the content layout
	const auto contentLayout = new QHBoxLayout;
	contentLayout->addStretch();
	contentLayout->addLayout(buttonsLayout);
	contentLayout->addSpacerItem(new QSpacerItem { 80, 0 });
	contentLayout->addWidget(iconSection);
	contentLayout->addStretch();

	// create the main layout
	const auto pageLayout = new QVBoxLayout;
	pageLayout->addSpacerItem(new QSpacerItem { 0, 70 });
	pageLayout->addWidget(titleLabel);
	pageLayout->addSpacerItem(new QSpacerItem { 0, 35 });
	pageLayout->addLayout(contentLayout);
	pageLayout->addStretch();

	// create a widget to bear this layout
	const auto page = new QWidget { this };
	page->setLayout(pageLayout);

	return page;
}

QWidget *JustGuessing::createHelpPage()
{
	// Back button
	const auto backBtn = new QPushButton { this };
	backBtn->setIcon(QIcon { ":/controls/back_button" });
	backBtn->setStyleSheet("background: rgba(0, 0, 0, 0); margin-top: 4px;");
	backBtn->setCursor(Qt::PointingHandCursor);
	backBtn->setMaximumSize(backBtn->minimumSizeHint());
	connect(backBtn, &QAbstractButton::clicked, this, &JustGuessing::showHomePage);

	// create help icon
	const auto helpIcon = new QLabel { this };
	helpIcon->setPixmap(QPixmap { ":/controls/help" });

	// create the Instructions label
	const auto pInstrLabel = new QLabel { sGameInstructions, this };
	pInstrLabel->setAlignment(Qt::AlignCenter);
	pInstrLabel->setStyleSheet("color: white; font-size: 25px;");

	// icon and title layout
	const auto iconTitleLayout = new QHBoxLayout;
	iconTitleLayout->addStretch();
	iconTitleLayout->addWidget(helpIcon);
	iconTitleLayout->addSpacerItem(new QSpacerItem { 10, 0 });
	iconTitleLayout->addWidget(pInstrLabel);
	iconTitleLayout->addStretch();

	// label for the instruction text
	const auto pInstrTextLabel = new QLabel { sInstructionsText, this };
	pInstrTextLabel->setFixedSize(QSize { 500, 350 });
	pInstrTextLabel->setWordWrap(true);

	// "Let's play" button
	const auto pLetsPlayBtn = new QPushButton { sLetsPlay, this };
	pLetsPlayBtn->setMinimumSize(QSize { 200,20 });
	pLetsPlayBtn->setCursor(Qt::PointingHandCursor);
	pLetsPlayBtn->setStyleSheet("padding: 15px 15px; border-radius: 25px; ");
	connect(pLetsPlayBtn, &QAbstractButton::clicked, this, &JustGuessing::showPlayerNamePage);

	// create the main layout
	const auto pageLayout = new QVBoxLayout;
	pageLayout->addWidget(backBtn);
	pageLayout->addSpacerItem(new QSpacerItem { 0, 35 });
	pageLayout->addLayout(iconTitleLayout);
	pageLayout->addSpacerItem(new QSpacerItem { 0, 20 });
	pageLayout->addWidget(pInstrTextLabel, 0, Qt::AlignHCenter);
	pageLayout->addSpacerItem(new QSpacerItem { 0, 35 });
	pageLayout->addWidget(pLetsPlayBtn, 0, Qt::AlignCenter);
	pageLayout->addStretch();

	// create a widget to bear this layout
	const auto page = new QWidget { this };
	page->setLayout(pageLayout);

	return page;
}

QWidget* JustGuessing::createGameOverPage()
{
	// "Game Over" label
	const auto pGameOverLabel = new QLabel { "Game Over!", this };
	pGameOverLabel->setStyleSheet("font-size: 50px;");

	// string to display for the user
	m_pGameOverStrLabel = new QLabel { this };
	m_pGameOverStrLabel->setStyleSheet("font-size: 30px;");

	// list of guessed words label
	m_pLstOfWordLabel = new QLabel { this };
	m_pLstOfWordLabel->setStyleSheet("font-size: 20px;");

	// the actual list of words
	m_pLstOfWords = new QLabel { this };
	m_pLstOfWords->setStyleSheet("font-size: 17px;");

	// Go Home button :D
	const auto pGoHomeBtn = new QPushButton { QObject::tr("Go Home") };
	pGoHomeBtn->setMinimumSize(QSize { 150, 50 });
	pGoHomeBtn->setStyleSheet("padding: 15px; border-radius: 25px;");
	pGoHomeBtn->setCursor(Qt::PointingHandCursor);
	connect(pGoHomeBtn, &QAbstractButton::clicked, this, &JustGuessing::showHomePage);

	const auto pPageLayout = new QVBoxLayout;
	pPageLayout->addStretch();
	pPageLayout->addWidget(pGameOverLabel, 0, Qt::AlignCenter);
	pPageLayout->addSpacerItem(new QSpacerItem { 0, 15 });
	pPageLayout->addWidget(m_pGameOverStrLabel, 0, Qt::AlignCenter);
	pPageLayout->addSpacerItem(new QSpacerItem { 0, 25 });
	pPageLayout->addWidget(m_pLstOfWordLabel, 0, Qt::AlignCenter);
	pPageLayout->addSpacerItem(new QSpacerItem { 0, 20 });
	pPageLayout->addWidget(m_pLstOfWords, 0, Qt::AlignCenter);
	pPageLayout->addStretch(128);
	pPageLayout->addWidget(pGoHomeBtn, 0, Qt::AlignCenter);
	pPageLayout->addStretch(255);

	const auto page = new QWidget { this };
	page->setLayout(pPageLayout);

	return page;
}

const QString &JustGuessing::getSettingsFilePath() const
{
	return m_sSettingsFile;
}

HighScoresPage* JustGuessing::getHighScoresPage() const
{
	return dynamic_cast<HighScoresPage *>(m_pPagesWidget->widget(1));
}

CategoriesPage *JustGuessing::getCategoriesPage() const
{
	return dynamic_cast<CategoriesPage *>(m_pPagesWidget->widget(2));
}

ChooseUsernamePage *JustGuessing::getChooseUsernamePage() const
{
	return dynamic_cast<ChooseUsernamePage *>(m_pPagesWidget->widget(3));
}

GamePage *JustGuessing::getGamePage() const
{
	return dynamic_cast<GamePage *>(m_pPagesWidget->widget(4));
}

CorrectGuessPage* JustGuessing::getCorrectGuessPage() const
{
	return dynamic_cast<CorrectGuessPage *>(m_pPagesWidget->widget(5));
}

void JustGuessing::showHomePage() const
{
	m_pPagesWidget->setCurrentIndex(0);
}

void JustGuessing::showHighScoresPage() const
{
	// emit this signal to update the entries
	emit pageAboutToShow();

	m_pPagesWidget->setCurrentIndex(1);
}

void JustGuessing::showCategoriesPage() const
{
	m_pPagesWidget->setCurrentIndex(2);
}

void JustGuessing::showPlayerNamePage() const
{
	m_pPagesWidget->setCurrentIndex(3);
}

void JustGuessing::showGamePage() const
{
	m_pPagesWidget->setCurrentIndex(4);
}

void JustGuessing::showCorrectGuessPage() const
{
	m_pPagesWidget->setCurrentIndex(5);
}

void JustGuessing::showHelpPage() const
{
	m_pPagesWidget->setCurrentIndex(6);
}

void JustGuessing::showGameOverPage() const
{
	m_pPagesWidget->setCurrentIndex(7);
}

void JustGuessing::setCurrentUsername(const QString &username)
{
	m_sCurrentUsername = username;
	emit usernameChanged(m_sCurrentUsername);
}

QString JustGuessing::getCurrentUsername() const
{
	return m_sCurrentUsername;
}

void JustGuessing::closeEvent(QCloseEvent *event)
{
	emit appIsClosing();
}

void JustGuessing::onGameOver() const
{
	// # of guessed letters for the current user
	const auto iGuessedLetters = getHighScoresPage()->getScore(m_sCurrentUsername);

	m_pGameOverStrLabel->setText(QString { "%1, you guessed %2 letter%3, %4" }
								 .arg(m_sCurrentUsername, QString::number(iGuessedLetters),
									  iGuessedLetters == 1 ? "" : "s",
									  iGuessedLetters == 0 ? "not good :("
									  : iGuessedLetters < 6 ? "not bad!"
									  : "good job!"));

	// if no letters were guessed, return
	if (iGuessedLetters == 0) return;

	QVector<QString> guessedWords;
	getGamePage()->getGuessedWords(guessedWords);

	QString words;
	for (const auto &word : guessedWords)
		words.append(word + '\n');

	// if no words were guessed, don't display anything
	if (guessedWords.isEmpty()) return;

	m_pLstOfWordLabel->setText("Here's is the list of your guessed words:\n");
	m_pLstOfWords->setText(words);
}

void JustGuessing::setStylesheets()
{
	setStyleSheet("QStackedWidget {"
				  "    background: qlineargradient("
				  "	       x1: 0, y1:0, x2: 1, y2: 1,"
				  "	       stop: 0.0 #4221D3, "
				  "        stop: 0.25 #9D14FF,"
				  "		   stop: 0.75 #AF4BA8,"
				  "        stop: 1.0 #4C05FF"
				  "    );"
				  "}"
				  "QPushButton, .categoryBtn {"
				  "    background-color: white;"
				  "    border-radius: 20px;"
				  "    color: #4A0C46;"
				  "    font: 17px;"
				  "}"
				  ".categoryBtn {"
				  "    border-radius: 0;"
				  "    font-size: 15px;"
				  "}"
				  "QLabel {"
				  "    color: white;"
				  "    border-radius: 25px;"
				  "}"
	);
}