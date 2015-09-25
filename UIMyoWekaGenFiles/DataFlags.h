#ifndef DATAFLAGS_H
#define DATAFLAGS_H
#pragma once

struct DataFlags
{
    int  mReps          { 1    };
    bool mTime          { true };
    bool mGyroscope     { true };
    bool mAccelerometer { true };
    bool mQuaternion    { true };
    bool mEuler         { true };
    bool mEmg           { true };
    DataFlags(){}
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

