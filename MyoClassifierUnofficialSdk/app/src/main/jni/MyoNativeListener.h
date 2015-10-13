//
// Created by Gabriele Di Bari on 12/10/15.
//

#ifndef MYOCLASSIFIER_MYONATIVELISTENER_H
#define MYOCLASSIFIER_MYONATIVELISTENER_H

#include <jni.h>
#include <DataFlags.h>

// forward declaration
class MyoDataCollector;
/**
 * Return myo data rec.
 */
namespace myo {
    const MyoDataCollector& getMyoDataCollector();
}
/**

    private native void onEmg(int emg0,
                              int emg1,
                              int emg2,
                              int emg3,
                              int emg4,
                              int emg5,
                              int emg6,
                              int emg7);
    private native void onQuaternion(double x, double y, double z, double w);
    private native void onAccelerometer(double x, double y, double z);
    private native void onGyroscope(double x, double y, double z);

 * */



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
                                                                            jint emg7);


JNIEXPORT void JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onQuaternion(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jdouble x,
                                                                                   jdouble y,
                                                                                   jdouble z,
                                                                                   jdouble w);

JNIEXPORT void JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onAccelerometer(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jdouble x,
                                                                                      jdouble y,
                                                                                      jdouble z);

JNIEXPORT void JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onGyroscope(JNIEnv *env,
                                                                                  jobject thiz,
                                                                                  jdouble x,
                                                                                  jdouble y,
                                                                                  jdouble z);

};

#endif //MYOCLASSIFIER_MYONATIVELISTENER_H
