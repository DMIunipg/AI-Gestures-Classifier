//
//  SVMImp.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 29/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef SVMImp_h
#define SVMImp_h


#include <string>
#include <memory>
#include <MyoThread.h>
#include <DataFlags.h>
#include <MyoClassifierInterface.h>


class MyoModelSVM;
class MyoClassifierSVM : public MyoClassifierInterface
{
    
    double mProbabilityFilter {  0.0 };
    double mLastClass         { -1.0 };
    std::shared_ptr<MyoModelSVM> mModel;

public:
    
    virtual ~MyoClassifierSVM();
    virtual MyoModelInterface* createModel(const DataSetReader& ds);
    virtual MyoModelInterface* modelFromfile(const std::string& path);
    virtual void setModel(MyoModelInterface* model);
    virtual void setProbabilityFilter(double probability =0.0);
    virtual void classification(MyoThread& myo,const std::function< void (const std::string& className) >);
};


#endif /* SVMImp_hpp */
