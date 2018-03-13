#include "correctGuessPage.h"
#include "strings.h"

// pages
#include "pages/gamePage.h"
#include "pages/highScoresPage.h"

// qt includes
#include <QPushButton>
#include <QGridLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QToolButton>

CorrectGuessPage::CorrectGuessPage(JustGuessing *pGame, QWidget *parent /* = nullptr */)
	: QWidget(parent)
	, m_pGame(pGame)
	, m_sGuessedWord(pGame->getGamePage()->getSecretWord()) {

	// "Correct" label
	const auto pCorrectLabel = new QLabel { "CORRECT!", this };
	pCorrectLabel->setStyleSheet("font-size: 45px;");

	m_pWikiPageBtn = new QToolButton { this };
	m_pWikiPageBtn->setIcon(QIcon { ":/controls/whats_this" });
	m_pWikiPageBtn->setIconSize(QSize { 20, 20 });
	m_pWikiPageBtn->setCursor(Qt::PointingHandCursor);
	m_pWikiPageBtn->setStyleSheet("border-radius: 25px;");
	connect(m_pWikiPageBtn, &QAbstractButton::clicked, this, &CorrectGuessPage::onWikiPageRequested);

	const auto pWordWordLabel = new QLabel { "WORD :", this };
	pWordWordLabel->setStyleSheet("font-size: 20px;");
	m_pWordLabel = new QLabel { this };

	const auto pScoreWordLabel = new QLabel { "SCORE : ", this };
	pScoreWordLabel->setStyleSheet("font-size: 20px;");
	m_pScoreLabel = new QLabel { this };

	// create info layout
	const auto pInfoLayout = new QGridLayout;
	pInfoLayout->setColumnStretch(0, 1);
	pInfoLayout->addWidget(pWordWordLabel, 0, 1);
	pInfoLayout->addWidget(m_pWordLabel, 0, 2);
	pInfoLayout->addWidget(m_pWikiPageBtn, 0, 3);
	pInfoLayout->addWidget(pScoreWordLabel, 1, 1);
	pInfoLayout->addWidget(m_pScoreLabel, 1, 2);
	pInfoLayout->setColumnStretch(4, 1);

	// Choose another category button
	/*const auto pChooseCategoryBtn = new QPushButton { sChooseCategory, this };
	pChooseCategoryBtn->setMinimumSize(QSize { 250, 50 });
	pChooseCategoryBtn->setStyleSheet("border-radius: 25px;");
	connect(pChooseCategoryBtn, &QAbstractButton::clicked, pGame, &JustGuessing::showCategoriesPage);*/

	// Next Puzzle button
	const auto pNextPuzzleBtn = new QPushButton { sNextPuzzle, this };
	pNextPuzzleBtn->setMinimumSize(QSize { 150, 50 });
	pNextPuzzleBtn->setStyleSheet("border-radius: 25px;");
	pNextPuzzleBtn->setCursor(Qt::PointingHandCursor);
	connect(pNextPuzzleBtn, &QAbstractButton::clicked, pGame, &JustGuessing::showGamePage);
	connect(pNextPuzzleBtn, &QAbstractButton::clicked, pGame->getGamePage(), &GamePage::reset);

	// buttons layout
	const auto pBtnLayout = new QVBoxLayout;
	pBtnLayout->addStretch();
	//pBtnLayout->addWidget(pChooseCategoryBtn, 0, Qt::AlignCenter);
	pBtnLayout->addWidget(pNextPuzzleBtn, 0, Qt::AlignCenter);
	pBtnLayout->addStretch();

	// create the page layout
	const auto pageLayout = new QVBoxLayout;
	pageLayout->addStretch();
	pageLayout->addWidget(pCorrectLabel, 0, Qt::AlignCenter);
	pageLayout->addStretch();
	pageLayout->addLayout(pInfoLayout, 0);
	pageLayout->addStretch();
	pageLayout->addLayout(pBtnLayout);
	pageLayout->addStretch();

	setLayout(pageLayout);

	connect(m_pGame->getGamePage(), &GamePage::wordIsGuessed, this, &CorrectGuessPage::onWordIsGuessed);

	setStylesheets();
}

void CorrectGuessPage::onWordIsGuessed(const QString &secretWord) {
	
	// save the word
	m_sGuessedWord = secretWord;

	// add a tooltip
	m_pWikiPageBtn->setToolTip(sWikiBtnToolTip.arg(m_sGuessedWord));

	// add the word to the label
	m_pWordLabel->setText(m_sGuessedWord);

	// add score to the score label
	m_pScoreLabel->setText(QString::number(m_pGame->getHighScoresPage()->getScore(m_pGame->getCurrentUsername())));
}

void CorrectGuessPage::onWikiPageRequested() const {
	QDesktopServices::openUrl(QUrl { "https://en.wikipedia.org/wiki/" + m_sGuessedWord });
}

void CorrectGuessPage::setStylesheets() {
	setStyleSheet(
		"QLabel {"
		"	font: 25px bold;"
		"}"
	);
}
