#include "unicastfiletransferclient.h"
#include <QFileInfo>
#include <QUuid>

FileClient::FileClient(QObject *parent)
    : QObject(parent)
{
    connect(&socket,&QTcpSocket::readyRead,
            this,&FileClient::readServer);

    connect(&socket,&QTcpSocket::connected,
            this,&FileClient::connected);
}

void FileClient::connectToServer(QString ip, quint16 port)
{
    socket.connectToHost(ip,port);
}

void FileClient::sendFile(QString filePath,
                          QString receiver,
                          QString savePath,
                          bool overwrite,
                          QString altName,
                          QString purpose,
                          QString username)
{
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly))
        return;

    QFileInfo info(file);

    currentTransferId = QUuid::createUuid().toString();
    currentReceiver = receiver;
    currentFilename = info.fileName();
    currentSender = socket.localAddress().toString();

    QString header =
    QString("FILE|%1|%2|%3|%4|%5|%6|%7|%8|%9|%10\n")
        .arg(currentTransferId)
        .arg(currentSender)
        .arg(receiver)
        .arg(savePath)
        .arg(info.fileName())
        .arg(file.size())
        .arg(overwrite ? "1":"0")
        .arg(altName)
        .arg(purpose)
        .arg(username);

    socket.write(header.toUtf8());

    while(!file.atEnd())
    {
        socket.write(file.read(64*1024));
        socket.waitForBytesWritten();
    }
}

void FileClient::readServer()
{
    QString msg = socket.readAll();

    if(msg.startsWith("DONE"))
    {
        QStringList p = msg.split("|");

        QString id = p[1].trimmed();

        emit transferFinished(
            currentSender,
            currentReceiver,
            currentFilename,
            id);
    }
}
