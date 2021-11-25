#include "virtualwinfuncs.h"

virtualWinFuncs::virtualWinFuncs()
{

}

QString virtualWinFuncs::getFileTypeBySuffix(QString strSuffix)
{
    QString retStr("");
    HKEY hClassRootKey = NULL;
    DWORD retCode;
    DWORD dwRegVType;
    TCHAR pBaseSuffixTypeName[_MAX_PATH];
    TCHAR pRegData[_MAX_PATH];
    DWORD dwRegDataLength;

    QString subKeyOfSuffix("");
    if(strSuffix[0] == ".")
        subKeyOfSuffix = strSuffix;
    else
        subKeyOfSuffix = "." + strSuffix;

    retCode = ::RegOpenKeyExW(HKEY_CLASSES_ROOT, \
                              TEXT(""), \
                              0, KEY_READ, &hClassRootKey);
    if(retCode == ERROR_SUCCESS)
    {
        dwRegDataLength = _MAX_PATH;
        pBaseSuffixTypeName[0] = 0;
        retCode = ::RegGetValueW(hClassRootKey, \
                                 subKeyOfSuffix.toStdWString().c_str(), \
                                 TEXT(""), RRF_RT_REG_SZ, &dwRegVType, \
                                 pBaseSuffixTypeName, &dwRegDataLength);
        if(retCode == ERROR_SUCCESS)
        {
            // found the effective File Type in Register;
            dwRegDataLength = _MAX_PATH;
            pRegData[0] = 0;
            retCode = ::RegGetValueW(hClassRootKey, \
                                     pBaseSuffixTypeName, \
                                     TEXT(""), RRF_RT_REG_SZ, &dwRegVType, \
                                     pRegData, &dwRegDataLength);
            if(retCode == ERROR_SUCCESS)
            {
                retStr = QString::fromWCharArray(pRegData);
            }
        }
    }

    if(hClassRootKey != NULL)
    {
        RegCloseKey(hClassRootKey);
        hClassRootKey = NULL;
    }

    return retStr;
}

QString virtualWinFuncs::getDefaultOpenCommandBySuffix(QString strSuffix)
{
    QString retStr("");
    HKEY hClassRootKey = NULL;
    DWORD retCode;
    DWORD dwRegVType;
    TCHAR pBaseSuffixTypeName[_MAX_PATH];
    TCHAR pOpenCmd[_MAX_PATH];
    DWORD dwRegDataLength;

    QString subKeyOfSuffix("");
    if(strSuffix[0] == ".")
        subKeyOfSuffix = strSuffix;
    else
        subKeyOfSuffix = "." + strSuffix;

    retCode = ::RegOpenKeyExW(HKEY_CLASSES_ROOT, \
                              TEXT(""), \
                              0, KEY_READ, &hClassRootKey);
    if(retCode == ERROR_SUCCESS)
    {
        dwRegDataLength = _MAX_PATH;
        pBaseSuffixTypeName[0] = 0;
        retCode = ::RegGetValueW(hClassRootKey, \
                                 subKeyOfSuffix.toStdWString().c_str(), \
                                 TEXT(""), RRF_RT_REG_SZ, &dwRegVType, \
                                 pBaseSuffixTypeName, &dwRegDataLength);
        if(retCode == ERROR_SUCCESS)
        {
            dwRegDataLength = _MAX_PATH;
            pOpenCmd[0] = 0;
            QString strSubKeyOfOpenCmd = QString::fromWCharArray(pBaseSuffixTypeName);
            strSubKeyOfOpenCmd += "\\shell\\open\\command\\";
            retCode = ::RegGetValueW(hClassRootKey, \
                                     strSubKeyOfOpenCmd.toStdWString().c_str(), \
                                     TEXT(""), RRF_RT_REG_SZ, &dwRegVType, \
                                     pOpenCmd, &dwRegDataLength);
            if(retCode == ERROR_SUCCESS)
            {
                retStr = QString::fromWCharArray(pOpenCmd);
            }
        }
    }

    if(hClassRootKey != NULL)
    {
        RegCloseKey(hClassRootKey);
        hClassRootKey = NULL;
    }

    return retStr;
}

bool virtualWinFuncs::createLinkFile(LPCTSTR szStartAppPath, LPCTSTR szAddCmdLine, LPCTSTR szDestLnkPath, LPCTSTR szIconPath)
{
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        IShellLink *pShellLink;
        hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
        if (SUCCEEDED(hr))
        {
            pShellLink->SetPath(szStartAppPath);
            std::wstring strTmp = szStartAppPath;
            int nStart = strTmp.find_last_of(LPTSTR("/\\"));
            pShellLink->SetWorkingDirectory(strTmp.substr(0, nStart).c_str());
            pShellLink->SetArguments(szAddCmdLine);
            if (szIconPath)
            {
                pShellLink->SetIconLocation(szIconPath, 0);
            }
            IPersistFile* pPersistFile;
            hr = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
            if (SUCCEEDED(hr))
            {
                hr = pPersistFile->Save(szDestLnkPath, FALSE);
                if (SUCCEEDED(hr))
                {
                    return true;
                }
                pPersistFile->Release();
            }
            pShellLink->Release();
        }
        CoUninitialize();
    }
    return false;
}

QString virtualWinFuncs::bytesToReadableStr(qint64 bytes, QString method)
{
    QList<QString> lstUnitsEN;
    lstUnitsEN.append("Byte"); lstUnitsEN.append("Kilobyte"); lstUnitsEN.append("Megabyte");
    lstUnitsEN.append("Gigabyte"); lstUnitsEN.append("Trillionbyte"); lstUnitsEN.append("Petabyte");
    lstUnitsEN.append("Exabyte"); lstUnitsEN.append("Zettabyte"); lstUnitsEN.append("Yottabyte");
    lstUnitsEN.append("Brontobyte");

    QList<QString> lstUnitsAbbrEN;
    lstUnitsAbbrEN.append("Byte"); lstUnitsAbbrEN.append("KB"); lstUnitsAbbrEN.append("MB");
    lstUnitsAbbrEN.append("GB"); lstUnitsAbbrEN.append("TB"); lstUnitsAbbrEN.append("PB");
    lstUnitsAbbrEN.append("EB"); lstUnitsAbbrEN.append("ZB"); lstUnitsAbbrEN.append("YB");
    lstUnitsAbbrEN.append("BB");

    QList<QString> lstUnitsCN;
    lstUnitsCN.append("字节"); lstUnitsCN.append("千字节"); lstUnitsCN.append("兆字节");
    lstUnitsCN.append("吉字节"); lstUnitsCN.append("太字节"); lstUnitsCN.append("拍字节");
    lstUnitsCN.append("艾字节"); lstUnitsCN.append("泽字节"); lstUnitsCN.append("尧字节");
    lstUnitsCN.append("千亿亿亿字节");

    QString retStr = "";
    if(method == "EN")
    {

    }
    else if(method == "CN")
    {

    }
    else    // default AbbrEN
    {
        qint64 dat = bytes, last;
        int base = 1024;
        int power = 0;

        if(dat >= base)
        {
            while(dat / base != 0)
            {
                last = dat;
                dat = dat / base;
                power++;
            }

            retStr = QString::number((double)last / double(base), 'f', 1) + lstUnitsAbbrEN[power];
        }
        else
            retStr = QString::number(double(dat), 'f', 1) + lstUnitsAbbrEN[0];
    }

    return retStr;
}

qint64 virtualWinFuncs::readableSizeToBytes(QString strBytes, bool *bSucceeded)
{
    *bSucceeded = false;
    QRegExp reSize(u8"^(?:\\s*)(\\d*\\.*\\d+)(?:\\s*)([A-Za-z\\x4e00-\\x9fa5]+)(?:\\s*)$");
    reSize.indexIn(strBytes);
    QStringList lstReStr = reSize.capturedTexts();
    if(lstReStr.count() > 3)
    {
        *bSucceeded = false;
        return -1;
    }

    QString numItem = lstReStr[1];
    double retV = numItem.toDouble(bSucceeded);
    if(!*bSucceeded)
        return -1;

    if(lstReStr.count() == 2)
        return (qint64)retV;

    QString unitItem = lstReStr[2];

    QList<QString> lstUnitsEN;
    lstUnitsEN.append("Byte"); lstUnitsEN.append("Kilobyte"); lstUnitsEN.append("Megabyte");
    lstUnitsEN.append("Gigabyte"); lstUnitsEN.append("Trillionbyte"); lstUnitsEN.append("Petabyte");
    lstUnitsEN.append("Exabyte"); lstUnitsEN.append("Zettabyte"); lstUnitsEN.append("Yottabyte");
    lstUnitsEN.append("Brontobyte");
    QList<QString> lstUnitsAbbrEN;
    lstUnitsAbbrEN.append("Byte"); lstUnitsAbbrEN.append("KB"); lstUnitsAbbrEN.append("MB");
    lstUnitsAbbrEN.append("GB"); lstUnitsAbbrEN.append("TB"); lstUnitsAbbrEN.append("PB");
    lstUnitsAbbrEN.append("EB"); lstUnitsAbbrEN.append("ZB"); lstUnitsAbbrEN.append("YB");
    lstUnitsAbbrEN.append("BB");
    QList<QString> lstUnitsCN;
    lstUnitsCN.append("字节"); lstUnitsCN.append("千字节"); lstUnitsCN.append("兆字节");
    lstUnitsCN.append("吉字节"); lstUnitsCN.append("太字节"); lstUnitsCN.append("拍字节");
    lstUnitsCN.append("艾字节"); lstUnitsCN.append("泽字节"); lstUnitsCN.append("尧字节");
    lstUnitsCN.append("千亿亿亿字节");

    bool bFounded = false;
    int i;
    if(!bFounded)
    {
        for(i = 0; i < lstUnitsEN.count(); i++)
        {
            if(unitItem.toLower().contains(lstUnitsEN[i].toLower()))
            {
                bFounded = true;
                retV = retV * (1 << (10 * i));
                break;
            }
        }
    }

    if(!bFounded)
    {
        for(i = 0; i < lstUnitsAbbrEN.count(); i++)
        {
            if(unitItem.toLower().contains(lstUnitsAbbrEN[i].toLower()))
            {
                bFounded = true;
                retV = retV * (1 << (10 * i));
                break;
            }
        }
    }

    if(!bFounded)
    {
        for(i = 0; i < lstUnitsCN.count(); i++)
        {
            if(unitItem.toLower().contains(lstUnitsCN[i].toLower()))
            {
                bFounded = true;
                retV = retV * (1 << (10 * i));
                break;
            }
        }
    }

    return retV;
}


bool virtualWinFuncs::parseCmdLine(QString strCmdLine, QString &strCmdExe, QList<QString> &lstCmdArgs)
{
    strCmdLine = strCmdLine.trimmed();
    if(!strCmdLine.contains(".exe", Qt::CaseInsensitive))
        return false;

    QStringList strLst;
    QStringList strLstArgs;
    QString strArg;
    QString strTmp;
    if(strCmdLine[0] != '"')
    {

        strLst = strCmdLine.split(".exe", Qt::KeepEmptyParts, Qt::CaseInsensitive);
        strCmdExe = strLst[0] + ".exe";
        if(strLst.count() <= 1)
        {
            lstCmdArgs.clear();
            return true;
        }

        lstCmdArgs.clear();
        strArg = strLst[1];
        strLstArgs = strArg.split(' ');
        for(auto strTmp: strLstArgs)
        {
            if(strTmp.isEmpty())
                continue;
            lstCmdArgs.append(strTmp);
        }
    }
    else
    {
        strLst = strCmdLine.split('"');
        strCmdExe = strLst[1];

        if(strLst.count() <= 2)
        {
            lstCmdArgs.clear();
            return true;
        }

        lstCmdArgs.clear();
        for(int i = 2; i < strLst.count(); i++)
        {
            strTmp = strLst[i].trimmed();
            if(strTmp.isEmpty())
                continue;
            else
                lstCmdArgs.append(strTmp);
        }
    }

    return true;
}
