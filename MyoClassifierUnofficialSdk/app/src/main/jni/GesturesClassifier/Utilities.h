//
//  Utilities.h
//  MyoWekaGenFiles
//
//  Created by Gabriele on 26/06/15.
//  Copyright (c) 2015 Gabriele. All rights reserved.
//
#ifndef MYO_UTILITIES_H
#define MYO_UTILITIES_H
#pragma once

#include <assert.h>
#include <cxx/Quaternion.hpp>
#include <cxx/Vector3.hpp>
#include <cxx/DeviceListener.hpp>
#include <cstring>
#include <cstdio>
#include <string>

#ifdef __ANDROID_API__
#include <sstream>
#include <array>
namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream ss;
        ss << n ;
        return ss.str() ;
    }
}
#endif

namespace myo
{
    /*!
     * \brief The EMG enum
     */
    enum EMG
    {
        EMG_TOP = 3,
        EMG_TOP_LEFT = 2,
        EMG_TOP_RIGHT = 4,
        EMG_MID_LEFT = 1,
        EMG_MID_RIGHT = 5,
        EMG_DOWN_LEFT = 0,
        EMG_DOWN_RIGHT = 6,
        EMG_DOWN = 7
    };
    
    /*!
     * \brief The EulerAngles struct
     * \tparam T
     */
    template < class T >
    struct EulerAngles
    {
        T mRoll { 0 }  //! roll angle
        , mPitch{ 0 }  //! pitch angle
        , mYaw  { 0 }; //! yaw angle
        
        /*!
         * \brief EulerAngles
         */
        EulerAngles(){}
        
        /*!
         * \brief EulerAngles
         * \param roll
         * \param pitch
         * \param yaw
         */
        EulerAngles(T roll,T pitch,T yaw)
        :mRoll(roll)
        ,mPitch(pitch)
        ,mYaw(yaw)
        {
            
        }
        
        /*!
         * \brief roll
         * \return roll factor
         */
        T roll() const
        {
            return mRoll;
        }
        
        /*!
         * \brief pitch
         * \return pitch factor
         */
        T pitch() const
        {
            return mPitch;
        }
        
        /*!
         * \brief yaw
         * \return yaw factor
         */
        T yaw() const
        {
            return mYaw;
        }
        
        /*!
         * \brief clear, put all values to 0
         */
        void clear()
        {
            mRoll  = 0;
            mPitch = 0;
            mYaw   = 0;
        }
        
        /*!
         * \brief toString
         * \return informations about the euler's angles
         */
        std::string toString()
        {
            return "Angles { roll: "  + std::to_string(mRoll) +
            " pitch: " + std::to_string(mPitch)+
            " yaw: "   + std::to_string(mYaw)  +
            " }";
        }
    };
    
    template < class T >
    EulerAngles< T > quaternionToEulerAngles(const myo::Quaternion<T>& quat)
    {
        EulerAngles<float> out
        (
         //roll
         std::atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                    1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y())),
         //Pitch
         std::asin(std::max(-1.0f, std::min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x())))),
         //Yaw
         std::atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                    1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()))
         );
        return out;
    }
    
    /*!
     * \brief The RawDatas class
     * \tparam T type of emg values
     * \tparam J type of components of gyroscope
     * \tparam H type of components of accelerometer
     * \tparam X type of components of quaternion
     * \tparam N the number of the emg sensors
     */
    template < class T,
    class J,
    class H,
    class X,
    const size_t N = 8 >
    class RawDatas
    {
    protected:
        
        double mTime;               //! time when sample was taken
        std::array< T , N > mEmg;   //! emg values
        Vector3< J > mGyro;         //! gyroscope values
        Vector3< H > mAccel;        //! accelerometer values
        Quaternion< X > mQuad;      //! quaternion values
        
    public:
        
        /*!
         * \brief RawDatas
         */
        RawDatas()
        {
            mTime = 0.0;
            mEmg.fill(0);
        }
        
        /*!
         * \brief clear
         */
        void clear()
        {
            mTime = 0.0;
            mEmg.fill(0);
            mGyro = Vector3< J >();
            mAccel = Vector3< H >();
            mQuad  = Quaternion< X >();
        }
        
        /*!
         * \brief setTime
         * \param time
         */
        void setTime(double time)
        {
            mTime = time;
        }
        
        /*!
         * \brief getTime
         * \return time when sample was taken
         */
        double getTime() const
        {
            return mTime;
        }
        
        /*!
         * \brief setEmg
         * \param values
         */
        void setEmg(const std::array< T , N >& values)
        {
            mEmg = values;
        }
        
        /*!
         * \brief setEmg
         * \param i
         * \param value
         */
        void setEmg(size_t i, T value)
        {
            mEmg[i]=value;
        }
        
        /*!
         * \brief setEmgCArray
         * \param array
         * \param n
         */
        void setEmgCArray(const T* array, size_t n)
        {
            assert(n <= N);
            std::memcpy(mEmg.data(), array, sizeof(T) * n);
        }
        
        /*!
         * \brief setGyroscope
         * \param gyroscope
         */
        void setGyroscope(const Vector3< J >& gyroscope)
        {
            mGyro=gyroscope;
        }
        
        /*!
         * \brief setAccelerometer
         * \param accelerometer
         */
        void setAccelerometer(const Vector3< H >& accelerometer)
        {
            mAccel=accelerometer;
        }
        
        /*!
         * \brief setQuaternion
         * \param quaternion
         */
        void setQuaternion(const Quaternion< X >& quaternion)
        {
            mQuad=quaternion;
        }
        
        /*!
         * \brief getEmg
         * \return emg values
         */
        const std::array< T , N >& getEmg() const
        {
            return mEmg;
        }
        
        /*!
         * \brief getEmg
         * \return emg values
         */
        std::array< T , N >& getEmg()
        {
            return mEmg;
        }
        
        /*!
         * \brief getGyroscope
         * \return gyroscope values
         */
        const Vector3< J >& getGyroscope() const
        {
            return mGyro;
        }
        
        /*!
         * \brief getGyroscope
         * \return gyroscope values
         */
        Vector3< J >& getGyroscope()
        {
            return mGyro;
        }
        
        /*!
         * \brief getAccelerometer
         * \return accelerometer values
         */
        const Vector3< H >& getAccelerometer() const
        {
            return mAccel;
        }
        
        /*!
         * \brief getAccelerometer
         * \return accelerometer values
         */
        Vector3< H >& getAccelerometer()
        {
            return mAccel;
        }
        
        /*!
         * \brief getQuaternion
         * \return quaternion value
         */
        const Quaternion< X >& getQuaternion() const
        {
            return mQuad;
        }
        
        /*!
         * \brief getQuaternion
         * \return quaternion value
         */
        Quaternion< X >& getQuaternion()
        {
            return mQuad;
        }
        
        /*!
         * \brief getEulerAngles
         * \return euler's angles
         */
        EulerAngles< X > getEulerAngles() const
        {
            return quaternionToEulerAngles(mQuad);
        }
        
        /*!
         * \brief toString
         * \return information about this object
         */
        std::string toString()
        {
            std::string str("Time: ");
            str+=std::to_string(mTime);
            
            str+="\nEmg data: ";
            for(auto emg : mEmg )
            {
                str += std::to_string( emg ) + " " ;
            }
            
            str+="\nGyroscope: "+
            std::to_string(mGyro.x())+ " "+
            std::to_string(mGyro.y())+ " "+
            std::to_string(mGyro.z())+ "\n"
            "Accelerometer: "+
            std::to_string(mAccel.x())+ " "+
            std::to_string(mAccel.y())+ " "+
            std::to_string(mAccel.z())+ "\n"
            "Quaternion: "+
            std::to_string(mQuad.x())+ " "+
            std::to_string(mQuad.y())+ " "+
            std::to_string(mQuad.z())+ " "+
            std::to_string(mQuad.w())+ "\n";
            
            return str;
        }
        
        /*!
         * \brief serialize
         * \param file
         */
        void serialize(FILE* file) const
        {
            //write time
            std::fwrite(&mTime,sizeof(double),1,file);
            //write emg
            std::fwrite((void*)mEmg.data(),sizeof(mEmg[0]),mEmg.size(),file);
            //write gyroscope
            {
                J x = mGyro.x(),
                y = mGyro.y(),
                z = mGyro.z();
                std::fwrite(&x,sizeof(x),1,file);
                std::fwrite(&y,sizeof(y),1,file);
                std::fwrite(&z,sizeof(z),1,file);
            }
            //write accelerometer
            {
                H x = mAccel.x(),
                y = mAccel.y(),
                z = mAccel.z();
                std::fwrite(&x,sizeof(x),1,file);
                std::fwrite(&y,sizeof(y),1,file);
                std::fwrite(&z,sizeof(z),1,file);
            }
            //write quaternion
            {
                X x = mQuad.x(),
                y = mQuad.y(),
                z = mQuad.z(),
                w = mQuad.w();
                std::fwrite(&x,sizeof(x),1,file);
                std::fwrite(&y,sizeof(y),1,file);
                std::fwrite(&z,sizeof(z),1,file);
                std::fwrite(&w,sizeof(z),1,file);
            }
        }
        
        /*!
         * \brief deserialize
         * \param file
         */
        void deserialize(FILE* file)
        {
            //write time
            std::fread(&mTime,sizeof(double),1,file);
            //write emg
            fread(mEmg.data(),sizeof(mEmg[0]),mEmg.size(),file);
            //write gyroscope
            {
                J x = 0 , y = 0, z = 0;
                std::fread(&x,sizeof(x),1,file);
                std::fread(&y,sizeof(y),1,file);
                std::fread(&z,sizeof(z),1,file);
                mGyro = Vector3< J > ( x,y,z );
            }
            //write accelerometer
            {
                H  x = 0 , y = 0, z = 0;
                std::fread(&x,sizeof(x),1,file);
                std::fread(&y,sizeof(y),1,file);
                std::fread(&z,sizeof(z),1,file);
                mAccel = Vector3< H > ( x,y,z );
            }
            //write quaternion
            {
                X x = 0 , y = 0, z = 0, w = 0;
                std::fread(&x,sizeof(x),1,file);
                std::fread(&y,sizeof(y),1,file);
                std::fread(&z,sizeof(z),1,file);
                std::fread(&w,sizeof(z),1,file);
                mQuad = Quaternion< H > ( x,y,z,w );
            }
        }
    };
    
    /*!
     * \brief The ArmStatus struct
     */
    struct ArmStatus
    {
        bool mUnlock { false }, //! myo is unloked
        mSync   { false }; //! myo is synced
        
        myo::Arm mArm{ myo::Arm::armUnknown }; //! last classification by myo library
        
        /*!
         * \brief clear
         */
        void clear()
        {
            mUnlock = false;
            mSync   = false;
            mArm    = myo::Arm::armUnknown;
        }
        
        /*!
         * \brief setArm
         * \param arm
         */
        void setArm(myo::Arm arm)
        {
            mArm = arm;
        }
        
        /*!
         * \brief sync
         */
        void sync()
        {
            mSync = true;
        }
        
        /*!
         * \brief unsync
         */
        void unsync()
        {
            mSync = false;
        }
        
        /*!
         * \brief lock
         */
        void lock()
        {
            mUnlock = false;
        }
        
        /*!
         * \brief unlock
         */
        void unlock()
        {
            mUnlock = true;
        }
        
        /*!
         * \brief isLeft
         * \return true if left is classified
         */
        bool isLeft() const
        {
            return mArm == myo::Arm::armLeft;
        }
        
        /*!
         * \brief isRight
         * \return true if right is classified
         */
        bool isRight() const
        {
            return mArm == myo::Arm::armRight;
        }
        
        /*!
         * \brief isRight
         * \return true if classification is failed
         */
        bool isUnknown() const
        {
            return mArm == myo::Arm::armUnknown;
        }
        
        /*!
         * \brief isSync
         * \return true if myo is synced
         */
        bool isSync() const
        {
            return mSync;
        }
        
        /*!
         * \brief isUnlock
         * \return true if myo is unloked
         */
        bool isUnlock() const
        {
            return mUnlock;
        }
        
        
        /*!
         * \brief isLock
         * \return true if myo is locked
         */
        bool isLock() const
        {
            return !mUnlock;
        }
        
        /*!
         * \brief toString
         * \return string information about object
         */
        std::string toString()
        {
            return
            std::string("Arm status{ ")
            + (mSync ? "sync, " : "unsync, " )
            + (mUnlock ? "unlock, " : "lock, " )
            + (mArm == myo::armLeft ? "left" :
               (mArm == myo::armRight ? "right": "unknown" ))
            + " }";
            
        }
    };
};

#endif
