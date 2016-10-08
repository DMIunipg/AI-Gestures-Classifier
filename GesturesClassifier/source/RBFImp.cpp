//
//  RBFImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 02/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <iostream>
#include <cctype>
#include "RBFImp.h"
#include "RBFNetwork/RBFNetwork.h"

class MyoModelRBFNetwork : public MyoModelInterface
{
public:
    
    struct TraningInfo
    {
        //params                 default
        double mLearningRate     { 0.5  };
        int mNumIterations       { 50000 };
        bool mCalcAccuracyMse    { true };
        bool mPrintFlag          { true };
        
        //serialize params
        void serialize(FILE* file) const
        {
            std::fwrite(&mLearningRate, sizeof(mLearningRate), 1, file);
            std::fwrite(&mNumIterations, sizeof(mNumIterations), 1, file);
            std::fwrite(&mCalcAccuracyMse, sizeof(mCalcAccuracyMse), 1, file);
            std::fwrite(&mPrintFlag, sizeof(mPrintFlag), 1, file);
        }
        
        //deserialize
        void deserialize(FILE* file)
        {
            const size_t paramsSize =  sizeof(mLearningRate)
                                      +sizeof(mNumIterations)
                                      +sizeof(mCalcAccuracyMse)
                                      +sizeof(mPrintFlag);
            //have info?
            size_t f_pos    = std::ftell(file);
            //go to end
            std::fseek(file, 0L, SEEK_END);
            //get end file
            size_t end_file = std::ftell(file);
            //return to f_pos
            std::fseek(file, f_pos, SEEK_SET);
            //...
            if( end_file-f_pos >= paramsSize )
            {
                std::fread(&mLearningRate, sizeof(mLearningRate), 1, file);
                std::fread(&mNumIterations, sizeof(mNumIterations), 1, file);
                std::fread(&mCalcAccuracyMse, sizeof(mCalcAccuracyMse), 1, file);
                std::fread(&mPrintFlag, sizeof(mPrintFlag), 1, file);
            }
            
        }
    };
    
    
    static bool compare(const char* ptr, const char* command)
    {
        return compare(ptr, command, strlen(command));
    }
    
    static bool compare(const char* ptr, const char* command,size_t len)
    {
        return std::strncmp(ptr, command, len) == 0;
    }
    
    static bool compareAndSkip(const char*& ptr, const char* command)
    {
        //string len
        size_t len = strlen(command);
        //compare..
        if( compare(ptr,command,len) )
        {
            ptr+=len;
            return true;
        }
        return  false;
    }
    
    static void skipSpace(const char*& ptr)
    {
        while(std::isspace(*ptr)) ++ptr;
    }
    
    static int parseIntAndSkip(const char*& ptr)
    {
        //compute end
        const char* ptr_end   = ptr;
        //get all digits
        while(isdigit(*ptr_end)) ++ptr_end;
        //isn't int number?
        if(ptr == ptr_end) return 0;
        //parse
        int output = std::atoi(ptr);
        //skip
        ptr = ptr_end;
        //return value
        return output;
    }
    
    static double parseDoubleAndSkip(const char*& ptr)
    {
        char* end_ptr = nullptr;
        //parsing double
        double output = std::strtod (ptr, &end_ptr);
        //go tu next ptr
        ptr = end_ptr;
        //return value
        return  output;
    }
    
    static bool parseBoolAndSkip(const char*& ptr)
    {
        //is false?
        if( compareAndSkip(ptr,"false") ) return false;
        //skip true
        compareAndSkip(ptr,"true");
        //return true
        return true;
    }
    
    static TraningInfo parseArguments(const std::string& args)
    {
        //kernel params
        TraningInfo param;
        //get ptr
        const char* ptr = args.c_str();
        //parse
        while(ptr && *ptr != EOF && *ptr != '\0')
        {
            //skip start space
            skipSpace(ptr);
            //..
            if(compareAndSkip(ptr, "iterations"))
            {
                //skip space
                skipSpace(ptr);
                //parse int
                param.mNumIterations = parseIntAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "learning rate"))
            {
                //skip space
                skipSpace(ptr);
                //parse int
                param.mLearningRate = parseDoubleAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "accuracy mse"))
            {
                //skip space
                skipSpace(ptr);
                //parse int
                param.mCalcAccuracyMse = parseBoolAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "print"))
            {
                //skip space
                skipSpace(ptr);
                //parse int
                param.mPrintFlag = parseBoolAndSkip(ptr);
            }
            else break;
        }
        
        return param;
    }
    
    MyoModelRBFNetwork(RBFNetwork* model = nullptr)
    :mModel(model)
    {
    }
    
    MyoModelRBFNetwork(const DataSetReader& data,const std::string& args)
    {
        //sava dataset
        mData = data;
        //create model
        createModel(parseArguments(args));
    }
    
    void createModel(TraningInfo params)
    {
        //save params
        mParams = params;
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
                              mParams.mLearningRate,
                              mParams.mNumIterations,
                              mse,
                              mParams.mCalcAccuracyMse,
                              mParams.mPrintFlag);
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
            //serialize dataset
            mData.serialize(file);
            //serialize last params
            mParams.serialize(file);
            //close
            fclose(file);
        }
    }
    
    virtual void deserialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "rb");
        
        if(file)
        {
            //dataset
            mData.deserialize(file);
            //params
            TraningInfo params;
            //deserialize params
            params.deserialize(file);
            //traning
            createModel(params);
            //close
            fclose(file);
        }
    }
    
public:
    
    TraningInfo   mParams;
    DataSetReader mData;
    std::unique_ptr<RBFNetwork> mModel { nullptr };
    
};

//implement interface
MyoClassifierRBFNetwork::~MyoClassifierRBFNetwork()
{
    /* none */
}

MyoModelInterface* MyoClassifierRBFNetwork::createModel(const DataSetReader& ds)
{
    mModel = std::make_shared<MyoModelRBFNetwork>(ds,"");
    return mModel.get();
}


MyoModelInterface* MyoClassifierRBFNetwork::createModel(const DataSetReader& ds,const std::string& args)
{
    mModel = std::make_shared<MyoModelRBFNetwork>(ds,args);
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

MyoModelInterface* MyoClassifierRBFNetwork::getModel() const
{
	return mModel.get();
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

