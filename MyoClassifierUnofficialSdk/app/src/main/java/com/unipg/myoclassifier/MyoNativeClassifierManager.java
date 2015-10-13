package com.unipg.myoclassifier;

/**
 * Created by Gabriele on 13/10/15.
 */
public class MyoNativeClassifierManager {

    public static int SVM = 0;
    public static int kNN = 1;
    public static int RBFNETWORK = 2;

    public interface ClassifierListener {
        public void classified(String className);
    }

    protected long mPtrManager = 0;
    protected native long myoClassifierManagerInit(int type);
    protected native void myoClassifierManagerLoadModel(String path, long pointer);
    protected native void myoClassifierManagerProbabilityFilter(double probability, long pointer);
    protected native void myoClassifierManagerCallback(ClassifierListener callback, long pointer);
    protected native void myoClassifierManagerFree(long pointer);

    MyoNativeClassifierManager(int type){
        mPtrManager = this.myoClassifierManagerInit(type);
    }

    void loadModel(String path){
        this.myoClassifierManagerLoadModel(path,mPtrManager);
    }

    void setListener(ClassifierListener callback){
        this.myoClassifierManagerCallback(callback, mPtrManager);
    }
    void setProbabilityFilter(double probability){
        this.myoClassifierManagerProbabilityFilter(probability, mPtrManager);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        this.myoClassifierManagerFree(mPtrManager);
    }
}
