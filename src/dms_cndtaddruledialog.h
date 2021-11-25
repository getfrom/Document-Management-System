#ifndef DMS_CNDTADDDIRDIALOG_H
#define DMS_CNDTADDDIRDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QList>
#include <QString>

#include "dms_struct.h"

enum DMS_CndtRuleAppendPolicy { DMSRuleAppend_OnlyOr = 0x0001, DMSRuleAppend_OnlyAnd = 0x0002, DMSRuleAppend_All = 0x0002};

class DMS_CndtAddRuleDialog : public QDialog
{
    Q_OBJECT
public:

    QVBoxLayout *m_pRootLayout;

    QHBoxLayout *m_pRow1Layout;
    QLabel *m_pLblName;
    QLineEdit *m_pTxtName;

    QHBoxLayout *m_pRow2Layout;
    QLabel *m_pLblType;
    QComboBox *m_pCbbType;

    QHBoxLayout *m_pRow3Layout;
    QLabel *m_pLblMethod;
    QComboBox *m_pCbbMethod;

    QHBoxLayout *m_pRow4Layout;
    QLabel *m_pLblExtV;
    QLineEdit *m_pTxtExtV;

    QHBoxLayout *m_pRow5Layout;
    QLabel *m_pLblAddMethod;
    QComboBox *m_pCbbAddMethod;

    QHBoxLayout *m_pDetermineLayout;
    QPushButton *m_pBtnConfirm;
    QPushButton *m_pBtnCancel;

    DMS_CndtAddRuleDialog();
    ~DMS_CndtAddRuleDialog();

    void createView();
    void setExistRuleName(QList<QString> lstName);
    int setCurrentRuleInfo(const DMS_CndtAddItem& item);
    void setCurrentAppendRulePolicy(DMS_CndtRuleAppendPolicy curPolicy = DMSRuleAppend_All) \
                                        { m_curRuleAppendPolicy = curPolicy; }
    void setCurrentRuleTypeMethodInfo(QList<DMS_CndtStringMatchRuleMethod> *pStrOfRuleMethod, \
                                      QList<DMS_CndtStringMatchRuleType> *pStrOfRuleType) \
                                      { m_pStrOfRuleMethod = pStrOfRuleMethod; \
                                        m_pStrOfRuleType = pStrOfRuleType; }
    void setCurrentRuleCombineMode(DMS_CndtRuleCombineMode combMode) { m_curRuleCombMode = combMode; }
    void applyToView() { this->applyStrOfView(); }

    DMS_CndtAddItem getCurrentRuleInfo();
    DMS_CndtRuleCombineMode getCurrentRuleCombMode();

protected:

    void applyStrOfView();

    QList<QString> m_lstOfExistNames;
    DMS_CndtAddItem m_curCndtItem;
    DMS_CndtRuleCombineMode m_curRuleCombMode;

    QString m_lastTxtName;
    QString m_lastTxtExtV;
    QString m_lastTxtAddLabel;

    DMS_CndtRuleAppendPolicy m_curRuleAppendPolicy;
    QList<DMS_CndtStringMatchRuleMethod> *m_pStrOfRuleMethod;
    QList<DMS_CndtStringMatchRuleType> *m_pStrOfRuleType;

private slots:

    void btnConfirmClicked();
    void btnCancelClicked();

    void txtNameChangeFinished();
    void txtExtVChangeFinished();

    void cbbRuleTypeIndexChanged(int index);
};

#endif // DMS_CNDTADDDIRDIALOG_H
