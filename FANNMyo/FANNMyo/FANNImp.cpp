//
//  FANNImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 29/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "FANNImp.h"
#include <fann/fann.h>
#include <string>
#include <iostream>

namespace myofann
{
    
static bool existsFile(const char* path)
{
    FILE* file = fopen(path,"rb");
    if(!file) return false;
    fclose(file);
    return true;
}
    
void create_model(const std::string& path)
{
    const unsigned int num_layers = 4;
    const float desired_error = (const float) 0.2;
    
    //train data
    struct fann_train_data *train_data= fann_read_train_from_file(path.c_str());
    //
    float momentum = 2.0/train_data->num_input;
    //ANN
    struct fann* ann = fann_create_standard(num_layers,
                                            train_data->num_input,
                                            train_data->num_input*3,
                                            train_data->num_input*3,
                                            train_data->num_output);
    
    std::cout << "Training network.\n";
#if 0
    fann_set_training_algorithm(ann, /*FANN_TRAIN_INCREMENTAL*/ FANN_TRAIN_QUICKPROP);
    fann_set_learning_rate(ann, 0.5f);
    fann_set_learning_momentum(ann,/*momentum*/ 0.1f);
    fann_set_train_error_function(ann, /*FANN_ERRORFUNC_LINEAR*/ FANN_ERRORFUNC_TANH);
#elif 0
    fann_set_training_algorithm(ann, FANN_TRAIN_INCREMENTAL);
    fann_set_learning_rate(ann, 0.31f);
    fann_set_learning_momentum(ann,/*momentum*/ 0.01);
    fann_set_train_error_function(ann, FANN_ERRORFUNC_LINEAR);
#else
    fann_set_activation_function_hidden(ann, FANN_GAUSSIAN);
    fann_set_training_algorithm( ann, FANN_TRAIN_BATCH );
    fann_set_learning_rate(ann, 0.35f);
    fann_set_learning_momentum(ann,0.02f);
    fann_set_train_error_function(ann, FANN_ERRORFUNC_TANH );
#endif
    
    fann_train_on_data(ann, train_data, 4000, 50, desired_error);
    
    
    if(existsFile((path+".test").c_str()))
    {
        std::cout << "Testing network.\n";
        
        struct fann_train_data* test_data = fann_read_train_from_file((path+".test").c_str());
        
        fann_reset_MSE(ann);
        for(int i = 0; i < fann_length_train_data(test_data); i++)
        {
            fann_test(ann, test_data->input[i], test_data->output[i]);
        }
        
        std::cout << "MSE error on test data: " << fann_get_MSE(ann) << "\n";
        
        std::cout << "Saving network.\n";
        
        fann_destroy_train(test_data);
    }
    
    fann_save(ann, (path+".model").c_str());
    
    std::cout << "Cleaning up.\n";
    
    fann_destroy_train(train_data);
}

void* myo_classification(const std::string& path,MyoThread& myo,DataFlags&  flags,ClassesNames& cnames)
{
    //load model
    fann* ann=fann_create_from_file( (path+".model").c_str() );
    //add function
    myo.start([&,ann](const MyoThread::Inputs& inputs,
                      const DataFlags& falgs,
                      std::mutex& mutex)
              {
                  if(!inputs.size()) return;
                  //loop constants
                  size_t rowSize = falgs.lineSize<8>() / flags.mReps;
                  size_t allSize = rowSize*inputs.size();
                  std::unique_ptr<fann_type> ubuffer(new fann_type[allSize]);
                  //ptr to buffer
                  fann_type* buffer = &(*ubuffer);
                  //put values
                  MyoThread::applay(inputs,
                                    flags,
                                    [&](size_t i,size_t ri, double value)
                                    {
                                        buffer[i] = value;
                                    });
                  //run
                  float* calcOut = fann_run(ann,buffer);
                  //print
                  std::cout << "-----------------------------------------------\n";
                  std::cout << "arm in in status: "<< (float)(calcOut[0]) << ", name: "
                            << cnames.getClassName(calcOut[0])<< "\n";
                  std::cout << "-----------------------------------------------\n";
                  std::cout.flush();
#if 0
                  for(size_t i=0;i!=allSize;++i)
                  {
                      std::cout << '(' << i << ", " << buffer[i] <<")";
                  }
                  std::cout << "\n-----------------------------------------------\n";
#endif
              },flags);
    //return NN
    return ann;
}

void myo_classification_free(void* ann)
{
    //free
    fann_destroy((struct fann*)ann);
}
    
}
