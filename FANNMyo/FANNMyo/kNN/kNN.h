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

class kNN
{
    
public:
    
    //type of a row
    using  DataRow     = std::vector< double >;
    //type of a field of the dataset
    struct DataField
    {
        double  mClasses;
        DataRow mRow;
        
        //helper methods
        size_t size() const
        {
            return mRow.size();
        }
        
        double& operator [] (size_t i)
        {
            return mRow[i];
        }
        
        double operator [] (size_t i) const
        {
            return mRow[i];
        }
    };
    //type of dataset
    using DataSet     = std::vector< DataField >;
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
    kNN (const std::string& dataset);
    kNN (const DataSet& dataset);
    //applay classify
    Result classify(const DataRow& values,
                    unsigned int nNN = 1,
                    DistanceType type = EUCLIDE_DISTANCE,
                    DistanceWeight typeWeight = DEMOCRATIC ) const;
    //read dataset from file
    static bool dataSetFromFile(const std::string& path,DataSet& out);
    
protected:
    
    //distance
    double distance(const DataRow& left,const DataRow& right) const;
    double manhattan(const DataRow& left,const DataRow& right) const;
    double hamming(const DataRow& left,const DataRow& right) const;
    //dataset
    DataSet mDataSet;
    
    
};

#endif /* kNN_hpp */
