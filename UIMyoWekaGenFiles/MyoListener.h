#ifndef MYOLISTENER_H
#define MYOLISTENER_H
#pragma once

#include <array>
#include <myo.hpp>
#include "Utilities.h"
#include "MyoDataOuput.h"
#include "MyoDataInput.h"
#include "DataSetOutput.h"
#include "WekaOuput.h"
#include "FANNOuput.h"

/*!
 * \brief The MyoListener class
 */
class MyoListener : public myo::DeviceListener
{
public:

    /*!
     * \brief Type Raw
     */
    using TypeRaw      = myo::RawDatas<int8_t, float, float, float, 8>;

    /*!
     * \brief Type Raws
     */
    using TypeRows     = WekaRows<int8_t, float, float, float, 8>;

    /*!
     * \brief Type Output
     */
    using TypeOuput    = MyoDataOuput<8>;

    /*!
     * \brief Type Input
     */
    using TypeInput    = MyoDataInput<int8_t, float, float, float, 8>;

    /*!
     * Type Ouput DataSet
     */
    using TypeOuputDataSet= DataSetOutput<8>;

    /*!
     * Type Ouput Weka
     */
    using TypeOuputWeka= WekaOuput<8>;

    /*!
     * Type Ouput FANN
     */
    using TypeOuputFANN= FANNOutput<8>;


    /*!
     * \brief MyoListener
     */
    MyoListener(){}

    /*!
     * \brief onUnpair
     * \param myo
     * \param timestamp
     */
    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        mPose = myo::Pose();
        mArm.clear();
        mRaw.clear();
    }

    /*!
     * \brief onArmSync
     * \param myo
     * \param timestamp
     * \param arm
     * \param xDirection
     * \param rotation
     * \param warmupState
     */
    void onArmSync(myo::Myo* myo, uint64_t timestamp,
                   myo::Arm arm,
                   myo::XDirection xDirection,
                   float rotation,
                   myo::WarmupState warmupState)
    {
        mArm.setArm(arm);
        mArm.sync();
    }

    /*!
     * \brief onArmUnsync
     * \param myo
     * \param timestamp
     */
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.unsync();
    }

    /*!
     * \brief onUnlock
     * \param myo
     * \param timestamp
     */
    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.unlock();
    }

    /*!
     * \brief onLock
     * \param myo
     * \param timestamp
     */
    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.lock();
    }

    /*!
     * \brief onAccelerometerData
     * \param myo
     * \param timestamp
     * \param accel
     */
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
    {
        mRaw.setAccelerometer(accel);
    }

    /*!
     * \brief onGyroscopeData
     * \param myo
     * \param timestamp
     * \param gyro
     */
    void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
    {
        mRaw.setGyroscope(gyro);
    }

    /*!
     * \brief onEmgData
     * \param myo
     * \param timestamp
     * \param emg
     */
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
    {
        mRaw.setEmgCArray(emg, 8);
    }

    /*!
     * \brief onOrientationData
     * \param myo
     * \param timestamp
     * \param quat
     */
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        mRaw.setQuaternion(quat);
    }

    /*!
     * \brief onPose
     * \param myo
     * \param timestamp
     * \param pose
     */
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        mPose = pose;
    }

    /*!
     * \brief toString
     * \return informations about last data inputs
     */
    std::string toString()
    {
        std::string out;
        out += mArm.toString()    + "\n";
        out += "Pos: " + mPose.toString();
        out += mRaw.toString()    + "\n";
        return out;
    }

    myo::Pose      mPose;  //! last myo gesture
    myo::ArmStatus mArm;   //! last myo status
    TypeRaw        mRaw;   //! last myo raw inputs
};

#endif // MYOLISTENER_H
