#ifndef DATAFLAGS_H
#define DATAFLAGS_H
#pragma once

struct DataFlags
{
    //mode
    enum Mode
    {
        SEMPLE_MODE,
        GESTURE_MODE
    };
    Mode mMode            { SEMPLE_MODE };
    //reps
    int  mReps            { 1    };
    //action time
    double mTimePerGesture{ 1.0  };
    double mDeltaTime     { 0.1  };
    //type off info
    bool mTime            { true };
    bool mGyroscope       { true };
    bool mAccelerometer   { true };
    bool mQuaternion      { true };
    bool mEuler           { true };
    bool mEmg             { true };

    DataFlags(){}

    DataFlags(Mode mode,
              int  reps,
              double  timePerGesture,
              double  deltaTime,
              bool gyroscope,
              bool accelerometer,
              bool quaternion,
              bool euler,
              bool emg)
    :mMode(mode)
    ,mReps(reps)
    ,mTimePerGesture(timePerGesture)
    ,mDeltaTime(deltaTime)
    ,mGyroscope(gyroscope)
    ,mAccelerometer(accelerometer)
    ,mQuaternion(quaternion)
    ,mEuler(euler)
    ,mEmg(emg)
    {

    }

    DataFlags(int  reps,
              bool gyroscope,
              bool accelerometer,
              bool quaternion,
              bool euler,
              bool emg)
    :mReps(reps)
    ,mGyroscope(gyroscope)
    ,mAccelerometer(accelerometer)
    ,mQuaternion(quaternion)
    ,mEuler(euler)
    ,mEmg(emg)
    {

    }

    DataFlags(double  timePerGesture,
              double  deltaTime,
              bool    gyroscope,
              bool    accelerometer,
              bool    quaternion,
              bool    euler,
              bool    emg)
    :mMode(GESTURE_MODE)
    ,mTimePerGesture(timePerGesture)
    ,mDeltaTime(deltaTime)
    ,mGyroscope(gyroscope)
    ,mAccelerometer(accelerometer)
    ,mQuaternion(quaternion)
    ,mEuler(euler)
    ,mEmg(emg)
    {

    }

    template < const size_t N >
    size_t lineSize() const
    {
        size_t size=0;
        //add all
        size += mTime ? 1 : 0;
        size += mGyroscope ? 3 : 0;
        size += mAccelerometer ? 3 : 0;
        size += mQuaternion ? 4 : 0;
        size += mEuler ? 3 : 0;
        size += mEmg ? N : 0;
        //rep
        size *= mReps;
        //size
        return size;
    }

};

#endif // DATAFLAGS_H

