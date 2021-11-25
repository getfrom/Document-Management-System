#ifndef DMS_SHOWDOCINFODIALOG_H
#define DMS_SHOWDOCINFODIALOG_H

#include <QList>
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

#include <QFileInfo>

#include "dms_struct.h"
#include "virtualwinfuncs.h"
#include "dms_selectlabeldialog.h"

class DMS_ShowDocInfoDialog : public QDialog
{
    Q_OBJECT
public:
    QVBoxLayout *rootLayout;
    QHBoxLayout *row1Layout;
    QLabel *lblName;
    QLineEdit *txtName;
    QHBoxLayout *row2Layout;
    QLabel *lblPath;
    QLineEdit *txtPath;
    QHBoxLayout *row3Layout;
    QLabel *lblType;
    QLineEdit *txtType;
    QHBoxLayout *row4Layout;
    QLabel *lblSize;
    QLineEdit *txtSize;
    QHBoxLayout *row5Layout;
    QVBoxLayout *labelLayout;
    QLabel *lblLabel;
    QListWidget *lblLst;
    QVBoxLayout *labelOperateLayout;
    QPushButton *btnLabelAddExist;
    QPushButton *btnLabelAddNew;
    QPushButton *btnLabelDelete;
    QVBoxLayout *briefLayout;
    QLabel *lblBrief;
    QTextEdit *txtBrief;
    QHBoxLayout *row6Layout;
    QHBoxLayout *row62Layout;
    QPushButton *btnBrowseFile;
    QPushButton *btnConfirm;
    QPushButton *btnCancel;

    uint32_t m_lblCnt;

    DMS_ShowDocInfoDialog(DMS_DocBriefItem &curItemInfo, QWidget *parent = 0);
    ~DMS_ShowDocInfoDialog();
    void setPointerOfLabelMatchInfo(QList<DMS_LabelMatchItem> *pLst) { m_pLstOfLabelMatchInfo = pLst; }
    void setPointerOfGlobalLabelMaxIndex(int *p) { m_pGlobalLabelMaxIndex = p; }
    DMS_DocBriefItem getCurrentDocInfo();
    void setCurrentDocInfo(DMS_DocBriefItem &docItem);
    QList<QString> getAddNewLabelName();

private:
    void setCurrentLabelEditEnable(int curRow, bool bEnable);

protected:
    int *m_pGlobalLabelMaxIndex;
    QList<DMS_LabelMatchItem> *m_pLstOfLabelMatchInfo;
    QList<QString> m_selectedLabelName;
    QList<QString> m_addNewLabelName;
    QString m_lastSelectedLabelName;
    int m_lastRow;
    bool m_bLabelAddNew;

signals:
    void addNewLabel(QString strLbl);
    void modifyLabel(QString strRawLbl, QString StrNewLbl);
    void deleteLabel(QString strLbl);

private slots:
    void addExistLabelClicked();
    void addNewLabelClicked();
    void deleteLabelClicked();
    void browserFileClicked();

    void lstLblWidgetItemRowChanged(int row);
    void lstLblWidgetItemChanged(QListWidgetItem *itemTextChanged);

    void addMoreLabel(QList<QString> lstLbls);

};

#endif // DMS_SHOWDOCINFODIALOG_H
