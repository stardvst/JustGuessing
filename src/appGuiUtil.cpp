#include "appGuiUtil.h"

#include <QIcon>
#include <QSize>
#include <QToolTip>
#include <QHelpEvent>
#include <QtWidgets/QApplication>
#include <QFont>
#include <QPropertyAnimation>
#include <QPainter>

//
// CategoryButton class implementation
//
CategoryButton::CategoryButton(const QString &iconStr,
							   const QString &text,
							   const QString &toolTipText,
							   QWidget *parent,
							   Qt::ToolButtonStyle /* = Qt::ToolButtonTextUnderIcon */)
	: QToolButton(parent)
	, m_sToolTipText("<font>" + toolTipText + "</font>")
{ // font tag helps make the tooltip wrap

	setIcon(QIcon { iconStr });
	setIconSize(QSize { 80, 80 });
	setText(text);
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setCursor(Qt::PointingHandCursor);
	setFixedSize(110, 110);
	setProperty("class", "categoryBtn");
	setMouseTracking(true);
}

bool CategoryButton::event(QEvent *event)
{
	if (event->type() == QEvent::ToolTip)
	{
		const auto helpEvent = static_cast<QHelpEvent *>(event);
		QToolTip::setFont(QFont { QApplication::font().family(), 9 });
		QToolTip::showText(helpEvent->globalPos(), m_sToolTipText);	// remove tooltip display delay
		return true;
	}
	return QToolButton::event(event);
}

//
// UsernameLineEdit class implementation
//
UsernameLineEdit::UsernameLineEdit(QWidget *parent /* = nullptr */) : QLineEdit(parent) {}

void UsernameLineEdit::onEditingFinished()
{
	if (text().isEmpty())
		setPlaceholderText("");
	else
		emit textAvailable();
}

void UsernameLineEdit::focusInEvent(QFocusEvent *event)
{
	setPlaceholderText("");
	QLineEdit::focusInEvent(event);
}

//
// ToggleButoon class implementation
//
ToggleButoon::ToggleButoon(QWidget* parent /* = nullptr */)
	: QAbstractButton(parent)
	, m_bSwitch(false)
	, m_dOpacity(0.000)
	, m_iHeight(16)
	, m_iY(m_iHeight / 2)
	, m_iMargin(3)
	, m_thumbBrush("#d5d5d5")
	, m_brush(QColor { "#009688" })
	, m_pAnimation(new QPropertyAnimation { this, "offset", this })
{
	setOffset(m_iHeight / 2);
	setMinimumWidth(43);
	//setCheckable(true);
}

ToggleButoon::ToggleButoon(const QBrush &brush, QWidget* parent /* = nullptr */)
	: QAbstractButton(parent)
	, m_bSwitch(false)
	, m_dOpacity(0.0)
	, m_iHeight(16)
	, m_iY(m_iHeight / 2)
	, m_iMargin(3)
	, m_thumbBrush("#d5d5d5")
	, m_brush(brush)
	, m_pAnimation(new QPropertyAnimation { this, "offset", this })
{

	setOffset(m_iHeight / 2);
}

QSize ToggleButoon::sizeHint() const
{
	return QSize { 2 * (m_iHeight + m_iMargin), m_iHeight + 2 * m_iMargin };
}

int ToggleButoon::offset() const
{
	return m_iX;
}

void ToggleButoon::setOffset(const int& offset)
{
	m_iX = offset;
	update();
}

QBrush ToggleButoon::brush() const
{
	return m_brush;
}

void ToggleButoon::setBrush(const QBrush& brush)
{
	m_brush = brush;
}

void ToggleButoon::setSwitch(const bool bSwitch)
{
	m_bSwitch = bSwitch;
}

void ToggleButoon::setThumbBrush(const QBrush& brush)
{
	m_thumbBrush = brush;
}

void ToggleButoon::onSwitchOff() const
{
	m_pAnimation->setStartValue(offset());
	m_pAnimation->setEndValue(m_iHeight / 2);
	m_pAnimation->setDuration(120);
	m_pAnimation->start();
}

void ToggleButoon::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.setPen(Qt::NoPen);
	if (isEnabled())
	{
		p.setBrush(m_bSwitch ? brush() : Qt::black);
		p.setOpacity(m_bSwitch ? 0.5 : 0.38);
		p.setRenderHint(QPainter::Antialiasing, true);
		p.drawRoundedRect(QRect(m_iMargin, m_iMargin, width() - 2 * m_iMargin, height() - 2 * m_iMargin), 8.0, 8.0);
		p.setBrush(m_thumbBrush);
		p.setOpacity(1.0);
		p.drawEllipse(QRectF(offset() - (m_iHeight / 2), m_iY - (m_iHeight / 2), height(), height()));
	}
	else
	{
		p.setBrush(Qt::black);
		p.setOpacity(0.12);
		p.drawRoundedRect(QRect(m_iMargin, m_iMargin, width() - 2 * m_iMargin, height() - 2 * m_iMargin), 8.0, 8.0);
		p.setOpacity(1.0);
		p.setBrush(QColor("#BDBDBD"));
		p.drawEllipse(QRectF(offset() - (m_iHeight / 2), m_iY - (m_iHeight / 2), height(), height()));
	}
}

void ToggleButoon::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() & Qt::LeftButton)
	{
		m_bSwitch = !m_bSwitch;
		m_thumbBrush = m_bSwitch ? m_brush : QBrush("#d5d5d5");
		if (m_bSwitch)
		{
			m_pAnimation->setStartValue(m_iHeight / 2);
			m_pAnimation->setEndValue(width() - m_iHeight);
			m_pAnimation->setDuration(120);
			m_pAnimation->start();
		}
		else
		{
			onSwitchOff();
		}
	}
	QAbstractButton::mouseReleaseEvent(event);
}

void ToggleButoon::enterEvent(QEvent *event)
{
	setCursor(Qt::PointingHandCursor);
	QAbstractButton::enterEvent(event);
}
