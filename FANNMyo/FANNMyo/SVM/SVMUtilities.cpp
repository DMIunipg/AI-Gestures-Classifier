//
//  SVMSerialize.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 29/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "SVMUtilities.h"
#include "svm.h"

svm_problem* svm_deserialize(const std::string& path)
{
    //ouput
    svm_problem* ouput = new svm_problem();
    
    //read file
    FILE* file=fopen(path.data(), "rb");
    //read
    if(file)
    {
        unsigned int ninput=0,natt=0,nout=0;
        fscanf(file, "%u %u %u\n", &ninput, &natt, &nout);
        //count of fiels
        ouput->l = ninput;
        //alloc of labels
        ouput->y = new double[ninput];
        //alloc
        const size_t ptrsSize   = sizeof(svm_node*) *   ninput;
        const size_t lineSize   = sizeof(svm_node)  *  (natt+1);
        const size_t matrixSize = lineSize          *   ninput;
        const char* ptrs   = new char[ptrsSize+matrixSize];
        const char* buffer = ptrs + ptrsSize;
        ouput->x = (svm_node**)ptrs;
        //init array
        for(size_t n=0;n!=ninput;++n)
        {
            ouput->x[n]=&(((svm_node*)buffer)[n*(natt+1)]);
        }
        //read rows
        for(size_t n=0;n!=ninput;++n)
        {
            //get fiels
            for(size_t a=0;a!=natt;++a)
            {
                //get value
                double value = 0.0;
                fscanf(file,"%le ",&value);
                //put value
                ouput->x[n][a].index = (int)(a+1);
                ouput->x[n][a].value = value;
            }
            //put end field
            ouput->x[n][natt].index = -1;
            //get class
            fscanf(file, "\n%le \n", &(ouput->y[n]));
        }
        //ouput
        for(size_t n=0;n!=ninput;++n)
        {
            printf("[%d]=%f\n", (int)n, ouput->y[n] );
        }
    }
    //return...
    return ouput;
}


void svm_unnormalize_classes_names(svm_problem* dataset)
{
    for(int n=0;n!=dataset->l;++n)
    {
        dataset->y[n] = dataset->y[n] != 0.0 ? 1.0/dataset->y[n] : dataset->y[n];
    }
}

double svm_normalize_class_name(double classname)
{
    return  classname != 0.0 ? 1.0/classname : classname;
}

void svm_free(svm_problem* dataset)
{
    delete [] dataset->y;
    delete [] ((char*)(dataset->x));
    delete dataset;
}

void svm_do_cross_validation(const svm_parameter& param, const svm_problem& prob,int nr_fold)
{
    int i;
    int total_correct = 0;
    double total_error = 0;
    double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
    double *target = new double[prob.l];
    memset(target, 0, sizeof(double)*prob.l);
    
    svm_cross_validation(&prob, &param, nr_fold, target);
    
    if(param.svm_type == EPSILON_SVR ||
       param.svm_type == NU_SVR)
    {
        for(i=0;i<prob.l;i++)
        {
            double y = prob.y[i];
            double v = target[i];
            total_error += (v-y)*(v-y);
            sumv += v;
            sumy += y;
            sumvv += v*v;
            sumyy += y*y;
            sumvy += v*y;
        }
        printf("Cross Validation Mean squared error = %g\n",total_error/prob.l);
        printf("Cross Validation Squared correlation coefficient = %g\n",
               ((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/
               ((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy))
               );
    }
    else
    {
        for(i=0;i<prob.l;i++)
            if(target[i] == prob.y[i])
                ++total_correct;
        printf("Cross Validation Accuracy = %g%%\n",100.0*total_correct/prob.l);
    }
    delete [] target;
}