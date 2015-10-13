//
// Created by Gabriele Di Bari on 12/10/15.
//

#ifndef MYOCLASSIFIER_MYONATIVELISTENER_H
#define MYOCLASSIFIER_MYONATIVELISTENER_H

#include <jni.h>

/**

    public native void onAttach(long l);
    public native void onDetach(long l);
    public native void onConnect(long l);
    public native void onDisconnect(long l);
    public native void onArmSync(long l);
    public native void onArmUnsync(long l);
    public native void onUnlock(long l);
    public native void onLock(long l);
    public native void onPose(long l, int pose);
    public native void onQuaternion(long l, double x, double y, double z, double w);
    public native void onAccelerometer(long l, double x, double y, double z);
    public native void onGyroscope(long l, double x, double y, double z);

 * */

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onAttach(JNIEnv* env,
                                                                               jobject thiz,
                                                                               jlong l);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onDetach(JNIEnv* env,
                                                                               jobject thiz,
                                                                               jlong l);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onConnect(JNIEnv* env,
                                                                                jobject thiz,
                                                                                jlong l);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onDisconnect(JNIEnv* env,
                                                                                   jobject thiz,
                                                                                   jlong l);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onArmSync(JNIEnv* env,
                                                                                jobject thiz,
                                                                                jlong l);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onArmUnsync(JNIEnv* env,
                                                                                  jobject thiz,
                                                                                  jlong l);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onUnlock(JNIEnv* env,
                                                                               jobject thiz,
                                                                               jlong l);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onLock(JNIEnv* env,
                                                                             jobject thiz,
                                                                             jlong l);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onPose(JNIEnv* env,
                                                                             jobject thiz,
                                                                             jlong l,
                                                                             jint pose);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onQuaternion(JNIEnv* env,
                                                                                   jobject thiz,
                                                                                   jlong l,
                                                                                   jdouble x,
                                                                                   jdouble y,
                                                                                   jdouble z,
                                                                                   jdouble w);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onAccelerometer(JNIEnv* env,
                                                                                      jobject thiz,
                                                                                      jlong l,
                                                                                      jdouble x,
                                                                                      jdouble y,
                                                                                      jdouble z);

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onGyroscope(JNIEnv* env,
                                                                                  jobject thiz,
                                                                                  jlong l,
                                                                                  jdouble x,
                                                                                  jdouble y,
                                                                                  jdouble z);

#endif //MYOCLASSIFIER_MYONATIVELISTENER_H
