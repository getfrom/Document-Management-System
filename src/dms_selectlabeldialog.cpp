#include "dms_selectlabeldialog.h"

DMS_SelectLabelDialog::DMS_SelectLabelDialog()
{
    baseLayout = new QVBoxLayout();
    btnLayout = new QHBoxLayout();

    lstLblsWdg = new QListWidget();
    lstLblsWdg->setSelectionMode(QAbstractItemView::ContiguousSelection);
    baseLayout->addWidget(lstLblsWdg);

    btnAdd = new QPushButton(u8"添加");
    btnCancel = new QPushButton(u8"取消");
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnCancel);
    baseLayout->addLayout(btnLayout);

    this->setLayout(baseLayout);

    connect(btnAdd, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancalClicked()));
}

DMS_SelectLabelDialog::~DMS_SelectLabelDialog()
{
    delete btnCancel;
    delete btnAdd;
    delete lstLblsWdg;
    delete btnLayout;
    delete baseLayout;
}

void DMS_SelectLabelDialog::setExistLabels(QList<QString> lstLbls)
{
    for(int i = 0; i < lstLbls.count(); i++)
    {
        lstLblsWdg->addItem(lstLbls[i]);
    }
}

void DMS_SelectLabelDialog::addClicked()
{
    QList<QListWidgetItem*> selectedLabels = lstLblsWdg->selectedItems();
    QList<QString> lstOfSelected;
    for(int i = 0; i < selectedLabels.count(); i++)
    {
        lstOfSelected.append(selectedLabels[i]->text());
    }
    emit returnSelectedLabels(lstOfSelected);
    this->close();
}

void DMS_SelectLabelDialog::cancalClicked()
{
    this->close();
}

