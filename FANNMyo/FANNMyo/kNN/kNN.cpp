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

kNN::Result kNN::classify(const kNN::DataRow& row,
                          unsigned int nNN,
                          DistanceType type,
                          DistanceWeight typeWeight ) const
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
        double rfdistance = 0.0;
        //select distance type
        switch (type)
        {
            default:
            case EUCLIDE_DISTANCE:   rfdistance = distance(field.mRow, row);  break;
            case MANHATTAN_DISTANCE: rfdistance = manhattan(field.mRow, row);  break;
            case HAMMING_DISTANCE:   rfdistance = hamming(field.mRow, row);  break;
        }
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
    struct WightAndCount
    {
        double mWeight { 0.0 };
        size_t mCount  { 0   };
        //utility operators
        WightAndCount& operator += (const WightAndCount& in)
        {
            mWeight += in.mWeight;
            mCount  += in.mCount;
            return (*this);
        }
    };
    std::map< double, WightAndCount > selectedMap;
    //count occurrences
    for(auto& select : selected)
        if(select.mField)
        {
            switch (typeWeight)
            {
                default:
                case DEMOCRATIC:
                    ++selectedMap[ select.mField->mClasses ].mWeight;
                break;
                case ONE_ON_DISTANCE:
                      selectedMap[ select.mField->mClasses ].mWeight += 1.0 / select.mDistance;
                break;
                case ONE_MINUS_DISTANCE:
                      selectedMap[ select.mField->mClasses ].mWeight += 1.0 - select.mDistance;
                break;
            }
            ++selectedMap[ select.mField->mClasses ].mCount;
        }
    //is a unsuccess
    if(selectedMap.empty()) {  return Result();  }
    //select class whit max weight
    double         classes  = selectedMap.begin()->first;
    WightAndCount    cinfo  = selectedMap.begin()->second;
    //find max info
    WightAndCount maxinfo;
    
    for(auto it:selectedMap)
    {
        if(it.second.mWeight > cinfo.mWeight)
        {
            classes   = it.first;
            cinfo     = it.second;
        }
        //add weight
        maxinfo += it.second;
    }
    //compute error
    double error = 0.0;
    switch (typeWeight)
    {
        default:
        case DEMOCRATIC:
            error = 1.0 - ( cinfo.mWeight / nNN );
        break;
        case ONE_ON_DISTANCE:
            error =  1.0 - ((1.0/maxinfo.mWeight) / (1.0/cinfo.mWeight)) ;
        break;
        case ONE_MINUS_DISTANCE:
            error = 1.0 - ( (-cinfo.mWeight  +cinfo.mCount) /
                            (-maxinfo.mWeight+maxinfo.mCount) );
        break;
    }
    //return selected
    return Result( true,  classes,  error);
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

double kNN::manhattan(const DataRow& left,const DataRow& right) const
{
    assert(left.size() == right.size());
    double distance = 0.0;
    //compute distance
    #pragma omp parallel for
    for(size_t i=0; i!= left.size(); ++i)
    {
        distance += std::abs(left[i] - right[i]);
    }
    return distance;
}

double kNN::hamming(const DataRow& left,const DataRow& right) const
{
    assert(left.size() == right.size());
    double distance = 0.0;
    //compute distance
    #pragma omp parallel for
    for(size_t i=0; i!= left.size(); ++i)
    {
        // hamming :
        // if is equal  then  0 else 1
        distance += (left[i] != right[i]);
    }
    return distance;
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