//
//  MyoClassifierInterface.h
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 10/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef MyoClassifierInterface_h
#define MyoClassifierInterface_h

#include <string>
#include <cstdio>
#include <functional>
#include <MyoThread.h>
#include <DataSetReader.h>
#include <MyoModelInterface.h>

class MyoClassifierInterface
{
    
public:
    
    virtual ~MyoClassifierInterface() {};
    virtual MyoModelInterface* createModel(const DataSetReader& ds) = 0;
    virtual MyoModelInterface* modelFromfile(const std::string& path) = 0;
    virtual void setModel(MyoModelInterface* model) = 0;
    virtual void setProbabilityFilter(double probability =0.0) = 0;
    virtual void classification(MyoThread& myo,const std::function< void (const std::string& className) >) = 0;

};

#endif /* MyoClassifierInterface_h */
