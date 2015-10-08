#ifndef DATAFLAGS_H
#define DATAFLAGS_H
#pragma once

#include "Utilities.h"
#include <cstdio>
#include <string>
#include <map>

/*!
 * \brief The DataFlags struct
 */
struct DataFlags
{
    /*!
     * \brief The Mode enum
     */
    enum Mode
    {
        SEMPLE_MODE,
        GESTURE_MODE
    };
    Mode mMode            { SEMPLE_MODE }; //! type of classification
    //reps
    int  mReps            { 1    }; //! number of sample per gesture
    //action time
    double mTimePerGesture{ 1.0  }; //! time per gesture
    double mDeltaTime     { 0.1  }; //! sampling delta time
    //type off info
    bool mTime            { true }; //! put time field into dataset
    bool mGyroscope       { true }; //! put gyroscope fields into dataset
    bool mAccelerometer   { true }; //! put accelerometer fields into dataset
    bool mQuaternion      { true }; //! put quaternion fields into dataset
    bool mPitch           { true }; //! put pitch field into dataset
    bool mYaw             { true }; //! put yaw field into dataset
    bool mRoll            { true }; //! put roll field into dataset
    bool mEmg             { true }; //! put emg field into dataset
    bool mNormalize       { true }; //! normalize dataset fields
    bool mPositive        { true }; //! absolute value of all dataset fields
    //filters
    double mEmgSmooth     { 0.0   };//! smooth emg dataset fields
    bool   mEmgAbs        { false };//! absolute value only for emg dataset fields

    /*!
     * \brief DataFlags default constructor
     */
    DataFlags(){}

    /*!
     * \brief DataFlags
     * \param mode
     * \param reps
     * \param timePerGesture
     * \param deltaTime
     * \param gyroscope
     * \param accelerometer
     * \param quaternion
     * \param pitch
     * \param yaw
     * \param roll
     * \param emg
     * \param normalize
     * \param positive
     * \param emgSmooth
     * \param emgAbs
     */
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
              bool positive,
              double emgSmooth,
              bool emgAbs)
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
    ,mEmgSmooth(emgSmooth)
    ,mEmgAbs(emgAbs)
    {

    }

    /*!
     * \brief DataFlags
     * \param reps
     * \param gyroscope
     * \param accelerometer
     * \param quaternion
     * \param pitch
     * \param yaw
     * \param roll
     * \param emg
     * \param normalize
     * \param positive
     * \param emgSmooth
     * \param emgAbs
     */
    DataFlags(int  reps,
              bool gyroscope,
              bool accelerometer,
              bool quaternion,
              bool pitch,
              bool yaw,
              bool roll,
              bool emg,
              bool normalize,
              bool positive,
              double emgSmooth,
              bool emgAbs)
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
    ,mEmgSmooth(emgSmooth)
    ,mEmgAbs(emgAbs)
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
              bool    positive,
              double  emgSmooth,
              bool    emgAbs)
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
    ,mEmgSmooth(emgSmooth)
    ,mEmgAbs(emgAbs)
    {

    }



    /*!
     * \brief lineSize
     * \return size of row
     * \tparam N size of emg inputs
     */
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

    /*!
     * \brief serialize
     * \param file
     */
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
        std::fwrite(&mEmgSmooth, sizeof(mEmgSmooth), 1, file);
        std::fwrite(&mEmgAbs, sizeof(mEmgAbs), 1, file);
    }
    /*!
     * \brief derialize
     * \param file
     */
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
        std::fread(&mEmgSmooth, sizeof(mEmgSmooth), 1, file);
        std::fread(&mEmgAbs, sizeof(mEmgAbs), 1, file);
    }

    /*!
     * \brief toPositive
     * \param value
     * \return positive value
     */
    double toPositive(double value) const
    {
        if(mPositive)
            return (value + 1.0) * 0.5;
        return value;
    }

    /*!
     * \brief toPositive
     * \param vec
     * \return posive vector
     * \tparam T
     */
    template < class T >
    myo::Vector3< T > toPositive(const myo::Vector3< T >& vec) const
    {
        if(mPositive)
            return myo::Vector3< T > ( (vec.x() + 1.0) * 0.5,
                                       (vec.y() + 1.0) * 0.5,
                                       (vec.z() + 1.0) * 0.5 );
        return vec;
    }

    /*!
     * \brief toPositive
     * \param vec
     * \return
     * \tparam T
     */
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

    /*!
     * \brief toNormalize
     * \param value
     * \param max
     * \return value normalized
     */
    double toNormalize(double value, double max=1.0) const
    {
        if(mNormalize) return value / max;
        return value;
    }

    /*!
     * \brief toNormalize
     * \param value
     * \param max
     * \return value normalized
     * \tparam T
     */
    template < class T >
    myo::Vector3< T > toNormalize(const myo::Vector3< T > & value) const
    {
        if(mNormalize) return value.normalized();
        return value;
    }

    /*!
     * \brief toNormalize
     * \param value
     * \param max
     * \return value normalized
     * \tparam T
     */
    template < class T >
    myo::Quaternion< T > toNormalize(const myo::Quaternion< T > & value) const
    {
        if(mNormalize) return value.normalized();
        return value;
    }


    /*!
     * \brief apply
     * \param value
     * \param max
     * \return the absolute value normalized
     */
    double apply(double value, double max = 1.0) const
    {
        return toPositive(toNormalize(value,max));
    }


    /*!
     * \brief apply
     * \param value
     * \param max
     * \return the absolute value normalized
     * \tparam T
     */
    template < class T >
    myo::Vector3< T > apply(const myo::Vector3< T >& vec) const
    {
        return toPositive(toNormalize(vec));
    }

    /*!
     * \brief apply
     * \param value
     * \param max
     * \return the absolute value normalized
     * \tparam T
     */
    template < class T >
    myo::Quaternion< T > apply(const myo::Quaternion< T >& quad) const
    {
        return toPositive(toNormalize(quad));
    }

    /*!
     * \brief smooth
     * \param from
     * \param to
     * \return the smooth value
     * \tparam T
     */
    template < typename T >
    T smooth(const T& from,const T& to) const
    {
        return from * mEmgSmooth + to * (1.0-mEmgSmooth);
    }

    /*!
     * \brief applayEmgSmooth
     * \param emgs
     * \param nrows
     * \tparam T
     * \tparam N size of vactor of emg values
     */
    template < typename T, size_t N >
    void applayEmgSmooth(std::array< T, N > emgs[],size_t nrows) const
    {
        for(int r=1;r < nrows;++r)
            for(int e=0;e!=N;     ++e)
            {
                emgs[r][e]=smooth(emgs[r-1][e],emgs[r][e]);
            }
    }

    /*!
     * \brief applayEmgAbs
     * \param emgs
     * \param nrows
     * \tparam T
     * \tparam N size of vactor of emg values
     */
    template < typename T, size_t N >
    void applayEmgAbs(std::array< T, N > emgs[],size_t nrows) const
    {
        for(int r=0;r!=nrows;++r)
            for(auto& emg:   emgs[r])
            {
                emg = std::abs(emg);
            }
    }

    /*!
     * \brief applayEmgFilter
     * \param emgs
     * \param nrows
     * \tparam T
     * \tparam N size of vactor of emg values
     */
    template < typename T, size_t N >
    void applayEmgFilter(std::array< T, N > emgs[],size_t nrows) const
    {
        if(mEmgAbs)    applayEmgAbs(emgs,nrows);
        if(mEmgSmooth) applayEmgSmooth(emgs,nrows);
    }

    /*!
     * \brief applayEmgSmooth
     * \param vecemgs
     * \param nrows
     * \tparam T
     * \tparam N size of vactor of emg values
     */
    template < typename T, size_t N >
    void applayEmgSmooth(std::array< T, N >* vecemgs[],size_t nrows) const
    {
        for(int r=1;r < nrows;++r)
        {
            auto& emgs0 = *(vecemgs[r-1]);
            auto& emgs1 = *(vecemgs[r]);
            for(int e=0;e!=N;     ++e)  emgs1[e]=smooth(emgs0[e],emgs1[e]);
        }
    }

    /*!
     * \brief applayEmgAbs
     * \param vecemgs
     * \param nrows
     * \tparam T
     * \tparam N size of vactor of emg values
     */
    template < typename T, size_t N >
    void applayEmgAbs(std::array< T, N >* vecemgs[],size_t nrows) const
    {
        for(int r=0;r!=nrows;++r)
        {
            auto& emgs = *(vecemgs[r]);
            for(auto& emg: emgs)  emg = std::abs(emg);
        }
    }

    /*!
     * \brief applayEmgFilter
     * \param emgs
     * \param nrows
     * \tparam T
     * \tparam N size of vactor of emg values
     */
    template < typename T, size_t N >
    void applayEmgFilter(std::array< T, N >* emgs[],size_t nrows) const
    {
        if(mEmgAbs)    applayEmgAbs(emgs,nrows);
        if(mEmgSmooth) applayEmgSmooth(emgs,nrows);
    }

};

/*!
 * \brief The ClassesNames struct
 */
struct ClassesNames
{
    std::map< double, std::string > mNames; //! Map of class name

    /*!
     * \brief derialize
     * \param file
     */
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

    /*!
     * \brief getClassName
     * \param uid
     * \return class name
     */
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

