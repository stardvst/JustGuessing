#ifndef CHOOSEUSERNAMEPAGE_H
#define CHOOSEUSERNAMEPAGE_H

#include "../appGuiUtil.h"
#include "../JustGuessing.h"

class ChooseUsernamePage : public QWidget
{
	Q_OBJECT
public:
	//using QStringSet_t = QSet<QString>;

	//friend QDataStream &operator<<(QDataStream &out, const QStringSet_t &sset);
	//friend QDataStream &operator>>(QDataStream &in, QStringSet_t &sset);

	explicit ChooseUsernamePage(JustGuessing *pGame, QWidget *parent = nullptr);

	bool usernameExists(const QString& username) const;
	void addUsername(const QString& username);

	//auto getCurrentUsername() const;

signals:
	void usernameChosen();
	void newUsernameRegistered(const QString &username);

private slots:
	void onUsernameAvailable();
	void writeSettings() const;

private:
	void readSettings();

private:
	JustGuessing * m_pGame;

	/* TODO: replace QStringList with QSet<QString> for fast lookup
	*/
	// list of all user names ever stored
	QStringList m_usernames;

	// page properties
	UsernameLineEdit *m_pUserameLineEdit;
	QLabel *m_pUsernameExistsLabel;
};

//Q_DECLARE_METATYPE(ChooseUsernamePage::QStringSet_t)

#endif // !CHOOSEUSERNAMEPAGE_H
