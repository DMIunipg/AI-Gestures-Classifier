//
//  DataSetReader.hpp
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 10/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef DataSetReader_h
#define DataSetReader_h

#include <stdio.h>
#include <string>
#include <functional>
#include <vector>
#include "DataFlags.h"

/*!
 * \brief DataSetReader
 */
class DataSetReader
{
public:
    
    /*!
     * \brief type of a list of raw data
     */
    using AttributesRaw = std::vector< double >;
    /*!
     * \brief Row the type of a dataset row
     */
    struct Row
    {
        double                mClass { 0.0 };  //! class name
        AttributesRaw         mRaw;            //! raw data
        
        /*!
         * \brief size
         * \return the count of raw data
         */
        size_t size() const{ return mRaw.size(); }
        
        /*!
         * \brief operator[]
         * \param i
         * \return the raw data at index i
         */
        double& operator[] (size_t i)      { return mRaw[i]; }
        
        /*!
         * \brief operator[]
         * \param i
         * \return the raw data at index i
         */
        double  operator[] (size_t i) const{ return mRaw[i]; }
    };
    
    /*!
     * \brief Rows, list of dataset row
     */
    using Rows    =  std::vector< Row >;
    
    /*!
     * \brief Rows iterator
     */
    using RowsIt  =  Rows::iterator;
    
    /*!
     * \brief Rows constant iterator
     */
    using RowsCIt =  Rows::const_iterator;
    
    /*!
     *  \brief read from file
     *  \param path
     *  \return true if success to read
     */
    bool read(const std::string& path);
    
    /*!
     * \brief serialize into a file
     */
    void serialize(FILE* file);
    
    /*!
     * \brief deserialize from a file
     */
    void deserialize(FILE* file);
    
    /*!
     * \brief getFlags, return metadata
     * \return metadata
     */
    const DataFlags&    getFlags()      const { return mDataFlags; }
    
    /*!
     * \brief getClassNames, return list of classes
     */
    const ClassesNames& getClassNames() const { return mClassNames; }
    
    /*!
     * \brief return update value
     */
    unsigned int        getUpdate()           { return mUpdate; }
    
    /*!
     * \brief return dataset rows
     */
    const Rows&         getRows() const       { return mRows; }
    
    /*!
     * \brief size 
     * \return number of rows
     */
    size_t size() const{ return mRows.size(); }
    
    /*!
     * \brief sizeLine
     * \return size of a row
     */
    size_t sizeLine() const{ return mRows.size() ?  mRows[0].size() : 0; }
    
    /*!
     * \brief operator[]
     * \param i
     * \return the row at index i
     */
    Row& operator[] (size_t i)       { return mRows[i]; }
    
    /*!
     * \brief operator[]
     * \param i
     * \return the row at index i
     */
    const Row& operator[] (size_t i) const { return mRows[i]; }
    
    /*!
     * \brief begin
     * \return the first iterator object
     */
    RowsIt begin()        { return mRows.begin(); }
    
    /*!
     * \brief end
     * \return the last iterator object
     */
    RowsIt end()          { return mRows.end();   }
    
    /*!
     * \brief begin
     * \return the first constant iterator object
     */
    RowsCIt begin() const { return mRows.begin(); }
    
    
    /*!
     * \brief end
     * \return the last constant iterator object
     */
    RowsCIt end() const   { return mRows.end();   }
    
    
    
protected:
    
    //meta data
    DataFlags    mDataFlags;
    ClassesNames mClassNames;
    unsigned int mUpdate{ 0 };
    //Rows
    Rows mRows;
    
    
};

#endif /* DataSetReader_h */
