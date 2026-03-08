#ifndef FILESERVER_H
#define FILESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include "transfersession.h"
#include<QDir>
class FileServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit FileServer(QObject *parent = nullptr);

signals:
    void transferCompleted(QString sender,
                           QString receiver,
                           QString filename,
                           QString altFilename,
                           QString purpose,
                           QString username);
protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void readClient();

private:
    QHash<QTcpSocket*, TransferSession*> sessions;
};

#endif
