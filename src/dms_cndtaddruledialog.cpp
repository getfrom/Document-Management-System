#include "dms_cndtaddruledialog.h"

DMS_CndtAddRuleDialog::DMS_CndtAddRuleDialog()
{
    m_lstOfExistNames.clear();
    m_lastTxtName = "";
    m_lastTxtExtV = "";

    m_curRuleCombMode = DMSRuleCombine_AND;
    m_curCndtItem.cndtName = "";
    m_curCndtItem.cndtLevel = 0;
    m_curCndtItem.nextAnds = NULL;
    m_curCndtItem.nextOrs = NULL;

    this->createView();
}

DMS_CndtAddRuleDialog::~DMS_CndtAddRuleDialog()
{
    m_lstOfExistNames.clear();

    delete m_pBtnCancel;
    delete m_pBtnConfirm;
    delete m_pDetermineLayout;
    delete m_pCbbAddMethod;
    delete m_pLblAddMethod;
    delete m_pRow5Layout;
    delete m_pTxtExtV;
    delete m_pLblExtV;
    delete m_pRow4Layout;
    delete m_pCbbMethod;
    delete m_pLblMethod;
    delete m_pRow3Layout;
    delete m_pCbbType;
    delete m_pLblType;
    delete m_pRow2Layout;
    delete m_pTxtName;
    delete m_pLblName;
    delete m_pRow1Layout;
    delete m_pRootLayout;
}

void DMS_CndtAddRuleDialog::createView()
{
    m_pRootLayout = new QVBoxLayout(this);

    m_pRow1Layout = new QHBoxLayout();
    m_pLblName = new QLabel(u8"规则名称", this);
    m_pTxtName = new QLineEdit(this);
    m_pRow1Layout->addWidget(m_pLblName);
    m_pRow1Layout->addWidget(m_pTxtName);
    m_pRootLayout->addLayout(m_pRow1Layout);

    m_pRow2Layout = new QHBoxLayout();
    m_pLblType = new QLabel(u8"规则类型", this);
    m_pCbbType = new QComboBox(this);
    m_pRow2Layout->addWidget(m_pLblType);
    m_pRow2Layout->addWidget(m_pCbbType);
    m_pRootLayout->addLayout(m_pRow2Layout);

    m_pRow3Layout = new QHBoxLayout();
    m_pLblMethod = new QLabel(u8"规则方法", this);
    m_pCbbMethod = new QComboBox(this);
    m_pRow3Layout->addWidget(m_pLblMethod);
    m_pRow3Layout->addWidget(m_pCbbMethod);
    m_pRootLayout->addLayout(m_pRow3Layout);

    m_pRow4Layout = new QHBoxLayout();
    m_pLblExtV = new QLabel(u8"规则额外值", this);
    m_pTxtExtV = new QLineEdit(this);
    m_pRow4Layout->addWidget(m_pLblExtV);
    m_pRow4Layout->addWidget(m_pTxtExtV);
    m_pRootLayout->addLayout(m_pRow4Layout);

    m_pRow5Layout = new QHBoxLayout();
    m_pLblAddMethod = new QLabel(u8"规则添加方式");
    m_pCbbAddMethod = new QComboBox(this);
    m_pRow5Layout->addWidget(m_pLblAddMethod);
    m_pRow5Layout->addWidget(m_pCbbAddMethod);
    m_pRootLayout->addLayout(m_pRow5Layout);

    m_pDetermineLayout = new QHBoxLayout();
    m_pBtnConfirm = new QPushButton(u8"确定", this);
    m_pBtnCancel = new QPushButton(u8"取消", this);
    m_pDetermineLayout->addWidget(m_pBtnConfirm);
    m_pDetermineLayout->addWidget(m_pBtnCancel);
    m_pRootLayout->addLayout(m_pDetermineLayout);

    connect(m_pTxtName, SIGNAL(editingFinished()), this, SLOT(txtNameChangeFinished()));
    connect(m_pTxtExtV, SIGNAL(editingFinished()), this, SLOT(txtExtVChangeFinished()));

    connect(m_pBtnConfirm, SIGNAL(clicked()), this, SLOT(btnConfirmClicked()));
    connect(m_pBtnCancel, SIGNAL(clicked()), this, SLOT(btnCancelClicked()));

    connect(m_pCbbType, SIGNAL(currentIndexChanged(int)), this, SLOT(cbbRuleTypeIndexChanged(int)));
}

void DMS_CndtAddRuleDialog::setExistRuleName(QList<QString> lstName)
{
    m_lstOfExistNames.clear();
    for(auto str : lstName)
        m_lstOfExistNames.append(str);
}

int DMS_CndtAddRuleDialog::setCurrentRuleInfo(const DMS_CndtAddItem& item)
{
    if(item.cndtName.isNull() || item.cndtName.isEmpty())
        return -1;

    m_curCndtItem.cndtName = item.cndtName;
    m_curCndtItem.cndtLevel = item.cndtLevel;
    m_curCndtItem.cndtRule = item.cndtRule;
    m_curCndtItem.nextOrs = item.nextOrs;
    m_curCndtItem.nextAnds = item.nextAnds;

    return 0;
}

void DMS_CndtAddRuleDialog::applyStrOfView()
{
    if(m_pStrOfRuleType == NULL || m_pStrOfRuleMethod == NULL)
        return;

    int indexOfCurItem;

    m_pTxtName->setText(m_curCndtItem.cndtName);
    m_lastTxtName = m_pTxtName->text();

    QList<QString> lstStrTmp;
    lstStrTmp.clear();
    indexOfCurItem = -1;

    for(int i = 0; i < m_pStrOfRuleType->count(); i++)
    {
        lstStrTmp.append((*m_pStrOfRuleType)[i].strMatched);
        if((*m_pStrOfRuleType)[i].ruleType == m_curCndtItem.cndtRule.ruleType)
            indexOfCurItem = i;
    }
    m_pCbbType->addItems(lstStrTmp);
    m_pCbbType->setCurrentIndex(indexOfCurItem);

    m_pCbbMethod->setCurrentIndex(indexOfCurItem);

    m_pTxtExtV->setText(m_curCndtItem.cndtRule.ruleExt);

    lstStrTmp.clear();
    if(m_curRuleAppendPolicy == DMSRuleAppend_All)
    {
        lstStrTmp.append(u8"且");
        lstStrTmp.append(u8"或");
    }
    else if(m_curRuleAppendPolicy == DMSRuleAppend_OnlyAnd)
    {
        lstStrTmp.append(u8"且");
    }
    else if(m_curRuleAppendPolicy == DMSRuleAppend_OnlyOr)
    {
        lstStrTmp.append(u8"或");
    }
    m_pCbbAddMethod->addItems(lstStrTmp);

    if(m_curRuleAppendPolicy == DMSRuleAppend_All)
    {
        if(m_curRuleCombMode == DMSRuleCombine_AND)
            m_pCbbAddMethod->setCurrentText(u8"且");
        else if(m_curRuleCombMode == DMSRuleCombine_OR)
            m_pCbbAddMethod->setCurrentText(u8"或");
    }

    lstStrTmp.clear();
}

DMS_CndtAddItem DMS_CndtAddRuleDialog::getCurrentRuleInfo()
{
    m_curCndtItem.cndtName = m_pTxtName->text();
    // continue from here

    QString curStrOfCbb = m_pCbbType->currentText();
    for(int i = 0; i < m_pStrOfRuleType->count(); i++)
    {
        if((*m_pStrOfRuleType)[i].strMatched == curStrOfCbb)
        {
            m_curCndtItem.cndtRule.ruleType = (*m_pStrOfRuleType)[i].ruleType;
            break;
        }
    }

    curStrOfCbb = m_pCbbMethod->currentText();
    for(int i = 0; i < m_pStrOfRuleMethod->count(); i++)
    {
        if((*m_pStrOfRuleMethod)[i].strMatched == curStrOfCbb)
        {
            m_curCndtItem.cndtRule.ruleMethod = (*m_pStrOfRuleMethod)[i].ruleMethod;
            break;
        }
    }

    m_curCndtItem.cndtRule.ruleExt = m_pTxtExtV->text();

    return m_curCndtItem;
}

DMS_CndtRuleCombineMode DMS_CndtAddRuleDialog::getCurrentRuleCombMode()
{
    QString strOfCombMode = m_pCbbAddMethod->currentText();
    if(strOfCombMode == u8"且")
        m_curRuleCombMode = DMSRuleCombine_AND;
    else if(strOfCombMode == u8"或")
        m_curRuleCombMode = DMSRuleCombine_OR;

    return m_curRuleCombMode;
}

// private slots
void DMS_CndtAddRuleDialog::btnConfirmClicked()
{
    this->accept();
}

void DMS_CndtAddRuleDialog::btnCancelClicked()
{
    this->reject();
}

void DMS_CndtAddRuleDialog::txtNameChangeFinished()
{
    QString curTxt = m_pTxtName->text();
    if(m_lstOfExistNames.contains(curTxt))
        m_pTxtName->setText(m_lastTxtName);

    m_lastTxtName = m_pTxtName->text();
}

void DMS_CndtAddRuleDialog::txtExtVChangeFinished()
{
    QString curExtV = m_pTxtExtV->text();
    m_lastTxtExtV = m_pTxtExtV->text();
}

void DMS_CndtAddRuleDialog::cbbRuleTypeIndexChanged(int index)
{
    QString curTxtType = m_pCbbType->itemText(index);
    DMS_CndtRuleType curRuleType;
    QList<QString> lstStrTmp;
    lstStrTmp.clear();
    for(int i = 0; i < (*m_pStrOfRuleType).count(); i++)
    {
        if(curTxtType == (*m_pStrOfRuleType)[i].strMatched)
        {
            curRuleType = (*m_pStrOfRuleType)[i].ruleType;

            switch((*m_pStrOfRuleType)[i].ruleType)
            {
            case DMSRuleType_FileName:
            case DMSRuleType_FileSuffix:

                m_pCbbMethod->clear();
                for(int j = 0; j < (*m_pStrOfRuleMethod).count(); j++)
                {
                    if((*m_pStrOfRuleType)[i].lstMethodMatched.contains((*m_pStrOfRuleMethod)[j].ruleMethod))

                        lstStrTmp.append((*m_pStrOfRuleMethod)[j].strMatched);
                }
                m_pCbbMethod->addItems(lstStrTmp);
                m_pCbbMethod->setCurrentIndex(0);

                break;
            case DMSRuleType_FileSize:

                m_pCbbMethod->clear();
                for(int j = 0; j < (*m_pStrOfRuleMethod).count(); j++)
                {
                    if((*m_pStrOfRuleType)[i].lstMethodMatched.contains((*m_pStrOfRuleMethod)[j].ruleMethod))
                        lstStrTmp.append((*m_pStrOfRuleMethod)[j].strMatched);
                }
                m_pCbbMethod->addItems(lstStrTmp);
                m_pCbbMethod->setCurrentIndex(0);

                break;
            default:
                break;
            }

            break;
        }
    }


}

