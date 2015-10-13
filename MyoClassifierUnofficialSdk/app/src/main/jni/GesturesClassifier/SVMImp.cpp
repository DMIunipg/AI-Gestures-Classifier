//
//  SVMImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 29/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "SVMImp.h"
#include "SVM/SVMUtilities.h"

class MyoModelSVM : public MyoModelInterface
{
public:

    DataFlags  mFlags;
    ClassesNames mClasses;
    unsigned int mUpdate { 20 };
    svm_model* mModel { nullptr };

    MyoModelSVM(svm_model* model = nullptr)
    :mModel(model)
    {
    }
    
    MyoModelSVM(const DataSetReader& data)
    {
        //alloc
        svm_problem* problem = new svm_problem();
        //count of fiels
        problem->l = (int)data.size();
        //alloc of labels
        problem->y = new double[data.size()];
        //alloc
        const size_t ptrsSize   = sizeof(svm_node*) *   data.size();
        const size_t lineSize   = sizeof(svm_node)  *  (data.sizeLine()+1);
        const size_t matrixSize = lineSize          *   data.size();
        const char* ptrs   = new char[ptrsSize+matrixSize];
        const char* buffer = ptrs + ptrsSize;
        problem->x = (svm_node**)ptrs;
        //init array
        for(size_t n=0;n!=data.size();++n)
        {
            problem->x[n]=&(((svm_node*)buffer)[n*(data.sizeLine()+1)]);
        }
        //put values
        for(size_t r = 0 ; r!=data.size() ; ++r)
        {
            for(size_t a = 0; a!=data[r].size(); ++a )
            {
                problem->x[r][a].index = (int)a + 1;
                problem->x[r][a].value = data[r][a];
            }
            problem->x[r][data[r].size()].index = -1;
            problem->y[r] = data[r].mClass;
            
        }
        //kernel params
        svm_parameter param = {0};
        //traning
        param.svm_type    = NU_SVC;
        param.kernel_type = RBF;
        param.cache_size  = 100.0f;
        param.coef0       = 0.1f;
        param.degree      = 3;
        param.eps         = 0.1f;
        param.gamma       = 0.2f;
        param.nu          = 0.03f;
        param.probability = true;
        param.shrinking   = true;
        //do cross validation
#if  0
        svm_do_cross_validation(param,*problem,100);
#endif
        //create model
        mModel   = svm_train(problem,&param);
        mFlags   = data.getFlags();
        mClasses = data.getClassNames();
        //delete problem
        delete [] problem->y;
        delete [] ((char*)(problem->x));
        delete problem;

    }
    
    virtual ~MyoModelSVM()
    {
        if(mModel)
        {
            svm_free_and_destroy_model(&mModel);
        }
    }
    
    virtual void serialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "wb");
        
        if(file)
        {
            mFlags.textSerialize(file);
            mClasses.serialize(file);
            fprintf(file, "update: %u\n",mUpdate);
            svm_save_model_to_file(file,mModel);
            fclose(file);
        }
    }
    
    virtual void deserialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "rb");
        
        if(file)
        {
            mFlags.textDeserialize(file);
            mClasses.deserialize(file);
            fscanf(file, "update: %u\n",&mUpdate);
            mModel=svm_load_model_from_file(file);
            fclose(file);
        }
    }
};

//implement interface
MyoClassifierSVM::~MyoClassifierSVM()
{
    /* none */
}

MyoModelInterface* MyoClassifierSVM::createModel(const DataSetReader& ds)
{
    mModel = std::make_shared<MyoModelSVM>(ds);
    return mModel.get();
}

MyoModelInterface* MyoClassifierSVM::modelFromfile(const std::string& path)
{
    mModel = std::make_shared<MyoModelSVM>();
    mModel->deserialize(path);
    return mModel.get();
}

void MyoClassifierSVM::setModel(MyoModelInterface* model)
{
    mModel = std::shared_ptr<MyoModelSVM>((MyoModelSVM*)model);
}

void MyoClassifierSVM::setProbabilityFilter(double probability)
{
    mProbabilityFilter = probability;
}

void MyoClassifierSVM::classification(MyoThread& myo,
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
                  std::unique_ptr<svm_node> ubuffer(new svm_node[allSize+1]);
                  //ptr to buffer
                  svm_node* buffer = &(*ubuffer);
                  //set last id
                  buffer[allSize].index = -1;
                  //put values
                  MyoThread::applay(inputs,
                                    flags,
                                    [&](size_t i,size_t ri, double value)
                                    {
                                        buffer[i].value = value;
                                    });
                  //set index
                  for(int i=0;i!=allSize;++i) buffer[i].index = i+1;
                  //classification
                  std::unique_ptr<double[]> classProb(new double[mModel->mModel->nr_class]);
                  double idclass = svm_predict_probability(mModel->mModel, buffer,&classProb[0]);
                  //get max probability
                  double probability = 0.0;
                  for(int c=0; c!=mModel->mModel->l; ++c)
                  {
                      probability = std::max(probability,classProb[c]);
                  }
                  //filters
                  //std::cout << "class: " << mModel->mClasses.getClassName(idclass) << "\n";
                  if(mProbabilityFilter <= probability)
                  if(mLastClass != idclass)
                  {
                      //callback
                      callback(mModel->mClasses.getClassName(idclass));
                      mLastClass = idclass;
                  }
              },
              mModel->mFlags,
              mModel->mUpdate);
}
