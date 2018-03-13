#include "highScoresPage.h"
#include "../strings.h"

#include <algorithm>

#include <QLabel>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QSettings>
#include <QApplication>
#include <QPushButton>
#include <functional>

HighScoresPage::HighScoresPage(JustGuessing *pGame, QWidget* parent /* = nullptr */)
	: QWidget(parent)
	, m_pGame(pGame)
	, m_pEntriesLayout(nullptr)
{ // get the settings file path

// register the type to be able to save it in settings
//qRegisterMetaTypeStreamOperators<QVariantMultiMap_t>("QVariantMultiMap_t");

	readSettings();

	// Back button
	const auto backBtn = new QPushButton { this };
	backBtn->setIcon(QIcon { ":/controls/back_button" });
	backBtn->setStyleSheet("background: rgba(0, 0, 0, 0); margin-top: 4px;");
	backBtn->setCursor(Qt::PointingHandCursor);
	backBtn->setMaximumSize(backBtn->minimumSizeHint());
	connect(backBtn, &QAbstractButton::clicked, m_pGame, &JustGuessing::showHomePage);

	// star icon
	const auto starIcon = new QLabel { this };
	starIcon->setPixmap(QPixmap { ":/controls/star" });

	// high score text label
	const auto highScoresLabel = new QLabel { sNHighestScores.arg(HIGH_SCORE_ENTRIES), this };
	highScoresLabel->setStyleSheet("font-size: 25px;");

	// icon and title layout
	const auto iconTitleLayout = new QHBoxLayout;
	iconTitleLayout->addStretch();
	iconTitleLayout->addWidget(starIcon);
	iconTitleLayout->addSpacerItem(new QSpacerItem { 10, 0 });
	iconTitleLayout->addWidget(highScoresLabel);
	iconTitleLayout->addStretch();

	// create the high score entries layout
	createHighScoreEntries();

	// create the main layout
	const auto pageLayout = new QVBoxLayout;
	pageLayout->addWidget(backBtn);
	pageLayout->addStretch();
	pageLayout->addLayout(iconTitleLayout);
	pageLayout->addSpacerItem(new QSpacerItem { 0, 30 });
	pageLayout->addLayout(m_pEntriesLayout, 2);
	pageLayout->addStretch(255); // push all label widgets to the top

	setLayout(pageLayout);

	setStylesheets();

	// update the high score entries
	connect(pGame, &JustGuessing::pageAboutToShow, this, &HighScoresPage::createHighScoreEntries);

	// remember settings upon exit
	connect(qApp, &QApplication::aboutToQuit, this, &HighScoresPage::writeSettings);
}

void HighScoresPage::createHighScoreEntries()
{
	// create the layout
	if (!m_pEntriesLayout)
		m_pEntriesLayout = new QVBoxLayout;

	const auto star = QString::fromWCharArray(L"\u2605");

	// add the labels to the layout
	auto iHighScores = m_highScores.size();
	if (iHighScores > HIGH_SCORE_ENTRIES)
		iHighScores = HIGH_SCORE_ENTRIES;

	// sort the high scores map based on values (scores)
	QVector<QPair<QVariant, QString>> tmpVec;

	for (const auto &it : m_highScores.keys())
		// create "score => username" entries from "username => score" entries
		tmpVec << QPair<QVariant, QString>(m_highScores[it], it);

	// sort the temporary storage
	sort(tmpVec.begin(), tmpVec.end(), std::greater<>()/* in desc order */);

	// first, remove all previous labels from the layout
	QLayoutItem *pItem;
	while ((pItem = m_pEntriesLayout->takeAt(0)) != nullptr)
	{
		delete pItem->widget();
		delete pItem;
	}

	// we need to make labels for first iHighScores items from high scores list
	auto it = tmpVec.cbegin();

	for (auto nEntry = 1; nEntry <= iHighScores; ++nEntry)
	{
		const auto username = it->second;
		const auto entryText = QString { "#%1%2%3%4" }
			.arg(QString::number(nEntry), -15)
			.arg(username, -(48 - username.length()))
			.arg(star, -2)
			.arg(QString::number(it->first.toInt())); // the score

		const auto entryLabel = new QLabel { entryText, this };
		entryLabel->setMinimumSize(QSize { 350, 50 });
		entryLabel->setObjectName("highScore");
		entryLabel->setTextFormat(Qt::AutoText);
		m_pEntriesLayout->addWidget(entryLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
		m_pEntriesLayout->addSpacerItem(new QSpacerItem { 0, 10 });

		// get the next high score entry from list
		++it;
	}
}

int HighScoresPage::getScore(const QString& username) const
{
	return m_highScores[username].toInt();
}

void HighScoresPage::addScore(const QString &username)
{
	auto ok = true;
	m_highScores[username] = m_highScores[username].toInt(&ok) + 1;
}

void HighScoresPage::readSettings()
{
	QSettings settings { m_pGame->getSettingsFilePath(), QSettings::IniFormat };

	// get high scores map from settings
	const auto highScores = settings.value("highScores").toMap();

	// save in local storage
	m_highScores = highScores;
}

void HighScoresPage::writeSettings() const
{
	QSettings settings { m_pGame->getSettingsFilePath(), QSettings::IniFormat };

	// save the high scores map in settings
	settings.setValue("highScores", m_highScores);
}

void HighScoresPage::setStylesheets()
{
	setStyleSheet("QLabel#highScore { "
				  "    background: white;"
				  "    color: #AF4BA8;"
				  "    margin-left: 20px;"
				  "    background-clip: content-box;"
				  "    border-radius: 25px; "
				  "}");
}

//QDataStream &operator<<(QDataStream &out, const HighScoresPage::QVariantMultiMap_t &mmap) {
//	const auto mmapEnd = mmap.constEnd();
//	for(auto it = mmap.constBegin(); it != mmapEnd; ++it)
//		out << it.key() << it.value();
//	return out;
//}
//
//QDataStream &operator>>(QDataStream &in, HighScoresPage::QVariantMultiMap_t &mmap) {
//	QVariant key;
//	QString value;
//	while(!in.atEnd()) {
//		in >> key >> value;
//		mmap.insert(key, value);
//	}
//	return in;
//}
