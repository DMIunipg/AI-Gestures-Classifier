//
//  MyoThread.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 28/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "MyoThread.h"
#include "MyoTime.h"

void MyoThread::start(MyoThread::Callback callback,const DataFlags& flags)
{
    mLoop  = true;
    mFlags = flags;
    mMyoHub = std::unique_ptr<myo::Hub>(new myo::Hub("com.runtime.fnnamyo"));
    mCallback = callback;
    mMyoDevice = mMyoHub->waitForMyo(10000);
    //exit case
    if(!mMyoDevice) return ;
    //else start stream
    mMyoDevice->setStreamEmg(myo::Myo::streamEmgEnabled);
    mMyoHub->addListener(&mMyoListener);
    mMyoHub->setLockingPolicy(myo::Hub::lockingPolicyNone);
    //start thread
    mThread = std::unique_ptr<std::thread>(new std::thread(&MyoThread::run,this));
}

void MyoThread::run()
{
    //get start time
    double timeStart = myo::GetTime();
    double timePush  = myo::GetTime();
    //thread loop
    while(mLoop)
    {
        //hub
        mMyoDevice->unlock(myo::Myo::unlockHold);
        mMyoHub->run(1000/20);
        //modes
        switch (mFlags.mMode)
        {
            case DataFlags::GESTURE_MODE:
            {
                if((myo::GetTime()-timePush) >= mFlags.mDeltaTime)
                {
                    //put value
                    mInputs.push_back(mMyoListener.getInput());
                    mInputs[mInputs.size()-1].setTime(myo::GetTime()-timeStart);
                    //update push time
                    timePush = myo::GetTime();
                }
                
                //... by time...
                //if((myo::GetTime()-timeStart) >= mFlags.mTimePerGesture)
                //or... by reps
                if (mInputs.size() >= mFlags.mReps)
                {
                    //put input 
                    mCallback(mInputs,mFlags,mMutex);
                    mInputs.clear();
                    //restart
                    timeStart = myo::GetTime();
                }
            }
            break;
            case DataFlags::SEMPLE_MODE:
            {
                //add all inputs
                mInputs.push_back(mMyoListener.getInput());
                mInputs[mInputs.size()-1].setTime(myo::GetTime()-timeStart);
                //reps
                if (mInputs.size() >= mFlags.mReps)
                {
                    mCallback(mInputs,mFlags,mMutex);
                    mInputs.clear();
                }
            }
            default:break;
        }
    }
}

void MyoThread::joint()
{
    //break loop
    mLoop = false;
    //remove listener
    mMyoHub->removeListener(&mMyoListener);
    //wait end....
    mThread->join();
}