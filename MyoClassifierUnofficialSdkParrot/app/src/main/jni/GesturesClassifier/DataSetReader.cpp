//
//  DataSetReader.cpp
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 10/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "DataSetReader.h"


void DataSetReader::serialize(FILE* file)
{
    
    //write flags
    mDataFlags.textSerialize(file);
    //serialize class names
    mClassNames.serialize(file);
    //serialize update value
    std::fprintf(file,"update: %d\n", mUpdate);
    //write vaues
    fprintf(file, "%u %u %u\n",
            (unsigned int)size(),
            (unsigned int)sizeLine(),
            1);
    //write rows
    for(auto& row:mRows)
    {
        for(double value:row.mRaw)  fprintf(file, "%le ",value);
        fprintf(file,"%le\n",row.mClass);
    }
}

void DataSetReader::deserialize(FILE* file)
{
    //read flags
    mDataFlags.textDeserialize(file);
    //deserialize class names
    mClassNames.deserialize(file);
    //deserialize update value
    std::fscanf(file,"update: %d\n", &mUpdate);
    //read vaues
    unsigned int ninput=0,natt=0,nout=0;
    fscanf(file, "%u %u %u\n", &ninput, &natt, &nout);
    //resize
    mRows.resize(ninput);
    //read rows
    for(size_t n=0;n!=ninput;++n)
    {
        //resize raw
        mRows[n].mRaw.resize(natt);
        //get fiels
        for(size_t a=0;a!=natt;++a)
        {
            //get value
            double value = 0.0;
            fscanf(file,"%le ",&value);
            //put value
            mRows[n].mRaw[a] = value;
        }
        //get class
        fscanf(file, "%le\n", &mRows[n].mClass);
    }
    
}

bool DataSetReader::read(const std::string &path)
{
    //read file
    FILE* file=fopen(path.data(), "rb");
    //read
    if(file)
    {
        deserialize(file);
        return true;
    }
    
    return false;
}
