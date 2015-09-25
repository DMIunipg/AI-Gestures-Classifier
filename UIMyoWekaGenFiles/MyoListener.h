#ifndef MYOLISTENER_H
#define MYOLISTENER_H
#pragma once

#include <array>
#include <myo.hpp>
#include "Utilities.h"
#include "WekaOuput.h"
#include "MyoDataOuput.h"
#include "MyoDataInput.h"
#include "FANNOuput.h"

class MyoListener : public myo::DeviceListener
{
public:
    //types
    using TypeRaw      = myo::RawDatas<int8_t, float, float, float, 8>;
    using TypeRows     = WekaRows<int8_t, float, float, float, 8>;
    using TypeOuput    = MyoDataOuput<8>;
    using TypeInput    = MyoDataInput<int8_t, float, float, float, 8>;
    using TypeOuputWeka= WekaOuput<8>;
    using TypeOuputFANN= FANNOutput<8>;


    //default constructor
    MyoListener(){}

    //init
    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        mPose = myo::Pose();
        mArm.clear();
        mRaw.clear();
    }

    //arm events
    void onArmSync(myo::Myo* myo, uint64_t timestamp,
                   myo::Arm arm,
                   myo::XDirection xDirection,
                   float rotation,
                   myo::WarmupState warmupState)
    {
        mArm.setArm(arm);
        mArm.sync();
    }
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.unsync();
    }
    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.unlock();
    }
    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.lock();
    }

    //raw values
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
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        mRaw.setQuaternion(quat);
    }
    //compute values
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        mPose = pose;
    }


    std::string toString()
    {
        std::string out;
        out += mArm.toString()    + "\n";
        out += "Pos: " + mPose.toString();
        out += mRaw.toString()    + "\n";
        return out;
    }
    myo::Pose      mPose;
    myo::ArmStatus mArm;
    TypeRaw        mRaw;
};

#endif // MYOLISTENER_H
