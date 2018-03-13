#include "chooseUsernamePage.h"
#include "JustGuessing.h"
#include "strings.h"

#include <QPushButton>
#include <QLabel>
#include <QSettings>
#include <QApplication>
#include <QVBoxLayout>

ChooseUsernamePage::ChooseUsernamePage(JustGuessing *pGame, QWidget *parent /* = nullptr */)
	: QWidget(parent)
	, m_pGame(pGame)
{

	/* TODO: replace QStringList with QSet<QString> for fast lookup
	*/
	// register the type to be able to save it in settings
	//qRegisterMetaTypeStreamOperators<QStringSet_t>("QStringSet_t");

	// Back button
	const auto backBtn = new QPushButton { this };
	backBtn->setIcon(QIcon { ":/controls/back_button" });
	backBtn->setStyleSheet("background: rgba(0, 0, 0, 0); margin-top: 4px;");
	backBtn->setCursor(Qt::PointingHandCursor);
	backBtn->setMaximumSize(backBtn->minimumSizeHint());
	connect(backBtn, &QAbstractButton::clicked, m_pGame, &JustGuessing::showHomePage);

	// "Enter player name" label
	const auto playerNameLabel = new QLabel { sPlayerUsername, this };
	playerNameLabel->setStyleSheet("font: 24px; color: white;");

	/*const auto leaveEmptyLabel = new QLabel { sPressEnterToLeaveEmpty, this };
	leaveEmptyLabel->setStyleSheet("font: 15px; color: white;");*/

	// Username line edit
	m_pUserameLineEdit = new UsernameLineEdit { this };
	m_pUserameLineEdit->setPlaceholderText("Enter a username...");
	m_pUserameLineEdit->setFixedSize(300, 50);
	m_pUserameLineEdit->setCursorPosition(10);
	m_pUserameLineEdit->setStyleSheet("border-radius: 25px; padding: 10px 25px; color: #82147B; selection-background-color: #82147B;");
	connect(m_pUserameLineEdit, &QLineEdit::editingFinished, m_pUserameLineEdit, &UsernameLineEdit::onEditingFinished);
	connect(m_pUserameLineEdit, &UsernameLineEdit::textAvailable, this, &ChooseUsernamePage::onUsernameAvailable);

	m_pUsernameExistsLabel = new QLabel { this };
	m_pUsernameExistsLabel->setStyleSheet("font: 14px; color: white;");

	const auto pageLayout = new QVBoxLayout;
	pageLayout->addWidget(backBtn);
	pageLayout->addStretch();
	pageLayout->addWidget(playerNameLabel, 0, Qt::AlignCenter);
	pageLayout->addSpacerItem(new QSpacerItem { 0, 10 });
	pageLayout->addWidget(m_pUserameLineEdit, 0, Qt::AlignCenter);
	pageLayout->addSpacerItem(new QSpacerItem { 0, 5 });
	pageLayout->addWidget(m_pUsernameExistsLabel, 0, Qt::AlignCenter);
	pageLayout->addStretch();

	setLayout(pageLayout);

	readSettings();
	connect(m_pGame, &JustGuessing::appIsClosing, this, &ChooseUsernamePage::writeSettings);
}

void ChooseUsernamePage::onUsernameAvailable()
{
	// if there's text in label, first clear it
	if (!m_pUsernameExistsLabel->text().isEmpty())
		m_pUsernameExistsLabel->setText("");

	const auto username = m_pUserameLineEdit->text();
	if (m_usernames.contains(username, Qt::CaseInsensitive))
		m_pUsernameExistsLabel->setText(sUsernameExists);
	else
	{
		addUsername(username);
		emit usernameChosen();
	}
}

bool ChooseUsernamePage::usernameExists(const QString& username) const
{
	return m_usernames.contains(username);
}

void ChooseUsernamePage::addUsername(const QString& username)
{
	m_usernames.push_back(username);
	emit newUsernameRegistered(username);
}

void ChooseUsernamePage::readSettings()
{
	QSettings settings { m_pGame->getSettingsFilePath(), QSettings::IniFormat };

	// get high scores map from settings
	const auto usernames = settings.value("usernames").toStringList();

	// save in local storage
	m_usernames = usernames;
}
#include <QDebug>
void ChooseUsernamePage::writeSettings() const
{
	QSettings settings { m_pGame->getSettingsFilePath(), QSettings::IniFormat };
	
	// save the usernames in settings
	settings.setValue("usernames", m_usernames);
}

/* TODO: replace QStringList with QSet<QString> for fast lookup
 */
 //QDataStream &operator<<(QDataStream &out, const ChooseUsernamePage::QStringSet_t &sset) {
 //	const auto ssetEnd = sset.constEnd();
 //	for(auto it = sset.constBegin(); it != ssetEnd; ++it)
 //		out << *it;
 //	return out;
 //}

 //QDataStream &operator>>(QDataStream &in, ChooseUsernamePage::QStringSet_t &sset) {
 //	QString username;
 //	while(!in.atEnd()) {
 //		in >> username;
 //		sset.insert(username);
 //	}
 //	return in;
 //}