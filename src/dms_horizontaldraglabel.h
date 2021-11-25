#ifndef DMS_DRAGLABELHORIZONTAL_H
#define DMS_DRAGLABELHORIZONTAL_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QImage>
#include <QMouseEvent>
#include <QFontMetrics>

#include <QColor>

class DMS_HorizontalDragLabel : public QWidget
{
    Q_OBJECT
public:
    DMS_HorizontalDragLabel(QWidget *parent = 0);
    ~DMS_HorizontalDragLabel();

    void setLabelText(QString str);
    void setIsHolded(bool flag);
    void setTimerIntervalOfHolded(int msec);
    void setColorFactorOfChosen(int factor) { m_colorFactorOfChosen = factor; this->update(); }
    void setColorFactorOfHolden(int factor) { m_colorFactorOfHolden = factor; this->update(); }
    void choose() { m_isChosen = 1; this->update(); }
    void unchoose() { m_isChosen = 0; this->update(); }
    int isChosen() { return m_isChosen; }
    QString getLabelText() { return m_labelText; }

    void updateFont(QFont *font = NULL);

    QString getLabelText(void) const {return m_labelText; }
    int isFontChanged(void) const { return false; }
    unsigned int getLabelHeight(void) const { return m_labelHeight; }
    unsigned int getLabelWidth(void) const { return m_labelWidth; }


protected:
    int m_groupID;

    int m_isPressed;
    int m_isHolded;
    int m_isChosen;

    int m_colorFactorOfChosen;
    int m_colorFactorOfHolden;

    unsigned int m_labelHeight;
    unsigned int m_labelWidth;

    unsigned int m_labelBorderInterval;


    QTimer *m_holdTimer;
    QColor m_labelColor;
    QString m_labelText;
    QPoint m_mouseSrcPos;

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


signals:
    void release_hdlabel_signal();
    void drag_hdlabel_signal();
    void hdlabel_clicked();
public slots:
    void hold_slot();

};

#endif // DMS_DRAGLABELHORIZONTAL_H
