#include "dms_horizontaldraglabel.h"

DMS_HorizontalDragLabel::DMS_HorizontalDragLabel(QWidget *parent):
    QWidget(parent), m_groupID(0), m_isPressed(0), m_isHolded(0), m_isChosen(0), \
    m_colorFactorOfChosen(110), m_colorFactorOfHolden(150), \
    m_labelHeight(20), m_labelWidth(8), m_labelBorderInterval(2), \
    m_mouseSrcPos(0, 0)
{
    m_labelText = QString(u8"");
    m_holdTimer = new QTimer(this);
    m_holdTimer->setInterval(300);
    m_holdTimer->setSingleShot(true);
    connect(m_holdTimer, SIGNAL(timeout()), this, SLOT(hold_slot()));

    m_labelColor.setRgb(0xA0, 0xEE, 0xE1);

}

DMS_HorizontalDragLabel::~DMS_HorizontalDragLabel()
{
    m_holdTimer->stop();
    delete m_holdTimer;
}

void DMS_HorizontalDragLabel::setLabelText(QString str)
{
    m_labelText = str;
    this->updateFont();
    this->update();
}

void DMS_HorizontalDragLabel::setIsHolded(bool flag)
{
    m_isHolded = flag;
    this->update();
}

void DMS_HorizontalDragLabel::setTimerIntervalOfHolded(int msec)
{
    m_holdTimer->stop();
    m_holdTimer->setInterval(msec);
}

void DMS_HorizontalDragLabel::updateFont(QFont *font)
{
    if(font != NULL)
        this->setFont(*font);

    QFontMetrics tmpFM = QFontMetrics(this->font());
    m_labelHeight = tmpFM.height() + (m_labelBorderInterval << 2);
    m_labelWidth = tmpFM.horizontalAdvance(m_labelText) + (m_labelBorderInterval << 2);
}

void DMS_HorizontalDragLabel::paintEvent(QPaintEvent *)
{
    if(m_labelText.isNull() || m_labelText.isEmpty())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor curBtnFillAreaColor;
    QColor curBtnBorderColor = m_labelColor.darker(300);
    QRect curBtnRect = QRect(m_labelBorderInterval, \
                             m_labelBorderInterval, \
                             width()-m_labelBorderInterval, \
                             height()-m_labelBorderInterval);

    if(m_isChosen)
        curBtnFillAreaColor = m_labelColor.darker(m_colorFactorOfChosen);
    else
        curBtnFillAreaColor = m_labelColor;

    if(m_isHolded)
    {
        curBtnFillAreaColor = m_labelColor.darker(m_colorFactorOfHolden);
    }

    painter.setPen(QPen(curBtnBorderColor));
    painter.setBrush(curBtnFillAreaColor);
    painter.drawRoundedRect(curBtnRect, 3, 2);

    painter.setPen(QColor(0,0,0));
    painter.drawText(curBtnRect, Qt::AlignCenter, m_labelText);

}

void DMS_HorizontalDragLabel::mousePressEvent(QMouseEvent *event)
{
    if(!m_isHolded)
        m_isPressed = 1;

    m_holdTimer->start();
    m_mouseSrcPos = event->pos();
    update();
}

void DMS_HorizontalDragLabel::mouseReleaseEvent(QMouseEvent *event)
{
    m_holdTimer->stop();

    if(rect().contains(event->pos()) && m_isHolded == 0)
        emit hdlabel_clicked();

    m_isPressed = 0;
    m_isHolded = 0;

    emit release_hdlabel_signal();
    update();
}

void DMS_HorizontalDragLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isHolded)
    {
        move(pos() - m_mouseSrcPos + event->pos());
        emit drag_hdlabel_signal();
    }
    else
        m_mouseSrcPos = event->pos();
}

void DMS_HorizontalDragLabel::hold_slot()
{
    m_isHolded = 1;
    m_isPressed = 0;
    update();
}

