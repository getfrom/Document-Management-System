#ifndef DMS_SELECTLABELDIALOG_H
#define DMS_SELECTLABELDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QList>
#include "dms_struct.h"

class DMS_SelectLabelDialog : public QDialog
{
    Q_OBJECT
public:
    DMS_SelectLabelDialog();
    ~DMS_SelectLabelDialog();

    void setExistLabels(QList<QString> lstLbls);

    QVBoxLayout *baseLayout;
    QHBoxLayout *btnLayout;
    QListWidget *lstLblsWdg;
    QPushButton *btnAdd;
    QPushButton *btnCancel;

    void setPointerOfLabelMatchInfo(QList<DMS_LabelMatchItem> *pLst) { m_pLstOfLabelMatchInfo = pLst; }
protected:
    QList<DMS_LabelMatchItem> *m_pLstOfLabelMatchInfo;

signals:
    void returnSelectedLabels(QList<QString>);

private slots:
    void addClicked();
    void cancalClicked();
};

#endif // DMS_SELECTLABELDIALOG_H
