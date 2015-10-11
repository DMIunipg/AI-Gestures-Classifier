//
//  MyoClassifierManager.hpp
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 10/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef MyoClassifierManager_h
#define MyoClassifierManager_h

#include <string>
#include <functional>
#include <MyoModelInterface.h>

enum Classifier
{
    CLA_SVM,
    CLA_kNN,
    CLA_RBFNETWORK
};

class MyoThread;
class MyoClassifierInterface;
class MyoClassifierManager
{
    
    Classifier mClassifier;
    MyoClassifierInterface* mInterface;
    MyoThread* mMyoThread;
    
public:
    
    MyoClassifierManager(Classifier type);
    ~MyoClassifierManager();
    MyoModelInterface* buildModel(const std::string& dataset);
    MyoModelInterface* loadModel(const std::string& model);
    void setProbabilityFilter(double probability =0.0);
    void classification(const std::function< void (const std::string& className) >);

};

#endif /* MyoClassifierManager.h */
