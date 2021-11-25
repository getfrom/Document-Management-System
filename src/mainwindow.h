#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>

#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QTextStream>
#include <QStringList>

#include "dms_struct.h"
#include "dms_horizontaldragwidget.h"
#include "dms_docbriefwidget.h"
#include "dms_adddocdialog.h"
#include "dms_managelabeldialog.h"
#include "dms_helpaboutdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString getLabelInfoPath();
    QString getDocItemsInfoPath();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createView();
    void createMenu();
    void createMenuOfRButtonClick();
    void applyCurrentSources();

    void loadLabelInfoDefault();
    void restoreLabelInfoDefault();
    void createDefaultLabelInfo(QString pathOfLabelInfo);

    void loadDocItemsInfoDefault();
    void restoreDocItemsInfoDefault();
    void createDefaultDocItemsInfo(QString pathOfDocItemsInfo);

    int addOrQuoteLabel(QString lblName);
    int quoteLabel(unsigned short lblIndex);
    int unQuoteLabel(QString lblName);
    int deleteUnQuoteLabel(QString lblName);

public:
    QWidget *pRootWidget;
    QGridLayout *pLabelLayout;
    QScrollArea *pLabelScrollArea;
    DMS_HorizontalDragWidget *pDms_labelWidget;
    DMS_DocBriefWidget *pDms_docItemViewer;

    int m_globalLabelMaxIndex;
    QList<DMS_LabelMatchItem> m_lstOflabelInfo;
    QList<DMS_LabelMatchItem> m_tmpLstOfLabelInfo;
    QList<DMS_DocBriefItem> m_lstOfDocItemInfo;
    QList<DMS_DocBriefItem> m_tmpLstOfDocItemInfo;

    QMenuBar *pMenuBar;
    QMenu *pMenuFile;
    QMenu *pMenuTool;
    QMenu *pMenuHelp;

    QAction *pMenuFileSave;
    QAction *pMenuFileClose;
    QAction *pMenuToolAddDoc;
    QAction *pMenuToolManageLabel;
    QAction *pMenuHelpAbout;

    QMenu *pMenuOfRBClickOfItems;
    QAction *pActionModifyThisItem;
    QAction *pActionOpenFilePos;
    QAction *pActionOpenFile;
    QAction *pActionSaveShortcutAs;
    QAction *pActionRemoveTheseItems;

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    bool m_bSaveOrUpdatelabelInfo;
    QString m_docNameOfLabelInfo;
    QString m_docNameOfDocItemsInfo;

public slots:
    void showMenuOfDocItem(QPoint pos);

    void menuFileSaveClicked();
    void menuFileCloseClicked();
    void menuToolAddDocClicked();
    void menuToolManageLabelClicked();
    void menuHelpAboutClicked();

    void labelStateChanged(DMS_DragWidgetLabelChangeState lblState, QString lblName);
    void labelInfoAddNew(QString strLblName);
    void labelInfoModify(QString strRawLblName, QString strNewLblName);
    void labelInfoDelete(QString strLblName);

    void docBriefItemAdd(QList<DMS_DocBriefItem> lstDocItems);

    void actionModifyThisItemClicked();
    void actionOpenFileClicked();
    void actionOpenFilePosClicked();
    void actionSaveShortcutAsClicked();
    void actionRemoveTheseItemsClicked();
};
#endif // MAINWINDOW_H
