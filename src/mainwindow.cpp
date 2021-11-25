#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_globalLabelMaxIndex(0),
      m_bSaveOrUpdatelabelInfo(false),
      m_docNameOfLabelInfo(u8"lblIndexMatch.ini"),
      m_docNameOfDocItemsInfo(u8"docItems.ini")

{
    m_lstOflabelInfo.clear();
    m_tmpLstOfLabelInfo.clear();
    m_lstOfDocItemInfo.clear();
    m_tmpLstOfLabelInfo.clear();

    pMenuOfRBClickOfItems = NULL;

    this->loadLabelInfoDefault();
    this->loadDocItemsInfoDefault();

    this->createView();
    this->createMenu();
    this->createMenuOfRButtonClick();
}

MainWindow::~MainWindow()
{
    this->restoreLabelInfoDefault();
    this->restoreDocItemsInfoDefault();

    delete pActionRemoveTheseItems;
    delete pActionSaveShortcutAs;
    delete pActionOpenFile;
    delete pActionOpenFilePos;
    delete pActionModifyThisItem;
    delete pMenuOfRBClickOfItems;
    delete pMenuHelpAbout;
    delete pMenuToolManageLabel;
    delete pMenuToolAddDoc;
    delete pMenuFileClose;
    delete pMenuFileSave;
    delete pMenuHelp;
    delete pMenuTool;
    delete pMenuFile;
    delete pMenuBar;

    m_lstOflabelInfo.clear();
    m_tmpLstOfLabelInfo.clear();
    m_lstOfDocItemInfo.clear();
    m_tmpLstOfDocItemInfo.clear();

    delete pDms_docItemViewer;
    delete pDms_labelWidget;
    delete pLabelScrollArea;
    delete pLabelLayout;
    delete pRootWidget;
}

void MainWindow::createView()
{
    pRootWidget = new QWidget(this);
    // build the display area of labels

    pDms_labelWidget = new DMS_HorizontalDragWidget();
    pDms_labelWidget->setAutoFillBackground(true);

    pLabelScrollArea = new QScrollArea();
    pLabelScrollArea->setBackgroundRole(QPalette::Dark);
    pLabelScrollArea->setVisible(true);
    pLabelScrollArea->setWidgetResizable(true);
    pLabelScrollArea->setWidget(pDms_labelWidget);

    pDms_docItemViewer = new DMS_DocBriefWidget(this);

    pLabelLayout = new QGridLayout();
    pLabelLayout->addWidget(pLabelScrollArea);
    pLabelLayout->addWidget(pDms_docItemViewer);

    pRootWidget->setLayout(pLabelLayout);
    this->setCentralWidget(pRootWidget);

    connect(pDms_labelWidget, SIGNAL(changeLabelState(DMS_DragWidgetLabelChangeState, QString)), \
            this, SLOT(labelStateChanged(DMS_DragWidgetLabelChangeState, QString)));
    connect(pDms_docItemViewer, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenuOfDocItem(QPoint)));

}

void MainWindow::createMenu()
{
    pMenuBar = new QMenuBar(this);

    pMenuFile = new QMenu(u8"文件", pMenuBar);
    pMenuFileSave = new QAction(u8"保存", pMenuFile);
    pMenuFileClose = new QAction(u8"退出", pMenuFile);
    connect(pMenuFileSave, SIGNAL(triggered()), this, SLOT(menuFileSaveClicked()));
    connect(pMenuFileClose, SIGNAL(triggered()), this, SLOT(menuFileCloseClicked()));
    pMenuFile->addAction(pMenuFileSave);
    pMenuFile->addAction(pMenuFileClose);
    pMenuBar->addMenu(pMenuFile);

    pMenuTool = new QMenu(u8"工具", pMenuBar);
    pMenuToolAddDoc = new QAction(u8"添加文档", pMenuTool);
    connect(pMenuToolAddDoc, SIGNAL(triggered()), this, SLOT(menuToolAddDocClicked()));
    pMenuTool->addAction(pMenuToolAddDoc);
    pMenuToolManageLabel = new QAction(u8"管理标签", pMenuTool);
    connect(pMenuToolManageLabel, SIGNAL(triggered()), this, SLOT(menuToolManageLabelClicked()));
    pMenuTool->addAction(pMenuToolManageLabel);
    pMenuBar->addMenu(pMenuTool);

    pMenuHelp = new QMenu(u8"帮助", pMenuBar);
    pMenuHelpAbout = new QAction(u8"关于", pMenuHelp);
    connect(pMenuHelpAbout, SIGNAL(triggered()), this, SLOT(menuHelpAboutClicked()));
    pMenuHelp->addAction(pMenuHelpAbout);
    pMenuBar->addMenu(pMenuHelp);

    this->setMenuBar(pMenuBar);
}

void MainWindow::createMenuOfRButtonClick()
{
    pMenuOfRBClickOfItems = new QMenu(this);

    pActionModifyThisItem = new QAction(u8"编辑该项", this);
    pActionOpenFilePos = new QAction(u8"打开文件所在位置", this);
    pActionOpenFile = new QAction(u8"打开文件", this);
    pActionSaveShortcutAs = new QAction(u8"将快捷方式另存为", this);
    pActionRemoveTheseItems = new QAction(u8"移除该项", this);

    connect(pActionModifyThisItem, SIGNAL(triggered()), this, SLOT(actionModifyThisItemClicked()));
    connect(pActionOpenFile, SIGNAL(triggered()), this, SLOT(actionOpenFileClicked()));
    connect(pActionOpenFilePos, SIGNAL(triggered()), this, SLOT(actionOpenFilePosClicked()));
    connect(pActionSaveShortcutAs, SIGNAL(triggered()), this, SLOT(actionSaveShortcutAsClicked()));
    connect(pActionRemoveTheseItems, SIGNAL(triggered()), this, SLOT(actionRemoveTheseItemsClicked()));
}

void MainWindow::applyCurrentSources()
{
    if(this->pDms_labelWidget != NULL)
    {
        for(int i = 0; i < m_tmpLstOfLabelInfo.count(); i++)
        {
            this->pDms_labelWidget->addLabelFromQString(m_tmpLstOfLabelInfo[i].lblName);
        }

        if(pDms_labelWidget->getMaxWidth() > pLabelScrollArea->width())
            pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 26);
        else
            pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 3);
    }

    if(this->pDms_docItemViewer != NULL)
    {
        this->pDms_docItemViewer->setPointerOfLabelMatchInfo(&m_tmpLstOfLabelInfo);
        for(int i = 0; i < m_tmpLstOfDocItemInfo.count(); i++)
            this->pDms_docItemViewer->appendDocBriefItem(m_tmpLstOfDocItemInfo[i]);
        this->pDms_docItemViewer->applyDocBriefItemsToShow();
    }

}

// FILE IO
void MainWindow::loadLabelInfoDefault()
{
    QString pathOfLblInfo = this->getLabelInfoPath();
    QFile fIn(pathOfLblInfo);
    if(!fIn.exists())
    {
        this->createDefaultLabelInfo(pathOfLblInfo);
    }

    if(!fIn.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    unsigned int numOfLabel = 0;
    QString curLine;
    QTextStream fStream(&fIn);
    fStream.setCodec("UTF-8");

    // confirm this file be corrected;
    curLine = fStream.readLine();
    if(curLine.startsWith(u8"DMS LABEL "))
    {
        auto lstStr = curLine.split(' ');
        numOfLabel = lstStr[2].toUInt();
        m_globalLabelMaxIndex = lstStr[3].toInt();
    }
    else    // error
    {
        fIn.close();
        return;
    }

    QStringList lstStr;
    DMS_LabelMatchItem lblItem;
    for(unsigned int iLine = 0; (!fStream.atEnd()) && (iLine < numOfLabel); iLine++)
    {
        curLine = fStream.readLine();
        lstStr = curLine.split(' ');
        lblItem.lblName = lstStr[0];
        lblItem.lblIndex = lstStr[1].toUShort();
        lblItem.lblQuoteCnt = lstStr[2].toUShort();
        m_lstOflabelInfo.append(lblItem);
        m_tmpLstOfLabelInfo.append(lblItem);
    }

    fIn.close();
}

void MainWindow::restoreLabelInfoDefault()
{
    QString pathOfLblInfo = this->getLabelInfoPath();
    QFile fOut(pathOfLblInfo);
    if(!fOut.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream fStream(&fOut);
    fStream.setCodec("UTF-8");
    fStream << u8"DMS LABEL " << m_lstOflabelInfo.length() << " " << m_globalLabelMaxIndex << endl;

    for(int i = 0; i < m_lstOflabelInfo.length(); i++)
    {
        fStream << m_lstOflabelInfo[i].lblName << " ";
        fStream << m_lstOflabelInfo[i].lblIndex << " ";
        fStream << m_lstOflabelInfo[i].lblQuoteCnt << endl;
    }

    fOut.close();
}

void MainWindow::createDefaultLabelInfo(QString pathOfLabelInfo)
{
    QFile fOut(pathOfLabelInfo);
    if(fOut.exists())
        return;

    if(!fOut.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream fStream(&fOut);
    fStream.setCodec("UTF-8");
    fStream << u8"DMS LABEL " << 0 << " " << 1 << endl;

    fOut.close();
}

void MainWindow::loadDocItemsInfoDefault()
{
    QString pathOfDocItemInfo = this->getDocItemsInfoPath();
    QFile fIn(pathOfDocItemInfo);
    if(!fIn.exists())
    {
        this->createDefaultDocItemsInfo(pathOfDocItemInfo);
    }

    if(!fIn.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    unsigned int numOfItems = 0;
    QString curLine;
    QTextStream fStream(&fIn);
    fStream.setCodec("UTF-8");

    curLine = fStream.readLine();
    if(curLine.startsWith(u8"DMS ITEM "))
    {
        auto lstStr = curLine.split(' ');
        numOfItems = lstStr[2].toUInt();
    }
    else    // error
    {
        fIn.close();
        return;
    }

    QList<QString> lstOfExtName;
    lstOfExtName.clear();
    uint64_t uTmp;
    QStringList lstStr;
    bool bTmp;
    QString strTmp;
    DMS_DocBriefItem tmpDocItem;
    for(unsigned int uItem = 0; (!fStream.atEnd()) && (uItem < numOfItems); uItem++)
    {
        tmpDocItem.docName = "";
        tmpDocItem.docPath = "";
        tmpDocItem.docSize = "";
        tmpDocItem.docType = "";
        tmpDocItem.docBrief = "";
        tmpDocItem.lstOfDocLabelIndices.clear();

        // line 1
        curLine = fStream.readLine();
        lstStr = curLine.split('"');
        tmpDocItem.docName = lstStr[1];
        strTmp = lstStr[3];
        tmpDocItem.docType = lstStr[3];
        if(!lstOfExtName.contains(strTmp))
            lstOfExtName.append(strTmp);

        // line 2
        if(!fStream.atEnd())
        {
            curLine = fStream.readLine();
            tmpDocItem.docBrief = curLine;
        }

        // line 3
        if(!fStream.atEnd())
        {
            curLine = fStream.readLine();
            tmpDocItem.docPath = curLine;
        }

        // line 4
        if(!fStream.atEnd())
        {
            curLine = fStream.readLine();
            bTmp = false;
            uTmp = curLine.toLongLong(&bTmp);
            if(!bTmp)
            {
                QFileInfo fInfo(tmpDocItem.docPath);
                if(fInfo.exists())
                {
                    uTmp = fInfo.size();
                }
                else
                    uTmp = 0;
            }
            tmpDocItem.docSize = virtualWinFuncs::bytesToReadableStr(uTmp);
        }

        // line 5
        if(!fStream.atEnd())
        {
            lstStr.clear();
            curLine = fStream.readLine();
            lstStr = curLine.split(' ');
            for(int i = 0; i < lstStr.count(); i++)
            {
                bTmp = false;
                uTmp = lstStr[i].toUShort(&bTmp);
                if(bTmp && uTmp < 0x10000)
                {
                    tmpDocItem.lstOfDocLabelIndices.append((unsigned short)uTmp);
                }
            }
        }

        if(tmpDocItem.docName.isEmpty() || tmpDocItem.docName == "")
            continue;

        m_lstOfDocItemInfo.append(tmpDocItem);
    }

    QList<QString> lstOfExtTypeMatchExtName;
    lstOfExtTypeMatchExtName.clear();
    for(int i = 0; i < lstOfExtName.count(); i++)
        lstOfExtTypeMatchExtName.append(virtualWinFuncs::getFileTypeBySuffix(lstOfExtName[i]));

    int index;
    for(int i = 0; i < m_lstOfDocItemInfo.count(); i++)
    {
        index = lstOfExtName.indexOf(m_lstOfDocItemInfo[i].docType);
        if(index >= 0)
            m_lstOfDocItemInfo[i].docType = "(." + lstOfExtName[index] + ")" + lstOfExtTypeMatchExtName[index];

        m_tmpLstOfDocItemInfo.append(m_lstOfDocItemInfo[i]);
    }

    fIn.close();
}

void MainWindow::restoreDocItemsInfoDefault()
{
    QString pathOfDocItemInfo = this->getDocItemsInfoPath();
    QFile fOut(pathOfDocItemInfo);
    if(!fOut.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream fStream(&fOut);
    fStream.setCodec("UTF-8");
    fStream << u8"DMS ITEM " << m_lstOfDocItemInfo.length() << endl;

    QString strTmp;
    uint64_t uTmp;
    for(int i = 0; i < m_lstOfDocItemInfo.length(); i++)
    {
        uTmp = 0;
        strTmp = m_lstOfDocItemInfo[i].docPath;
        if(!strTmp.isEmpty())
        {
            QFileInfo fInfo(strTmp);
            if(fInfo.exists())
            {
                strTmp = fInfo.suffix();
                uTmp = fInfo.size();
            }
            else
                strTmp = "";
        }
        else
            strTmp = "";

        fStream << "\"" << m_lstOfDocItemInfo[i].docName << "\" \"" << strTmp << "\"" << endl;
        fStream << m_lstOfDocItemInfo[i].docBrief << endl;
        fStream << m_lstOfDocItemInfo[i].docPath << endl;
        fStream << uTmp << endl;
        strTmp = "";
        for(int j = 0; j < m_lstOfDocItemInfo[i].lstOfDocLabelIndices.count(); j++)
            strTmp.append(QString::number(m_lstOfDocItemInfo[i].lstOfDocLabelIndices[j]) + " ");
        fStream << strTmp << endl;
    }

    fOut.close();

}

void MainWindow::createDefaultDocItemsInfo(QString pathOfDocItemsInfo)
{
    QFile fOut(pathOfDocItemsInfo);
    if(fOut.exists())
        return;

    if(!fOut.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream fStream(&fOut);
    fStream.setCodec("UTF-8");
    fStream << u8"DMS ITEM " << 0 << endl;

    fOut.close();
}

// LABEL handler
int MainWindow::addOrQuoteLabel(QString lblName)
{
    for(int i = 0; i < m_tmpLstOfLabelInfo.length(); i++)
    {
        if(m_tmpLstOfLabelInfo[i].lblName == lblName)
        {
            m_tmpLstOfLabelInfo[i].lblQuoteCnt += 1;
            return m_tmpLstOfLabelInfo[i].lblQuoteCnt;
        }
    }

    DMS_LabelMatchItem item;
    item.lblName = lblName;
    item.lblIndex = (++m_globalLabelMaxIndex);
    item.lblQuoteCnt = 0;
    m_tmpLstOfLabelInfo.append(item);
    pDms_labelWidget->addLabelFromQString(lblName);

    if(pDms_labelWidget->getMaxWidth() > pLabelScrollArea->width())
    {
        pLabelScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 26);
    }
    else
    {
        pLabelScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 3);
    }

    return 0;
}

int MainWindow::quoteLabel(unsigned short lblIndex)
{
    for(int i = 0; i < m_tmpLstOfLabelInfo.length(); i++)
    {
        if(m_tmpLstOfLabelInfo[i].lblIndex == lblIndex)
        {
            m_tmpLstOfLabelInfo[i].lblQuoteCnt += 1;
            return m_tmpLstOfLabelInfo[i].lblQuoteCnt;
        }
    }

    return -1;
}

int MainWindow::unQuoteLabel(QString lblName)
{
    for(int i = 0; i < m_tmpLstOfLabelInfo.length(); i++)
    {
        if(m_tmpLstOfLabelInfo[i].lblName == lblName)
        {
            if(m_tmpLstOfLabelInfo[i].lblQuoteCnt >= 1)
                m_tmpLstOfLabelInfo[i].lblQuoteCnt -= 1;

            return m_tmpLstOfLabelInfo[i].lblQuoteCnt;
        }
    }

    return -1;
}

int MainWindow::deleteUnQuoteLabel(QString lblName)
{
    for(int i = 0; i < m_tmpLstOfLabelInfo.length(); i++)
    {
        if(m_tmpLstOfLabelInfo[i].lblName == lblName)
        {
            if(m_tmpLstOfLabelInfo[i].lblQuoteCnt > 0)
                return m_tmpLstOfLabelInfo[i].lblQuoteCnt;
            else
            {
                m_tmpLstOfLabelInfo.removeAt(i);
                pDms_labelWidget->removeLabelFromQString(lblName);

                if((pDms_labelWidget->getMaxWidth()) > (pLabelScrollArea->width()))
                {
                    pLabelScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                    pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 26);
                }
                else
                {
                    pLabelScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 3);
                }

                return 0;
            }
        }
    }
    return -1;
}

// protected event handler
void MainWindow::closeEvent(QCloseEvent *event)
{
    this->menuFileCloseClicked();
    QMainWindow::closeEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(pDms_labelWidget->getMaxWidth() > pLabelScrollArea->width())
        pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 26);
    else
        pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 3);
    QMainWindow::resizeEvent(event);
}

// slot
void MainWindow::showMenuOfDocItem(QPoint pos)
{
    if(pMenuOfRBClickOfItems == NULL)
        return;

    int x = pos.x();
    int y = pos.y();
    QModelIndex index = pDms_docItemViewer->indexAt(QPoint(x, y));
    int row = index.row();
    if(row < 0)
    {
        return;
    }
    else
    {
        QModelIndexList lstIndices = pDms_docItemViewer->getSelectedIndexes();
        if(lstIndices.count() <= 0)
            return;
        int row;
        QSet<int> setRows;
        for(auto index: lstIndices)
        {
            row = index.row();
            if(row >= m_tmpLstOfDocItemInfo.count())
                continue;
            setRows.insert(row);
        }

        if(pMenuOfRBClickOfItems != NULL)
        {
            pMenuOfRBClickOfItems->clear();
            delete pMenuOfRBClickOfItems;
            pMenuOfRBClickOfItems = NULL;
        }

        pMenuOfRBClickOfItems = new QMenu(this);

        if(setRows.count() <= 1)
            pMenuOfRBClickOfItems->addAction(pActionModifyThisItem);
        pMenuOfRBClickOfItems->addAction(pActionOpenFile);
        if(setRows.count() <= 1)
            pMenuOfRBClickOfItems->addAction(pActionOpenFilePos);
        pMenuOfRBClickOfItems->addAction(pActionSaveShortcutAs);
        pMenuOfRBClickOfItems->addAction(pActionRemoveTheseItems);

        pMenuOfRBClickOfItems->move(cursor().pos());
        pMenuOfRBClickOfItems->show();
    }
}

void MainWindow::menuFileSaveClicked()
{
    m_lstOflabelInfo.clear();
    for(int i = 0; i < m_tmpLstOfLabelInfo.length(); i++)
        m_lstOflabelInfo.append(m_tmpLstOfLabelInfo[i]);
    this->restoreLabelInfoDefault();

    m_lstOfDocItemInfo.clear();
    for(int i = 0; i < m_tmpLstOfDocItemInfo.length(); i++)
        m_lstOfDocItemInfo.append(m_tmpLstOfDocItemInfo[i]);
    this->restoreDocItemsInfoDefault();

    m_bSaveOrUpdatelabelInfo = false;
}

void MainWindow::menuFileCloseClicked()
{
    if(m_bSaveOrUpdatelabelInfo)
    {
        int ret = QMessageBox::information(this, u8"提示", u8"是否保存已修改内容?",
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(ret == QMessageBox::Yes)
            this->menuFileSaveClicked();
    }
    this->close();
}

void MainWindow::menuToolAddDocClicked()
{
    DMS_AddDocDialog *addDocDialog = new DMS_AddDocDialog();
    addDocDialog->setPointerOfLabelMatchInfo(&m_tmpLstOfLabelInfo);
    addDocDialog->setWindowTitle(u8"添加文档");

    connect(addDocDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    connect(addDocDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    connect(addDocDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));
    connect(addDocDialog, SIGNAL(addDocBriefItem(QList<DMS_DocBriefItem>)), this, SLOT(docBriefItemAdd(QList<DMS_DocBriefItem>)));

    addDocDialog->exec();

    disconnect(addDocDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    disconnect(addDocDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    disconnect(addDocDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));
    disconnect(addDocDialog, SIGNAL(addDocBriefItem(QList<DMS_DocBriefItem>)), this, SLOT(docBriefItemAdd(QList<DMS_DocBriefItem>)));

    delete addDocDialog;
}

void MainWindow::menuToolManageLabelClicked()
{

    QList<QString> lstLabels;
    QList<unsigned short> lstLabelQuoteCnt;
    for(int i = 0; i < m_tmpLstOfLabelInfo.length(); i++)
    {
        lstLabels.append(m_tmpLstOfLabelInfo[i].lblName);
        lstLabelQuoteCnt.append(m_tmpLstOfLabelInfo[i].lblQuoteCnt);
    }

    DMS_ManageLabelDialog *manageLblDialog = new DMS_ManageLabelDialog();
    manageLblDialog->setWindowTitle(u8"标签管理器");
    manageLblDialog->setExistLabels(lstLabels, lstLabelQuoteCnt);

    connect(manageLblDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    connect(manageLblDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    connect(manageLblDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));

    manageLblDialog->exec();

    disconnect(manageLblDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    disconnect(manageLblDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    disconnect(manageLblDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));

    delete manageLblDialog;
}

void MainWindow::menuHelpAboutClicked()
{
    DMS_HelpAboutDialog *aboutDialog = new DMS_HelpAboutDialog();
    aboutDialog->createView();
    aboutDialog->exec();
    delete aboutDialog;
}

// private function
QString MainWindow::getDocItemsInfoPath()
{
    QString strExeDir = QCoreApplication::applicationDirPath();
    if(!strExeDir.endsWith('/'))
        strExeDir += u8"/";

    QString strLblInfoPath = strExeDir + m_docNameOfDocItemsInfo;
    return strLblInfoPath;
}

QString MainWindow::getLabelInfoPath()
{
    QString strExeDir = QCoreApplication::applicationDirPath();
    if(!strExeDir.endsWith('/'))
        strExeDir += u8"/";

    QString strLblInfoPath = strExeDir + m_docNameOfLabelInfo;
    return strLblInfoPath;
}

// private slots;
void MainWindow::labelStateChanged(DMS_DragWidgetLabelChangeState lblState, QString lblName)
{
    int labelIndex = -1;
    for(int i = 0; i < m_tmpLstOfLabelInfo.count(); i++)
    {
        if(m_tmpLstOfLabelInfo[i].lblName == lblName)
        {
            labelIndex = m_tmpLstOfLabelInfo[i].lblIndex;
            break;
        }
    }
    if(labelIndex < 0)
        return;

    QList<QString> lstOfChosenLblNames;
    QList<unsigned short> lstOfChosenLblIndices;
    pDms_labelWidget->getChosenLabelNames(lstOfChosenLblNames);
    QList<DMS_LabelMatchItem> lstOfLabelInfo;
    for(int i = 0; i < m_tmpLstOfLabelInfo.count(); i++)
        lstOfLabelInfo.append(m_tmpLstOfLabelInfo[i]);

    for(int i = 0; i < lstOfChosenLblNames.count(); i++)
    {
        for(int j = 0; j < lstOfLabelInfo.count(); j++)
        {
            if(lstOfChosenLblNames[i] == lstOfLabelInfo[j].lblName)
            {
                lstOfChosenLblIndices.append(lstOfLabelInfo[j].lblIndex);
                lstOfLabelInfo.removeAt(j);
            }
        }
    }

    //pDms_docItemViewer->hideRows(0, 0xffff);
    //pDms_docItemViewer->showRowsByLabelIndices(lstOfChosenLblIndices);
    pDms_docItemViewer->sortRowsByLabelIndicesWithMatchingDegree(lstOfChosenLblIndices);

    lstOfChosenLblNames.clear();
    lstOfChosenLblIndices.clear();
    lstOfLabelInfo.clear();
}

void MainWindow::labelInfoAddNew(QString strLblName)
{
    this->addOrQuoteLabel(strLblName);
    m_bSaveOrUpdatelabelInfo = true;
}

void MainWindow::labelInfoModify(QString strRawLblName, QString strNewLblName)
{
    if(strRawLblName == strNewLblName)
        return;

    for(int i = 0; i < m_tmpLstOfLabelInfo.length(); i++)
    {
        if(m_tmpLstOfLabelInfo[i].lblName == strRawLblName)
        {
            m_tmpLstOfLabelInfo[i].lblName = strNewLblName;
            pDms_labelWidget->modifyLabelFromQString(strRawLblName, strNewLblName);
            break;
        }
    }

    if(pDms_labelWidget->getMaxWidth() > pLabelScrollArea->width())
    {
        pLabelScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 26);
    }
    else
    {
        pLabelScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        pLabelScrollArea->setFixedHeight(pDms_labelWidget->getMaxHeight() + 3);
    }

    m_bSaveOrUpdatelabelInfo = true;
}

void MainWindow::labelInfoDelete(QString strLblName)
{
    this->deleteUnQuoteLabel(strLblName);
    m_bSaveOrUpdatelabelInfo = true;
}

void MainWindow::docBriefItemAdd(QList<DMS_DocBriefItem> lstDocItems)
{
    if(lstDocItems.length() <= 0)
        return;

    if(pDms_docItemViewer == NULL)
        return;

    for(auto item: lstDocItems)
    {
        for(auto lblIndex : item.lstOfDocLabelIndices)
            this->quoteLabel(lblIndex);
        m_tmpLstOfDocItemInfo.append(item);
        pDms_docItemViewer->appendDocBriefItem(item);
    }
    pDms_docItemViewer->applyDocBriefItemsToShow();

    m_bSaveOrUpdatelabelInfo = true;
}

// slot of RButton clicked in docItemViewer
void MainWindow::actionModifyThisItemClicked()
{
    QModelIndex index = pDms_docItemViewer->currentIndex();
    int row = index.row();
    if(row >= m_tmpLstOfDocItemInfo.count())
        return;

    DMS_DocBriefItem rawItem = m_tmpLstOfDocItemInfo[row], curItem;
    DMS_ShowDocInfoDialog *infoDialog = new DMS_ShowDocInfoDialog(rawItem, this);
    infoDialog->setPointerOfLabelMatchInfo(&m_tmpLstOfLabelInfo);
    infoDialog->setPointerOfGlobalLabelMaxIndex(&m_globalLabelMaxIndex);
    infoDialog->setWindowTitle(u8"编辑文档信息");
    infoDialog->setCurrentDocInfo(rawItem);
    connect(infoDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    connect(infoDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    connect(infoDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));

    int retCode = infoDialog->exec();
    if(retCode == QDialog::Accepted)
    {
        curItem = infoDialog->getCurrentDocInfo();
        m_tmpLstOfDocItemInfo[row] = curItem;
        pDms_docItemViewer->modifyDocBriefItem(row, curItem);
        for(int i = 0; i < curItem.lstOfDocLabelIndices.count(); i++)
        {
            if(!rawItem.lstOfDocLabelIndices.contains(curItem.lstOfDocLabelIndices[i]))
                this->quoteLabel(curItem.lstOfDocLabelIndices[i]);
        }
    }

    disconnect(infoDialog, SIGNAL(addNewLabel(QString)), this, SLOT(labelInfoAddNew(QString)));
    disconnect(infoDialog, SIGNAL(modifyLabel(QString, QString)), this, SLOT(labelInfoModify(QString, QString)));
    disconnect(infoDialog, SIGNAL(deleteLabel(QString)), this, SLOT(labelInfoDelete(QString)));

    delete infoDialog;
}

void MainWindow::actionOpenFileClicked()
{
    QModelIndexList lstIndices = pDms_docItemViewer->getSelectedIndexes();
    if(lstIndices.count() <= 0)
        return;
    int row;
    QSet<int> setRows;
    for(auto index: lstIndices)
    {
        row = index.row();
        if(row >= m_tmpLstOfDocItemInfo.count())
            continue;
        setRows.insert(row);
    }

    for(auto row : setRows)
    {
        DMS_DocBriefItem docItem = m_tmpLstOfDocItemInfo[row];
        ::ShellExecuteW(NULL, NULL, docItem.docPath.toStdWString().c_str(), NULL, NULL, SW_NORMAL);
    }

    /*
    QModelIndex index = pDms_docItemViewer->currentIndex();
    int row = index.row();
    if(row >= m_tmpLstOfDocItemInfo.count())
        return;

    DMS_DocBriefItem docItem = m_tmpLstOfDocItemInfo[row];
    ::ShellExecuteW(NULL, NULL, docItem.docPath.toStdWString().c_str(), NULL, NULL, SW_NORMAL);
    */

}

void MainWindow::actionOpenFilePosClicked()
{
    QModelIndex index = pDms_docItemViewer->currentIndex();
    int row = index.row();
    if(row >= m_tmpLstOfDocItemInfo.count())
        return;

    DMS_DocBriefItem docItem = m_tmpLstOfDocItemInfo[row];
    QFileInfo fInfo(docItem.docPath);
    if(!fInfo.exists())
        return;

    QString shlExeOpera = u8"open";
    QString shlExeFile = u8"explorer.exe";
    QString shlExeArgs = u8"/n,/select,";
    QString filePathName = fInfo.absoluteFilePath();
    filePathName.replace('/', '\\');
    shlExeArgs += filePathName;

    ::ShellExecuteW(NULL, shlExeOpera.toStdWString().c_str(), \
                    shlExeFile.toStdWString().c_str(), \
                    shlExeArgs.toStdWString().c_str(), NULL, SW_SHOWNORMAL);
}

void MainWindow::actionSaveShortcutAsClicked()
{
    QModelIndexList lstIndices = pDms_docItemViewer->getSelectedIndexes();
    if(lstIndices.count() <= 0)
        return;

    QString chosenDir = QFileDialog::getExistingDirectory(this);
    QFileInfo dirInfo(chosenDir);
    if(!dirInfo.exists())
        return;

    int row;
    QSet<int> setRows;
    for(auto index: lstIndices)
    {
        row = index.row();
        if(row >= m_tmpLstOfDocItemInfo.count())
            continue;
        setRows.insert(row);
    }

    for(auto row : setRows)
    {
        QFileInfo fInfo(m_tmpLstOfDocItemInfo[row].docPath);
        if(!fInfo.exists())
            continue;

        QString strCmdLine = virtualWinFuncs::getDefaultOpenCommandBySuffix(fInfo.suffix());
        if(strCmdLine.isEmpty())
            continue;

        QString strCmdExe;
        QList<QString> lstCmdArgs;
        bool bRet = virtualWinFuncs::parseCmdLine(strCmdLine, strCmdExe, lstCmdArgs);
        if((!bRet) || (!lstCmdArgs.contains("%1")))
            continue;

        QString startAppPath = fInfo.absoluteFilePath().replace('/', '\\');
        QString addCmdLine = "";
        for(int i = 0; i < lstCmdArgs.count(); i++)
        {
            if(lstCmdArgs[i] == "%1")
            {
                addCmdLine += "\"" + fInfo.absoluteFilePath() + "\"";
                break;
            }
            addCmdLine += lstCmdArgs[i] + " ";
        }
        addCmdLine = addCmdLine.replace('/', '\\');
        QString destLnkPath = chosenDir + "/" + fInfo.fileName() + ".lnk";
        destLnkPath = destLnkPath.replace('/', '\\');

        bRet = virtualWinFuncs::createLinkFile(startAppPath.toStdWString().c_str(), \
                                               addCmdLine.toStdWString().c_str(), \
                                               destLnkPath.toStdWString().c_str(), NULL);
        if(!bRet)
            continue;
    }

}

void MainWindow::actionRemoveTheseItemsClicked()
{
    QModelIndexList lstIndices = pDms_docItemViewer->getSelectedIndexes();
    if(lstIndices.count() <= 0)
        return;

    int row;
    QSet<int> setRows;
    for(auto index: lstIndices)
    {
        row = index.row();
        if(row >= m_tmpLstOfDocItemInfo.count())
            continue;
        setRows.insert(row);
    }

    for(auto row : setRows)
    {
        pDms_docItemViewer->removeDocBriefItem(m_tmpLstOfDocItemInfo[row]);
        m_tmpLstOfDocItemInfo.removeAt(row);
        m_bSaveOrUpdatelabelInfo = true;
    }
}
