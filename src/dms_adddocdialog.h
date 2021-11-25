#ifndef DMS_ADDDOCDIALOG_H
#define DMS_ADDDOCDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QItemSelection>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QTreeWidget>

#include "dms_struct.h"
#include "dms_showdocinfodialog.h"
#include "dms_cndtaddruledialog.h"

class DMS_AddDocDialog : public QDialog
{
    Q_OBJECT
public:

    QVBoxLayout *m_pRootLayout;
    QHBoxLayout *m_pBaseInfoLayout;
    QListWidget *m_pFileLstWdg;
    QVBoxLayout *m_pBaseOperateLayout;
    QPushButton *m_pBtnBaseNew;
    QPushButton *m_pBtnBaseModify;
    QPushButton *m_pBtnBaseDelete;
    QPushButton *m_pBtnBaseMoveUp;
    QPushButton *m_pBtnBaseMoveDown;


    QGroupBox *m_pGrpBoxOfCndtAdd;
    QVBoxLayout *m_pCndtAddRootLayout;
    QCheckBox *m_pCkbEnableCndtAdd;
    QTreeWidget *m_pCndtTreWdg;
    QHBoxLayout *m_pCndtOperateLayoutDir;
    QPushButton *m_pBtnCndtAddDir;
    QPushButton *m_pBtnCndtModifyDir;
    QPushButton *m_pBtnCndtDeleteDir;
    QHBoxLayout *m_pCndtOperateLayoutRule;
    QPushButton *m_pBtnCndtAddRule;
    QPushButton *m_pBtnCndtModifyRule;
    QPushButton *m_pBtnCndtDeleteRule;
    QPushButton *m_pCndtAddByRules;

    QHBoxLayout *m_pFinalOperateLayout;
    QLabel *m_pLblOfSelectInfo;
    QPushButton *m_pBtnAddConfirm;
    QPushButton *m_pBtnCancelAdd;

    QList<DMS_CndtStringMatchRuleMethod> m_lstOfStrMatchRuleMethods;
    QList<DMS_CndtStringMatchRuleType> m_lstOfStrMatchRuleType;

    DMS_AddDocDialog();
    ~DMS_AddDocDialog();

    void createMainView();
    void loadStrMatchText();

    void setPointerOfLabelMatchInfo(QList<DMS_LabelMatchItem> *pLst) { m_pLstOfLabelMatchInfo = pLst; }
    void setPointerOfGlobalLabelMaxIndex(int *p) { m_pGlobalLabelMaxIndex = p; }

    int getAddDocBriefItems(QList<DMS_DocBriefItem> &lstOutput);

private:
    void setupTabOfAddOnes();
    void setupTabOfAddMulti();

    void addDocItemToCache(DMS_DocBriefItem &docItem);
    void modifyDocItemToCache(DMS_DocBriefItem &docRawItem, DMS_DocBriefItem &docNewItem);

    bool judgeFileSatisfiedRule(const QFileInfo &curFileInfo, const DMS_CndtRule &curRule);

protected:
    QList<DMS_DocBriefItem> m_lstOfDocItems;
    QList<DMS_LabelMatchItem> *m_pLstOfLabelMatchInfo;
    int *m_pGlobalLabelMaxIndex;
    bool m_bEnableCndtAdd;
    QList<DMS_CndtAddItem> m_lstOfCndtAddInfo;
    QList<int> m_indexLstOfCndtAddDirBase;
signals:
    void addNewLabel(QString strLbl);
    void modifyLabel(QString strRawLbl, QString StrNewLbl);
    void deleteLabel(QString strLbl);

    void addDocBriefItem(QList<DMS_DocBriefItem>);

public slots:
    void labelInfoAddNew(QString strLblName);
    void labelInfoModify(QString strRawLblName, QString strNewLblName);
    void labelInfoDelete(QString strLblName);

private slots:
    void btnBaseNewClicked();
    void btnBaseModifyClicked();
    void btnBaseDeleteClicked();
    void btnBaseMoveUpClicked();
    void btnBaseMoveDownClicked();

    void btnCndtAddDirClicked();
    void btnCndtModifyDirClicked();
    void btnCndtDeleteDirClicked();
    void btnCndtAddRuleClicked();
    void btnCndtModifyRuleClicked();
    void btnCndtDeleteRuleClicked();
    void btnCndtAddByRulesClicked();

    void addDocOfAddClicked();
    void addDocOfCancelClicked();

    void ckbCndtAddStateChanged(int iState);


};

#endif // DMS_ADDDOCDIALOG_H
