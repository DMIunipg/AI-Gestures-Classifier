//
// Created by Gabriele Di Bari on 12/10/15.
//
#include "MyoNativeListener.h"
#include "MyoThread.h"
//static values
static MyoDataCollector tmpMyoDataCollector;
//return last data collected
const MyoDataCollector& myo::getMyoDataCollector()
{
    return tmpMyoDataCollector;
}

extern "C"
{


JNIEXPORT void JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onEmg(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jint emg0,
                                                                            jint emg1,
                                                                            jint emg2,
                                                                            jint emg3,
                                                                            jint emg4,
                                                                            jint emg5,
                                                                            jint emg6,
                                                                            jint emg7)
{
    const int8_t emgs[] =
    {
            (int8_t)emg0,
            (int8_t)emg1,
            (int8_t)emg2,
            (int8_t)emg3,
            (int8_t)emg4,
            (int8_t)emg5,
            (int8_t)emg6,
            (int8_t)emg7
    };
    tmpMyoDataCollector.onEmgData(nullptr,0,emgs);
}


JNIEXPORT void JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onQuaternion(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jdouble x,
                                                                                   jdouble y,
                                                                                   jdouble z,
                                                                                   jdouble w)
{
    tmpMyoDataCollector.onOrientationData(nullptr,0,myo::Quaternion<float>(x,y,z,w));
}

JNIEXPORT void JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onAccelerometer(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jdouble x,
                                                                                      jdouble y,
                                                                                      jdouble z)
{
    tmpMyoDataCollector.onAccelerometerData(nullptr,0,myo::Vector3<float>(x,y,z));
}

JNIEXPORT void JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onGyroscope(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jdouble x,
                                                                                  jdouble y,
                                                                                  jdouble z)
{
    tmpMyoDataCollector.onGyroscopeData(nullptr,0,myo::Vector3<float>(x,y,z));
}

};