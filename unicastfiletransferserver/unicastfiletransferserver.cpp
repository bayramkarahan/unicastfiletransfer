#include "unicastfiletransferserver.h"
#include <QDebug>

FileServer::FileServer(QObject *parent)
    : QTcpServer(parent)
{
}

void FileServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    sessions[socket] = new TransferSession();

    connect(socket,&QTcpSocket::readyRead,
            this,&FileServer::readClient);
}

void FileServer::readClient()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    TransferSession *session = sessions[socket];

    if(session->fileSize == 0)
    {
        QByteArray line = socket->readLine();
        if(line.isEmpty())
            return;

        QString header(line);
        QStringList p = header.split("|");

        if(p.size() < 11)
        {
            qDebug() << "HEADER ERROR";
            return;
        }

        session->transferId = p[1];
        session->sender     = p[2];
        session->receiver   = p[3];

        QString savePath = p[4];
        session->filename = p[5];
        session->fileSize = p[6].trimmed().toLongLong();

        bool overwrite = p[7].trimmed() == "1";

        session->altFilename = p[8];
        session->purpose     = p[9];
        session->username    = p[10].trimmed();

        QDir().mkpath(savePath);

        QString fullPath = savePath + "/" + session->filename;

        if(!overwrite)
        {
            QFileInfo fi(fullPath);

            QString base = fi.completeBaseName();
            QString ext  = fi.suffix();

            int counter = 1;

            while(QFile::exists(fullPath))
            {
                if(ext.isEmpty())
                {
                    fullPath = QString("%1/%2(%3)")
                            .arg(savePath)
                            .arg(base)
                            .arg(counter++);
                }
                else
                {
                    fullPath = QString("%1/%2(%3).%4")
                            .arg(savePath)
                            .arg(base)
                            .arg(counter++)
                            .arg(ext);
                }
            }
        }

        session->file.setFileName(fullPath);

        if(!session->file.open(QIODevice::WriteOnly))
        {
            qDebug() << "FILE OPEN ERROR";
            return;
        }

        session->received = 0;

        qDebug() << "HEADER OK"
                 << session->sender
                 << "->"
                 << session->receiver
                 << fullPath;
    }

    QByteArray data = socket->readAll();

    if(!data.isEmpty())
    {
        session->file.write(data);
        session->received += data.size();
    }

    if(session->received >= session->fileSize)
    {
        session->file.close();
        emit transferCompleted(
            session->sender,
            session->receiver,
            session->filename,
            session->altFilename,
            session->purpose,
            session->username);

        QString ack =
        QString("DONE|%1\n").arg(session->transferId);

        socket->write(ack.toUtf8());

        qDebug() << "Transfer finished:"
                 << session->filename
                 << "from"
                 << session->sender;
    }
}
