//
//  MyoThread.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 28/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef MyoThread_h
#define MyoThread_h

#include <thread>
#include <mutex>
#include <memory>
#include <mutex>
#include <myo.hpp>
#include <array>
#include <functional>
#include <time.h>
#include "DataFlags.h"
#include "Utilities.h"


class MyoDataCollector : public myo::DeviceListener
{
public:
    //raw inputs
    using InputRaw = myo::RawDatas<int8_t, float, float, float, 8>;

    //listeners
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        mRaw.setQuaternion(quat);
    }
    
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
    {
        mRaw.setAccelerometer(accel);
    }
    
    void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
    {
        mRaw.setGyroscope(gyro);
    }
    
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
    {
        mRaw.setEmgCArray(emg, 8);
    }
    
    const InputRaw& getInput() const
    {
        return mRaw;
    }
    
private:
    //input row
    InputRaw mRaw;
};

class MyoThread
{
public:
    //callback input
    using Inputs = std::vector< MyoDataCollector::InputRaw >;
    using Callback = std::function< void(const Inputs&,const DataFlags&,std::mutex& mutex) >;
    using ApplyCallback = std::function< void( size_t gindex, size_t idrow, double value) >;
    //start the thread
    void start(Callback callback,const DataFlags& flags);
    //utility method
    static void applay(const Inputs& inputs,const DataFlags& flags,ApplyCallback callback);
    //run 
    void run();
    void joint();
    
private:
    //MyO data
    myo::Myo* mMyoDevice;
    MyoDataCollector mMyoListener;
    std::unique_ptr< myo::Hub > mMyoHub;
    //thread data
    bool mLoop;
    Callback mCallback;
    std::mutex  mMutex;
    std::unique_ptr< std::thread > mThread;
    //input data
    DataFlags mFlags;
    std::vector< MyoDataCollector::InputRaw > mInputs;
    
};

#endif /* MyoThread_h */
