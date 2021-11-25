#ifndef DMS_HORIZONTALDRAGWIDGET__H
#define DMS_HORIZONTALDRAGWIDGET__H

#include <QGridLayout>
#include <QScrollArea>

#include <QVector>
#include <QFontMetrics>
#include <QList>
#include "dms_horizontaldraglabel.h"

enum DMS_DragWidgetLabelChangeState{ NewChosen = 1, NewUnchosen };

class DMS_HorizontalDragWidget : public QWidget
{
    Q_OBJECT
public:
    DMS_HorizontalDragWidget(QWidget *parent = 0);
    ~DMS_HorizontalDragWidget();

    void addLabelFromChar(const char *p, int bChosen = 0);
    void addLabelFromQString(QString &str, int bChosen = 0);
    void removeLabelFromChar(const char *p);
    void removeLabelFromQString(QString &str);
    void modifyLabelFromChar(const char *pRaw, const char *pNew);
    void modifyLabelFromQString(QString &strRaw, QString &strNew);

    int getChosenLabelNames(QList<QString> &lstLblNames);
    int getUnchosenLabelNames(QList<QString> &lstLblNames);

    int getMaxHeight() { return (int)m_pixelHeightOfMaxLabel; }
    int getMaxWidth() { return (int)m_pixelWidthOfAllLabels; }

    // Settings
    void setTimeIntervalOfJudgeHold(int millisecond);

protected:

    bool m_bNeedScrollArea;

    QVector<DMS_HorizontalDragLabel*> m_vecOfUnChosenLabels;
    QVector<DMS_HorizontalDragLabel*> m_vecOfChosenLabels;

    QPoint m_mouseSrcPointer;
    unsigned int curIndexOfChosenLabel;

    unsigned int m_pixelWidthOfAllLabels;
    unsigned int m_pixelIntervalOfEachLabel;
    unsigned int m_pixelHeightOfMaxLabel;

    void resetInterface();
    void addLabel(DMS_HorizontalDragLabel*, int bChosen = 0);

signals:
    void changeLabelState(DMS_DragWidgetLabelChangeState state, QString lblName); // 1 for new chosen, 1 for new unchosen

public slots:
    void dragLabelMove_slots();
    void dragLabelRelease_slots();
    void labelClicked_slots();

};

#endif // DMS_HORIZONTALDRAGWIDGET__H
