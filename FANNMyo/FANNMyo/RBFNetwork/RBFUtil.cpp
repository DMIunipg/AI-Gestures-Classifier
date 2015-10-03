//
//  RBFUtil.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 03/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "RBFUtil.h"
#include <stdio.h>
#include <algorithm>

std::shared_ptr< Dataset > Utility::ReadDataSet(const std::string& path)
{
    auto buffer = std::make_shared< Dataset >();
    //read fann
    FILE* file=fopen(path.data(), "rb");
    //read
    if(file)
    {
        unsigned int ninput=0,natt=0,nout=0;
        fscanf(file, "%u %u %u\n", &ninput, &natt, &nout);
        //alloc
        buffer->mDatas.resize(ninput);
        buffer->mLabels.resize(ninput);
        //alloc labels double
        std::vector< double > labels(ninput);
        //alloc al values
        for(auto& row: buffer->mDatas) row.resize(natt);
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
                buffer->mDatas[n][a] = value;
            }
            //get class
            fscanf(file, "\n%le \n", &labels[n]);
        }
        //double labels into labels "integer"
        for(size_t n = 0;n != ninput;++n)
        {
            buffer->mLabels[n] = (int)(labels[n] != 0.0 ? 1.0 / labels[n] : 0.0);
            //....
            if ( buffer->mLabels[n] < 0.0 )
                 buffer->mLabels[n] *= -1.0;
            else
                 buffer->mLabels[n] *= 2.0;
        }
    }
    //count classes
    std::vector< int > classes;
    //count
    for(int id:buffer->mLabels)
    {
        auto it = std::find(classes.begin(), classes.end(), id);
        if( it == classes.end() ) classes.push_back(id);
    }
    //save count
    buffer->mCCount = classes.size();
    //return buffer
    return buffer;
}