#ifndef DMS_MANAGELABELDIALOG_H
#define DMS_MANAGELABELDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QList>


// here need more informations to infor the mainwindow to modify labels;

class DMS_ManageLabelDialog : public QDialog
{
    Q_OBJECT
public:
    DMS_ManageLabelDialog();
    ~DMS_ManageLabelDialog();

    QVBoxLayout *rootLayout;
    QHBoxLayout *infoLayout;
    QHBoxLayout *mainLayout;
    QVBoxLayout *btnLayout;
    QLabel      *lblQuoteInfo;
    QCheckBox   *ckbEditQuote;
    QListWidget *lstLblsWdg;
    QPushButton *btnAdd;
    QPushButton *btnModify;
    QPushButton *btnDelete;

    void setExistLabels(QList<QString> lstLbls, QList<unsigned short> lstQuoteCnt);

private:
    void setCurrentLabelEditEnable(int curRow, bool bEnable);

    QList<QString> m_lstOfLabels;
    QList<unsigned short> m_lstOfLabelQuoteCnt;
    bool m_bLabelAddNew;
    bool m_bQuoteLabelEditable;
    QString m_lastLabelText;
    int m_lastRow;

    QString m_quoteCntInfoPre;
    QString m_quoteCntInfoSuf;

signals:
    void addNewLabel(QString strLbl);
    void modifyLabel(QString strRawLbl, QString StrNewLbl);
    void deleteLabel(QString strLbl);

public slots:

private slots:
    void btnAddClicked();
    void btnModifyClicked();
    void btnDeleteClicked();

    void lstLblWidgetItemRowChanged(int row);
    void lstLblWidgetItemChanged(QListWidgetItem *itemTextChanged);

    void ckbStateChanged(int iState);
};

#endif // DMS_MANAGELABELDIALOG_H
