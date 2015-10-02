#ifndef DATAFLAGS_H
#define DATAFLAGS_H
#pragma once

#include "Utilities.h"
#include <cstdio>
#include <string>
#include <map>

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
    bool mPitch           { true };
    bool mYaw             { true };
    bool mRoll            { true };
    bool mEmg             { true };
    bool mNormalize       { true };
    bool mPositive        { true };
    
    DataFlags(){}
    
    DataFlags(Mode mode,
              int  reps,
              double  timePerGesture,
              double  deltaTime,
              bool gyroscope,
              bool accelerometer,
              bool quaternion,
              bool pitch,
              bool yaw,
              bool roll,
              bool emg,
              bool normalize,
              bool positive)
    :mMode(mode)
    ,mReps(reps)
    ,mTimePerGesture(timePerGesture)
    ,mDeltaTime(deltaTime)
    ,mGyroscope(gyroscope)
    ,mAccelerometer(accelerometer)
    ,mQuaternion(quaternion)
    ,mPitch(pitch)
    ,mYaw(yaw)
    ,mRoll(roll)
    ,mEmg(emg)
    ,mNormalize(normalize)
    ,mPositive(positive)
    {
        
    }
    
    DataFlags(int  reps,
              bool gyroscope,
              bool accelerometer,
              bool quaternion,
              bool pitch,
              bool yaw,
              bool roll,
              bool emg,
              bool normalize,
              bool positive)
    :mReps(reps)
    ,mGyroscope(gyroscope)
    ,mAccelerometer(accelerometer)
    ,mQuaternion(quaternion)
    ,mPitch(pitch)
    ,mYaw(yaw)
    ,mRoll(roll)
    ,mEmg(emg)
    ,mNormalize(normalize)
    ,mPositive(positive)
    {
        
    }
    
    DataFlags(double  timePerGesture,
              double  deltaTime,
              bool    gyroscope,
              bool    accelerometer,
              bool    quaternion,
              bool    pitch,
              bool    yaw,
              bool    roll,
              bool    emg,
              bool    normalize,
              bool    positive)
    :mMode(GESTURE_MODE)
    ,mTimePerGesture(timePerGesture)
    ,mDeltaTime(deltaTime)
    ,mGyroscope(gyroscope)
    ,mAccelerometer(accelerometer)
    ,mQuaternion(quaternion)
    ,mPitch(pitch)
    ,mYaw(yaw)
    ,mRoll(roll)
    ,mEmg(emg)
    ,mNormalize(normalize)
    ,mPositive(positive)
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
        size += mPitch ? 1 : 0;
        size += mYaw ? 1 : 0;
        size += mRoll ? 1 : 0;
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
        std::fwrite(&mPitch, sizeof(mPitch), 1, file);
        std::fwrite(&mYaw, sizeof(mYaw), 1, file);
        std::fwrite(&mRoll, sizeof(mRoll), 1, file);
        std::fwrite(&mEmg, sizeof(mEmg), 1, file);
        std::fwrite(&mNormalize, sizeof(mNormalize), 1, file);
        std::fwrite(&mPositive, sizeof(mPositive), 1, file);
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
        std::fread(&mPitch, sizeof(mPitch), 1, file);
        std::fread(&mYaw, sizeof(mYaw), 1, file);
        std::fread(&mRoll, sizeof(mRoll), 1, file);
        std::fread(&mEmg, sizeof(mEmg), 1, file);
        std::fread(&mNormalize, sizeof(mNormalize), 1, file);
        std::fread(&mPositive, sizeof(mPositive), 1, file);
    }
    
    /**
     * Applay to positive
     */
    double toPositive(double value) const
    {
        if(mPositive)
            return (value + 1.0) * 0.5;
        return value;
    }
    
    template < class T >
    myo::Vector3< T > toPositive(const myo::Vector3< T >& vec) const
    {
        if(mPositive)
            return myo::Vector3< T > ( (vec.x() + 1.0) * 0.5,
                                      (vec.y() + 1.0) * 0.5,
                                      (vec.z() + 1.0) * 0.5 );
        return vec;
    }
    
    template < class T >
    myo::Quaternion< T > toPositive(const myo::Quaternion< T >& vec) const
    {
        if(mPositive)
            return myo::Quaternion< T > ( (vec.x() + 1.0) * 0.5,
                                         (vec.y() + 1.0) * 0.5,
                                         (vec.z() + 1.0) * 0.5,
                                         (vec.w() + 1.0) * 0.5 );
        return vec;
    }
    
    /**
     * Applay to normalize
     */
    double toNormalize(double value, double max=1.0) const
    {
        if(mNormalize) return value / max;
        return value;
    }
    
    template < class T >
    myo::Vector3< T > toNormalize(const myo::Vector3< T > & value) const
    {
        if(mNormalize) return value.normalized();
        return value;
    }
    
    template < class T >
    myo::Quaternion< T > toNormalize(const myo::Quaternion< T > & value) const
    {
        if(mNormalize) return value.normalized();
        return value;
    }
    
    
    /**
     * Applay all to value
     */
    double apply(double value, double max = 1.0) const
    {
        return toPositive(toNormalize(value,max));
    }
    
    template < class T >
    myo::Vector3< T > apply(const myo::Vector3< T >& vec) const
    {
        return toPositive(toNormalize(vec));
    }
    
    template < class T >
    myo::Quaternion< T > apply(const myo::Quaternion< T >& quad) const
    {
        return toPositive(toNormalize(quad));
    }
};

struct ClassesNames
{
    std::map< double, std::string > mNames;
    
    void derialize(FILE* file)
    {
        //get map size
        unsigned int nclasses = 0;
        std::fscanf(file, "%u\n", &nclasses);
        //read all classes
        for(unsigned int i=0;i!=nclasses;++i)
        {
            double key = 0.0;
            char   buffer[255] = {0};
            std::fscanf(file, "%le, %s",&key,buffer);
            mNames[key] = buffer;
        }
    }
    
    const char* getClassName(double uid) const
    {
        //search
        auto it=mNames.find(uid);
        //not found?
        if(it==mNames.end()) return "unknow";
        //return name of class
        return it->second.c_str();
    }
};

#endif // DATAFLAGS_H

