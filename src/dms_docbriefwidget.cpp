#include "dms_docbriefwidget.h"

DMS_DocBriefWidget::DMS_DocBriefWidget(QWidget * parent):
    QTableView(parent),
    m_curDataModel(NULL)
{
    m_iStartIndexOfAppendDocBriefItem = 0;
    m_setOfShowIndexFromDocBriefItem.clear();
    m_lstOfInfoItems.clear();

    m_lstOfInfoItems.append(u8"文件名称");
    m_lstOfInfoItems.append(u8"文件类型");
    m_lstOfInfoItems.append(u8"大小");
    m_lstOfInfoItems.append(u8"标签");

    m_curDataModel = new QStandardItemModel(this);

    m_curDataModel->setColumnCount(m_lstOfInfoItems.length() + 1);
    for(int i = 0; i < m_lstOfInfoItems.length(); i++)
        m_curDataModel->setHeaderData(i, Qt::Horizontal, m_lstOfInfoItems[i]);
    m_curDataModel->setHeaderData(m_lstOfInfoItems.length(), Qt::Horizontal, u8"其他");

    this->setModel(m_curDataModel);

    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

}

DMS_DocBriefWidget::~DMS_DocBriefWidget()
{
    m_lstOfInfoItems.clear();
    m_setOfShowIndexFromDocBriefItem.clear();
    m_curDataModel->clear();
    delete m_curDataModel;
    m_vecOfLoadedDocBriefItems.clear();
}

void DMS_DocBriefWidget::appendDocBriefItem(const DMS_DocBriefItem &item)
{
    if(item.docName.isEmpty())
        return;

    m_setOfShowIndexFromDocBriefItem.insert(m_vecOfLoadedDocBriefItems.count());
    m_vecOfLoadedDocBriefItems.append(item);
    m_iStartIndexOfAppendDocBriefItem = m_vecOfLoadedDocBriefItems.count() - 1;
}

void DMS_DocBriefWidget::modifyDocBriefItem(int row, const DMS_DocBriefItem &item)
{
    if(row <= 0 || row >= m_curDataModel->rowCount())
        return;

    if(item.docName.isEmpty())
        return;

    m_curDataModel->setItem(row, 0, new QStandardItem(item.docName));
    if(!item.docType.isEmpty())
        m_curDataModel->setItem(row, 1, new QStandardItem(item.docType));
    if(!item.docSize.isEmpty())
        m_curDataModel->setItem(row, 2, new QStandardItem(item.docSize));
    if(m_pLstOfLabelMatchInfo == NULL)
        return;

    QList<QString> lstOfLblName;
    QList<unsigned short> lstOfLblIndex;
    int index;
    for(int i = 0; i < m_pLstOfLabelMatchInfo->count(); i++)
    {
        lstOfLblName.append((*m_pLstOfLabelMatchInfo)[i].lblName);
        lstOfLblIndex.append((*m_pLstOfLabelMatchInfo)[i].lblIndex);
    }

    QString strLabelName = u8"";
    for(int i = 0; i < item.lstOfDocLabelIndices.length(); i++)
    {
        index = lstOfLblIndex.indexOf(item.lstOfDocLabelIndices[i]);
        if(index >= 0)
            strLabelName += lstOfLblName[index];
        strLabelName += u8";";
    }
    m_curDataModel->setItem(row, 3, new QStandardItem(strLabelName));

}

void DMS_DocBriefWidget::removeDocBriefItem(const DMS_DocBriefItem &item)
{
    int index = -1;
    for(int i = 0; i < m_vecOfLoadedDocBriefItems.count(); i++)
    {
        if(m_vecOfLoadedDocBriefItems[i].docName == item.docName && \
                m_vecOfLoadedDocBriefItems[i].docPath == item.docPath && \
                m_vecOfLoadedDocBriefItems[i].docSize == item.docSize && \
                m_vecOfLoadedDocBriefItems[i].docType == item.docType && \
                m_vecOfLoadedDocBriefItems[i].docBrief == item.docBrief && \
                m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices == item.lstOfDocLabelIndices)
        {
            index = i;
            break;
        }
    }
    if(index < 0)
        return;

    DMS_DocBriefItem takeItem = m_vecOfLoadedDocBriefItems.takeAt(index);
    QList<QStandardItem *> lstOfStdItem = m_curDataModel->takeRow(index);
    for(auto stdItem: lstOfStdItem)
    {
        if(stdItem != NULL)
            stdItem->clearData();
    }

    if(index >= m_iStartIndexOfAppendDocBriefItem)
        m_iStartIndexOfAppendDocBriefItem--;    // the m_vec... decrease one item

    m_setOfShowIndexFromDocBriefItem.remove(index);
}

void DMS_DocBriefWidget::applyDocBriefItemsToShow()
{
    QList<QString> lstOfLblName;
    QList<unsigned short> lstOfLblIndex;
    int index;
    for(int i = 0; i < m_pLstOfLabelMatchInfo->count(); i++)
    {
        lstOfLblName.append((*m_pLstOfLabelMatchInfo)[i].lblName);
        lstOfLblIndex.append((*m_pLstOfLabelMatchInfo)[i].lblIndex);
    }

    for(int i = 0; i < m_vecOfLoadedDocBriefItems.count(); i++)
    {
        if(m_vecOfLoadedDocBriefItems[i].docName.isEmpty())
            continue;
        else
            m_curDataModel->setItem(i, 0, new QStandardItem(m_vecOfLoadedDocBriefItems[i].docName));
        if(!m_vecOfLoadedDocBriefItems[i].docType.isEmpty())
            m_curDataModel->setItem(i, 1, new QStandardItem(m_vecOfLoadedDocBriefItems[i].docType));
        if(!m_vecOfLoadedDocBriefItems[i].docSize.isEmpty())
            m_curDataModel->setItem(i, 2, new QStandardItem(m_vecOfLoadedDocBriefItems[i].docSize));
        if(m_pLstOfLabelMatchInfo == NULL)
            continue;
        QString strLabelName = u8"";
        for(int j = 0; j < m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices.length(); j++)
        {
            index = lstOfLblIndex.indexOf(m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices[j]);
            if(index >= 0)
                strLabelName += lstOfLblName[index];
            strLabelName += u8";";
        }
        m_curDataModel->setItem(i, 3, new QStandardItem(strLabelName));
    }
}

void DMS_DocBriefWidget::showRows(int _iStart, int _iEnd)
{
    int iStart = _iStart, iEnd = _iEnd;
    if(iStart < 0)
        iStart = 0;

    // only show these QStandardItems already set to data model;
    if(iEnd > m_iStartIndexOfAppendDocBriefItem)
        iEnd = m_iStartIndexOfAppendDocBriefItem;

    for(int i = iStart; i < iEnd; i++)
    {
        this->showRow(i);
        m_setOfShowIndexFromDocBriefItem.insert(i);
    }
}

void DMS_DocBriefWidget::showRowsByLabelIndex(unsigned short labelIndex)
{
    for(int i = 0; i < m_vecOfLoadedDocBriefItems.count(); i++)
    {
        if(m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices.contains(labelIndex))
        {
            this->showRow(i);
            m_setOfShowIndexFromDocBriefItem.insert(i);
        }
        else
        {
            this->hideRow(i);
            m_setOfShowIndexFromDocBriefItem.remove(i);
        }
    }
}

void DMS_DocBriefWidget::showRowsByLabelIndices(QList<unsigned short> labelIndices)
{
    bool bFound;
    for(int i = 0; i < m_vecOfLoadedDocBriefItems.count(); i++)
    {
        bFound = false;
        for(int j = 0; j < labelIndices.count(); j++)
        {
            if(m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices.contains(labelIndices[j]))
            {
                this->showRow(i);
                m_setOfShowIndexFromDocBriefItem.insert(i);
                bFound = true;
                break;
            }
        }
        if(!bFound)
        {
            this->hideRow(i);
            m_setOfShowIndexFromDocBriefItem.remove(i);
        }
    }
}

void DMS_DocBriefWidget::hideRows(int _iStart, int _iEnd)
{
    int iStart = _iStart, iEnd = _iEnd;
    if(iStart < 0)
        iStart = 0;

    // only show these QStandardItems already set to data model;
    if(iEnd > m_iStartIndexOfAppendDocBriefItem)
        iEnd = m_iStartIndexOfAppendDocBriefItem;

    for(int i = iStart; i < iEnd; i++)
    {
        this->hideRow(i);
        m_setOfShowIndexFromDocBriefItem.remove(i);
    }
}

void DMS_DocBriefWidget::hideRowsByLabelIndex(unsigned short labelIndex)
{
    for(int i = 0; i < m_vecOfLoadedDocBriefItems.count(); i++)
    {
        if(m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices.contains(labelIndex))
        {
            this->hideRow(i);
            m_setOfShowIndexFromDocBriefItem.remove(i);
        }
        else
        {
            this->showRow(i);
            m_setOfShowIndexFromDocBriefItem.insert(i);
        }
    }
}

void DMS_DocBriefWidget::hideRowsByLabelIndices(QList<unsigned short> labelIndices)
{
    bool bFound;
    for(int i = 0; i < m_vecOfLoadedDocBriefItems.count(); i++)
    {
        bFound = false;
        for(int j = 0; j < labelIndices.count(); j++)
        {
            if(m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices.contains(labelIndices[j]))
            {
                this->hideRow(i);
                m_setOfShowIndexFromDocBriefItem.remove(i);
                bFound = true;
                break;
            }
        }
        if(!bFound)
        {
            this->showRow(i);
            m_setOfShowIndexFromDocBriefItem.insert(i);

        }
    }
}

void DMS_DocBriefWidget::sortRowsByLabelIndex(unsigned short labelIndex)
{
    QList<DMS_DocBriefItem> lstOfChosenItems;
    QList<DMS_DocBriefItem> lstOfUnChosenItems;

    QVector<DMS_DocBriefItem> vecOfDocBriefItems;
    vecOfDocBriefItems.clear();

    int iChosenEnd = 0;
    for(int i = 0; i < m_vecOfLoadedDocBriefItems.count(); i++)
    {
        if(m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices.contains(labelIndex))
        {
            m_vecOfLoadedDocBriefItems.insert(iChosenEnd, m_vecOfLoadedDocBriefItems.takeAt(i));
            m_curDataModel->insertRow(iChosenEnd++, m_curDataModel->takeRow(i));
        }
    }
}

void DMS_DocBriefWidget::sortRowsByLabelIndicesWithMatchingDegree(QList<unsigned short> labelIndices)
{
    QList<int> lstOfTimesOfAllChosenEnd; // used to record the times repeated in labelIndices,
                                         //index of lstOf... stand for the (index) times appeared;
    for(int i = 0; i <= labelIndices.count(); i++)
        lstOfTimesOfAllChosenEnd.append(0);

    // 实现不完全正确
    int curTimes;
    for(int i = 0; i < m_vecOfLoadedDocBriefItems.count(); i++)
    {
        curTimes = 0;
        for(int j = 0; j < labelIndices.count(); j++)
        {
            if(m_vecOfLoadedDocBriefItems[i].lstOfDocLabelIndices.contains(labelIndices[j]))
                curTimes++;
        }

        if(curTimes <= 0)
            continue;

        m_vecOfLoadedDocBriefItems.insert(lstOfTimesOfAllChosenEnd[curTimes], \
                                          m_vecOfLoadedDocBriefItems.takeAt(i));
        m_curDataModel->insertRow(lstOfTimesOfAllChosenEnd[curTimes], \
                                  m_curDataModel->takeRow(i));

        for(int k = curTimes; k > 0; k--)
            lstOfTimesOfAllChosenEnd[k] += 1;
    }
}

DMS_DocBriefItem DMS_DocBriefWidget::getDocItemByRowIndex(int row, bool *bSucceed)
{
    DMS_DocBriefItem docItem;
    docItem.docName = "";
    docItem.docPath = "";
    docItem.docSize = "";
    docItem.docType = "";
    docItem.docBrief = "";
    docItem.lstOfDocLabelIndices.clear();
    *bSucceed = false;

    if(row < m_vecOfLoadedDocBriefItems.count())
    {
        *bSucceed = true;
        docItem = m_vecOfLoadedDocBriefItems[row];
    }

    return docItem;
}
