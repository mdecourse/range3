/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   command_processor.cpp                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Command processor class definition                  *
 *********************************************************************/

//#include <QTextStream>
#include <QDataStream>

//#ifdef _WIN32
//#include <windows.h>
//#else
//#include <unistd.h>
//#endif

#include <rblib.h>

#include "command_processor.h"

CommandProcessor::CommandProcessor(const QString &taskID, QCoreApplication *application) :
    QObject(application),
    enabled(false),
    localSocket(new QLocalSocket(this))
{
    RLogger::info("Connecting to: %s\n",taskID.toUtf8().constData());
    this->localSocket->connectToServer(taskID);
    if (!this->localSocket->waitForConnected())
    {
        RLogger::error("Failed to connect to: %s\n",taskID.toUtf8().constData());
    }
    RLogger::info("Connected to: %s\n",taskID.toUtf8().constData());
    QObject::connect(this->localSocket, &QLocalSocket::readyRead, this, &CommandProcessor::readSocket);
//#ifdef _WIN32
//    this->stdinNotifier = new QWinEventNotifier(GetStdHandle(STD_INPUT_HANDLE),this);
//    QObject::connect(this->stdinNotifier,&QWinEventNotifier::activated,this,&CommandProcessor::readStdin);
//    this->stdinNotifier->setEnabled(false);
//#else
//    this->stdinNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read,this);

//    QObject::connect(this->stdinNotifier,
//                     &QSocketNotifier::activated,
//                     this,
//                     &CommandProcessor::readStdin);
//#endif
}

void CommandProcessor::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

//void CommandProcessor::readStdin(HANDLE socket)
//{
//    if (!this->enabled)
//    {
//        return;
//    }
//    RLogger::warning("Notified\n");
//    return;

//    QTextStream textStream(stdin,QIODevice::ReadOnly);
//    textStream.skipWhiteSpace();
//    QString line = textStream.readLine();
//    RLogger::info("Received command: \'%s\'\n",line.toUtf8().constData());
//    if (line.contains("STOP"))
//    {
//        RApplicationState::getInstance().setStateType(R_APPLICATION_STATE_STOP);
//    }
//}

void CommandProcessor::readSocket(void)
{
    if (this->localSocket->bytesAvailable() < (int)sizeof(quint32))
    {
        return;
    }
    QDataStream in;
    in.setDevice(this->localSocket);
    in.setVersion(QDataStream::Qt_5_7);
    quint32 blockSize;
    in >> blockSize;
    if (this->localSocket->bytesAvailable() < blockSize || in.atEnd())
    {
        return;
    }
    QString message;
    in >> message;
    RLogger::info("Received command: \'%s\'\n",message.toUtf8().constData());
    if (message.contains("STOP"))
    {
        RApplicationState::getInstance().setStateType(R_APPLICATION_STATE_STOP);
    }
}
