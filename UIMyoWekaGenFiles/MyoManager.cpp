#include "myomanager.h"
#include "MyoTime.h"
MyoManager::MyoManager()
{
}

MyoManager::~MyoManager()
{
}


//return true if a myo is connected
bool MyoManager::isConnected() const
{
    return mMyoHub && mMyo  && mListener.mConnected ;
}

//return true if myo thread is running
bool MyoManager::isRunning() const
{
    return mLoop;
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
    try
    {
        //alloc
        mMyoHub = new myo::Hub("com.weka.genfiles");
    }
    catch ( const std::runtime_error&  error )
    {
        mMyoHub = nullptr;
    }
    //max try count
    const ushort mac_count_try = 5;
    //wait connection
    for(ushort count_try = 0;
               count_try != mac_count_try && (mLoop && mMyoHub && !mMyo);
             ++count_try)
    {
        mMyo    = mMyoHub->waitForMyo(1000);
    }
    //if connected
    if(mMyo)
    {
        mMyo->setStreamEmg(myo::Myo::streamEmgEnabled);
        mMyoHub->addListener(&mListener);
        mMyoHub->setLockingPolicy(myo::Hub::lockingPolicyNone);

#ifndef USE_SIGNALS
        if(mCBConnection) mCBConnection(true);
#else
        emit connectionStatusChange(true);
#endif

        while(mLoop)
        {
            //send all message
            mMyoHub->run(MyoData::msupadate);
            //if not connected stop.
            if(!mListener.mConnected) mLoop = false;
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
    //loop is stopped
    mLoop = false;
    //is disconected
    mListener.mConnected = false;
    //myo last
    mMyo = nullptr;
    //dealloc
    if(mMyoHub) delete mMyoHub;
    //callback
#ifndef USE_SIGNALS
    if(mCBConnection) mCBConnection(false);
#else
    emit connectionStatusChange(false);
#endif
}
