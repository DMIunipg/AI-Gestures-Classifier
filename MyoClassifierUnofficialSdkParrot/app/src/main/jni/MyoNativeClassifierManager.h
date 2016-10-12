//
// Created by Gabriele Di Bari on 13/10/15.
//

#ifndef MYOCLASSIFIER_MYONATIVECLASSIFIERMANAGER_H
#define MYOCLASSIFIER_MYONATIVECLASSIFIERMANAGER_H

#include <jni.h>

extern "C"
{

/*
    protected long native myoClassifierManagerInit(int type);
    protected void native myoClassifierManagerLoadModel(String path, long pointer);
    protected native void myoClassifierManagerProbabilityFilter(double probability, long pointer);
    protected void native myoClassifierManagerCallback(ClassifierListener callback, long pointer);
    protected void native myoClassifierManagerFree(long pointer);
*/
JNIEXPORT jlong JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerInit(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jint type);

JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerChangeClassifierType(JNIEnv *env,
                                                                                                      jobject thiz,
                                                                                                      jint type,
                                                                                                      jlong pointer);
JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerLoadModel(JNIEnv *env,
                                                                                      jobject thiz,
                                                                                      jstring path,
                                                                                      jlong pointer);

JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerProbabilityFilter(JNIEnv *env,
                                                                                              jobject thiz,
                                                                                              jdouble probability,
                                                                                              jlong pointer);

JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerCallback(JNIEnv *env,
                                                                                     jobject thiz,
                                                                                     jobject listener,
                                                                                     jlong pointer);
JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerFree(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jlong pointer);

};

#endif //MYOCLASSIFIER_MYONATIVECLASSIFIERMANAGER_H
