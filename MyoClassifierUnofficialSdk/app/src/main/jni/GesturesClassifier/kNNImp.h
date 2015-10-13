//
//  kNNImp.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 04/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef kNNImp_h
#define kNNImp_h

#include <string>
#include <memory>
#include <MyoThread.h>
#include <DataFlags.h>
#include <MyoClassifierInterface.h>


class MyoModelkNN;
class MyoClassifierkNN : public MyoClassifierInterface
{
    double mProbabilityFilter {  0.0 };
    double mLastClass         { -1.0 };
    std::shared_ptr<MyoModelkNN> mModel;
    
public:
    
    virtual ~MyoClassifierkNN();
    virtual MyoModelInterface* createModel(const DataSetReader& ds);
    virtual MyoModelInterface* modelFromfile(const std::string& path);
    virtual void setModel(MyoModelInterface* model);
    virtual void setProbabilityFilter(double probability =0.0);
    virtual void classification(MyoThread& myo,const std::function< void (const std::string& className) >);
};

#endif /* kNNImp_hpp */
