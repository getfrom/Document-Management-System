#include "dms_horizontaldragwidget.h"

DMS_HorizontalDragWidget::DMS_HorizontalDragWidget(QWidget *parent ):
    QWidget(parent), m_pixelWidthOfAllLabels(0), m_pixelIntervalOfEachLabel(4), \
    m_pixelHeightOfMaxLabel(0)
{
    m_bNeedScrollArea = false;
    m_vecOfUnChosenLabels.clear();
    m_vecOfChosenLabels.clear();
}

DMS_HorizontalDragWidget::~DMS_HorizontalDragWidget()
{
    for(int i = 0; i < m_vecOfChosenLabels.length(); i++)
    {
        m_vecOfChosenLabels[i]->close();
        delete m_vecOfChosenLabels[i];
    }
    for(int i = 0; i < m_vecOfUnChosenLabels.length(); i++)
    {
        m_vecOfUnChosenLabels[i]->close();
        delete m_vecOfUnChosenLabels[i];
    }

    m_vecOfChosenLabels.clear();
    m_vecOfUnChosenLabels.clear();
}

void DMS_HorizontalDragWidget::addLabelFromChar(const char *p, int bChosen)
{
    DMS_HorizontalDragLabel *pLabel = new DMS_HorizontalDragLabel(this);
    pLabel->setLabelText(QString(p));
    this->addLabel(pLabel, bChosen);
}

void DMS_HorizontalDragWidget::addLabelFromQString(QString &str, int bChosen)
{
    DMS_HorizontalDragLabel *pLabel = new DMS_HorizontalDragLabel(this);
    pLabel->setLabelText(str);
    this->addLabel(pLabel, bChosen);
}

void DMS_HorizontalDragWidget::removeLabelFromChar(const char *p)
{
    QString strTmp(p);
    this->removeLabelFromQString(strTmp);
}

void DMS_HorizontalDragWidget::removeLabelFromQString(QString &str)
{
    DMS_HorizontalDragLabel *lbl;

    for(int i = 0; i < m_vecOfChosenLabels.length(); i++)
    {
        if(m_vecOfChosenLabels[i]->getLabelText() == str)
        {
            lbl = m_vecOfChosenLabels.takeAt(i);
            if(lbl->getLabelWidth() > m_pixelWidthOfAllLabels)
                m_pixelWidthOfAllLabels = 0;
            else
                m_pixelWidthOfAllLabels -= lbl->getLabelWidth();

            disconnect(lbl, SIGNAL(drag_hdlabel_signal()),     this, SLOT(dragLabelMove_slots()));
            disconnect(lbl, SIGNAL(release_hdlabel_signal()),  this, SLOT(dragLabelRelease_slots()));
            disconnect(lbl, SIGNAL(hdlabel_clicked()),         this, SLOT(labelClicked_slots()));

            this->resetInterface();
            delete lbl;
            return;
        }
    }

    for(int i = 0; i < m_vecOfUnChosenLabels.length(); i++)
    {
        if(m_vecOfUnChosenLabels[i]->getLabelText() == str)
        {
            lbl = m_vecOfUnChosenLabels.takeAt(i);
            if(lbl->getLabelWidth() > m_pixelWidthOfAllLabels)
                m_pixelWidthOfAllLabels = 0;
            else
                m_pixelWidthOfAllLabels -= lbl->getLabelWidth();

            disconnect(lbl, SIGNAL(drag_hdlabel_signal()),     this, SLOT(dragLabelMove_slots()));
            disconnect(lbl, SIGNAL(release_hdlabel_signal()),  this, SLOT(dragLabelRelease_slots()));
            disconnect(lbl, SIGNAL(hdlabel_clicked()),         this, SLOT(labelClicked_slots()));

            this->resetInterface();
            delete lbl;
            return;
        }
    }

    resetInterface();
}

void DMS_HorizontalDragWidget::modifyLabelFromChar(const char *pRaw, const char *pNew)
{
    QString strRaw(pRaw), strNew(pNew);
    this->modifyLabelFromQString(strRaw, strNew);
}

void DMS_HorizontalDragWidget::modifyLabelFromQString(QString &strRaw, QString &strNew)
{
    DMS_HorizontalDragLabel *lbl;
    for(int i = 0; i < m_vecOfChosenLabels.length(); i++)
    {
        if(m_vecOfChosenLabels[i]->getLabelText() == strRaw)
        {
            lbl = m_vecOfChosenLabels[i];
            if(lbl->getLabelWidth() > m_pixelWidthOfAllLabels)
                m_pixelWidthOfAllLabels = 0;
            else
                m_pixelWidthOfAllLabels -= lbl->getLabelWidth();

            lbl->setLabelText(strNew);
            m_pixelWidthOfAllLabels += lbl->getLabelWidth();
            if(m_pixelWidthOfAllLabels > 0xfffffff)
                m_pixelWidthOfAllLabels = 0xfffffff;

            this->resetInterface();
            return;
        }
    }

    for(int i = 0; i < m_vecOfUnChosenLabels.length(); i++)
    {
        if(m_vecOfUnChosenLabels[i]->getLabelText() == strRaw)
        {
            lbl = m_vecOfUnChosenLabels[i];
            if(lbl->getLabelWidth() > m_pixelWidthOfAllLabels)
                m_pixelWidthOfAllLabels = 0;
            else
                m_pixelWidthOfAllLabels -= lbl->getLabelWidth();

            lbl->setLabelText(strNew);
            m_pixelWidthOfAllLabels += lbl->getLabelWidth();
            if(m_pixelWidthOfAllLabels > 0xfffffff)
                m_pixelWidthOfAllLabels = 0xfffffff;

            this->resetInterface();
            return;
        }
    }

}

int DMS_HorizontalDragWidget::getChosenLabelNames(QList<QString> &lstLblNames)
{
    for(int i = 0; i < m_vecOfChosenLabels.length(); i++)
        lstLblNames.append(m_vecOfChosenLabels[i]->getLabelText());
    return m_vecOfChosenLabels.length();
}

int DMS_HorizontalDragWidget::getUnchosenLabelNames(QList<QString> &lstLblNames)
{
    for(int i = 0; i < m_vecOfUnChosenLabels.length(); i++)
        lstLblNames.append(m_vecOfUnChosenLabels[i]->getLabelText());
    return m_vecOfUnChosenLabels.length();
}

void DMS_HorizontalDragWidget::setTimeIntervalOfJudgeHold(int millisecond)
{
    int i;
    for(i = 0; i < m_vecOfChosenLabels.length(); i++)
        m_vecOfChosenLabels[i]->setTimerIntervalOfHolded(millisecond);
    for(i = 0; i < m_vecOfUnChosenLabels.length(); i++)
        m_vecOfUnChosenLabels[i]->setTimerIntervalOfHolded(millisecond);
}

void DMS_HorizontalDragWidget::addLabel(DMS_HorizontalDragLabel * lbl, int bChosen)
{
    m_pixelWidthOfAllLabels += lbl->getLabelWidth();
    if(m_pixelWidthOfAllLabels > 0xfffffff)
        m_pixelWidthOfAllLabels = 0xfffffff;

    this->setMinimumWidth(m_pixelWidthOfAllLabels);
    if(lbl->getLabelHeight() > m_pixelHeightOfMaxLabel)
        m_pixelHeightOfMaxLabel = lbl->getLabelHeight();

    this->setGeometry(0, 0, m_pixelWidthOfAllLabels, m_pixelHeightOfMaxLabel);

    connect(lbl, SIGNAL(drag_hdlabel_signal()),     this, SLOT(dragLabelMove_slots()));
    connect(lbl, SIGNAL(release_hdlabel_signal()),  this, SLOT(dragLabelRelease_slots()));
    connect(lbl, SIGNAL(hdlabel_clicked()),         this, SLOT(labelClicked_slots()));

    if(bChosen)
    {
        lbl->choose();
        m_vecOfChosenLabels.push_back(lbl);
    }
    else
    {
        lbl->unchoose();
        m_vecOfUnChosenLabels.push_back(lbl);
    }

    lbl->show();
    resetInterface();
}

void DMS_HorizontalDragWidget::resetInterface()
{
    unsigned curPosX = 0;
    unsigned curPosY = 0;
    unsigned curWidth = 0;
    unsigned curHeight = 0;
    int i;

    for(i = 0; i < m_vecOfChosenLabels.length(); i++)
    {
        curPosX += curWidth;
        curPosY = 0;
        curWidth = m_vecOfChosenLabels[i]->getLabelWidth();
        curHeight = m_pixelHeightOfMaxLabel;
        m_vecOfChosenLabels[i]->setGeometry(curPosX, curPosY, curWidth, curHeight);
    }

    for(i = 0; i < m_vecOfUnChosenLabels.length(); i++)
    {
        curPosX += curWidth;
        curPosY = 0;
        curWidth = m_vecOfUnChosenLabels[i]->getLabelWidth();
        curHeight = m_pixelHeightOfMaxLabel;
        m_vecOfUnChosenLabels[i]->setGeometry(curPosX, curPosY, curWidth, curHeight);
    }

}

void DMS_HorizontalDragWidget::dragLabelMove_slots()
{
    int senderPosXMin, senderPosXMax;
    int curPosXCenter, curPosXMin, curPosXMax;
    int accOfLabelWidth;
    int bInChosen = 0;

    for(int i = 0; i < m_vecOfChosenLabels.length(); i++)
    {
        if(m_vecOfChosenLabels[i] == sender())
        {
            senderPosXMin = m_vecOfChosenLabels[i]->pos().x();
            senderPosXMax = senderPosXMin + m_vecOfChosenLabels[i]->width();
            accOfLabelWidth = m_vecOfChosenLabels[0]->pos().x();

            for(int j = 0; j < m_vecOfChosenLabels.length(); accOfLabelWidth += m_vecOfChosenLabels[j++]->width())
            {
                curPosXCenter = m_vecOfChosenLabels[j]->pos().x() + (m_vecOfChosenLabels[j]->width() >> 1);
                curPosXMin = m_vecOfChosenLabels[j]->pos().x();
                curPosXMax = curPosXMin + m_vecOfChosenLabels[j]->width();

                //qDebug("i=(%3d, %3d), j=(%3d, %3d, %3d)", senderPosXMin, senderPosXMax, curPosXMin, curPosXMax, curPosXCenter);

                if(curPosXMin < senderPosXMin && senderPosXMin < curPosXCenter) // sender 前置
                {
                    if(j < i)
                        m_vecOfChosenLabels[j]->setGeometry(accOfLabelWidth + m_vecOfChosenLabels[i]->width(), 0, \
                                                      m_vecOfChosenLabels[j]->width(), this->height());
                    else
                        m_vecOfChosenLabels[j]->setGeometry(accOfLabelWidth, 0, \
                                                      m_vecOfChosenLabels[j]->width(), this->height());
                }
                else if(curPosXCenter < senderPosXMax && senderPosXMax < curPosXMax)  // sender 后置
                {
                    if(i < j)
                        m_vecOfChosenLabels[j]->setGeometry(accOfLabelWidth - m_vecOfChosenLabels[i]->width(), 0, \
                                                      m_vecOfChosenLabels[j]->width(), this->height());
                    else
                        m_vecOfChosenLabels[j]->setGeometry(accOfLabelWidth, 0, \
                                                      m_vecOfChosenLabels[j]->width(), this->height());
                }
            }

            bInChosen = 1;
            break;
        }
    }

    if(bInChosen)
        return;

    for(int i = 0; i < m_vecOfUnChosenLabels.length(); i++)
    {
        if(m_vecOfUnChosenLabels[i] == sender())
        {
            senderPosXMin = m_vecOfUnChosenLabels[i]->pos().x();
            senderPosXMax = senderPosXMin + m_vecOfUnChosenLabels[i]->width();
            accOfLabelWidth = m_vecOfUnChosenLabels[0]->pos().x();

            for(int j = 0; j < m_vecOfUnChosenLabels.length(); accOfLabelWidth += m_vecOfUnChosenLabels[j++]->width())
            {
                curPosXCenter = m_vecOfUnChosenLabels[j]->pos().x() + (m_vecOfUnChosenLabels[j]->width() >> 1);
                curPosXMin = m_vecOfUnChosenLabels[j]->pos().x();
                curPosXMax = curPosXMin + m_vecOfUnChosenLabels[j]->width();

                //qDebug("i=(%3d, %3d), j=(%3d, %3d, %3d)", senderPosXMin, senderPosXMax, curPosXMin, curPosXMax, curPosXCenter);

                if(curPosXMin < senderPosXMin && senderPosXMin < curPosXCenter) // sender 前置
                {
                    if(j < i)
                        m_vecOfUnChosenLabels[j]->setGeometry(accOfLabelWidth + m_vecOfUnChosenLabels[i]->width(), 0, \
                                                      m_vecOfUnChosenLabels[j]->width(), this->height());
                    else
                        m_vecOfUnChosenLabels[j]->setGeometry(accOfLabelWidth, 0, \
                                                      m_vecOfUnChosenLabels[j]->width(), this->height());
                }
                else if(curPosXCenter < senderPosXMax && senderPosXMax < curPosXMax)  // sender 后置
                {
                    if(i < j)
                        m_vecOfUnChosenLabels[j]->setGeometry(accOfLabelWidth - m_vecOfUnChosenLabels[i]->width(), 0, \
                                                      m_vecOfUnChosenLabels[j]->width(), this->height());
                    else
                        m_vecOfUnChosenLabels[j]->setGeometry(accOfLabelWidth, 0, \
                                                      m_vecOfUnChosenLabels[j]->width(), this->height());
                }
            }

            break;
        }
    }

}

void DMS_HorizontalDragWidget::dragLabelRelease_slots()
{
    int accOfLabelWidth, lastLabelHalfWidth;
    int bInChosen = 0, curClassStartPosX = 0;
    int i;

    curClassStartPosX = accOfLabelWidth = 0;
    for(i = 0, lastLabelHalfWidth = 0; i < m_vecOfChosenLabels.length(); accOfLabelWidth += m_vecOfChosenLabels[i++]->width())
    {
        if(m_vecOfChosenLabels[i] == sender())
        {
            int senderPosX = m_vecOfChosenLabels[i]->pos().x();
            if(senderPosX < curClassStartPosX)
                senderPosX = curClassStartPosX;
            else if(senderPosX > this->width())
                senderPosX = this->width();

            DMS_HorizontalDragLabel *lbl = m_vecOfChosenLabels[i];

            int j = 0;
            if(senderPosX + lastLabelHalfWidth < accOfLabelWidth) // sender 前置
            {
                for(j = i; j>0 && m_vecOfChosenLabels[j-1]->pos().x() > senderPosX; j--)
                {
                    m_vecOfChosenLabels[j] = m_vecOfChosenLabels[j-1];
                }
            }
            else // sender 原位 或 后置
            {
                for(j = i; j+1 < m_vecOfChosenLabels.length() && m_vecOfChosenLabels[j+1]->pos().x() < senderPosX; j++)
                {
                    m_vecOfChosenLabels[j] = m_vecOfChosenLabels[j+1];
                }
            }

            m_vecOfChosenLabels[j] = lbl;
            bInChosen = 1;
        }
        lastLabelHalfWidth = m_vecOfChosenLabels[i]->width() / 2;
    }

    if(bInChosen)
        return;

    curClassStartPosX = accOfLabelWidth;
    for(i = 0, lastLabelHalfWidth = 0; i < m_vecOfUnChosenLabels.length(); accOfLabelWidth += m_vecOfUnChosenLabels[i++]->width())
    {
        if(m_vecOfUnChosenLabels[i] == sender())
        {
            int senderPosX = m_vecOfUnChosenLabels[i]->pos().x();
            if(senderPosX < curClassStartPosX)
                senderPosX = curClassStartPosX;
            else if(senderPosX > this->width())
                senderPosX = this->width();

            DMS_HorizontalDragLabel *lbl = m_vecOfUnChosenLabels[i];

            int j = 0;
            if(senderPosX + lastLabelHalfWidth < accOfLabelWidth) // sender 前置
            {
                for(j = i; j>0 && m_vecOfUnChosenLabels[j-1]->pos().x() > senderPosX; j--)
                {
                    m_vecOfUnChosenLabels[j] = m_vecOfUnChosenLabels[j-1];
                }
            }
            else // sender 原位 或 后置
            {
                for(j = i; j+1 < m_vecOfUnChosenLabels.length() && m_vecOfUnChosenLabels[j+1]->pos().x() < senderPosX; j++)
                {
                    m_vecOfUnChosenLabels[j] = m_vecOfUnChosenLabels[j+1];
                }
            }

            m_vecOfUnChosenLabels[j] = lbl;
        }
        lastLabelHalfWidth = m_vecOfUnChosenLabels[i]->width() / 2;
    }

    this->resetInterface();
}

void DMS_HorizontalDragWidget::labelClicked_slots()
{
    int bInChosen = 0, i;
    DMS_HorizontalDragLabel *lbl = NULL;
    DMS_DragWidgetLabelChangeState curChangeState;
    QString curLblName;

    for(i = 0; i < m_vecOfChosenLabels.length(); i++)
    {
        if(m_vecOfChosenLabels[i] == sender())
        {
            lbl = m_vecOfChosenLabels[i];
            lbl->unchoose();
            m_vecOfUnChosenLabels.prepend(lbl);
            m_vecOfChosenLabels.removeAt(i);
            bInChosen = 1;
            curChangeState = DMS_DragWidgetLabelChangeState::NewUnchosen;
            curLblName = lbl->getLabelText();
            break;
        }
    }

    if(!bInChosen)
    {
        for(i = 0; i < m_vecOfUnChosenLabels.length(); i++)
        {
            if(m_vecOfUnChosenLabels[i] == sender())
            {
                lbl = m_vecOfUnChosenLabels[i];
                lbl->choose();
                m_vecOfChosenLabels.append(lbl);
                m_vecOfUnChosenLabels.removeAt(i);
                curChangeState = DMS_DragWidgetLabelChangeState::NewChosen;
                curLblName = lbl->getLabelText();
                break;
            }
        }
    }

    this->resetInterface();

    if(!curLblName.isEmpty())
    emit changeLabelState(curChangeState, curLblName);
}
