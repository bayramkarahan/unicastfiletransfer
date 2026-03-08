#include "unicastfiletransferserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileServer server;

       if(!server.listen(QHostAddress::Any,45454))
       {
           qDebug()<<"Server başlatılamadı";
           return -1;
       }

       qDebug()<<"Server çalışıyor";

       QObject::connect(&server,&FileServer::transferCompleted,
       [](QString sender,
          QString receiver,
          QString file,
          QString altFile,
          QString purpose,
          QString user)
       {
           qDebug()
           << sender
           << "->"
           << receiver
           << file
           << "alt:" << altFile
           << "purpose:" << purpose
           << "user:" << user;
       });

         return a.exec();
}
