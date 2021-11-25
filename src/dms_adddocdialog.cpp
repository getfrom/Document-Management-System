#include "dms_adddocdialog.h"

DMS_AddDocDialog::DMS_AddDocDialog()
{
    m_bEnableCndtAdd = false;
    m_lstOfDocItems.clear();
    m_lstOfCndtAddInfo.clear();
    m_indexLstOfCndtAddDirBase.clear();
    m_pLstOfLabelMatchInfo = NULL;
    m_pGlobalLabelMaxIndex = NULL;

    this->loadStrMatchText();
    this->setMinimumWidth(600);
    this->setMinimumHeight(250);
    this->createMainView();
}

DMS_AddDocDialog::~DMS_AddDocDialog()
{
    for(int i = 0; i < m_lstOfDocItems.count(); i++)
        m_lstOfDocItems[i].lstOfDocLabelIndices.clear();
    m_lstOfDocItems.clear();
    m_lstOfCndtAddInfo.clear();

    m_lstOfStrMatchRuleType.clear();
    m_lstOfStrMatchRuleMethods.clear();

    delete m_pBtnCancelAdd;
    delete m_pBtnAddConfirm;
    delete m_pLblOfSelectInfo;
    delete m_pFinalOperateLayout;

    delete m_pCndtAddByRules;
    delete m_pBtnCndtDeleteRule;
    delete m_pBtnCndtModifyRule;
    delete m_pBtnCndtAddRule;
    delete m_pCndtOperateLayoutRule;
    delete m_pBtnCndtDeleteDir;
    delete m_pBtnCndtModifyDir;
    delete m_pCndtOperateLayoutDir;
    delete m_pCndtTreWdg;
    delete m_pCkbEnableCndtAdd;
    delete m_pCndtAddRootLayout;
    delete m_pGrpBoxOfCndtAdd;

    delete m_pBtnBaseMoveDown;
    delete m_pBtnBaseMoveUp;
    delete m_pBtnBaseDelete;
    delete m_pBtnBaseModify;
    delete m_pBtnBaseNew;
    delete m_pBaseOperateLayout;

    delete m_pFileLstWdg;
    delete m_pBaseInfoLayout;
    delete m_pRootLayout;
}

void DMS_AddDocDialog::createMainView()
{
    m_pRootLayout = new QVBoxLayout(this);

    //  base add
    m_pBaseInfoLayout = new QHBoxLayout();
    m_pFileLstWdg = new QListWidget();
    m_pBaseInfoLayout->addWidget(m_pFileLstWdg);
    m_pBaseOperateLayout = new QVBoxLayout();
    m_pBtnBaseNew = new QPushButton(u8"新建");
    m_pBtnBaseModify = new QPushButton(u8"编辑");
    m_pBtnBaseDelete = new QPushButton(u8"删除");
    m_pBtnBaseMoveUp = new QPushButton(u8"上移");
    m_pBtnBaseMoveDown = new QPushButton(u8"下移");
    m_pBaseOperateLayout->addWidget(m_pBtnBaseNew);
    m_pBaseOperateLayout->addWidget(m_pBtnBaseModify);
    m_pBaseOperateLayout->addWidget(m_pBtnBaseDelete);
    m_pBaseOperateLayout->addWidget(m_pBtnBaseMoveUp);
    m_pBaseOperateLayout->addWidget(m_pBtnBaseMoveDown);
    m_pBaseInfoLayout->addLayout(m_pBaseOperateLayout);
    m_pRootLayout->addLayout(m_pBaseInfoLayout);

    // condition add
    m_pGrpBoxOfCndtAdd = new QGroupBox(u8"条件添加");
    m_pCndtAddRootLayout = new QVBoxLayout(m_pGrpBoxOfCndtAdd);
    m_pGrpBoxOfCndtAdd->setLayout(m_pCndtAddRootLayout);

    m_pCkbEnableCndtAdd = new QCheckBox(u8"启用条件添加", this);
    m_pCkbEnableCndtAdd->setCheckState(Qt::Unchecked);
    m_pCndtAddRootLayout->addWidget(m_pCkbEnableCndtAdd);

    m_pCndtTreWdg = new QTreeWidget();
    m_pCndtTreWdg->setColumnCount(2);
    QStringList strLst;
    strLst.append(u8"项"); strLst.append(u8"规则");
    m_pCndtTreWdg->setHeaderLabels(strLst);

    m_pCndtAddRootLayout->addWidget(m_pCndtTreWdg);

    m_pCndtOperateLayoutDir = new QHBoxLayout();
    m_pBtnCndtAddDir = new QPushButton(u8"添加目录");
    m_pBtnCndtModifyDir = new QPushButton(u8"修改目录");
    m_pBtnCndtDeleteDir = new QPushButton(u8"删除目录");
    m_pCndtOperateLayoutDir->addWidget(m_pBtnCndtAddDir);
    m_pCndtOperateLayoutDir->addWidget(m_pBtnCndtModifyDir);
    m_pCndtOperateLayoutDir->addWidget(m_pBtnCndtDeleteDir);
    m_pCndtAddRootLayout->addLayout(m_pCndtOperateLayoutDir);
    m_pCndtOperateLayoutRule = new QHBoxLayout();
    m_pBtnCndtAddRule = new QPushButton(u8"添加规则");
    m_pBtnCndtModifyRule = new QPushButton(u8"修改规则");
    m_pBtnCndtDeleteRule = new QPushButton(u8"删除规则");
    m_pCndtOperateLayoutRule->addWidget(m_pBtnCndtAddRule);
    m_pCndtOperateLayoutRule->addWidget(m_pBtnCndtModifyRule);
    m_pCndtOperateLayoutRule->addWidget(m_pBtnCndtDeleteRule);
    m_pCndtAddRootLayout->addLayout(m_pCndtOperateLayoutRule);
    m_pCndtAddByRules = new QPushButton(u8"根据规则添加至预览");
    m_pCndtAddRootLayout->addWidget(m_pCndtAddByRules);
    m_pRootLayout->addWidget(m_pGrpBoxOfCndtAdd);

    m_pFinalOperateLayout = new QHBoxLayout();
    m_pLblOfSelectInfo = new QLabel(u8"预添加 0 项");
    m_pBtnAddConfirm = new QPushButton(u8"添加");
    m_pBtnCancelAdd = new QPushButton(u8"取消");
    m_pFinalOperateLayout->addWidget(m_pLblOfSelectInfo);
    m_pFinalOperateLayout->addWidget(m_pBtnAddConfirm);
    m_pFinalOperateLayout->addWidget(m_pBtnCancelAdd);
    m_pRootLayout->addLayout(m_pFinalOperateLayout);

    this->setLayout(m_pRootLayout);

    connect(m_pBtnBaseNew, SIGNAL(clicked()), this, SLOT(btnBaseNewClicked()));
    connect(m_pBtnBaseModify, SIGNAL(clicked()), this, SLOT(btnBaseModifyClicked()));
    connect(m_pBtnBaseDelete, SIGNAL(clicked()), this, SLOT(btnBaseDeleteClicked()));
    connect(m_pBtnBaseMoveUp, SIGNAL(clicked()), this, SLOT(btnBaseMoveUpClicked()));
    connect(m_pBtnBaseMoveDown, SIGNAL(clicked()), this, SLOT(btnBaseMoveDownClicked()));

    connect(m_pCkbEnableCndtAdd, SIGNAL(stateChanged(int)), this, SLOT(ckbCndtAddStateChanged(int)));

    connect(m_pBtnAddConfirm, SIGNAL(clicked()), this, SLOT(addDocOfAddClicked()));
    connect(m_pBtnCancelAdd, SIGNAL(clicked()), this, SLOT(addDocOfCancelClicked()));

    connect(m_pBtnCndtAddDir, SIGNAL(clicked()), this, SLOT(btnCndtAddDirClicked()));
    connect(m_pBtnCndtModifyDir, SIGNAL(clicked()), this, SLOT(btnCndtModifyDirClicked()));
    connect(m_pBtnCndtDeleteDir, SIGNAL(clicked()), this, SLOT(btnCndtDeleteDirClicked()));
    connect(m_pBtnCndtAddRule, SIGNAL(clicked()), this, SLOT(btnCndtAddRuleClicked()));
    connect(m_pBtnCndtModifyRule, SIGNAL(clicked()), this, SLOT(btnCndtModifyRuleClicked()));
    connect(m_pBtnCndtDeleteRule, SIGNAL(clicked()), this, SLOT(btnCndtDeleteRuleClicked()));
    connect(m_pCndtAddByRules, SIGNAL(clicked()), this, SLOT(btnCndtAddByRulesClicked()));

    if(!m_bEnableCndtAdd)
    {
        m_pCndtTreWdg->setEnabled(false);
        m_pBtnCndtAddDir->setEnabled(false);
        m_pBtnCndtModifyDir->setEnabled(false);
        m_pBtnCndtDeleteDir->setEnabled(false);
        m_pBtnCndtAddRule->setEnabled(false);
        m_pBtnCndtModifyRule->setEnabled(false);
        m_pBtnCndtDeleteRule->setEnabled(false);
        m_pCndtAddByRules->setEnabled(false);
    }
}

void DMS_AddDocDialog::loadStrMatchText()
{
    m_lstOfStrMatchRuleMethods.clear();
    m_lstOfStrMatchRuleType.clear();

    DMS_CndtStringMatchRuleMethod tmpMethod;
    DMS_CndtStringMatchRuleType tmpType;

    // DMS Rule Method String Matching
    tmpMethod.strMatched = u8"包含";
    tmpMethod.ruleMethod = DMSRuleMethod_Include;
    m_lstOfStrMatchRuleMethods.append(tmpMethod);

    tmpMethod.strMatched = u8"除外";
    tmpMethod.ruleMethod = DMSRuleMethod_Exclude;
    m_lstOfStrMatchRuleMethods.append(tmpMethod);

    tmpMethod.strMatched = u8"小于";
    tmpMethod.ruleMethod = DMSRuleMethod_LESS;
    m_lstOfStrMatchRuleMethods.append(tmpMethod);

    tmpMethod.strMatched = u8"大于";
    tmpMethod.ruleMethod = DMSRuleMethod_GREATER;
    m_lstOfStrMatchRuleMethods.append(tmpMethod);

    // DMS Rule Type String Matching
    tmpType.strMatched = u8"文件名称";
    tmpType.ruleType = DMSRuleType_FileName;
    tmpType.lstMethodMatched.clear();
    tmpType.lstMethodMatched.append(DMSRuleMethod_Include);
    tmpType.lstMethodMatched.append(DMSRuleMethod_Exclude);
    m_lstOfStrMatchRuleType.append(tmpType);

    tmpType.strMatched = u8"文件后缀";
    tmpType.ruleType = DMSRuleType_FileSuffix;
    m_lstOfStrMatchRuleType.append(tmpType);

    tmpType.strMatched = u8"文件大小";
    tmpType.ruleType = DMSRuleType_FileSize;
    tmpType.lstMethodMatched.clear();
    tmpType.lstMethodMatched.append(DMSRuleMethod_LESS);
    tmpType.lstMethodMatched.append(DMSRuleMethod_GREATER);
    m_lstOfStrMatchRuleType.append(tmpType);
}

int DMS_AddDocDialog::getAddDocBriefItems(QList<DMS_DocBriefItem> &lstOutput)
{
    lstOutput.clear();
    for(int i = 0; i < m_lstOfDocItems.count(); i++)
        lstOutput.append(m_lstOfDocItems[i]);
    return m_lstOfDocItems.count();
}

void DMS_AddDocDialog::labelInfoAddNew(QString strLblName)
{
    emit addNewLabel(strLblName);
}

void DMS_AddDocDialog::labelInfoModify(QString strRawLblName, QString strNewLblName)
{
    emit modifyLabel(strRawLblName, strNewLblName);
}

void DMS_AddDocDialog::labelInfoDelete(QString strLblName)
{
    emit deleteLabel(strLblName);
}

void DMS_AddDocDialog::addDocItemToCache(DMS_DocBriefItem &docItem)
{
    bool bFoundSamePath = false;
    bool bFoundSameSize = false;

    if(docItem.docName.isEmpty())
        return;

    for(int i = 0; i < m_lstOfDocItems.length(); i++)
    {
        if(docItem.docPath == m_lstOfDocItems[i].docPath)
        {
            bFoundSamePath = true;
            break;
        }
    }

    if(bFoundSamePath)
        return;

    if(bFoundSameSize)
        return;

    m_lstOfDocItems.append(docItem);
    m_pFileLstWdg->addItem(docItem.docName);
}

void DMS_AddDocDialog::modifyDocItemToCache(DMS_DocBriefItem &docRawItem, DMS_DocBriefItem &docNewItem)
{
    if(docRawItem.docName.isEmpty())
        return;

    for(int i = 0; i < m_lstOfDocItems.length(); i++)
    {
        if(m_lstOfDocItems[i].docPath == docRawItem.docPath)
        {
            m_lstOfDocItems[i] = docNewItem;

            QList<QListWidgetItem*> lstFinder = m_pFileLstWdg->findItems(docRawItem.docName, Qt::MatchExactly);
            if(lstFinder.length() > 0)
                lstFinder[0]->setText(docNewItem.docName);
            break;
        }
    }

    int row = m_pFileLstWdg->currentRow();
    m_lstOfDocItems[row] = docNewItem;

}

// slot of base operation
void DMS_AddDocDialog::btnBaseNewClicked()
{
    DMS_DocBriefItem curItem;
    DMS_ShowDocInfoDialog *infoDialog = new DMS_ShowDocInfoDialog(curItem, this);
    if(m_pLstOfLabelMatchInfo != NULL)
        infoDialog->setPointerOfLabelMatchInfo(m_pLstOfLabelMatchInfo);
    //if(m_pGlobalLabelMaxIndex != NULL)
    //    infoDialog->setPointerOfGlobalLabelMaxIndex(m_pGlobalLabelMaxIndex);
    infoDialog->setWindowTitle(u8"新建文档信息");
    connect(infoDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    connect(infoDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    connect(infoDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));

    int retCode = infoDialog->exec();
    if(retCode == QDialog::Accepted)
    {
        curItem = infoDialog->getCurrentDocInfo();
        this->addDocItemToCache(curItem);
    }

    disconnect(infoDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    disconnect(infoDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    disconnect(infoDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));

    delete infoDialog;

    QString strSelectInfo = u8"预添加 ";
    strSelectInfo += QString::number(m_lstOfDocItems.count());
    strSelectInfo += u8" 项";
    m_pLblOfSelectInfo->setText(strSelectInfo);
}

void DMS_AddDocDialog::btnBaseModifyClicked()
{
    int row = m_pFileLstWdg->currentRow();
    if(row < 0 || row >= m_lstOfDocItems.length())
        return;

    DMS_DocBriefItem rawItem = m_lstOfDocItems[row], curItem;
    DMS_ShowDocInfoDialog *infoDialog = new DMS_ShowDocInfoDialog(rawItem, this);
    if(m_pLstOfLabelMatchInfo != NULL)
        infoDialog->setPointerOfLabelMatchInfo(m_pLstOfLabelMatchInfo);
    if(m_pGlobalLabelMaxIndex != NULL)
        infoDialog->setPointerOfGlobalLabelMaxIndex(m_pGlobalLabelMaxIndex);
    infoDialog->setWindowTitle(u8"编辑文档信息");
    infoDialog->setCurrentDocInfo(rawItem);
    connect(infoDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    connect(infoDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    connect(infoDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));

    int retCode = infoDialog->exec();
    if(retCode == QDialog::Accepted)
    {
        curItem = infoDialog->getCurrentDocInfo();
        this->modifyDocItemToCache(rawItem, curItem);
    }

    disconnect(infoDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    disconnect(infoDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    disconnect(infoDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));

    delete infoDialog;
}

void DMS_AddDocDialog::btnBaseDeleteClicked()
{
    int curRow = m_pFileLstWdg->currentRow();
    if(curRow < 0)
        return;

    if(m_lstOfDocItems.length() <= 0)
        return;

    QListWidgetItem *item = m_pFileLstWdg->takeItem(curRow);
    delete item;
    item = NULL;
    m_lstOfDocItems.removeAt(curRow);

}

void DMS_AddDocDialog::btnBaseMoveUpClicked()
{
    int curRow = m_pFileLstWdg->currentRow();
    if(curRow < 1)
        return;
    if(m_lstOfDocItems.length() <= 1)
        return;

    QListWidgetItem *curItem = m_pFileLstWdg->takeItem(curRow);
    m_pFileLstWdg->insertItem(curRow-1, curItem);
    m_lstOfDocItems.swapItemsAt(curRow-1, curRow);
    m_pFileLstWdg->setCurrentRow(curRow-1);
}

void DMS_AddDocDialog::btnBaseMoveDownClicked()
{
    int curRow = m_pFileLstWdg->currentRow();
    if(curRow < 0 || curRow > (m_pFileLstWdg->count() - 2))
        return;
    if(m_lstOfDocItems.length() <= 1 || curRow > (m_lstOfDocItems.length() - 2))
        return;

    QListWidgetItem *curItem = m_pFileLstWdg->takeItem(curRow);
    m_pFileLstWdg->insertItem(curRow+1, curItem);
    m_lstOfDocItems.swapItemsAt(curRow, curRow+1);
    m_pFileLstWdg->setCurrentRow(curRow+1);
}

// slot of condition add operation
void DMS_AddDocDialog::ckbCndtAddStateChanged(int iState)
{
    if(iState == Qt::Checked)
    {
        m_bEnableCndtAdd = true;
        m_pCndtTreWdg->setEnabled(true);
        m_pBtnCndtAddDir->setEnabled(true);
        m_pBtnCndtModifyDir->setEnabled(true);
        m_pBtnCndtDeleteDir->setEnabled(true);
        m_pBtnCndtAddRule->setEnabled(true);
        m_pBtnCndtModifyRule->setEnabled(true);
        m_pBtnCndtDeleteRule->setEnabled(true);
        m_pCndtAddByRules->setEnabled(true);
    }
    else
    {
        m_bEnableCndtAdd = false;
        m_pCndtTreWdg->setEnabled(false);
        m_pBtnCndtAddDir->setEnabled(false);
        m_pBtnCndtModifyDir->setEnabled(false);
        m_pBtnCndtDeleteDir->setEnabled(false);
        m_pBtnCndtAddRule->setEnabled(false);
        m_pBtnCndtModifyRule->setEnabled(false);
        m_pBtnCndtDeleteRule->setEnabled(false);
        m_pCndtAddByRules->setEnabled(false);
    }
}

void DMS_AddDocDialog::btnCndtAddDirClicked()
{
    QString chosenDir = QFileDialog::getExistingDirectory(this);
    QFileInfo dirInfo(chosenDir);
    if(!dirInfo.exists())
        return;

    DMS_CndtAddItem item;

    item.cndtName = chosenDir;
    item.cndtLevel = 0;
    item.nextOrs = NULL;
    item.nextAnds = NULL;
    m_indexLstOfCndtAddDirBase.append(m_lstOfCndtAddInfo.count());
    m_lstOfCndtAddInfo.append(item);

    QTreeWidgetItem *treVItem = new QTreeWidgetItem();
    treVItem->setText(0, chosenDir);
    m_pCndtTreWdg->addTopLevelItem(treVItem);
}

void DMS_AddDocDialog::btnCndtModifyDirClicked()
{
    QList<QTreeWidgetItem *> lstItems = m_pCndtTreWdg->selectedItems();
    if(lstItems.count() == 0 || lstItems.count() > 1)
        return;

    QTreeWidgetItem * item = lstItems[0];
    QString txt = item->text(0);
    if(txt.isEmpty())
        return;

    QString chosenDir = QFileDialog::getExistingDirectory(this);
    QFileInfo dirInfo(chosenDir);
    if(!dirInfo.exists())
        return;

    int indexTopTreWdgItem = m_pCndtTreWdg->indexOfTopLevelItem(item);
    if(indexTopTreWdgItem < 0)
        return;

    item->setText(0, chosenDir);
    m_lstOfCndtAddInfo[m_indexLstOfCndtAddDirBase[indexTopTreWdgItem]].cndtName = chosenDir;
}

void DMS_AddDocDialog::btnCndtDeleteDirClicked()
{
    QList<QTreeWidgetItem *> lstItems = m_pCndtTreWdg->selectedItems();
    if(lstItems.count() == 0 || lstItems.count() > 1)
        return;

    QTreeWidgetItem * item = lstItems[0];
    QString txt = item->text(0);
    if(txt.isEmpty())
        return;

    int indexTopTreWdgItem = m_pCndtTreWdg->indexOfTopLevelItem(item);
    if(indexTopTreWdgItem < 0)
        return;

    // there is a program that can't delete data in this.lst correctly.
    int cnt = 0;
    int itemIndex = 0;
    if((indexTopTreWdgItem + 1) >= m_indexLstOfCndtAddDirBase.count())
    {
        // the last top level item
        cnt = m_lstOfCndtAddInfo.count() - itemIndex;
    }
    else
    {
        cnt = m_indexLstOfCndtAddDirBase[indexTopTreWdgItem+1] -\
                m_indexLstOfCndtAddDirBase[indexTopTreWdgItem];
    }

    itemIndex = m_indexLstOfCndtAddDirBase.takeAt(indexTopTreWdgItem);
    for(int i = itemIndex; i < m_indexLstOfCndtAddDirBase.count(); i++)
        m_indexLstOfCndtAddDirBase[i] -= 1;
    for(int i = 0; i < cnt; i++)
    {
        m_lstOfCndtAddInfo.removeAt(itemIndex);
    }

    cnt = item->childCount();
    for(int i = 0; i < cnt; i++)
    {
        delete item->takeChild(i);
    }


    item = m_pCndtTreWdg->takeTopLevelItem(indexTopTreWdgItem);
    delete item;

}

void DMS_AddDocDialog::btnCndtAddRuleClicked()
{
    QList<QTreeWidgetItem *> lstItems = m_pCndtTreWdg->selectedItems();
    if(lstItems.count() == 0 || lstItems.count() > 1)
        return;

    QTreeWidgetItem * item = lstItems[0];
    QString txt = item->text(0), txt2 = item->text(1);
    if(txt.isEmpty() && txt2.isEmpty())
        return;

    // get the max serial number of label of '新建规则xx'
    QList<QString> lstOfAllRuleNames;
    lstOfAllRuleNames.clear();
    QList<int> lstOfStartIndexOfRuleNameForEachDir;
    lstOfStartIndexOfRuleNameForEachDir.clear();
    QList<PDMS_CndtAddItem> lstTmpItems;
    lstTmpItems.clear();
    PDMS_CndtAddItem pTmpRule;
    // get all name of each rules
    for(int i = 0; i < m_lstOfCndtAddInfo.count(); i++)
    {
        lstOfStartIndexOfRuleNameForEachDir.append(lstOfAllRuleNames.count());
        lstTmpItems.clear();
        lstTmpItems.append(&m_lstOfCndtAddInfo[i]);
        while(lstTmpItems.count() > 0)
        {
            if(lstTmpItems.back()->nextAnds != NULL && \
                    !(lstOfAllRuleNames.contains(lstTmpItems.back()->nextAnds->cndtName)))
            {
                lstTmpItems.append(lstTmpItems.back()->nextAnds);
                lstOfAllRuleNames.append(lstTmpItems.back()->cndtName);
            }
            else
            {
                pTmpRule = lstTmpItems.takeLast();
                if(pTmpRule->nextOrs != NULL)
                {
                    lstTmpItems.append(pTmpRule->nextOrs);
                    lstOfAllRuleNames.append(lstTmpItems.back()->cndtName);
                }
                else if(lstTmpItems.count() <= 1)
                    break;
                else
                {
                    if(lstTmpItems.back()->nextOrs != NULL)
                    {
                        pTmpRule = lstTmpItems.takeLast();
                        lstTmpItems.append(pTmpRule->nextOrs);
                        lstOfAllRuleNames.append(lstTmpItems.back()->cndtName);
                    }
                }
            }
        }
    }

    int maxSerialNumberOfNewRule = -1;
    bool bToOK;
    int iTmp;
    QString ruleName;
    for(int i = 0; i < lstOfAllRuleNames.count(); i++)
    {
        ruleName = lstOfAllRuleNames[i];
        if(ruleName.startsWith(u8"新建规则") && ruleName.size() > 4)
        {
            iTmp = ruleName.mid(4).toInt(&bToOK);
            if(bToOK && iTmp > maxSerialNumberOfNewRule)
                maxSerialNumberOfNewRule = iTmp;
        }
    }


    int indexTopTreWdgItem = m_pCndtTreWdg->indexOfTopLevelItem(item);
    if(indexTopTreWdgItem >= 0)
    {
        // append new rule to current directory
        DMS_CndtAddItem addItemTmp;
        addItemTmp.cndtName = u8"新建规则" + QString::number(maxSerialNumberOfNewRule+1);
        addItemTmp.cndtLevel = 1;
        addItemTmp.cndtRule.ruleMethod = DMSRuleMethod_Include;
        addItemTmp.cndtRule.ruleType = DMSRuleType_FileName;
        addItemTmp.cndtRule.ruleExt = "";
        DMS_CndtAddRuleDialog *ruleDialog = new DMS_CndtAddRuleDialog();
        ruleDialog->setCurrentRuleInfo(addItemTmp);
        ruleDialog->setWindowTitle(u8"新建规则");
        QList<QString> lstStrTmp;
        lstStrTmp.clear();
        if((indexTopTreWdgItem+1) >= lstOfStartIndexOfRuleNameForEachDir.count())
            lstStrTmp = lstOfAllRuleNames.mid(lstOfStartIndexOfRuleNameForEachDir[indexTopTreWdgItem]);
        else
        {
            iTmp = lstOfStartIndexOfRuleNameForEachDir[indexTopTreWdgItem+1] - lstOfStartIndexOfRuleNameForEachDir[indexTopTreWdgItem];
            lstStrTmp = lstOfAllRuleNames.mid(lstOfStartIndexOfRuleNameForEachDir[indexTopTreWdgItem], iTmp);
        }
        ruleDialog->setExistRuleName(lstStrTmp);
        ruleDialog->setCurrentAppendRulePolicy(DMSRuleAppend_OnlyAnd);
        ruleDialog->setCurrentRuleTypeMethodInfo(&m_lstOfStrMatchRuleMethods, \
                                                 &m_lstOfStrMatchRuleType);
        ruleDialog->applyToView();
        if(ruleDialog->exec() == Accepted)
        {
            PDMS_CndtAddItem addItemNew = new DMS_CndtAddItem();
            addItemTmp = ruleDialog->getCurrentRuleInfo();
            addItemNew->cndtName = addItemTmp.cndtName;
            addItemNew->cndtRule = addItemTmp.cndtRule;
            addItemNew->cndtLevel = 1;
            addItemNew->nextAnds = NULL;
            addItemNew->nextOrs = NULL;
            if(m_lstOfCndtAddInfo[indexTopTreWdgItem].nextAnds == NULL)
                m_lstOfCndtAddInfo[indexTopTreWdgItem].nextAnds = addItemNew;
            else
            {
                PDMS_CndtAddItem pItem = m_lstOfCndtAddInfo[indexTopTreWdgItem].nextAnds;
                while(pItem->nextOrs != NULL)
                    pItem = pItem->nextOrs;
                pItem->nextOrs = addItemNew;
            }

            QTreeWidgetItem *treVItem = new QTreeWidgetItem();
            treVItem->setText(0, addItemTmp.cndtName);
            item->addChild(treVItem);
        }

        if(ruleDialog != NULL)
            delete ruleDialog;
    }
    else
    {
        // append new rule to current rule
        QTreeWidgetItem *pCurTreVItem = item, *pLastTreVItem = NULL;
        QList<int> lstOfIndexFromEachParent;
        lstOfIndexFromEachParent.clear();
        while(m_pCndtTreWdg->indexOfTopLevelItem(pCurTreVItem) < 0)
        {
            // continue from here
            pLastTreVItem = pCurTreVItem;
            pCurTreVItem = pCurTreVItem->parent();
            lstOfIndexFromEachParent.append(pCurTreVItem->indexOfChild(pLastTreVItem));
        }
        lstOfIndexFromEachParent.append(m_pCndtTreWdg->indexOfTopLevelItem(pCurTreVItem));
        indexTopTreWdgItem = lstOfIndexFromEachParent.back();

        DMS_CndtAddItem addItemTmp;
        addItemTmp.cndtName = u8"新建规则" + QString::number(maxSerialNumberOfNewRule+1);
        addItemTmp.cndtLevel = 1;
        addItemTmp.cndtRule.ruleMethod = DMSRuleMethod_Include;
        addItemTmp.cndtRule.ruleType = DMSRuleType_FileName;
        addItemTmp.cndtRule.ruleExt = "";
        DMS_CndtAddRuleDialog *ruleDialog = new DMS_CndtAddRuleDialog();
        ruleDialog->setCurrentRuleInfo(addItemTmp);
        ruleDialog->setWindowTitle(u8"新建规则");
        QList<QString> lstStrTmp;
        lstStrTmp.clear();
        if((indexTopTreWdgItem+1) >= lstOfStartIndexOfRuleNameForEachDir.count())
            lstStrTmp = lstOfAllRuleNames.mid(lstOfStartIndexOfRuleNameForEachDir[indexTopTreWdgItem]);
        else
        {
            iTmp = lstOfStartIndexOfRuleNameForEachDir[indexTopTreWdgItem+1] - lstOfStartIndexOfRuleNameForEachDir[indexTopTreWdgItem];
            lstStrTmp = lstOfAllRuleNames.mid(lstOfStartIndexOfRuleNameForEachDir[indexTopTreWdgItem], iTmp);
        }
        ruleDialog->setExistRuleName(lstStrTmp);
        ruleDialog->setCurrentAppendRulePolicy(DMSRuleAppend_OnlyAnd);
        ruleDialog->setCurrentRuleTypeMethodInfo(&m_lstOfStrMatchRuleMethods, \
                                                 &m_lstOfStrMatchRuleType);
        ruleDialog->applyToView();
        if(ruleDialog->exec() == Accepted)
        {
            PDMS_CndtAddItem addItemNew = new DMS_CndtAddItem();
            DMS_CndtRuleCombineMode ruleCombMode = ruleDialog->getCurrentRuleCombMode();
            addItemTmp = ruleDialog->getCurrentRuleInfo();
            addItemNew->cndtName = addItemTmp.cndtName;
            addItemNew->cndtRule = addItemTmp.cndtRule;
            addItemNew->cndtLevel = lstOfIndexFromEachParent.count();
            addItemNew->nextAnds = NULL;
            addItemNew->nextOrs = NULL;

            QTreeWidgetItem *treVItem = new QTreeWidgetItem();
            treVItem->setText(0, addItemTmp.cndtName);

            PDMS_CndtAddItem pCurAddItem = NULL;
            int curIndex = 0;
            if(!lstOfIndexFromEachParent.isEmpty())
                curIndex = lstOfIndexFromEachParent.takeLast();
            if(curIndex < m_lstOfCndtAddInfo.count())
            {
                pCurAddItem = &(m_lstOfCndtAddInfo[curIndex]);
                curIndex = 0;
                while(!lstOfIndexFromEachParent.isEmpty())
                {
                    pCurAddItem = pCurAddItem->nextAnds;
                    curIndex = lstOfIndexFromEachParent.takeLast();
                    for(int i = 0; i < curIndex; i++)
                        pCurAddItem = pCurAddItem->nextOrs;
                    if(pCurAddItem->nextAnds == NULL)
                        break;
                }

                if(ruleCombMode == DMSRuleCombine_AND)
                {
                    if(pCurAddItem->nextAnds == NULL)
                        pCurAddItem->nextAnds = addItemNew;
                    else
                    {
                        pCurAddItem = pCurAddItem->nextAnds;
                        while(pCurAddItem->nextOrs != NULL)
                            pCurAddItem = pCurAddItem->nextOrs;
                        pCurAddItem->nextOrs = addItemNew;
                    }
                    item->addChild(treVItem);
                }
                else
                {
                    if(pCurAddItem->nextOrs == NULL)
                        pCurAddItem->nextOrs = addItemNew;
                    else
                    {
                        pCurAddItem = pCurAddItem->nextOrs;
                        while(pCurAddItem->nextOrs != NULL)
                            pCurAddItem = pCurAddItem->nextOrs;
                        pCurAddItem->nextOrs = addItemNew;
                    }
                    item->parent()->addChild(treVItem);
                }
            }

        }
        if(ruleDialog != NULL)
            delete ruleDialog;

    }

}

void DMS_AddDocDialog::btnCndtModifyRuleClicked()
{
    QList<QTreeWidgetItem *> lstItems = m_pCndtTreWdg->selectedItems();
    if(lstItems.count() == 0 || lstItems.count() > 1)
        return;

    QTreeWidgetItem * item = lstItems[0];
    QString txt = item->text(0), txt2 = item->text(1);
    if(txt.isEmpty() && txt2.isEmpty())
        return;

    if(m_pCndtTreWdg->indexOfTopLevelItem(item) >= 0)
        return;

    QList<QString> lstOfAllRuleNames;
    lstOfAllRuleNames.clear();
    QList<int> lstOfStartIndexOfRuleNameForEachDir;
    lstOfStartIndexOfRuleNameForEachDir.clear();
    QList<PDMS_CndtAddItem> lstTmpItems;
    lstTmpItems.clear();
    PDMS_CndtAddItem pTmpRule;
    for(int i = 0; i < m_lstOfCndtAddInfo.count(); i++)
    {
        lstOfStartIndexOfRuleNameForEachDir.append(lstOfAllRuleNames.count());
        lstTmpItems.clear();
        lstTmpItems.append(&m_lstOfCndtAddInfo[i]);
        while(lstTmpItems.count() > 0)
        {
            if(lstTmpItems.back()->nextAnds != NULL && \
                    !(lstOfAllRuleNames.contains(lstTmpItems.back()->nextAnds->cndtName)))
            {
                lstTmpItems.append(lstTmpItems.back()->nextAnds);
                lstOfAllRuleNames.append(lstTmpItems.back()->cndtName);
            }
            else
            {
                pTmpRule = lstTmpItems.takeLast();
                if(pTmpRule->nextOrs != NULL)
                {
                    lstTmpItems.append(pTmpRule->nextOrs);
                    lstOfAllRuleNames.append(lstTmpItems.back()->cndtName);
                }
                else if(lstTmpItems.count() <= 1)
                    break;
                else
                {
                    if(lstTmpItems.back()->nextOrs != NULL)
                    {
                        pTmpRule = lstTmpItems.takeLast();
                        lstTmpItems.append(pTmpRule->nextOrs);
                        lstOfAllRuleNames.append(lstTmpItems.back()->cndtName);
                    }
                }
            }
        }
    }

    // get the index within each layer
    QTreeWidgetItem *pCurTreVItem = item, *pLastTreVItem = NULL;
    QList<int> lstOfIndexFromEachParent;
    lstOfIndexFromEachParent.clear();
    while(m_pCndtTreWdg->indexOfTopLevelItem(pCurTreVItem) < 0)
    {
        pLastTreVItem = pCurTreVItem;
        pCurTreVItem = pCurTreVItem->parent();
        lstOfIndexFromEachParent.append(pCurTreVItem->indexOfChild(pLastTreVItem));
    }
    lstOfIndexFromEachParent.append(m_pCndtTreWdg->indexOfTopLevelItem(pCurTreVItem));

    // get the dest rule chosen;
    PDMS_CndtAddItem pCurAddItem, pLastAddItem;
    int index = lstOfIndexFromEachParent.takeLast();
    if(index >= m_lstOfCndtAddInfo.count())
        return;
    pCurAddItem = &(m_lstOfCndtAddInfo[index]);
    bool bError = false, bInAnds = true;
    for(; !bError && lstOfIndexFromEachParent.count() > 0;)
    {
        if(pCurAddItem->nextAnds == NULL || lstOfIndexFromEachParent.count() <= 0)
        {
            bError = true;
            break;
        }

        bInAnds = true;
        pLastAddItem = pCurAddItem;
        pCurAddItem = pCurAddItem->nextAnds;
        index = lstOfIndexFromEachParent.takeLast();
        for(int j = 0; !bError && j < index; j++)
        {
            if(pCurAddItem->nextOrs == NULL)
                bError = true;
            else
            {
                bInAnds = false;
                pLastAddItem = pCurAddItem;
                pCurAddItem = pCurAddItem->nextOrs;
            }
        }
    }

    if(bError)
        return;

    DMS_CndtAddRuleDialog *ruleDialog = new DMS_CndtAddRuleDialog();
    ruleDialog->setCurrentRuleInfo(*pCurAddItem);
    ruleDialog->setWindowTitle(u8"修改规则");
    ruleDialog->setExistRuleName(lstOfAllRuleNames);
    if(bInAnds)
        ruleDialog->setCurrentAppendRulePolicy(DMSRuleAppend_OnlyAnd);
    else
        ruleDialog->setCurrentAppendRulePolicy(DMSRuleAppend_OnlyOr);
    ruleDialog->setCurrentRuleTypeMethodInfo(&m_lstOfStrMatchRuleMethods, \
                                             &m_lstOfStrMatchRuleType);
    ruleDialog->applyToView();
    if(ruleDialog->exec() == Accepted)
    {
        DMS_CndtAddItem addItemTmp;
        addItemTmp = ruleDialog->getCurrentRuleInfo();
        pCurAddItem->cndtName = addItemTmp.cndtName;
        pCurAddItem->cndtRule = addItemTmp.cndtRule;

        item->setText(0, addItemTmp.cndtName);
    }

    if(ruleDialog != NULL)
        delete ruleDialog;
}

void DMS_AddDocDialog::btnCndtDeleteRuleClicked()
{
    QList<QTreeWidgetItem *> lstItems = m_pCndtTreWdg->selectedItems();
    if(lstItems.count() == 0 || lstItems.count() > 1)
        return;

    QTreeWidgetItem * item = lstItems[0];
    QString txt = item->text(0), txt2 = item->text(1);
    if(txt.isEmpty() && txt2.isEmpty())
        return;

    if(m_pCndtTreWdg->indexOfTopLevelItem(item) >= 0)
        return;

    // get the index within each layer
    QTreeWidgetItem *pCurTreVItem = item, *pLastTreVItem = NULL;
    QList<int> lstOfIndexFromEachParent;
    lstOfIndexFromEachParent.clear();
    while(m_pCndtTreWdg->indexOfTopLevelItem(pCurTreVItem) < 0)
    {
        pLastTreVItem = pCurTreVItem;
        pCurTreVItem = pCurTreVItem->parent();
        lstOfIndexFromEachParent.append(pCurTreVItem->indexOfChild(pLastTreVItem));
    }
    lstOfIndexFromEachParent.append(m_pCndtTreWdg->indexOfTopLevelItem(pCurTreVItem));

    // get the dest rule chosen;
    PDMS_CndtAddItem pCurAddItem, pLastAddItem, pNextOrAddItem;
    int index = lstOfIndexFromEachParent.takeLast();
    if(index >= m_lstOfCndtAddInfo.count())
        return;
    pCurAddItem = &(m_lstOfCndtAddInfo[index]);
    bool bError = false, bInAnds = true;
    for(; !bError && lstOfIndexFromEachParent.count() > 0;)
    {
        if(pCurAddItem->nextAnds == NULL || lstOfIndexFromEachParent.count() <= 0)
        {
            bError = true;
            break;
        }

        bInAnds = true;
        pLastAddItem = pCurAddItem;
        pCurAddItem = pCurAddItem->nextAnds;
        index = lstOfIndexFromEachParent.takeLast();
        for(int j = 0; !bError && j < index; j++)
        {
            if(pCurAddItem->nextOrs == NULL)
                bError = true;
            else
            {
                bInAnds = false;
                pLastAddItem = pCurAddItem;
                pCurAddItem = pCurAddItem->nextOrs;
            }
        }
    }

    pNextOrAddItem = pCurAddItem->nextOrs;
    if(bError)
        return;

    // need to clear for its each child
    QList<PDMS_CndtAddItem> lstTmpItems;
    lstTmpItems.clear();
    PDMS_CndtAddItem pTmpRule;
    // get all name of each rules

    lstTmpItems.clear();
    lstTmpItems.append(pCurAddItem);
    while(lstTmpItems.count() > 0)
    {
        if(lstTmpItems.back()->nextAnds != NULL)
        {   // deep into Ands
            pTmpRule = lstTmpItems.back();
            lstTmpItems.append(lstTmpItems.back()->nextAnds);
            pTmpRule->nextAnds = NULL;
        }
        else if(lstTmpItems.count() <= 1)
            break;
        else if(lstTmpItems.back()->nextOrs != NULL)
        {
            pTmpRule = lstTmpItems.back();
            lstTmpItems.append(lstTmpItems.back()->nextOrs);
            pTmpRule->nextOrs = NULL;
        }
        else
        {
            delete lstTmpItems.takeLast();
        }
    }

    if(bInAnds)
    {
        pLastAddItem->nextAnds = pNextOrAddItem;
    }
    else
    {
        pLastAddItem->nextOrs = pNextOrAddItem;
    }

    delete pCurAddItem;
    pCurAddItem = NULL;

    pLastTreVItem = item->parent();
    pCurTreVItem = pLastTreVItem->takeChild(pLastTreVItem->indexOfChild(item));
    // need to clear for its each child
    QList<QTreeWidgetItem*> lstOfEachTreWdgItem, lstOfBaseTreWdgItem;
    lstOfBaseTreWdgItem.append(pCurTreVItem->takeChildren());
    QTreeWidgetItem *pParentTreVItem, *pChildTreVItem;
    pParentTreVItem = pChildTreVItem = pCurTreVItem;
    for(int i = 0; i < lstOfBaseTreWdgItem.count(); i++)
    {
        lstOfEachTreWdgItem.clear();
        lstOfEachTreWdgItem.append(lstOfBaseTreWdgItem[i]);
        while(lstOfEachTreWdgItem.count() > 0)
        {
            if(lstOfEachTreWdgItem.back()->childCount() > 0)
                lstOfEachTreWdgItem.append(lstOfEachTreWdgItem.back()->takeChildren());
            else
                delete lstOfEachTreWdgItem.takeLast();

        }
    }

}

void DMS_AddDocDialog::btnCndtAddByRulesClicked()
{
    QList<PDMS_CndtAddItem> lstRulesOfCurDir;
    QList<PDMS_CndtAddItem> lstTmpRulesOfCurDir;
    QList<DMS_StateIndexNode> lstRuleStateMachineOfCurDir;
    int indexOfCurRules = 0; // for [0]
    QList<int> lstRuleIndexOfRetreat; // -1 for no way to retreat
    PDMS_CndtAddItem pCurItem, pLastItem;
    DMS_StateIndexNode sinTmp;
    for(int i = 0; i < m_lstOfCndtAddInfo.count(); i++)
    {
        lstRulesOfCurDir.clear();
        lstTmpRulesOfCurDir.clear();
        lstRuleStateMachineOfCurDir.clear();
        lstRuleIndexOfRetreat.clear();
        lstRuleIndexOfRetreat.append(0);
        sinTmp = { -1, -1, -1 }; // -1 for failed. -2 for succeeded.

        // using breadth searching to generate the state machine
        if(m_lstOfCndtAddInfo[i].nextAnds == NULL)
            break;
        lstTmpRulesOfCurDir.append(m_lstOfCndtAddInfo[i].nextAnds);
        lstRulesOfCurDir.append(m_lstOfCndtAddInfo[i].nextAnds);
        indexOfCurRules = 0;
        while(lstTmpRulesOfCurDir.count() > 0)
        {
            pCurItem = pLastItem = lstTmpRulesOfCurDir.first();
            sinTmp.curIndex = indexOfCurRules;
            while(pCurItem->nextOrs != NULL)
            {
                pCurItem = pCurItem->nextOrs;
                if(!lstTmpRulesOfCurDir.contains(pCurItem))
                {
                    lstTmpRulesOfCurDir.append(pCurItem);
                    lstRulesOfCurDir.append(pCurItem);
                }
            }

            if(pLastItem->nextOrs != NULL)
            {
                if(!lstRuleIndexOfRetreat.isEmpty())
                {
                    if(lstRulesOfCurDir[lstRuleIndexOfRetreat.back()]->cndtLevel == pLastItem->cndtLevel)
                        lstRuleIndexOfRetreat.takeLast();
                    lstRuleIndexOfRetreat.append(lstRulesOfCurDir.indexOf(pLastItem->nextOrs));
                }
            }

            if(pLastItem->nextAnds != NULL)
            {
                pCurItem = pLastItem->nextAnds;
                lstTmpRulesOfCurDir.append(pCurItem);
                lstRulesOfCurDir.append(pCurItem);
                while(pCurItem->nextOrs != NULL) // to make list ordered
                {
                    pCurItem = pCurItem->nextOrs;
                    lstTmpRulesOfCurDir.append(pCurItem);
                    lstRulesOfCurDir.append(pCurItem);
                }
            }

            if(lstRuleIndexOfRetreat.back() != indexOfCurRules && lstRuleIndexOfRetreat.back() != 0)
                sinTmp.falseToIndex = lstRuleIndexOfRetreat.back();
            else
                sinTmp.falseToIndex = -1;

            if(pLastItem->nextAnds != NULL)
                sinTmp.trueToIndex = lstRulesOfCurDir.indexOf(pLastItem->nextAnds);
            else
                sinTmp.trueToIndex = -2;

            lstRuleStateMachineOfCurDir.append(sinTmp);
            indexOfCurRules++;
            lstTmpRulesOfCurDir.takeFirst();
        }

        // get all files under current directory, and analysis this by state machine.
        QString curDirPath = m_lstOfCndtAddInfo[i].cndtName;
        QDir curDirInfo = QDir(curDirPath);
        QFileInfo curFileInfo;
        int iRetIndex;
        if(curDirInfo.exists())
        {
            QFileInfoList lstOfFileInfo = curDirInfo.entryInfoList(QDir::Files);
            for(int j = 0; j < lstOfFileInfo.count(); j++)
            {
                curFileInfo = lstOfFileInfo[j];
                if(curFileInfo.isFile())
                {
                    iRetIndex = 0;
                    while(iRetIndex != -1 && iRetIndex != -2)
                    {
                        if(this->judgeFileSatisfiedRule(curFileInfo, lstRulesOfCurDir[iRetIndex]->cndtRule))
                            iRetIndex = lstRuleStateMachineOfCurDir[iRetIndex].trueToIndex;
                        else
                            iRetIndex = lstRuleStateMachineOfCurDir[iRetIndex].falseToIndex;
                    }

                    if(iRetIndex == -1) // failed, not satisfied.
                        ;
                    else if(iRetIndex == -2)    // succeeded, satisfied.
                    {
                        DMS_DocBriefItem curBriefItem;

                        curBriefItem.docName = curFileInfo.completeBaseName();
                        curBriefItem.docPath = curFileInfo.absoluteFilePath();
                        curBriefItem.docType = "(." + curFileInfo.suffix() + ")" + \
                                    virtualWinFuncs::getFileTypeBySuffix(curFileInfo.suffix());
                        curBriefItem.docSize = virtualWinFuncs::bytesToReadableStr(curFileInfo.size());
                        curBriefItem.docBrief = "";
                        curBriefItem.lstOfDocLabelIndices.clear();
                        this->addDocItemToCache(curBriefItem);
                    }
                    else    // error
                        ;

                }
            }
        }

    }
}

bool DMS_AddDocDialog::judgeFileSatisfiedRule(const QFileInfo &curFileInfo, \
                                              const DMS_CndtRule &curRule)
{
    bool bToIntOk = false;
    qint64 retBytes = 0;

    switch(curRule.ruleType)
    {
    case DMSRuleType_FileName:
        switch(curRule.ruleMethod)
        {
        case DMSRuleMethod_Include:
            if(curFileInfo.fileName().contains(curRule.ruleExt))
                return true;
            break;
        case DMSRuleMethod_Exclude:
            if(!curFileInfo.fileName().contains(curRule.ruleExt))
                return true;
            break;
        default:
            break;
        }
        break;

    case DMSRuleType_FileSuffix:
        switch(curRule.ruleMethod)
        {
        case DMSRuleMethod_Include:
            if(curFileInfo.suffix().contains(curRule.ruleExt))
                return true;
            break;
        case DMSRuleMethod_Exclude:
            if(!curFileInfo.suffix().contains(curRule.ruleExt))
                return true;
            break;
        default:
            break;
        }
        break;

    case DMSRuleType_FileSize:

        retBytes = virtualWinFuncs::readableSizeToBytes(curRule.ruleExt, &bToIntOk);
        if(!bToIntOk)
            break;

        switch(curRule.ruleMethod)
        {
        case DMSRuleMethod_LESS:
            if(curFileInfo.size() < retBytes)
                return true;
            break;
        case DMSRuleMethod_GREATER:
            if(curFileInfo.size() > retBytes)
                return true;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
    return false;
}

// final operation
void DMS_AddDocDialog::addDocOfAddClicked()
{
    if(m_lstOfDocItems.length() > 0)
    {
        emit addDocBriefItem(m_lstOfDocItems);
    }

    this->close();
}

void DMS_AddDocDialog::addDocOfCancelClicked()
{
    this->close();
}

