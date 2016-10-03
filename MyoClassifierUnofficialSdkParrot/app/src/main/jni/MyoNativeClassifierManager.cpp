//
// Created by Gabriele Di Bari on 13/10/15.
//

#include <MyoClassifierManager.h>
#include <memory>
#include "MyoNativeClassifierManager.h"

//safe java object listener
class JavaListener
{
public:

    JavaVM* mJvm  = nullptr;
    jobject mRef = nullptr;

    JavaListener(JavaVM* jvm,jobject ref)
    {
        mJvm = jvm;
        mRef = ref;
    }

    virtual ~JavaListener()
    {
        JNIEnv *env;
        mJvm->AttachCurrentThread(&env, NULL);
        env->DeleteGlobalRef(mRef);
        mJvm->DetachCurrentThread();
    }
};

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerInit(JNIEnv *env,
                                                                                       jobject thiz,
                                                                                       jint type)
{
    return (jlong) new MyoClassifierManager((Classifier) type);
}

JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerLoadModel(JNIEnv *env,
                                                                                            jobject thiz,
                                                                                            jstring jPath,
                                                                                            jlong pointer)
{
    //alloc native string
    const char* path = env->GetStringUTFChars(jPath,0);
    ((MyoClassifierManager*)pointer)->loadModel(path);
    //dealloc native string
    env->ReleaseStringUTFChars(jPath, path);
}

JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerProbabilityFilter(JNIEnv *env,
                                                                                                    jobject thiz,
                                                                                                    jdouble probability,
                                                                                                    jlong pointer)
{
    ((MyoClassifierManager*)pointer)->setProbabilityFilter(probability);
}

JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerCallback(JNIEnv *env,
                                                                                           jobject thiz,
                                                                                           jobject arglistener,
                                                                                           jlong pointer)
{
    //get virtual machine
    JavaVM* jvm = nullptr;
    env->GetJavaVM(&jvm);
    //create ref
    std::shared_ptr < JavaListener > listener = std::make_shared<JavaListener>(jvm,
                                                                               env->NewGlobalRef(arglistener));
    //get class
    jclass cls = env->GetObjectClass(listener->mRef);
    //get id method
    jmethodID clsMID = env->GetMethodID( cls, "classified", "(Ljava/lang/String;)V");
    //reg callback
    ((MyoClassifierManager*)pointer)->classification([listener,clsMID](const std::string& className)
     {
         //attach to jvm thread
         JNIEnv *env;
         listener->mJvm->AttachCurrentThread(&env, NULL);
         //alloc string
         jstring jstr = env->NewStringUTF(className.c_str());
         //callback
         env->CallVoidMethod(listener->mRef,clsMID,jstr);
         //dt to jvm thread
         listener->mJvm->DetachCurrentThread();
     });

}

JNIEXPORT void JNICALL
Java_com_unipg_myoclassifierparrot_MyoNativeClassifierManager_myoClassifierManagerFree(JNIEnv *env,
                                                                                       jobject thiz,
                                                                                       jlong pointer)
{
    delete ((MyoClassifierManager*)pointer);
}

};