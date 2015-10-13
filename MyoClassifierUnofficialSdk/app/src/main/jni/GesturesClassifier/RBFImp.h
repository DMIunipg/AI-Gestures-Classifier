//
//  RBFImp.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 02/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RBFImp_h
#define RBFImp_h

#include <string>
#include <memory>
#include <MyoThread.h>
#include <DataFlags.h>
#include <MyoClassifierInterface.h>


class MyoModelRBFNetwork;
class MyoClassifierRBFNetwork : public MyoClassifierInterface
{
    
    double mProbabilityFilter {  0.0 };
    double mLastClass         { -1.0 };
    std::shared_ptr<MyoModelRBFNetwork> mModel;
    
public:
    
    virtual ~MyoClassifierRBFNetwork();
    virtual MyoModelInterface* createModel(const DataSetReader& ds);
    virtual MyoModelInterface* modelFromfile(const std::string& path);
    virtual void setModel(MyoModelInterface* model);
    virtual void setProbabilityFilter(double probability =0.0);
    virtual void classification(MyoThread& myo,const std::function< void (const std::string& className) >);
};
#endif /* RBFImp_hpp */
