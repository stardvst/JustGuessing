#ifndef APPGUIUTIL_H
#define APPGUIUTIL_H

#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>
#include <QLabel>

// forward declarations
class QEvent;
class QFocusEvent;
class QPropertyAnimation;

//
// Button for Categories page
//
class CategoryButton : public QToolButton
{
	Q_OBJECT
public:
	CategoryButton(const QString &icon,
				   const QString &text,
				   const QString &toolTipText,
				   QWidget *parent,
				   Qt::ToolButtonStyle = Qt::ToolButtonTextUnderIcon);

protected:
	bool event(QEvent *event) override;

private:
	QString m_sToolTipText;
};

//
// Line edit for username input
//
class UsernameLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit UsernameLineEdit(QWidget *parent = nullptr);

signals:
	void textAvailable();

	public slots:
	void onEditingFinished();

protected:
	void focusInEvent(QFocusEvent *event) override;
};

//
// Button to toggle sound on/off
// 
class ToggleButoon : public QAbstractButton
{
	Q_OBJECT
		Q_PROPERTY(int offset READ offset WRITE setOffset)
		Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
public:
	explicit ToggleButoon(QWidget *parent = nullptr);
	explicit ToggleButoon(const QBrush &brush, QWidget *parent = nullptr);

	QSize sizeHint() const override;

	// get/set offset
	int offset() const;
	void setOffset(const int &offset);

	// get/set brush
	QBrush brush() const;
	void setBrush(const QBrush &brush);

	void setSwitch(const bool bSwitch);
	void setThumbBrush(const QBrush &brush);

	void onSwitchOff() const;

protected:
	void paintEvent(QPaintEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void enterEvent(QEvent *event) override;

private:

	// a boolean to indicate whether or not the switcher button is turned on
	bool m_bSwitch;

	// the opacity of the button
	double m_dOpacity;

	//
	int m_iHeight;
	int m_iMargin;
	int m_iX;
	int m_iY;

	// 
	QBrush m_thumbBrush;
	QBrush m_trackBrush;
	QBrush m_brush;

	QPropertyAnimation *m_pAnimation = nullptr;
};

#endif // !APPGUIUTIL_H
