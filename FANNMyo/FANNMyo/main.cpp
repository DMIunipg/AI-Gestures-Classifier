//
//  main.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 28/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <stdio.h>
#include <fann/floatfann.h>
#include "MyoThread.h"
#include "FANNImp.h"
#include "SVMImp.h"
#include "RBFImp.h"
#include "kNNImp.h"

#define SVM 1
#define FANN 2
#define RBF 3
#define kNN 4

#define SVM_OR_FANN   kNN  // SVM//RBF //FANN
#define CREATE_MODEL  true
//#define PATH_DATA_SET "datas/NRL11/data.fann"
#define PATH_DATA_SET "datas/GAB4/data.fann"
//#define PATH_DATA_SET "datas/DAN2/data.fann"

//select method
#if SVM_OR_FANN == FANN
namespace myomethod = myofann;
#elif SVM_OR_FANN == SVM
namespace myomethod = myosvm;
#elif SVM_OR_FANN == RBF
namespace myomethod = myorbf;
#elif SVM_OR_FANN == kNN
namespace myomethod = myoknn;
#else
#error "Select SVM or FANN"
#endif

//init
int main(int argc, const char * argv[])
{
    std::string path(PATH_DATA_SET);
    //create model
    if(CREATE_MODEL) myomethod::create_model(path);
    //myo thread
    MyoThread myo;
    //data
    DataFlags flags;
    ClassesNames cnames;
    //read flag
    {
        FILE* file = std::fopen((path+".meta").c_str(), "rb");
        if(file)
        {
            flags.derialize(file);
            std::fclose(file);
        }
    }
    //read names
    {
        FILE* file = std::fopen((path+".classes").c_str(), "rb");
        if(file)
        {
            cnames.derialize(file);
            std::fclose(file);
        }
    }
    //start thread
    void* context=myomethod::myo_classification(path,myo,flags,cnames);
    //wait
    for(;;)
    {
        char buffer[255]={0};
        scanf("exit? %s",buffer);
        if(strcmp(buffer, "yes")==0) break;
    }
    //wait to end
    myo.joint();
    //free
    myomethod::myo_classification_free(context);
    
    
    return 0;
}