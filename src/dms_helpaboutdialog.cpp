#include "dms_helpaboutdialog.h"

DMS_HelpAboutDialog::DMS_HelpAboutDialog()
{

}

void DMS_HelpAboutDialog::createView()
{
    m_pRootLayout = new QHBoxLayout();

    m_pInfoLayout = new QVBoxLayout();
    m_pLbl_1 = new QLabel("https://github.com/getfrom");
    m_pLbl_2 = new QLabel("Document Management System(DMS ver 1.0)");
    m_pLbl_3 = new QLabel(u8"基于 Qt Creator 4.11.1");
    m_pInfoLayout->addWidget(m_pLbl_1);
    m_pInfoLayout->addWidget(m_pLbl_2);
    m_pInfoLayout->addWidget(m_pLbl_3);
    m_pBtnOk = new QPushButton(u8"确定");
    m_pInfoLayout->addWidget(m_pBtnOk);
    m_pRootLayout->addLayout(m_pInfoLayout);

    this->setLayout(m_pRootLayout);
    this->setWindowTitle(u8"关于 DMS");

    connect(m_pBtnOk, SIGNAL(clicked()), this, SLOT(accept()));
}

DMS_HelpAboutDialog::~DMS_HelpAboutDialog()
{
    delete m_pBtnOk;
    delete m_pLbl_3;
    delete m_pLbl_2;
    delete m_pLbl_1;
    delete m_pInfoLayout;
    delete m_pRootLayout;
}
