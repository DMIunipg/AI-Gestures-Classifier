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
#include <array>
#include <vector>
#include <string>
#include <functional>
#include <time.h>
#include <DataFlags.h>
#include <Utilities.h>
#include <cxx/DeviceListener.hpp>

/*!
 * \brief MyoDataCollector
 */
class MyoDataCollector : public myo::DeviceListener
{
public:
    
    /*!
     * \brief InputRaw the conteier of raw data
     */
    using InputRaw = myo::RawDatas<int8_t, float, float, float, 8>;

    /*!
     * \brief onOrientationData
     * \param myo
     * \param timestamp
     * \param quaternion
     */
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        mRaw.setQuaternion(quat);
    }
    
    /*!
     * \brief onAccelerometerData
     * \param myo
     * \param timestamp
     * \param accelerometer values
     */
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
    {
        mRaw.setAccelerometer(accel);
    }
    
    /*!
     * \brief onGyroscopeData
     * \param myo
     * \param timestamp
     * \param gyroscope values
     */
    void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
    {
        mRaw.setGyroscope(gyro);
    }
    
    /*!
     * \brief onEmgData
     * \param myo
     * \param timestamp
     * \param emg array
     */
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
    {
        mRaw.setEmgCArray(emg, 8);
    }
    
    /*!
     * \brief getInput
     * \return input raw
     */
    const InputRaw& getInput() const
    {
        return mRaw;
    }
    
private:
    //input raw
    InputRaw mRaw;
};

namespace myo
{
    /*!
     * forward declaration
     */
    class Hub;
}

/*!
 * \brief MyoThread
 */
class MyoThread
{
public:
    
    /*!
     * \brief Inputs the type of the input datas
     */
    using Inputs = std::vector< MyoDataCollector::InputRaw >;
    
    /*!
     * \brief Callback the type of the callback, it will be called when a input is received
     */
    using Callback = std::function< void(const Inputs&,const DataFlags&,std::mutex& mutex) >;
    
    /*!
     * \brief Callback the type of the callback, it will be called when a input is filtered
     */
    using ApplyCallback = std::function< void( size_t gindex, size_t idrow, double value) >;

    /*!
     * \brief start
     * \param callback
     * \param flags
     * \param update
     */
    void start(Callback callback,const DataFlags& flags,unsigned int update = 20);
    
    /*!
     * \brief applay the filter to the input datas
     * \param callback
     * \param flags
     * \param update
     */
    static void applay(const Inputs& inputs,const DataFlags& flags,ApplyCallback callback);
    
    /*!
     * \brief run
     */
    void run();
    
    /*!
     * \brief join wait the myo thread
     */
    void joint();
    
    /*!
     * \brief MyoThread
     */
    MyoThread();
    
    /*!
     * \brief ~MyoThread
     */
    ~MyoThread();
    
private:
    //MyO data
    myo::Myo* mMyoDevice;
    MyoDataCollector mMyoListener;
    myo::Hub* mMyoHub;
    //thread data
    unsigned int mUpdate { 20 };
    bool mLoop;
    Callback mCallback;
    std::mutex  mMutex;
    std::unique_ptr< std::thread > mThread;
    //input data
    DataFlags mFlags;
    std::vector< MyoDataCollector::InputRaw > mInputs;
    
};

#endif /* MyoThread_h */
