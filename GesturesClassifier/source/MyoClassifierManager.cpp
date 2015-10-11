//
//  MyoClassifierManager.cpp
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 10/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <stdio.h>
#include <SVMImp.h>
#include <kNNImp.h>
#include <RBFImp.h>
#include <MyoClassifierManager.h>


MyoClassifierManager::MyoClassifierManager(Classifier type)
{
    mClassifier = type;
    //select classifier
    switch (mClassifier)
    {
        default:
        case CLA_SVM:        mInterface = new MyoClassifierSVM; break;
        case CLA_kNN:        mInterface = new MyoClassifierkNN; break;
        case CLA_RBFNETWORK: mInterface = new MyoClassifierRBFNetwork; break;
    }
    //alloc myo thread
    mMyoThread = new MyoThread;
}

MyoClassifierManager::~MyoClassifierManager()
{
    delete mMyoThread;
    delete mInterface;
}

MyoModelInterface* MyoClassifierManager::buildModel(const std::string& path)
{
    DataSetReader dsReader;
    dsReader.read(path);
    return mInterface->createModel(dsReader);
}

MyoModelInterface* MyoClassifierManager::loadModel(const std::string& path)
{
    return mInterface->modelFromfile(path);
}

void MyoClassifierManager::setProbabilityFilter(double probability)
{
    mInterface->setProbabilityFilter(probability);
}

void MyoClassifierManager::classification(const std::function< void (const std::string& className) > callback)
{
    mInterface->classification(*mMyoThread, callback);
}


