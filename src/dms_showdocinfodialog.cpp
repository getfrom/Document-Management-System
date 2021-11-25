#include "dms_showdocinfodialog.h"

DMS_ShowDocInfoDialog::DMS_ShowDocInfoDialog(DMS_DocBriefItem &curItemInfo, QWidget *parent)
{
    m_lblCnt = 0;
    m_pLstOfLabelMatchInfo = NULL;
    m_pGlobalLabelMaxIndex = NULL;
    m_lastSelectedLabelName = "";
    m_lastRow = 0;
    m_bLabelAddNew = false;
    m_addNewLabelName.clear();
    m_selectedLabelName.clear();

    rootLayout = new QVBoxLayout();

    row1Layout = new QHBoxLayout();
    lblName = new QLabel(u8"文档名称:", parent);
    txtName = new QLineEdit(curItemInfo.docName, parent);
    row1Layout->addWidget(lblName);
    row1Layout->addWidget(txtName);
    rootLayout->addLayout(row1Layout);

    row2Layout = new QHBoxLayout();
    lblPath = new QLabel(u8"文档路径:", parent);
    txtPath = new QLineEdit(curItemInfo.docPath, parent);
    row2Layout->addWidget(lblPath);
    row2Layout->addWidget(txtPath);
    rootLayout->addLayout(row2Layout);

    row3Layout = new QHBoxLayout();
    lblType = new QLabel(u8"文档类型:", parent);
    txtType = new QLineEdit(curItemInfo.docType, parent);
    row3Layout->addWidget(lblType);
    row3Layout->addWidget(txtType);
    rootLayout->addLayout(row3Layout);

    row4Layout = new QHBoxLayout();
    lblSize = new QLabel(u8"文档大小:", parent);
    txtSize = new QLineEdit(curItemInfo.docSize, parent);
    row4Layout->addWidget(lblSize);
    row4Layout->addWidget(txtSize);
    rootLayout->addLayout(row4Layout);

    row5Layout = new QHBoxLayout();
    labelLayout = new QVBoxLayout();
    lblLabel = new QLabel(u8"标签", parent);
    lblLst = new QListWidget(parent);
    labelLayout->addWidget(lblLabel);
    labelLayout->addWidget(lblLst);
    row5Layout->addLayout(labelLayout);
    labelOperateLayout = new QVBoxLayout();
    btnLabelAddExist = new QPushButton(u8"添加已有标签", parent);
    btnLabelAddNew = new QPushButton(u8"添加新标签", parent);
    btnLabelDelete = new QPushButton(u8"删除", parent);
    labelOperateLayout->addWidget(btnLabelAddExist);
    labelOperateLayout->addWidget(btnLabelAddNew);
    labelOperateLayout->addWidget(btnLabelDelete);
    row5Layout->addLayout(labelOperateLayout);
    briefLayout = new QVBoxLayout();
    lblBrief = new QLabel(u8"简介", parent);
    txtBrief = new QTextEdit(curItemInfo.docBrief, parent);
    briefLayout->addWidget(lblBrief);
    briefLayout->addWidget(txtBrief);
    row5Layout->addLayout(briefLayout);
    rootLayout->addLayout(row5Layout);

    row6Layout = new QHBoxLayout();
    btnBrowseFile = new QPushButton(u8"浏览文件", parent);
    row62Layout = new QHBoxLayout();
    btnConfirm = new QPushButton(u8"确定", parent);
    btnCancel = new QPushButton(u8"取消", parent);
    row62Layout->addWidget(btnConfirm);
    row62Layout->addWidget(btnCancel);
    row62Layout->setAlignment(Qt::AlignRight);
    row6Layout->addWidget(btnBrowseFile);
    row6Layout->addLayout(row62Layout);
    row6Layout->setAlignment(btnBrowseFile, Qt::AlignLeft);
    rootLayout->addLayout(row6Layout);

    this->setLayout(rootLayout);

    connect(btnLabelAddExist, SIGNAL(clicked()), this, SLOT(addExistLabelClicked()));
    connect(btnLabelAddNew, SIGNAL(clicked()), this, SLOT(addNewLabelClicked()));
    connect(btnLabelDelete, SIGNAL(clicked()), this, SLOT(deleteLabelClicked()));
    connect(btnBrowseFile, SIGNAL(clicked()), this, SLOT(browserFileClicked()));
    connect(btnConfirm, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

    connect(lblLst, SIGNAL(itemChanged(QListWidgetItem *)), this, \
            SLOT(lstLblWidgetItemChanged(QListWidgetItem *)));
    connect(lblLst, SIGNAL(currentRowChanged(int)), this, \
            SLOT(lstLblWidgetItemRowChanged(int)));


}

DMS_ShowDocInfoDialog::~DMS_ShowDocInfoDialog()
{
    m_selectedLabelName.clear();
    m_addNewLabelName.clear();

    delete btnCancel;
    delete btnConfirm;
    delete btnBrowseFile;
    delete row62Layout;
    delete row6Layout;
    delete txtBrief;
    delete lblBrief;
    delete briefLayout;
    delete btnLabelDelete;
    delete btnLabelAddNew;
    delete btnLabelAddExist;
    delete labelOperateLayout;
    delete lblLst;
    delete lblLabel;
    delete labelLayout;
    delete row5Layout;
    delete txtSize;
    delete lblSize;
    delete row4Layout;
    delete txtType;
    delete lblType;
    delete row3Layout;
    delete txtPath;
    delete lblPath;
    delete row2Layout;
    delete txtName;
    delete lblName;
    delete row1Layout;
    delete rootLayout;
}

void DMS_ShowDocInfoDialog::addExistLabelClicked()
{
    m_bLabelAddNew = true;
    DMS_SelectLabelDialog *selectLabelDialog = new DMS_SelectLabelDialog();
    // need more exist labels to invoke here.
    QList<QString> lstLabels;
    if(m_pLstOfLabelMatchInfo != NULL)
        for(int i = 0; i < m_pLstOfLabelMatchInfo->length(); i++)
            lstLabels.append((*m_pLstOfLabelMatchInfo)[i].lblName);

    selectLabelDialog->setExistLabels(lstLabels);

    connect(selectLabelDialog, SIGNAL(returnSelectedLabels(QList<QString>)), this, SLOT(addMoreLabel(QList<QString>)));
    selectLabelDialog->exec();
    delete selectLabelDialog;
    m_bLabelAddNew = false;
}

DMS_DocBriefItem DMS_ShowDocInfoDialog::getCurrentDocInfo()
{
    DMS_DocBriefItem item;
    item.docName = txtName->text();
    item.docType = txtType->text();
    item.docBrief = txtBrief->toPlainText();
    item.docPath = txtPath->text();
    item.docSize = txtSize->text();
    item.lstOfDocLabelIndices.clear();

    for(int i = 0; i < m_selectedLabelName.length(); i++)
    {
        for(int j = 0; j < m_pLstOfLabelMatchInfo->length(); j++)
        {
            if((*m_pLstOfLabelMatchInfo)[j].lblName == m_selectedLabelName[i])
            {
                item.lstOfDocLabelIndices.append((*m_pLstOfLabelMatchInfo)[j].lblIndex);
                break;
            }
        }
    }

    return item;
}

void DMS_ShowDocInfoDialog::setCurrentDocInfo(DMS_DocBriefItem &docItem)
{
    if(docItem.docName.isEmpty())
        return;
    if(m_pLstOfLabelMatchInfo == NULL)
        return;

    txtName->setText(docItem.docName);
    txtPath->setText(docItem.docPath);
    txtSize->setText(docItem.docSize);
    txtType->setText(docItem.docType);
    txtBrief->setText(docItem.docBrief);

    int curIndexOfLabel;
    QList<QString> lstLabelName;
    for(int i = 0; i < docItem.lstOfDocLabelIndices.length(); i++)
    {
        curIndexOfLabel = docItem.lstOfDocLabelIndices[i];
        for(int j = 0; j < (*m_pLstOfLabelMatchInfo).length(); j++)
        {
            if((*m_pLstOfLabelMatchInfo)[j].lblIndex == curIndexOfLabel)
            {
                lstLabelName.append((*m_pLstOfLabelMatchInfo)[j].lblName);
                break;
            }
        }
    }

    this->addMoreLabel(lstLabelName);
}

QList<QString> DMS_ShowDocInfoDialog::getAddNewLabelName()
{
    return m_addNewLabelName;
}

void DMS_ShowDocInfoDialog::setCurrentLabelEditEnable(int curRow, bool bEnable)
{
    QListWidgetItem *item = lblLst->item(curRow);
    if(bEnable)
    {
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->listWidget()->setEditTriggers(QAbstractItemView::DoubleClicked);
    }
    else
    {
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        item->listWidget()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void DMS_ShowDocInfoDialog::addNewLabelClicked()
{
    m_bLabelAddNew = true;
    QListWidgetItem *item = new QListWidgetItem;
    QString strNewLabel = u8"新增标签";
    QString strNewLabelIndex = "";
    int maxIndexOfNewLabel = 0, tmp;
    bool bStr2Int = false;
    for(int i = 0; i < m_pLstOfLabelMatchInfo->length(); i++)
    {
        if((*m_pLstOfLabelMatchInfo)[i].lblName.startsWith(strNewLabel))
        {
            strNewLabelIndex = (*m_pLstOfLabelMatchInfo)[i].lblName.mid(strNewLabel.count());
            tmp = strNewLabelIndex.toInt(&bStr2Int);
            if((tmp == 0) || (!bStr2Int))
                continue;
            if(tmp > maxIndexOfNewLabel)
                maxIndexOfNewLabel = tmp;
        }
    }

    maxIndexOfNewLabel++;
    strNewLabel += QString::number(maxIndexOfNewLabel);
    m_addNewLabelName.append(strNewLabel);
    m_selectedLabelName.append(strNewLabel);
    item->setText(strNewLabel);

    lblLst->addItem(item);
    lblLst->setCurrentItem(item);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->listWidget()->setEditTriggers(QAbstractItemView::DoubleClicked);
    lblLst->editItem(item);

    emit addNewLabel(item->text());
    m_bLabelAddNew = false;
}

void DMS_ShowDocInfoDialog::deleteLabelClicked()
{
    if(lblLst->count() <= 0)
        return;
    QListWidgetItem *item = lblLst->currentItem();
    QString strRemoveLbl = item->text();

    int index = m_selectedLabelName.indexOf(strRemoveLbl);
    if(index >= 0 && index < m_selectedLabelName.length())
    {
        item = lblLst->takeItem(lblLst->row(item));
        m_selectedLabelName.removeAt(index);
        delete item;
    }

    index = m_addNewLabelName.indexOf(strRemoveLbl);
    if(index >= 0 && index < m_addNewLabelName.length())
    {
        m_addNewLabelName.removeAt(index);
        emit deleteLabel(strRemoveLbl);
    }
}

void DMS_ShowDocInfoDialog::browserFileClicked()
{
    QString file_name;
    QString file_path = QFileDialog::getOpenFileName(this, u8"选择添加的文档", "c:\\", "*.*");;
    QString file_type;
    QString file_size;

    if(file_path.isEmpty())
        return;

    // continue from here
    QFileInfo fInfo(file_path);
    if(fInfo.exists())
    {
        file_name = fInfo.completeBaseName();
        file_path = fInfo.absoluteFilePath();
        file_type = "(." + fInfo.suffix() + ")" + \
                virtualWinFuncs::getFileTypeBySuffix(fInfo.suffix());
        file_size = virtualWinFuncs::bytesToReadableStr(fInfo.size());

        txtName->setText(file_name);
        txtPath->setText(file_path);
        txtType->setText(file_type);
        txtSize->setText(file_size);
    }
}

void DMS_ShowDocInfoDialog::addMoreLabel(QList<QString> addLstLbls)
{
    for(int i = 0; i < addLstLbls.count(); i++)
    {
        auto finder = lblLst->findItems(addLstLbls[i], Qt::MatchExactly);
        if(finder.count() == 0)
        {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(addLstLbls[i]);
            lblLst->addItem(item);
            m_selectedLabelName.append(addLstLbls[i]);
            lblLst->setCurrentItem(item);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            item->listWidget()->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
    }
}

void DMS_ShowDocInfoDialog::lstLblWidgetItemRowChanged(int row)
{
    if(row < 0)
        return;

    m_lastRow = row;
    m_lastSelectedLabelName = lblLst->item(row)->text();
    int quoteCnt = -1;
    for(int i = 0; i < m_pLstOfLabelMatchInfo->length(); i++)
    {
        if((*m_pLstOfLabelMatchInfo)[i].lblName == m_lastSelectedLabelName)
        {
            quoteCnt = (*m_pLstOfLabelMatchInfo)[i].lblQuoteCnt;
            break;
        }
    }

    if(quoteCnt > 0)
        this->setCurrentLabelEditEnable(row, false);
    else
        this->setCurrentLabelEditEnable(row, true);
}

void DMS_ShowDocInfoDialog::lstLblWidgetItemChanged(QListWidgetItem *itemTextChanged)
{
    QListWidgetItem *item;
    bool bFoundSame;

    if(!m_bLabelAddNew)
    {
        QString strNow = itemTextChanged->text();
        bFoundSame = false;
        for(int i = 0; i < m_lastRow; i++)
        {
            if(strNow == m_selectedLabelName[i])
            {
                bFoundSame = true;
                break;
            }
        }
        for(int j = m_lastRow + 1; !bFoundSame && j < m_selectedLabelName.length(); j++)
        {
            if(strNow == m_selectedLabelName[j])
            {
                bFoundSame = true;
                break;
            }
        }

        if(bFoundSame)
        {
            item = lblLst->item(m_lastRow);
            item->setText(m_lastSelectedLabelName);
            return;
        }

        int index = m_selectedLabelName.indexOf(m_lastSelectedLabelName);
        if(index >= 0)
            m_selectedLabelName[index] = strNow;
        emit modifyLabel(m_lastSelectedLabelName, strNow);
    }
}
