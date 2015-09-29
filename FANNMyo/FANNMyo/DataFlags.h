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
    
    void serialize(FILE* file) const
    {
        std::fwrite(&mMode, sizeof(mMode), 1, file);
        std::fwrite(&mReps, sizeof(mReps), 1, file);
        std::fwrite(&mTimePerGesture, sizeof(mTimePerGesture), 1, file);
        std::fwrite(&mDeltaTime, sizeof(mDeltaTime), 1, file);
        std::fwrite(&mTime, sizeof(mTime), 1, file);
        std::fwrite(&mGyroscope, sizeof(mGyroscope), 1, file);
        std::fwrite(&mAccelerometer, sizeof(mAccelerometer), 1, file);
        std::fwrite(&mQuaternion, sizeof(mQuaternion), 1, file);
        std::fwrite(&mEuler, sizeof(mEuler), 1, file);
        std::fwrite(&mEmg, sizeof(mEmg), 1, file);
    }
    
    void derialize(FILE* file)
    {
        std::fread(&mMode, sizeof(mMode), 1, file);
        std::fread(&mReps, sizeof(mReps), 1, file);
        std::fread(&mTimePerGesture, sizeof(mTimePerGesture), 1, file);
        std::fread(&mDeltaTime, sizeof(mDeltaTime), 1, file);
        std::fread(&mTime, sizeof(mTime), 1, file);
        std::fread(&mGyroscope, sizeof(mGyroscope), 1, file);
        std::fread(&mAccelerometer, sizeof(mAccelerometer), 1, file);
        std::fread(&mQuaternion, sizeof(mQuaternion), 1, file);
        std::fread(&mEuler, sizeof(mEuler), 1, file);
        std::fread(&mEmg, sizeof(mEmg), 1, file);
    }

};

#endif // DATAFLAGS_H

