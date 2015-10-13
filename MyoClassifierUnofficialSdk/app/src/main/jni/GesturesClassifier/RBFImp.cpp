//
//  RBFImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 02/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <RBFImp.h>
#include <RBFNetwork/RBFNetwork.h>
#include <iostream>

class MyoModelRBFNetwork : public MyoModelInterface
{
public:
    
    DataSetReader mData;
    std::unique_ptr<RBFNetwork> mModel { nullptr };
    
    MyoModelRBFNetwork(RBFNetwork* model = nullptr)
    :mModel(model)
    {
    }
    
    MyoModelRBFNetwork(const DataSetReader& data)
    {
        //sava dataset
        mData = data;
        //create model
        createModel();
    }
    
    void createModel()
    {
        //rows
        std::vector< std::vector<double> > raws;
        std::vector<int> labels;
        //alloc
        raws.resize(mData.size());
        labels.resize(mData.size());
        //put values
        for(size_t r=0; r != mData.size(); ++r)
        {
            //copy all values
            raws[r] = mData[r].mRaw;
            //copy class
            labels[r] = (int)mData[r].mClass;
        }
        //get cunt of class
        int nclasses = (int)mData.getClassNames().mNames.size();
        //create model
        mModel = std::unique_ptr<RBFNetwork>(new RBFNetwork(raws,labels,nclasses));
        //mse
        double mse = 0.0;
        //traning
        double accuracy=
        mModel->startTraining(nclasses,
                              0.5,
                              50000,
                              mse,
                              true,
                              true);
        //print mse
        std::cout << "RBFNetwork accuracy="<< accuracy <<", mse: " << mse << "\n";
    }
    
    virtual ~MyoModelRBFNetwork()
    {
    }
    
    virtual void serialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "wb");
        
        if(file)
        {
            mData.serialize(file);
            fclose(file);
        }
    }
    
    virtual void deserialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "rb");
        
        if(file)
        {
            mData.deserialize(file);
            createModel();
            fclose(file);
        }
    }
};

//implement interface
MyoClassifierRBFNetwork::~MyoClassifierRBFNetwork()
{
    /* none */
}

MyoModelInterface* MyoClassifierRBFNetwork::createModel(const DataSetReader& ds)
{
    mModel = std::make_shared<MyoModelRBFNetwork>(ds);
    return mModel.get();
}

MyoModelInterface* MyoClassifierRBFNetwork::modelFromfile(const std::string& path)
{
    mModel = std::make_shared<MyoModelRBFNetwork>();
    mModel->deserialize(path);
    return mModel.get();
}

void MyoClassifierRBFNetwork::setModel(MyoModelInterface* model)
{
    mModel = std::shared_ptr<MyoModelRBFNetwork>((MyoModelRBFNetwork*)model);
}

void MyoClassifierRBFNetwork::setProbabilityFilter(double probability)
{
    mProbabilityFilter = probability;
}

void MyoClassifierRBFNetwork::classification(MyoThread& myo,
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
                  int classes = mModel->mModel->predictLabel(buffer, error);
                  //filters
                  if(mProbabilityFilter <= (1.0-error))
                  if(mLastClass != (double)classes)
                  {
                      //callback
                      callback(mModel->mData.getClassNames().getClassName(classes));
                      mLastClass = (double) classes;
                  }
              },
              mModel->mData.getFlags(),
              mModel->mData.getUpdate());
}

