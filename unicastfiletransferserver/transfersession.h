#ifndef TRANSFERSESSION_H
#define TRANSFERSESSION_H

#include <QTcpSocket>
#include <QFile>

struct TransferSession
{
    QString transferId;
    QString sender;
    QString receiver;

    QString filename;
    QString altFilename;
    QString purpose;
    QString username;

    qint64 fileSize = 0;
    qint64 received = 0;

    QFile file;
};

#endif
