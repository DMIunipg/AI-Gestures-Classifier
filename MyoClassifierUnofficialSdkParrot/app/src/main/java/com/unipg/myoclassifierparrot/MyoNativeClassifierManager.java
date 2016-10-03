package com.unipg.myoclassifierparrot;

/**
 * Created by Gabriele on 13/10/15.
 */
public class MyoNativeClassifierManager {

    public static int SVM = 0;
    public static int kNN = 1;
    public static int RBFNETWORK = 2;

    /**
     * ClassifierListener, interface for callback listener
     */
    public interface ClassifierListener {
        /**
         * @brief classified, method called when is classified a new gestur 
         * @param className, name of the class classified
         */
        public void classified(String className);
    }

    /**
     * @brief mPtrManager, save the CPP obj pointer
     */
    protected long mPtrManager = 0;

    /**
     * @brief myoClassifierManagerInit, alloc the cpp object
     * @param type of classifier (SVM, kNN, RBFNetwork)
     * @return pointer to CPP object (N.B. work on 16/32/64bit machines)
     */
    protected native long myoClassifierManagerInit(int type);

    /**
     * @brief myoClassifierManagerLoadModel
     * @param path
     * @param pointer, CPP object
     */
    protected native void myoClassifierManagerLoadModel(String path, long pointer);

    /**
     * @brief myoClassifierManagerProbabilityFilter
     * @param probability
     * @param pointer, CPP object
     */
    protected native void myoClassifierManagerProbabilityFilter(double probability, long pointer);

    /**
     * @brief myoClassifierManagerCallback
     * @param callback, add a ClassifierListener callback
     * @param pointer, CPP object
     */
    protected native void myoClassifierManagerCallback(ClassifierListener callback, long pointer);

    /**
     * @brief myoClassifierManagerFree
     * @param pointer, CPP object
     */
    protected native void myoClassifierManagerFree(long pointer);

    /**
     * @brief MyoNativeClassifierManager
     * @param type, type of classifier (SVM, kNN, RBFNetwork)
     */
    MyoNativeClassifierManager(int type){
        mPtrManager = this.myoClassifierManagerInit(type);
    }

    /**
     * @brief loadModel, load a model from a path
     * @param path [description]
     */
    void loadModel(String path){
        this.myoClassifierManagerLoadModel(path,mPtrManager);
    }

    /**
     * @brief setListener, add a ClassifierListener callback
     * @param callback 
     */
    void setListener(ClassifierListener callback){
        this.myoClassifierManagerCallback(callback, mPtrManager);
    }
    /**
     * @brief setProbability
     * @param probability
     */
    void setProbabilityFilter(double probability){
        this.myoClassifierManagerProbabilityFilter(probability, mPtrManager);
    }

    /**
     * @brief finalize, called when GC destroy this object (this method delete the CPP object)
     * @throws Throwable
     */
    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        this.myoClassifierManagerFree(mPtrManager);
    }
}
