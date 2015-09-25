//
//  Utilities.h
//  MyoWekaGenFiles
//
//  Created by Daniele on 26/06/15.
//  Copyright (c) 2015 Daniele. All rights reserved.
//
#ifndef MYO_UTILITIES_H
#define MYO_UTILITIES_H

#include <assert.h>
#include <myo.hpp>
#include <cstring>

namespace myo
{
    template < class T >
    struct EulerAngles
    {
        T mRoll{ 0 }, mPitch{ 0 }, mYaw{ 0 };
        
        EulerAngles(){}
        
        EulerAngles(T roll,T pitch,T yaw)
        :mRoll(roll)
        ,mPitch(pitch)
        ,mYaw(yaw)
        {
            
        }
        
        T roll() const
        {
            return mRoll;
        }
        
        T pitch() const
        {
            return mPitch;
        }
        
        T yaw() const
        {
            return mYaw;
        }
        
        void clear()
        {
            mRoll  = 0;
            mPitch = 0;
            mYaw   = 0;
        }
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
    
    template < class T,
               class J,
               class H,
               class X,
               const size_t N = 8 >
    class RawDatas
    {
    protected:
        
        std::array< T , N > mEmg;
        Vector3< J > mGyro;
        Vector3< H > mAccel;
        Quaternion< X > mQuad;
        
    public:
        
        RawDatas()
        {
            mEmg.fill(0);
        }
        
        void clear()
        {
            mEmg.fill(0);
            mGyro = Vector3< J >();
            mAccel = Vector3< H >();
            mQuad  = Quaternion< X >();
        }
        
        void setEmg(const std::array< T , N >& value)
        {
            mEmg = value;
        }
        
        const std::array< T , N >& getEmg() const
        {
            return mEmg;
        }
        
        void setEmgCArray(const T* array, size_t n)
        {
            assert(n <= N);
            std::memcpy(mEmg.data(), array, sizeof(T) * n);
        }
        
        void setGyroscope(const Vector3< J >& gyroscope)
        {
            mGyro=gyroscope;
        }
        
        void setAccelerometer(const Vector3< H >& accelerometer)
        {
            mAccel=accelerometer;
        }

        void setQuaternion(const Quaternion< X >& quaternion)
        {
            mQuad=quaternion;
        }
        
        const Vector3< J >& getGyroscope() const
        {
            return mGyro;
        }
        
        const Vector3< H >& getAccelerometer() const
        {
            return mAccel;
        }

        const Quaternion< X >& getQuaternion() const
        {
            return mQuad;
        }

        const EulerAngles< X >& getEulerAngles() const
        {
            return quaternionToEulerAngles(mQuad);
        }

        std::string toString()
        {
            std::string str("Emg data: ");
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
        
    };
    
    struct ArmStatus
    {
        bool mUnlock { false },
        mSync   { false };
        
        myo::Arm mArm{ myo::Arm::armUnknown };
        
        void clear()
        {
            mUnlock = false;
            mSync   = false;
            mArm    = myo::Arm::armUnknown;
        }
        
        void setArm(myo::Arm arm)
        {
            mArm = arm;
        }
        
        void sync()
        {
            mSync = true;
        }
        
        void unsync()
        {
            mSync = false;
        }
        
        void lock()
        {
            mUnlock = false;
        }
        
        void unlock()
        {
            mUnlock = true;
        }
        
        bool isLeft() const
        {
            return mArm == myo::Arm::armLeft;
        }
        
        bool isRight() const
        {
            return mArm == myo::Arm::armRight;
        }
        
        bool isUnknown() const
        {
            return mArm == myo::Arm::armUnknown;
        }
        
        bool isSync() const
        {
            return mSync;
        }
        
        bool isUnlock() const
        {
            return mUnlock;
        }
        
        bool isLock() const
        {
            return !mUnlock;
        }
        
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
