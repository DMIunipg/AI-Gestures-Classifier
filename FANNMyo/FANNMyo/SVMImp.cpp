//
//  SVMImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 29/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "SVMImp.h"
#include "SVM/SVMUtilities.h"

namespace myosvm
{

void create_model(const std::string& path)
{
    svm_problem* dataset = svm_deserialize(path);
    svm_unnormalize_classes_names(dataset);
    svm_parameter param = {0};

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
    
    svm_do_cross_validation(param,*dataset,100);
    svm_model* model = svm_train(dataset,&param);
    if(svm_save_model((path+".svm.model").c_str(),model))
    {
        fprintf(stderr, "can't save model to file %s\n", (path+".svm.model").c_str());
    }
    svm_free_and_destroy_model(&model);
    svm_free(dataset);
}
void* myo_classification(const std::string& path,MyoThread& myo,DataFlags&  flags,ClassesNames& cnames)
{
        svm_model* model=svm_load_model((path+".svm.model").c_str());
        //add function
        myo.start([&,model](const MyoThread::Inputs& inputs,
                            const DataFlags& falgs,
                            std::mutex& mutex)
                  {
                      if(!inputs.size()) return;
                      //loop constants
                      size_t rowSize = falgs.lineSize<8>() / flags.mReps;
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
#if 0
                      
                      for(size_t x=0;x!=inputs.size();++x)
                      {
                          for(size_t a=0;a!=rowSize;++a)
                          {
                              size_t id = rowSize*x+a;
                              std::cout << '(' << buffer[id].index  ;
                              std::cout << ',' << buffer[id].value << ")";
                          }
                          std::cout<<"\n";
                      }
                      std::cout << "-----------------------------------------------\n";
#endif
                      //run
                      static size_t countOuput = 0;
                      std::unique_ptr<double[]> classProb(new double[model->nr_class]);
                      double calcOut = svm_predict_probability(model, buffer,&classProb[0]);
                      //print
                      std::cout << "-----------------------------------------------\n";
                      std::cout << "arm in in status: "
                                << calcOut << '(' << ++countOuput << "), name: "
                                << cnames.getClassName(svm_normalize_class_name(calcOut))
                                << "\n";
                      std::cout << "( ";
                      for(int c=0;c!=model->nr_class;++c)
                      {
                          std::cout << classProb[c] << " ";
                      }
                      std::cout << ")\n";
                      std::cout << "-----------------------------------------------\n";
                      static double cNameMoreThenHl = 0.0;
                      static double cNameMoreThenHlProb = 0.0;
                      cNameMoreThenHlProb = 0.0;
                      for(int c=0;c!=model->nr_class;++c)
                          cNameMoreThenHlProb  = std::max(cNameMoreThenHlProb,classProb[c]);
                      if(cNameMoreThenHlProb>0.5) cNameMoreThenHl = calcOut;
                      std::cout << "last more then 0.5% is "
                                << cnames.getClassName(svm_normalize_class_name(cNameMoreThenHl))
                                << "\n";
                      std::cout << "-----------------------------------------------\n";
                      std::cout.flush();

                  },flags);
    return model;
}
    
void  myo_classification_free(void* model)
{
    svm_free_model_content((svm_model*)model);
}

}