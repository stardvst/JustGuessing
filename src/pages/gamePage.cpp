#include "../JustGuessing.h"
#include "../builders/categoryProperties.h"
#include "pages/categoriesPage.h"
#include "pages/highScoresPage.h"
#include "pages/gamePage.h"
#include "pages/correctGuessPage.h"
#include "../ResourceSound.h"

#include <random>

#include <QPushButton>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QtMultimedia/QMediaPlayer>
#include <QApplication>
#include <QMessageBox>
#include <QAccessible>
#include <QUrl>
#include <QDir>
#include <QDebug>

GamePage::GamePage(JustGuessing *pGame, QWidget *parent /* = nullptr */)
	: QWidget(parent)
	, m_pGame(pGame)
	, m_pScoreLabel(nullptr)
	, m_pGuessesLeftLabel(nullptr)
	, m_pUsernameLabel(nullptr)
	, m_iCorrectLetters(0)
	, m_iWrongLetters(0)
	, m_pProductImage(nullptr)
	, m_pHiddenLettersLayout(nullptr)
	, m_pFirstRowLettersLayout(nullptr)
	, m_pSecondRowLettersLayout(nullptr)
	, m_pCtgProps(nullptr)
{
	const auto pCategoriesPage = m_pGame->getCategoriesPage();
	connect(pCategoriesPage, &CategoriesPage::canStartGame, this, &GamePage::init);
	connect(pCategoriesPage, &CategoriesPage::canStartGame, this, &GamePage::onCategoryChanged);
	connect(pCategoriesPage, &CategoriesPage::canStartGame, this, &GamePage::reset);

	connect(pGame, &JustGuessing::usernameChanged, this, &GamePage::onUsernameChanged);
}

void GamePage::init()
{

	// init only once
	if (m_bInited) return;

	onCategoryChanged();
	makeSecretWord();

	// Back button
	const auto pBackBtn = new QPushButton { this };
	pBackBtn->setIcon(QIcon { ":/controls/back_button" });
	pBackBtn->setStyleSheet("background: rgba(0, 0, 0, 0); margin-top: 4px;");
	pBackBtn->setCursor(Qt::PointingHandCursor);
	pBackBtn->setMaximumSize(pBackBtn->minimumSizeHint());
	connect(pBackBtn, &QAbstractButton::clicked, m_pGame, &JustGuessing::showCategoriesPage);

	// Username and score section
	const auto sCurrentUsername = m_pGame->getCurrentUsername();

	// score label
	m_pScoreLabel = new QLabel { QString::number(m_pGame->getHighScoresPage()->getScore(sCurrentUsername)), this };

	// guesses left label
	m_pGuessesLeftLabel = new QLabel { QString::number(MAX_WRONG_LETTERS), this };

	// create hearts layout
	const auto pHeartsLayout = new QHBoxLayout;
	for (auto i = 0; i < LIVES; ++i)
	{
		const auto pHeartLabel = new QLabel { this };
		pHeartLabel->setPixmap(QPixmap { ":/controls/heart" });
		m_pHeartImages.push_back(pHeartLabel);
		pHeartsLayout->addWidget(pHeartLabel);
	}

	// the username label
	m_pUsernameLabel = new QLabel { sCurrentUsername, this };

	const auto infoLayout = new QGridLayout;
	infoLayout->addWidget(new QLabel { QObject::tr("Lives left:"), this }, 0, 0);
	infoLayout->addLayout(pHeartsLayout, 0, 1);
	infoLayout->addWidget(new QLabel { QObject::tr("Username: "), this }, 1, 0);
	infoLayout->addWidget(m_pUsernameLabel, 1, 1);
	infoLayout->addWidget(new QLabel { QObject::tr("Score: "), this }, 2, 0);
	infoLayout->addWidget(m_pScoreLabel, 2, 1);
	infoLayout->addWidget(new QLabel { QObject::tr("Guesses left: "), this }, 3, 0);
	infoLayout->addWidget(m_pGuessesLeftLabel, 3, 1);

	const auto headerLayout = new QHBoxLayout;
	headerLayout->addWidget(pBackBtn, 0, Qt::AlignLeft | Qt::AlignTop);
	headerLayout->addSpacerItem(new QSpacerItem { 650, 0 });
	headerLayout->addLayout(infoLayout, 0);
	headerLayout->addStretch();

	// Item image section
	m_pProductImage = new QLabel { this };
	m_pProductImage->setPixmap(QPixmap { m_pCtgProps->m_iconPaths.front() }); // set initial picture

	const auto currentProductLayout = new QVBoxLayout;
	currentProductLayout->addWidget(m_pProductImage, 0, Qt::AlignHCenter);

	QFont font { "Monospace", 50 };

	// guessed word layout
	m_pHiddenLettersLayout = new QHBoxLayout;
	m_pHiddenLettersLayout->addStretch();
	const auto secretWordSize = m_secretWord.size();
	for (auto letter = 0; letter < secretWordSize; ++letter)
	{
		const auto letterLabel = new QLabel { "_", this };
		letterLabel->setFont(font);
		letterLabel->setFixedWidth(60);
		m_pHiddenLettersLayout->addWidget(letterLabel);
		m_secretWordLetters.push_back(letterLabel);
	}
	m_pHiddenLettersLayout->addStretch();

	const auto wordLayout = new QVBoxLayout;
	wordLayout->addLayout(m_pHiddenLettersLayout);
	wordLayout->addSpacerItem(new QSpacerItem { 0, 5 });
	wordLayout->addWidget(new QLabel { QObject::tr("The secret word") }, 0, Qt::AlignHCenter);

	// available letters section
	font.setFamily(QApplication::font().family()); // restore the application's default font
	font.setPointSize(11);

	// first row of letters
	m_pFirstRowLettersLayout = new QHBoxLayout;
	m_pFirstRowLettersLayout->addStretch();
	for (auto ch = 'A'; ch <= 'M'; ++ch)
	{
		const auto letterBtn = new QPushButton { QString{ch} };
		letterBtn->setMaximumSize(30, 30);
		letterBtn->setCursor(Qt::PointingHandCursor);
		connect(letterBtn, &QAbstractButton::clicked, this, &GamePage::hideLetter);
		connect(letterBtn, &QAbstractButton::clicked, this, &GamePage::updateHiddenWord);

		m_pFirstRowLettersLayout->addWidget(letterBtn);
		m_letterBtns.push_back(letterBtn);
	}
	m_pFirstRowLettersLayout->addStretch();

	// second row of letters
	m_pSecondRowLettersLayout = new QHBoxLayout;
	m_pSecondRowLettersLayout->addStretch(0);
	for (auto ch = 'N'; ch <= 'Z'; ++ch)
	{
		const auto letterBtn = new QPushButton { QString{ch} };
		letterBtn->setMaximumSize(30, 30);
		letterBtn->setCursor(Qt::PointingHandCursor);
		connect(letterBtn, &QAbstractButton::clicked, this, &GamePage::hideLetter);
		connect(letterBtn, &QAbstractButton::clicked, this, &GamePage::updateHiddenWord);

		m_pSecondRowLettersLayout->addWidget(letterBtn);
		m_letterBtns.push_back(letterBtn);
	}
	m_pSecondRowLettersLayout->addStretch();

	setStylesheets();

	const auto availLettersLayout = new QVBoxLayout;
	availLettersLayout->addLayout(m_pFirstRowLettersLayout, 0);
	availLettersLayout->addLayout(m_pSecondRowLettersLayout, 0);
	availLettersLayout->addSpacerItem(new QSpacerItem { 0, 5 });
	availLettersLayout->addWidget(new QLabel(QObject::tr("Available Letters")), 0, Qt::AlignHCenter);

	// sum up the layouts
	const auto pageLayout = new QVBoxLayout;
	pageLayout->addLayout(headerLayout);
	pageLayout->addStretch();
	pageLayout->addLayout(currentProductLayout);
	pageLayout->addLayout(wordLayout);
	pageLayout->addStretch();
	pageLayout->addLayout(availLettersLayout);
	pageLayout->addStretch();

	setLayout(pageLayout);

	m_bInited = true;
}

QString GamePage::getSecretWord() const
{
	return m_secretWord;
}

void GamePage::hideLetter() const
{
	const auto btn = dynamic_cast<QPushButton *>(sender());
	hide(btn);
}

void GamePage::hide(QPushButton *btn) const
{
	btn->setStyleSheet("background: rgba(0, 0, 0, 0); color: rgba(0, 0, 0, 0);"); // this basically means: hide the button
	btn->setDisabled(true);
}

void GamePage::hide(const QChar &letter) const
{
	if (const auto pLetterBtn = getLetterButton(letter); pLetterBtn->isEnabled())
		hide(pLetterBtn);
}

void GamePage::updateHiddenWord()
{
	// get the letter from the pressed button
	const QChar btnLetter = static_cast<QPushButton *>(sender())->text()[0];

	// do the actual update
	updateWord(btnLetter);
}

void GamePage::updateWord(const QChar &btnLetter)
{
	auto bFound = false;
	const auto secretWordSize = m_secretWord.size();

	for (auto letter = 0; letter < secretWordSize; ++letter)
	{
		if (QString::compare(QString(m_secretWord[letter]), btnLetter, Qt::CaseInsensitive) == 0)
		{ // are equal
			m_secretWordLetters[letter]->setText(btnLetter.toUpper()); // display the letter instead of underscore
			bFound = true;

			onLetterIsGuessed();
		}
	}

	// if all letters of the word are guessed
	if (m_iCorrectLetters == secretWordSize)
	{
		// play a sound!
		ResourceSound::play(":/sounds/win");

		// add the words to the list of guessed words
		m_guessedWords.push_back(m_secretWord);

		// notify the game
		emit wordIsGuessed(m_secretWord);
	}
	else if (!bFound)
	{
		// the letter wasn't guessed correctly, increment wrong letter count
		++m_iWrongLetters;

		auto ok = true;
		m_pGuessesLeftLabel->setText(QString::number(m_pGuessesLeftLabel->text().toInt(&ok) - 1));

		// if reached max wrong letter count
		if (m_iWrongLetters == MAX_WRONG_LETTERS)
		{
			// if no lives left
			if (m_iLives == 0)
			{
				emit gameOver();
				m_pGame->showGameOverPage();
				return;
			}

			// show nothing in place of image
			QPixmap pm { m_pProductImage->pixmap()->size() };
			pm.fill(Qt::transparent);
			m_pProductImage->setPixmap(pm);

			// create a custom message box
			const auto propName = m_pCtgProps->m_propertyName;
			QMessageBox msgBox;
			msgBox.setText(QString { "<font size=\"25px\">You guessed the letters wrong and lost all parts of %1 :(\n"
						   "The correct word was <font color=\"#9D14FF\">%2</font>.\nMaybe try again?" }.arg(propName, m_secretWord));
			msgBox.setWindowTitle(QString { "%1 is gone :(" }.arg("The " + propName));

			// create a "yes" button
			const auto yesBtn = msgBox.addButton(QObject::tr("Yes, try again!"), QMessageBox::YesRole);
			yesBtn->setFixedWidth(130);
			yesBtn->setStyleSheet("font-size: 15px;");

			// create a "no" button
			const auto noBtn = msgBox.addButton(QObject::tr("No, I give up :("), QMessageBox::NoRole);
			noBtn->setFixedWidth(130);
			noBtn->setStyleSheet("font-size: 15px;");

			// if the sounds are turned on
			/*if (!m_bMuted)
			{*/
				// play a sound for game lost
				ResourceSound::play(":/sounds/lose");
			//}

			msgBox.exec();
			if (msgBox.clickedButton() == yesBtn)
			{
				// decrement lives count
				--m_iLives;

				// remove one heart from lives
				QPixmap pixmap { m_pHeartImages[m_iLives]->pixmap()->size() };
				pixmap.fill(Qt::transparent);
				m_pHeartImages[m_iLives]->setPixmap(pixmap);

				// reset the layouts
				reset();
			}
			else
				m_pGame->showHomePage();
		}
		else
		{ // there are still guesses left

			 // update the image
			m_pProductImage->setPixmap(QPixmap { m_pCtgProps->m_iconPaths[m_iWrongLetters] });

			//// if the sounds are turned on
			/*if (!m_bMuted)
			{*/
				// play a sound for wrong guess
				ResourceSound::play(":/sounds/wrong");
			//}
		}
	}
	else //if (!m_bMuted)
	{ // guessed a correct letter
		// play a sound for right guess
		ResourceSound::play(":/sounds/right");
	}
}

void GamePage::onUsernameChanged(const QString &newUsername)
{
	if (m_pUsernameLabel)
	{
		// update the username label
		m_pUsernameLabel->setText(newUsername);
		// update the score label
		m_pScoreLabel->setText(QString::number(0));
		// update lives count
		m_iLives = LIVES;
	}
}

QPushButton *GamePage::getLetterButton(const QChar &letter) const
{
	for (const auto btn : m_letterBtns)
		if (QString::compare(btn->text(), letter, Qt::CaseInsensitive) == 0)
			return btn;
	return nullptr; // should not happen
}

void GamePage::reset()
{

	// can't reset if is not init-ed
	if (!m_bInited) return;

	makeSecretWord();

	// reset correct and wrong letter counts, tries left label
	m_iCorrectLetters = 0;
	m_iWrongLetters = 0;
	m_pGuessesLeftLabel->setText(QString::number(MAX_WRONG_LETTERS));

	// set the initial image
	m_pProductImage->setPixmap(QPixmap { m_pCtgProps->m_iconPaths.front() });

	// update the letter buttons
	for (const auto &letterBtn : m_letterBtns)
	{
		if (!letterBtn->isEnabled())
		{
			letterBtn->setStyleSheet("background: white; color: #AF4BA8;"); // show the button
			letterBtn->setEnabled(true);
		}
	}

	// remove pointers from vector
	m_secretWordLetters.clear();

	// remove the previous hidden letter labels
	QLayoutItem *pItem;
	while ((pItem = m_pHiddenLettersLayout->takeAt(0)) != nullptr)
	{
		delete pItem->widget();
		delete pItem;
	}

	const QFont font { "Monospace", 50 };

	// create new underscore labels for the new secret word
	m_pHiddenLettersLayout->addStretch();

	// construct the underscores layout
	const auto secretWordSize = m_secretWord.size();
	for (auto letter = 0; letter < secretWordSize; ++letter)
	{
		const auto letterLabel = new QLabel { "_", this };
		letterLabel->setFont(font);
		letterLabel->setFixedWidth(60);
		m_pHiddenLettersLayout->addWidget(letterLabel);
		m_secretWordLetters.push_back(letterLabel);
	}
	m_pHiddenLettersLayout->addStretch();
}

bool GamePage::loadWords(CategoryProperties *pCtgProps)
{

	// set the file path 
	QFile wordsFile { pCtgProps->m_wordsFilePath };
	if (!wordsFile.exists()) { qDebug() << "The words file does not exist"; return false; }

	// open the file
	wordsFile.open(QIODevice::ReadOnly);
	if (!wordsFile.isOpen()) { qDebug() << "The words file cannot be opened"; return false; }

	// first clear previous words
	m_words.clear();

	// read file contents into local storage
	while (!wordsFile.atEnd())
		m_words.insert(wordsFile.readLine().simplified());

	// loading went okay
	return true;
}

void GamePage::makeSecretWord()
{
	std::random_device rd;
	std::mt19937_64 mt { rd() };
	const std::uniform_int_distribution<> dist(0, m_words.size() - 1);

	auto it = m_words.begin();
	std::advance(it, dist(mt));

	m_secretWord = *it;

	// remove the word from word list so it won't be generated again
	m_words.remove(m_secretWord);
}

void GamePage::onLetterIsGuessed()
{

	// increment correct letter count
	++m_iCorrectLetters;

	// give the user points
	m_pGame->addPoint();

	// update the score label
	auto ok = true;
	m_pScoreLabel->setText(QString::number(m_pScoreLabel->text().toInt(&ok) + 1));
}

void GamePage::onCategoryChanged()
{
	// update the category
	m_pCtgProps = m_pGame->getCategoriesPage()->getCategoryProps();
	if (!m_pCtgProps) return; // should not happen

	// load the words of the category
	loadWords(m_pCtgProps);
}

void GamePage::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_A: if (getLetterButton('a')->isEnabled()) { hide('a');	updateWord('a'); } break;
		case Qt::Key_B: if (getLetterButton('b')->isEnabled()) { hide('b');	updateWord('b'); } break;
		case Qt::Key_C: if (getLetterButton('c')->isEnabled()) { hide('c');	updateWord('c'); } break;
		case Qt::Key_D: if (getLetterButton('d')->isEnabled()) { hide('d');	updateWord('d'); } break;
		case Qt::Key_E: if (getLetterButton('e')->isEnabled()) { hide('e');	updateWord('e'); } break;
		case Qt::Key_F: if (getLetterButton('f')->isEnabled()) { hide('f');	updateWord('f'); } break;
		case Qt::Key_G: if (getLetterButton('g')->isEnabled()) { hide('g');	updateWord('g'); } break;
		case Qt::Key_H: if (getLetterButton('h')->isEnabled()) { hide('h');	updateWord('h'); } break;
		case Qt::Key_I: if (getLetterButton('i')->isEnabled()) { hide('i');	updateWord('i'); } break;
		case Qt::Key_J: if (getLetterButton('j')->isEnabled()) { hide('j');	updateWord('j'); } break;
		case Qt::Key_K: if (getLetterButton('k')->isEnabled()) { hide('k');	updateWord('k'); } break;
		case Qt::Key_L: if (getLetterButton('l')->isEnabled()) { hide('l');	updateWord('l'); } break;
		case Qt::Key_M: if (getLetterButton('m')->isEnabled()) { hide('m');	updateWord('m'); } break;
		case Qt::Key_N: if (getLetterButton('n')->isEnabled()) { hide('n');	updateWord('n'); } break;
		case Qt::Key_O: if (getLetterButton('o')->isEnabled()) { hide('o');	updateWord('o'); } break;
		case Qt::Key_P: if (getLetterButton('p')->isEnabled()) { hide('p');	updateWord('p'); } break;
		case Qt::Key_Q: if (getLetterButton('q')->isEnabled()) { hide('q');	updateWord('q'); } break;
		case Qt::Key_R: if (getLetterButton('r')->isEnabled()) { hide('r');	updateWord('r'); } break;
		case Qt::Key_S: if (getLetterButton('s')->isEnabled()) { hide('s');	updateWord('s'); } break;
		case Qt::Key_T: if (getLetterButton('t')->isEnabled()) { hide('t');	updateWord('t'); } break;
		case Qt::Key_U: if (getLetterButton('u')->isEnabled()) { hide('u');	updateWord('u'); } break;
		case Qt::Key_V: if (getLetterButton('v')->isEnabled()) { hide('v');	updateWord('v'); } break;
		case Qt::Key_W: if (getLetterButton('w')->isEnabled()) { hide('w');	updateWord('w'); } break;
		case Qt::Key_X: if (getLetterButton('x')->isEnabled()) { hide('x');	updateWord('x'); } break;
		case Qt::Key_Y: if (getLetterButton('y')->isEnabled()) { hide('y');	updateWord('y'); } break;
		case Qt::Key_Z: if (getLetterButton('z')->isEnabled()) { hide('z');	updateWord('z'); } break;
		default: QWidget::keyPressEvent(event);
	}
}

void GamePage::getGuessedWords(QVector<QString> &guessedWords) const
{
	guessedWords = m_guessedWords;
}

void GamePage::setStylesheets()
{
	setStyleSheet("QPushButton {"
				  "padding: 5px;"
				  "width: 30px;"
				  "height: 30px;"
				  "color: #AF4BA8;"
				  "}"
				  "QLabel {"
				  "color: white;"
				  "}"
	);
}
