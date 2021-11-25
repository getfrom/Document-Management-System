#ifndef VIRTUALWINFUNCS_H
#define VIRTUALWINFUNCS_H

#include <qt_windows.h>
#include <ShlObj.h>
#include <Shellapi.h>

#include <QString>
#include <QList>
#include <QRegExp>

class virtualWinFuncs
{
public:
    virtualWinFuncs();

    static QString getFileTypeBySuffix(QString strSuffix);
    static QString getDefaultOpenCommandBySuffix(QString strSuffix);
    // Quoted from https://blog.csdn.net/weixin_33755557/article/details/94652040,
    // Parameters of 2nd and 4th can be NULL
    static bool createLinkFile(LPCTSTR szStartAppPath, LPCTSTR szAddCmdLine, LPCTSTR szDestLnkPath, LPCTSTR szIconPath);
    static QString bytesToReadableStr(qint64 bytes, QString method = "AbbrEN");
    static qint64 readableSizeToBytes(QString strBytes, bool *bSucceeded);
    static bool parseCmdLine(QString strCmdLine, QString &strCmdExe, QList<QString> &lstCmdArgs);
};

#endif // VIRTUALWINFUNCS_H

/* 部分函数使用例子
 * 函数virtualWinFuncs::createLinkFile(...)使用例程
 * [注意]: 在参数szDestLnkPath的末尾需在原文档后缀的基础上添加快捷方式的后缀(.lnk)
    QString startAppPath = u8"E:\\NORMAL\\福昕PDF\\FOXIT READER PLUS\\FOXITREADERPLUS.EXE";
    QString cmdFilePath = u8"\"G:\\全国计算机等级考试（NCRE）成绩查询 - 中国教育考试网.pdf\"";
    QString dstFilePath = u8"F:\\test\\全国计算机等级考试（NCRE）成绩查询 - 中国教育考试网.pdf.lnk";
    bool bRet = virtualWinFuncs::createLinkFile(startAppPath.toStdWString().c_str(), \
                                                cmdFilePath.toStdWString().c_str(), \
                                                dstFilePath.toStdWString().c_str(), NULL);
*/
