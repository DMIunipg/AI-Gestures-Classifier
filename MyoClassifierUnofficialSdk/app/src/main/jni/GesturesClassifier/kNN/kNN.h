//
//  kNN.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 04/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef kNN_h
#define kNN_h

#include <vector>
#include <string>
#include <DataSetReader.h>


class kNN
{
    
public:
    
    //type of a raw
    using  DataRaw  = DataSetReader::AttributesRaw;
    //type of a field of the dataset
    using  DataField = DataSetReader::Row;
    //type of dataset
    using  DataSet   = DataSetReader::Rows;
    //type of classification result
    struct Result
    {
        bool   mSuccess { false };
        double mClasses { 0.0   };
        double mError   { 1.0   };
        //default
        Result(){}
        //init
        Result(bool   success,
               double classes,
               double error)
        {
            mSuccess = success;
            mClasses = classes;
            mError   = error;
        }
    };
    //distance method
    enum DistanceType
    {
        EUCLIDE_DISTANCE,
        MANHATTAN_DISTANCE,
        HAMMING_DISTANCE
    };
    //distance Weight
    enum DistanceWeight
    {
        DEMOCRATIC,
        ONE_ON_DISTANCE,
        ONE_MINUS_DISTANCE
    };
    
    //public methods
    kNN (const DataSet& dataset);
    //applay classify
    Result classify(const DataRaw& values,
                    unsigned int nNN = 1,
                    DistanceType type = EUCLIDE_DISTANCE,
                    DistanceWeight typeWeight = DEMOCRATIC ) const;
    
    
protected:
    
    //distance
    double distance(const DataRaw& left,const DataRaw& right) const;
    double manhattan(const DataRaw& left,const DataRaw& right) const;
    double hamming(const DataRaw& left,const DataRaw& right) const;
    //dataset
    DataSet mDataSet;
    
    
};

#endif /* kNN_hpp */
