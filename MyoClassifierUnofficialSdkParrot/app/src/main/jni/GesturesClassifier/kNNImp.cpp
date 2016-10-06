//
//  kNNImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 04/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <climits>
#include "kNNImp.h"
#include "kNN/kNN.h"

class MyoModelkNN : public MyoModelInterface
{
public:
    
    int mMinRowPerClass { 1 };
    DataSetReader mData;
    std::unique_ptr<kNN> mModel { nullptr };
    
    MyoModelkNN(kNN* model = nullptr)
    :mModel(model)
    {
    }
    
    MyoModelkNN(const DataSetReader& data)
    {
        //sava dataset
        mData  = data;
        //map of count row per class
        std::map< double, int > mapCount;
        //count..
        for(auto it : data)
        {
            mapCount[it.mClass] += 1;
        }
        //serach min
        mMinRowPerClass = INT_MAX;
        for(auto it:mapCount)
        {
            mMinRowPerClass = std::max(1,std::min(it.second,mMinRowPerClass));
        }
        //create model
        mModel = std::unique_ptr<kNN>(new kNN(data.getRows()));
    }
    
    virtual ~MyoModelkNN()
    {
    }
    
    virtual void serialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "wb");
        
        if(file)
        {
            fprintf(file, "MinRowPerClass: %d\n", mMinRowPerClass);
            mData.serialize(file);
            fclose(file);
        }
    }
    
    virtual void deserialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "rb");
        
        if(file)
        {
            fscanf(file, "MinRowPerClass: %d\n", &mMinRowPerClass);
            //deserialize
            mData.deserialize(file);
            //close file
            fclose(file);
            //create model
            mModel = std::unique_ptr<kNN>(new kNN(mData.getRows()));
        }
    }
};

//implement interface
MyoClassifierkNN::~MyoClassifierkNN()
{
}


MyoModelInterface* MyoClassifierkNN::createModel(const DataSetReader& ds)
{
    mModel = std::make_shared<MyoModelkNN>(ds);
    return mModel.get();
}

MyoModelInterface* MyoClassifierkNN::createModel(const DataSetReader& ds,const std::string& args)
{
    mModel = std::make_shared<MyoModelkNN>(ds);
    return mModel.get();
}

MyoModelInterface* MyoClassifierkNN::modelFromfile(const std::string& path)
{
    mModel = std::make_shared<MyoModelkNN>();
    mModel->deserialize(path);
    return mModel.get();
}

void MyoClassifierkNN::setModel(MyoModelInterface* model)
{
    mModel = std::shared_ptr<MyoModelkNN>((MyoModelkNN*)model);
}

void MyoClassifierkNN::setProbabilityFilter(double probability)
{
    mProbabilityFilter = probability;
}

void MyoClassifierkNN::classification(MyoThread& myo,
                                      const std::function< void (const std::string& className) > callback)
{
    myo.start([this,callback](const MyoThread::Inputs& inputs,
                              const DataFlags& flags,
                              std::mutex& mutex)
              {
                  if(!inputs.size()) return;
                  //loop constants
                  size_t rowSize = flags.lineSize<8>() / flags.mReps;
                  size_t allSize = rowSize*inputs.size();
                  //alloc row
                  kNN::DataRaw row(allSize);
                  //put values
                  MyoThread::applay(inputs,
                                    flags,
                                    [&](size_t i,size_t ri, double value)
                                    {
                                        row[i] = value;
                                    });
                  //do classification
                  kNN::Result res = mModel->
                                    mModel->classify(row,
                                                     std::min(5,mModel->mMinRowPerClass),
                                                     kNN::EUCLIDE_DISTANCE,
                                                     kNN::ONE_ON_DISTANCE);
                  
                  //filters
                  if(mProbabilityFilter <= (1.0-res.mError))
                  if(mLastClass != res.mClasses)
                  {
                      //callback
                      callback(mModel->mData.getClassNames().getClassName(res.mClasses));
                      mLastClass = res.mClasses;
                  }
              },
              mModel->mData.getFlags(),
              mModel->mData.getUpdate());
}


