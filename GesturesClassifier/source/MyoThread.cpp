//
//  MyoThread.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 28/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <MyoThread.h>
#include <MyoTime.h>
#include <MyoData.h>
#include <myo.hpp>

void MyoThread::start(MyoThread::Callback callback,const DataFlags& flags,unsigned int update)
{
    mLoop  = true;
    mFlags = flags;
    mUpdate = update;
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
    //alloc for ptrs for smooth
    auto emgPtrArray = new std::array< int8_t , 8UL >*[mFlags.mReps] ;
    //thread loop
    while(mLoop)
    {
        //hub
        mMyoDevice->unlock(myo::Myo::unlockHold);
        mMyoHub->run(1000/mUpdate);
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
                    //set ptrs
                    for (size_t i=0; i!= mFlags.mReps; ++i)
                    {
                        emgPtrArray[i] = &mInputs[i].getEmg();
                    }
                    //applay
                    mFlags.applayEmgFilter(emgPtrArray, mFlags.mReps);
                    //put input 
                    mCallback(mInputs,mFlags,mMutex);
                    mInputs.clear();
                    //restart
                    timeStart = myo::GetTime();
                }
            }
            break;
            case DataFlags::SAMPLE_MODE:
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
    //dealloc
    delete [] emgPtrArray;
}

void MyoThread::applay(const Inputs& inputs,const DataFlags& flags,ApplyCallback callback)
{
    //size of row
    size_t count = 0;
    size_t rowSize = flags.lineSize<8>() / flags.mReps;
    //get max time
    double maxTime = inputs[inputs.size()-1].getTime();
    //for all rows
    for(const auto& row : inputs)
    {
        //index
        size_t i = 0;
        size_t irow = rowSize*count;
        //
        if(flags.mTime)
        {
            callback(i + irow, i, flags.toNormalize( row.getTime() , maxTime )); ++i;
        }
        if(flags.mGyroscope)
        {
            auto gyr = flags.apply( row.getGyroscope() );
            callback(i + irow, i, gyr.x()); ++i;
            callback(i + irow, i, gyr.y()); ++i;
            callback(i + irow, i, gyr.z()); ++i;
        }
        if(flags.mAccelerometer)
        {
            auto acc = flags.apply( row.getAccelerometer() );
            callback(i + irow, i, acc.x()); ++i;
            callback(i + irow, i, acc.y()); ++i;
            callback(i + irow, i, acc.z()); ++i;
            
        }
        if(flags.mQuaternion)
        {
            auto quad =  flags.apply( row.getQuaternion() );
            callback(i + irow, i, quad.x()); ++i;
            callback(i + irow, i, quad.y()); ++i;
            callback(i + irow, i, quad.z()); ++i;
            callback(i + irow, i, quad.z()); ++i;
        }
        if(flags.mPitch || flags.mYaw || flags.mRoll)
        {
            auto euler = row.getEulerAngles();
            
            if(flags.mPitch)
            {
                callback(i + irow, i, flags.apply( (double)euler.pitch() , (M_PI*2.0) )); ++i;
            }
            if(flags.mYaw)
            {
                callback(i + irow, i, flags.apply( (double)euler.yaw() , (M_PI*2.0) )); ++i;
            }
            if(flags.mRoll)
            {
                callback(i + irow, i, flags.apply( (double)euler.roll() , (M_PI*2.0) )); ++i;
            }
        }
        if(flags.mEmg)
        {
            for(auto emg:row.getEmg())
            {
                callback(i + irow, i,  flags.apply( ((double)emg) , 128.0 ));  ++i;
            }
        }
        //next
        ++count;
    }
}

//terminate ...
void MyoThread::joint()
{
    //break loop
    mLoop = false;
    //remove listener
    mMyoHub->removeListener(&mMyoListener);
    //wait end....
    if(mThread.get()) mThread->join();
}
//contructor
MyoThread::MyoThread()
{
    mMyoHub = new myo::Hub("com.runtime.gesturesclassifier");
}
//destructor
MyoThread::~MyoThread()
{
    if(mLoop) joint();
    if(mMyoHub) delete mMyoHub;
}