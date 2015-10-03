//
//  RBFImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 02/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "RBFImp.h"
#include "RBFNetwork/RBFNetwork.h"

namespace myorbf
{
    
void create_model(const std::string& path)
{

}
    
void* myo_classification(const std::string& path,MyoThread& myo,DataFlags&  flags,ClassesNames& cnames)
{
    //alloc dataset
    std::shared_ptr< Dataset > dataset=Utility::ReadDataSet(path);
    //traning
    auto model = new RBFNetwork(dataset->mDatas, dataset->mLabels, (int)dataset->mCCount);
    //mse
    double mse = 0.0;
    //traning
    double accuracy=
    model->startTraining((int)dataset->mCCount,
                         0.5,
                         50000,
                         mse,
                         true,
                         true);
    //print mse
    std::cout << "RBFNetwork accuracy="<< accuracy <<", mse: " << mse << "\n";
    //add function
    myo.start([&,model](const MyoThread::Inputs& inputs,
                        const DataFlags& falgs,
                        std::mutex& mutex)
              {
                  if(!inputs.size()) return;
                  //loop constants
                  size_t rowSize = falgs.lineSize<8>() / flags.mReps;
                  size_t allSize = rowSize*inputs.size();
                  datapoint buffer(allSize);
                  //put values
                  MyoThread::applay(inputs,
                                    flags,
                                    [&](size_t i,size_t ri, double value)
                                    {
                                        buffer[i] = value;
                                    });
                  //classifay
                  double error;
                  int classes = model->predictLabel(buffer, error);
                  //print
                  std::cout << "-----------------------------------------------\n";
                  std::cout << "arm in in status: " << classes << " ("<< error << ")\n";
                  std::cout << "-----------------------------------------------\n";
                  
                  
              },flags);
    
    return model;
}

void  myo_classification_free(void* model)
{
    delete (RBFNetwork*)model;
}

};