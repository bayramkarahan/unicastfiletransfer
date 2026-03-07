#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QHostAddress>
class FileClient : public QObject
{
    Q_OBJECT

public:
    explicit FileClient(QObject *parent = nullptr);

    void connectToServer(QString ip, quint16 port);

    void sendFile(QString filePath,
                  QString receiver,
                  QString savePath,
                  bool overwrite);

signals:


    void transferFinished(QString senderIp,
                              QString receiverIp,
                              QString filename,
                              QString transferId);
    void connected();

private slots:

    void readServer();

private:
    QTcpSocket socket;

    QString currentTransferId;
    QString currentReceiver;
    QString currentFilename;
    QString currentSender;
};

#endif
