//
// Created by Gabriele Di Bari on 12/10/15.
//
#include "MyoNativeListener.h"


JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onAttach(JNIEnv* env,
                                                                               jobject thiz,
                                                                               jlong l)
{
    return 0;
}

JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onDeattach(JNIEnv* env,
                                                                                 jobject thiz,
                                                                                 jlong l)
{
    return 0;
}


JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onConnect(JNIEnv* env,
                                                                                jobject thiz,
                                                                                jlong l)
{
    return 0;
}


JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onDisconnect(JNIEnv* env,
                                                                                   jobject thiz,
                                                                                   jlong l)
{
    return 0;
}


JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onArmSync(JNIEnv* env,
                                                                                jobject thiz,
                                                                                jlong l)
{
    return 0;
}


JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onArmUnsync(JNIEnv* env,
                                                                                  jobject thiz,
                                                                                  jlong l)
{
    return 0;
}



JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onUnlock(JNIEnv* env,
                                                                               jobject thiz,
                                                                               jlong l)
{
    return 0;
}


JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onLock(JNIEnv* env,
                                                                             jobject thiz,
                                                                             jlong l)
{
    return 0;
}



JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onPose(JNIEnv* env,
                                                                             jobject thiz,
                                                                             jlong l,
                                                                             jint pose)
{
    return 0;
}


JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onQuaternion(JNIEnv* env,
                                                                                   jobject thiz,
                                                                                   jlong l,
                                                                                   jdouble x,
                                                                                   jdouble y,
                                                                                   jdouble z,
                                                                                   jdouble w)
{
    return 0;
}



JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onAccelerometer(JNIEnv* env,
                                                                                      jobject thiz,
                                                                                      jlong l,
                                                                                      jdouble x,
                                                                                      jdouble y,
                                                                                      jdouble z)
{
    return 0;
}



JNIEXPORT jint JNICALL Java_com_unipg_myoclassifier_MyoNativeListener_onGyroscope(JNIEnv* env,
                                                                                  jobject thiz,
                                                                                  jlong l,
                                                                                  jdouble x,
                                                                                  jdouble y,
                                                                                  jdouble z)
{
    return 0;
}


