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
#include "../DataSetReader.h"

/*!
 * \brief kNN implementation
 */
class kNN
{
    
public:
    
    /*!
     * \brief type of a raw
     */
    using  DataRaw  = DataSetReader::AttributesRaw;
    
    /*!
     * \brief type of a field of the dataset
     */
    using  DataField = DataSetReader::Row;
    
    /*!
     * \brief type of dataset
     */
    using  DataSet   = DataSetReader::Rows;

    /*!
     * \brief type of classification result
     */
    struct Result
    {
        bool   mSuccess { false };
        double mClasses { 0.0   };
        double mError   { 1.0   };
        
        /*!
         * \brief Result
         */
        Result(){}
        
        /*!
         * \brief Result
         * \param success
         * \param classes
         * \param error
         */
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
    
    /*!
     * \brief enum DistanceType
     */
    enum DistanceType
    {
        EUCLIDE_DISTANCE,
        MANHATTAN_DISTANCE,
        HAMMING_DISTANCE
    };
    
    /*!
     * \brief enum DistanceWeight
     */
    enum DistanceWeight
    {
        DEMOCRATIC,
        ONE_ON_DISTANCE,
        ONE_MINUS_DISTANCE
    };
    
    /*!
     * \brief kNN
     */
    kNN (const DataSet& dataset);
    
    /*!
     * \brief classify execute the classification
     * \param values
     * \param nNN
     * \param type
     * \param typeWeight
     */
    Result classify(const DataRaw& values,
                    unsigned int nNN = 1,
                    DistanceType type = EUCLIDE_DISTANCE,
                    DistanceWeight typeWeight = DEMOCRATIC ) const;
    
    
protected:
    
    //distance
    static double distance(const DataRaw& left,const DataRaw& right);
    static double manhattan(const DataRaw& left, const DataRaw& right);
    static double hamming(const DataRaw& left, const DataRaw& right);
    //dataset
    DataSet mDataSet;
    
    
};

#endif /* kNN_hpp */
