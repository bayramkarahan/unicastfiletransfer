#include<unicastfiletransferclient.h>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileClient client;

    QObject::connect(&client,&FileClient::connected,[&]()
    {
        client.sendFile(
        "/home/etapadmin/ab.deb",
        "deneme",
        "/tmp",
        true,
        "b.txt",
        "desktopfile",
        "karahan");
    });

    QObject::connect(&client,
    &FileClient::transferFinished,
    [](QString sender,
       QString receiver,
       QString file,
       QString id)
    {
        qDebug()
        << sender
        << "->"
        << receiver
        << file
        << "transfer OK"
        << "id:" << id;
    });

    client.connectToServer("192.168.1.100",45454);
    return a.exec();
}
