#ifndef DMS_HELPABOUTDIALOG_H
#define DMS_HELPABOUTDIALOG_H

#include <QString>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>


class DMS_HelpAboutDialog : public QDialog
{
    Q_OBJECT
public:
    DMS_HelpAboutDialog();
    ~DMS_HelpAboutDialog();

    void createView();

    QHBoxLayout *m_pRootLayout;
    QVBoxLayout *m_pInfoLayout;
    QLabel *m_pLbl_1;
    QLabel *m_pLbl_2;
    QLabel *m_pLbl_3;
    QPushButton *m_pBtnOk;



};

#endif // DMS_HELPABOUTDIALOG_H
