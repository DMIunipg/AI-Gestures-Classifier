//
//  kNN.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 04/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include "kNN.h"
#include <assert.h>
#include <cmath>
#include <map>

kNN::kNN(const std::string& path)
{
    assert(dataSetFromFile(path, mDataSet));
}
kNN::kNN(const kNN::DataSet& dataset)
:mDataSet(dataset)
{
}

kNN::Result kNN::classify(const kNN::DataRow& row, unsigned int nNN) const
{
    //field selected
    struct SelectField
    {
        double           mDistance = 0.0;
        const DataField* mField    = nullptr;
    };
    //list of 'k' NN
    std::vector< SelectField > selected;
    //resize the buffer K's list buffer
    selected.resize(nNN);
    //serach in data set
    for(auto& field:mDataSet)
    {
        //compute distance
        double rfdistance = distance(field.mRow, row);
        //compare with already selected
        for(auto& select : selected)
            if(!select.mField || select.mDistance > rfdistance)
            {
                select.mDistance = rfdistance;
                select.mField    = &field;
                break;
            }
    }
    //democratic selection
    std::map< double, size_t > selectedMap;
    //count occurrences
    for(auto& select : selected)
        if(select.mField)
        {
            ++selectedMap[ select.mField->mClasses ];
        }
    //is a unsuccess
    if(selectedMap.empty()) {  return Result();  }
    //select class whit max count
    double classes = selectedMap.begin()->first;
    size_t   count = selectedMap.begin()->second;
    
    for(auto it:selectedMap)
        if(it.second > count)
        {
            classes = it.first;
            count   = it.second;
        }
    //return selected
    return Result( true,  classes, 1.0-((double)count / nNN) );
}

double kNN::distance(const kNN::DataRow& left,const kNN::DataRow& right) const
{
    assert(left.size() == right.size());
    double distance = 0.0;
    //compute distance
    #pragma omp parallel for
    for(size_t i=0; i!= left.size(); ++i)
    {
        double idiff = left[i] - right[i];
        distance    += idiff * idiff;
    }
    return std::sqrt(distance);
}

bool kNN::dataSetFromFile(const std::string& path,kNN::DataSet& out)
{
    //read file
    FILE* file=fopen(path.data(), "rb");
    //read
    if(file)
    {
        unsigned int ninput=0,natt=0,nout=0;
        fscanf(file, "%u %u %u\n", &ninput, &natt, &nout);
        //alloc
        out.resize(ninput);
        //alloc array
        for(auto& row:out)
        {
            row.mClasses = 0.0;
            row.mRow.resize(natt);
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
                out[n][a] = value;
            }
            //get class
            fscanf(file, "\n%le \n", &(out[n].mClasses));
        }
        
        return true;
    }
    return false;
}