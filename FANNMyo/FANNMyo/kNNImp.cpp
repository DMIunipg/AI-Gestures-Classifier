//
//  kNNImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 04/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "kNNImp.h"
#include "kNN/kNN.h"
namespace myoknn
{

void create_model(const std::string& path)
{
    //none
}
void* myo_classification(const std::string& path,MyoThread& myo,DataFlags&  flags,ClassesNames& cnames)
{
    kNN* knn = new kNN(path);
    
    //add function
    myo.start([&,knn](const MyoThread::Inputs& inputs,
                      const DataFlags& flags,
                      std::mutex& mutex)
              {
                  //compute size
                  size_t rowSize = flags.lineSize<8>() / flags.mReps;
                  size_t allSize = rowSize*inputs.size();
                  //alloc row
                  kNN::DataRow row(allSize);
                  //put values
                  MyoThread::applay(inputs,
                                    flags,
                                    [&](size_t i,size_t ri, double value)
                                    {
                                        row[i] = value;
                                    });
                  //do classification
                  kNN::Result res = knn->classify(row,
                                                  3,
                                                  kNN::EUCLIDE_DISTANCE,
                                                  kNN::ONE_ON_DISTANCE);
                  //print res
                  std::cout << "-----------------------------------------------\n";
                  std::cout << "arm in in status: " << cnames.getClassName(res.mClasses);
                  std::cout << " ( error: " << res.mError << " ) \n";
                  std::cout << "-----------------------------------------------\n";
              },
              flags);
    
    return knn;
}
void  myo_classification_free(void* knn)
{
    delete (kNN*)(knn);
}

};
