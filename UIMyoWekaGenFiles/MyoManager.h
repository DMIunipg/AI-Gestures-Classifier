#ifndef MYOMANAGER_H
#define MYOMANAGER_H
#pragma once


#include <ctime>
#include <QMutex>
#include <QThread>
#include "MyoListener.h"

class MyoManager : protected QThread
{
public:

    MyoManager();

    ~MyoManager();

    //start recording
    void startRecording();
    void startRecording(size_t n,
                        std::function<void(QMutex&,MyoListener::TypeRows&)>);

    //end Recording
    MyoListener::TypeRows endRecording();

    //get current row
    MyoListener::TypeRaw getCurrentRow();

    //loop
    void run();

    //start
    void start(Priority priority = InheritPriority)
    {
        mLoop = true;
        QThread::start(priority);
    }

    //close
    void close()
    {
        mLoop = false;
        QThread::wait();
    }

    //close
    void terminate()
    {
        mLoop = false;
        QThread::terminate();
    }

    //close
    void quit()
    {
        mLoop = false;
        QThread::quit();
    }

protected:

     //myo utilities
     myo::Hub*             mMyoHub   { nullptr };
     myo::Myo*             mMyo      { nullptr };
     MyoListener           mListener;
     //thead utilities
     bool                  mLoop{ false };
     QMutex                mMutex;
     //recording utilities
     enum State
     {
        NO_REC,
        LIST_REC,
        CALLBACK_REC
     };
     State                 mRecording { NO_REC };
     MyoListener::TypeRows mDatas;
     //rec info
     double                                               mTime{ 0 };
     //callback info
     size_t                                               mSizeData{ 0 };
     size_t                                               mSizeNext{ 0 };
     std::function<void(QMutex&,MyoListener::TypeRows&)>  mCallback{ nullptr };


};

#endif // MYOMANAGER_H
