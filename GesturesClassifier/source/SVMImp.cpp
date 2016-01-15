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
    
    bool compare(const char* ptr, const char* command)
    {
        return compare(ptr, command, strlen(command));
    }
    
    bool compare(const char* ptr, const char* command,size_t len)
    {
        return std::strncmp(ptr, command, len) == 0;
    }
    
    bool compareAndSkip(const char*& ptr, const char* command)
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
    
    void skipSpace(const char*& ptr)
    {
        while(std::isspace(*ptr)) ++ptr;
    }
    
    int parseIntAndSkip(const char*& ptr)
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
    
    double parseDoubleAndSkip(const char*& ptr)
    {
        char* end_ptr = nullptr;
        //parsing double
        double output = std::strtod (ptr, &end_ptr);
        //go tu next ptr
        ptr = end_ptr;
        //return value
        return  output;
    }
    
    bool parseBoolAndSkip(const char*& ptr)
    {
        //is false?
        if( compareAndSkip(ptr,"false") ) return false;
        //skip true
        compareAndSkip(ptr,"true");
        //return true
        return true;
    }
    
    svm_parameter parseArguments(const std::string& args)
    {
        //kernel params
        svm_parameter param = {0};
        //default
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
        //get ptr
        const char* ptr = args.c_str();
        //parse
        while(ptr && *ptr != EOF && *ptr != '\0')
        {
            //skip start space
            skipSpace(ptr);
            //type...
            if(compareAndSkip(ptr, "type"))
            {
                skipSpace(ptr);
                //types
                //enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
                     if(compareAndSkip(ptr,"C_SVC")) param.svm_type  = C_SVC;
                else if(compareAndSkip(ptr,"NU_SVC")) param.svm_type = NU_SVC;
                else if(compareAndSkip(ptr,"ONE_CLASS")) param.svm_type  = ONE_CLASS;
                else if(compareAndSkip(ptr,"EPSILON_SVR")) param.svm_type  = EPSILON_SVR;
                else if(compareAndSkip(ptr,"NU_SVR")) param.svm_type  = NU_SVR;
                else break;
            }
            else if(compareAndSkip(ptr, "kernel"))
            {
                skipSpace(ptr);
                //kernels
                //enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED }; /* kernel_type */
                     if(compareAndSkip(ptr,"LINEAR")) param.svm_type  = LINEAR;
                else if(compareAndSkip(ptr,"POLY")) param.svm_type = POLY;
                else if(compareAndSkip(ptr,"RBF")) param.svm_type  = RBF;
                else if(compareAndSkip(ptr,"SIGMOID")) param.svm_type  = SIGMOID;
                else if(compareAndSkip(ptr,"PRECOMPUTED")) param.svm_type  = PRECOMPUTED;
                else break;
            }
            else if(compareAndSkip(ptr, "cache"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.cache_size = parseDoubleAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "coef0"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.coef0 = parseDoubleAndSkip(ptr);
                
            }
            else if(compareAndSkip(ptr, "degree"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.degree = parseIntAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "eps"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.eps = parseDoubleAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "gamma"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.gamma = parseDoubleAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "nu"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.nu = parseDoubleAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "probability"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.probability = parseBoolAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "shrinking"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.shrinking = parseBoolAndSkip(ptr);
            }
            else if(compareAndSkip(ptr, "p"))
            {
                //parse
                skipSpace(ptr);
                //parse
                param.p = parseDoubleAndSkip(ptr);
            }
            else
            {
                break;
            }
        }
        
        return param;
    }
    
    MyoModelSVM(svm_model* model = nullptr)
    :mModel(model)
    {
    }
    
    MyoModelSVM(const DataSetReader& data,const std::string& args)
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
        svm_parameter param = parseArguments(args);
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
    mModel = std::make_shared<MyoModelSVM>(ds,"");
    return mModel.get();
}

MyoModelInterface* MyoClassifierSVM::createModel(const DataSetReader& ds,const std::string& args)
{
    mModel = std::make_shared<MyoModelSVM>(ds,args);
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
