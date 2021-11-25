#ifndef DMS_DOCBRIEFWIDGET_H
#define DMS_DOCBRIEFWIDGET_H

#include <QStandardItemModel>
#include <QTableView>
#include <QList>
#include <QVector>
#include <QMenu>

#include "dms_struct.h"


class DMS_DocBriefWidget : public QTableView
{
    Q_OBJECT
public:
    DMS_DocBriefWidget(QWidget *parent = 0);
    ~DMS_DocBriefWidget();

    int loadDocBriefData(void);

    void setPointerOfLabelMatchInfo(QList<DMS_LabelMatchItem> *pLst) { m_pLstOfLabelMatchInfo = pLst; }
    void appendDocBriefItem(const DMS_DocBriefItem &item); // default to represent
    void modifyDocBriefItem(int row, const DMS_DocBriefItem &item);
    void removeDocBriefItem(const DMS_DocBriefItem &item);
    void applyDocBriefItemsToShow();
    void showRows(int iStart, int iEnd); // index of m_vec...
    void showRowsByLabelIndex(unsigned short labelIndex);
    void showRowsByLabelIndices(QList<unsigned short> labelIndices);
    void hideRows(int iStart, int iEnd);
    void hideRowsByLabelIndex(unsigned short labelIndex);
    void hideRowsByLabelIndices(QList<unsigned short> labelIndices);
    void sortRowsByLabelIndex(unsigned short labelIndex);
    void sortRowsByLabelIndicesWithMatchingDegree(QList<unsigned short> labelIndices);

    // continue from here

    QModelIndexList getSelectedIndexes() { return this->selectedIndexes(); }
    DMS_DocBriefItem getDocItemByRowIndex(int row, bool *bSucceed);

protected:

    QList<DMS_LabelMatchItem> *m_pLstOfLabelMatchInfo;

    QVector<DMS_DocBriefItem> m_vecOfLoadedDocBriefItems;
    QStandardItemModel *m_curDataModel;

    QSet<int> m_setOfShowIndexFromDocBriefItem;

    int m_iStartIndexOfAppendDocBriefItem;

    QList<QString> m_lstOfInfoItems;

public slots:

};

#endif // DMS_DOCBRIEFWIDGET_H
