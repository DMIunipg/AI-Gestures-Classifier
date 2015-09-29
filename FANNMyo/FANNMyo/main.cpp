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
#include "SVMSerialize.h"
#include "FANNImp.h"
#include "SVMImp.h"
//select method
//namespace myomethod = myofann;
namespace myomethod = myosvm;
//init
int main(int argc, const char * argv[])
{
    std::string path("datas/pugno_normale.fann");
    //create model
    //myomethod::create_model(path);
    //myo thread
    MyoThread myo;
    //data
    DataFlags flags;
    //read flag
    FILE* file = std::fopen((path+".meta").c_str(), "rb");
    flags.derialize(file);
    std::fclose(file);
    //start thread
    void* context=myomethod::myo_classification(path,myo,flags);
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