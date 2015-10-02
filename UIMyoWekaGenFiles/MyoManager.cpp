#include "myomanager.h"
#include "MyoTime.h"
MyoManager::MyoManager()
{
}

MyoManager::~MyoManager()
{
}

//start recording
void MyoManager::startRecording()
{
    mMutex.lock();
    mDatas.clear();
    mRecording = LIST_REC;
    mTime      = myo::GetTime();
    mMutex.unlock();
}
void MyoManager::startRecording(size_t n,
                                std::function<void( QMutex& , MyoListener::TypeRows& )> fun)
{
    mMutex.lock();
    mDatas.clear();
    mRecording = CALLBACK_REC;
    mCallback  = fun;
    mSizeData  = n;
    mSizeNext  = n;
    mTime      = myo::GetTime();
    mMutex.unlock();
}

//end Recording
MyoListener::TypeRows MyoManager::endRecording()
{
    mMutex.lock();
    mRecording = NO_REC;
    mMutex.unlock();
    return mDatas;
}

//get current row
MyoListener::TypeRaw MyoManager::getCurrentRow()
{
    MyoListener::TypeRaw row;
    mMutex.lock();
    row  = mListener.mRaw;
    mMutex.unlock();
    return row;
}

//loop
void MyoManager::run()
{
    //alloc
    mMyoHub = new myo::Hub("com.weka.genfiles");
    //wait connection
    while(mLoop && !mMyo)
    {
        mMyo    = mMyoHub->waitForMyo(1000);
    }
    //if connected
    if(mMyo)
    {
        mMyo->setStreamEmg(myo::Myo::streamEmgEnabled);
        mMyoHub->addListener(&mListener);

        while(mLoop)
        {
            mMyoHub->run(1000/20);
            //rec
            if(mRecording)
            {
                //lock
                mMutex.lock();
                //compute diff
                double recTime = (myo::GetTime()-mTime) / 1000.0;
                //append
                mDatas.append(mListener.mRaw);
                //set time
                mDatas[mDatas.size()-1].setTime(recTime);
                //callback
                if(mDatas.size()==mSizeNext)
                {
                    mCallback(mMutex,mDatas);
                    //next
                    mSizeNext=mSizeData+mDatas.size();
                }
                //unlock
                mMutex.unlock();
            }

        }
    }
    //dealloc
    delete mMyoHub;
}
