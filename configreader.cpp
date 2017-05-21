#include "configreader.h"

#include "core.h"
#include "exception.h"
#include "data.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QObject>
#include <QDebug>

/**
 *
 */
QString readFromConfigFile(QFile& file)
{
    return file.readAll();
}

QString readFromSourceFile(QFile& file)
{
    return QString();
}

bool interpretCode(const QString& code)
{
    QStringList pCode=preprocessCode(code);
    int line=1;
    for(const QString& i: pCode)
    {
        QStringList x=i.split(' ');
        if (x.size()<2) throw Exception(QObject::tr("Unexpected end.")).setLine(line);
        QString variableName=x[0];
        if (!isRightVariableName(variableName)) throw Exception(QObject::tr("Unlawful variable name")).setLine(line);
        variableName = variableName.toLower();
        QString op=x[1];
        if (op=="=")
        {
            QVariantList valueList;
            for(int i=2; i<x.size(); ++i)
            {
                valueList.push_back(x[i]);
            }
            Data::setVariable(variableName,valueList);
        }
        else if (op=="+=")
        {
            //
        }
        else if (op=="-=")
        {
            //
        }
        else throw Exception(QObject::tr("Unknow operator %1").arg(op));
        ++line;
    }
}

QString deleteSpace(const QString& code)
{
    QString formattedCode;
    //auto it=code.begin();
    //while(it!=code.end() && (*it==' ' || *it=='\t')) ++it;
    bool isSpace=false;
    for(auto it:code)
    {
        if (isSpace)
        {
            if (it==' ' || it=='\t') continue;
            formattedCode.append(' ');
            if (!formattedCode.isEmpty()) formattedCode.append(it);
            isSpace=false;
        }
        else
        {
            if (it==' ' || it=='\t') isSpace=true;
            else formattedCode.append(it);
        }
    }
    return formattedCode;
}

QStringList preprocessCode(const QString &code)
{
    QStringList preprocessedCode;
    QTextStream ts(code.toLatin1());
    //ts<<code;
    while(!ts.atEnd())
    {
        QString line=ts.readLine();
        int pos=line.indexOf('#');
        if (pos!=-1)
        {
            line=line.mid(0,pos);
        }
        line=deleteSpace(line);
        //if (!line.isEmpty() && line[line.size()-1]=='\\')
        if (!line.isEmpty()) preprocessedCode.push_back(line);
    }
    return preprocessedCode;
}

bool isRightVariableName(const QString &code)
{
    return true;
}
