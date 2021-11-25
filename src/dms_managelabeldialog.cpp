#include "dms_managelabeldialog.h"

DMS_ManageLabelDialog::DMS_ManageLabelDialog()
{
    m_bLabelAddNew = false;
    m_bQuoteLabelEditable = false;
    m_lastLabelText.clear();
    m_lastRow = 0;
    m_quoteCntInfoPre = u8"此标签共引用 ";
    m_quoteCntInfoSuf = u8" 次";
    m_lstOfLabels.clear();
    m_lstOfLabelQuoteCnt.clear();

    rootLayout = new QVBoxLayout(this);

    infoLayout = new QHBoxLayout();
    lblQuoteInfo = new QLabel(m_quoteCntInfoPre + QString::number(0) + m_quoteCntInfoSuf);
    ckbEditQuote = new QCheckBox(u8"已引用可编辑");
    infoLayout->addWidget(lblQuoteInfo);
    infoLayout->addWidget(ckbEditQuote);
    rootLayout->addLayout(infoLayout);

    mainLayout = new QHBoxLayout();
    lstLblsWdg = new QListWidget();
    mainLayout->addWidget(lstLblsWdg);

    btnLayout = new QVBoxLayout();
    btnAdd = new QPushButton(u8"新增");
    btnModify = new QPushButton(u8"编辑");
    btnDelete = new QPushButton(u8"删除");
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnModify);
    btnLayout->addWidget(btnDelete);
    mainLayout->addLayout(btnLayout);

    rootLayout->addLayout(mainLayout);
    this->setLayout(rootLayout);

    connect(btnAdd, SIGNAL(clicked()), this, SLOT(btnAddClicked()));
    connect(btnModify, SIGNAL(clicked()), this, SLOT(btnModifyClicked()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(btnDeleteClicked()));

    connect(lstLblsWdg, SIGNAL(itemChanged(QListWidgetItem *)), this, \
            SLOT(lstLblWidgetItemChanged(QListWidgetItem *)));
    connect(lstLblsWdg, SIGNAL(currentRowChanged(int)), this, \
            SLOT(lstLblWidgetItemRowChanged(int)));

    connect(ckbEditQuote, SIGNAL(stateChanged(int)), this, \
            SLOT(ckbStateChanged(int)));
}

DMS_ManageLabelDialog::~DMS_ManageLabelDialog()
{
    m_lstOfLabelQuoteCnt.clear();
    m_lstOfLabels.clear();

    delete btnDelete;
    delete btnModify;
    delete btnAdd;
    delete lstLblsWdg;
    delete ckbEditQuote;
    delete lblQuoteInfo;
    delete btnLayout;
    delete mainLayout;
    delete infoLayout;
    delete rootLayout;
}

void DMS_ManageLabelDialog::setExistLabels(QList<QString> lstLbls, QList<unsigned short> lstQuoteCnt)
{
    QListWidgetItem * item;
    m_lstOfLabels.clear();
    m_lstOfLabelQuoteCnt.clear();
    m_bLabelAddNew = true;
    for(int i = 0; i < lstLbls.count(); i++)
    {
        m_lstOfLabelQuoteCnt.append(lstQuoteCnt[i]);
        m_lstOfLabels.append(lstLbls[i]);

        item = new QListWidgetItem();
        item->setText(lstLbls[i]);
        lstLblsWdg->addItem(item);
        //lstLblsWdg->setCurrentItem(item);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->listWidget()->setEditTriggers(QAbstractItemView::DoubleClicked);
    }
    m_bLabelAddNew = false;
}

void DMS_ManageLabelDialog::setCurrentLabelEditEnable(int curRow, bool bEnable)
{
    QListWidgetItem *item;
    if(bEnable)
    {
        btnModify->setEnabled(bEnable);
        item = lstLblsWdg->item(curRow);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->listWidget()->setEditTriggers(QAbstractItemView::DoubleClicked);
    }
    else // disable
    {
        btnModify->setEnabled(bEnable);
        item = lstLblsWdg->item(curRow);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        item->listWidget()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void DMS_ManageLabelDialog::btnAddClicked()
{
    m_bLabelAddNew = true;
    QListWidgetItem *item = new QListWidgetItem;
    QString strNewLabel = u8"新增标签";
    QString strNewLabelIndex = "";
    int maxIndexOfNewLabel = 0, tmp;
    bool bStr2Int = false;
    for(int i = 0; i < m_lstOfLabels.length(); i++)
    {
        if(m_lstOfLabels[i].startsWith(strNewLabel))
        {
            strNewLabelIndex = m_lstOfLabels[i].mid(strNewLabel.count());
            tmp = strNewLabelIndex.toInt(&bStr2Int);
            if((tmp == 0) || (!bStr2Int))
                continue;
            if(tmp > maxIndexOfNewLabel)
                maxIndexOfNewLabel = tmp;
        }
    }

    maxIndexOfNewLabel++;
    strNewLabel += QString::number(maxIndexOfNewLabel);
    m_lstOfLabels.append(strNewLabel);
    m_lstOfLabelQuoteCnt.append(0);
    item->setText(strNewLabel);

    lstLblsWdg->addItem(item);
    lstLblsWdg->setCurrentItem(item);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->listWidget()->setEditTriggers(QAbstractItemView::DoubleClicked);

    lstLblsWdg->editItem(item);

    emit addNewLabel(item->text());
    m_bLabelAddNew = false;
}

void DMS_ManageLabelDialog::btnModifyClicked()
{
    m_bLabelAddNew = false;
    QListWidgetItem *lbl = lstLblsWdg->currentItem();
    lstLblsWdg->editItem(lbl);
}

void DMS_ManageLabelDialog::btnDeleteClicked()
{
    m_bLabelAddNew = false;
    QListWidgetItem *lbl = lstLblsWdg->currentItem();
    if(lbl == NULL)
        return;

    QString strRemoveLbl = lbl->text();

    int index = m_lstOfLabels.indexOf(strRemoveLbl);
    if(m_lstOfLabelQuoteCnt[index] == 0)
    {
        lbl = lstLblsWdg->takeItem(lstLblsWdg->row(lbl));
        emit deleteLabel(strRemoveLbl);
        delete lbl;
    }
}

void DMS_ManageLabelDialog::lstLblWidgetItemRowChanged(int row)
{
    if(row < 0)
        return;

    m_lastRow = row;
    m_lastLabelText = lstLblsWdg->item(row)->text();
    int index = m_lstOfLabels.indexOf(m_lastLabelText);
    if(m_bQuoteLabelEditable)
    {
        this->setCurrentLabelEditEnable(row, true);
    }
    else
    {   
        if(index >= 0 && m_lstOfLabelQuoteCnt[index] > 0)
            this->setCurrentLabelEditEnable(row, false);
        else
            this->setCurrentLabelEditEnable(row, true);
    }

    if(index >= 0 && m_lstOfLabelQuoteCnt[index] > 0)
        this->btnDelete->setEnabled(false);
    else
        this->btnDelete->setEnabled(true);

    if(index >= 0)
        lblQuoteInfo->setText(m_quoteCntInfoPre + QString::number(m_lstOfLabelQuoteCnt[index]) + m_quoteCntInfoSuf);

}

void DMS_ManageLabelDialog::lstLblWidgetItemChanged(QListWidgetItem *itemTextChanged)
{
    QListWidgetItem *item;
    bool bFoundSame;
    if(!m_bLabelAddNew)
    {
        QString strNow = itemTextChanged->text();
        bFoundSame = false;
        for(int i = 0; i < m_lastRow; i++)
        {
            if(strNow == m_lstOfLabels[i])
            {
                bFoundSame = true;
                break;
            }
        }
        for(int j = m_lastRow + 1; j < m_lstOfLabels.length(); j++)
        {
            if(strNow == m_lstOfLabels[j])
            {
                bFoundSame = true;
                break;
            }
        }

        if(bFoundSame)
        {
            item = lstLblsWdg->item(m_lastRow);
            item->setText(m_lastLabelText);
            return;
        }

        int indexRaw = m_lstOfLabels.indexOf(m_lastLabelText);
        if(indexRaw >= 0)
        {
            m_lstOfLabels[indexRaw] = strNow;
        }
        emit modifyLabel(m_lastLabelText, strNow);
    }
}

void DMS_ManageLabelDialog::ckbStateChanged(int iState)
{
    int curRow = lstLblsWdg->currentRow();
    int index = m_lstOfLabels.indexOf(m_lastLabelText);

    if(iState != Qt::Unchecked)
    {
        m_bQuoteLabelEditable = true;
        this->setCurrentLabelEditEnable(curRow, true);
    }
    else
    {
        m_bQuoteLabelEditable = false;
        if(index >= 0 && m_lstOfLabelQuoteCnt[index] > 0)
            this->setCurrentLabelEditEnable(curRow, false);
    }
}
