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


class DataSetReader
{
public:
    
    //raw data
    using AttributesRaw = std::vector< double >;
    //row type
    struct Row
    {
        double                mClass { 0.0 };
        AttributesRaw         mRaw;
        //count raw
        size_t size() const{ return mRaw.size(); }
        //get raw
        double& operator[] (size_t i)      { return mRaw[i]; }
        double  operator[] (size_t i) const{ return mRaw[i]; }
    };
    //rows types
    using Rows    =  std::vector< Row >;
    using RowsIt  =  Rows::iterator;
    using RowsCIt =  Rows::const_iterator;
    
    //read file
    bool read(const std::string& path);
    void serialize(FILE* file);
    void deserialize(FILE* file);
    //get meta info
    const DataFlags&    getFlags()      const { return mDataFlags; }
    const ClassesNames& getClassNames() const { return mClassNames; }
    unsigned int        getUpdate()           { return mUpdate; }
    const Rows&         getRows() const       { return mRows; }
    //read raw
    size_t size() const{ return mRows.size(); }
    size_t sizeLine() const{ return mRows.size() ?  mRows[0].size() : 0; }
    //get row
          Row& operator[] (size_t i)       { return mRows[i]; }
    const Row& operator[] (size_t i) const { return mRows[i]; }
    //for each
    RowsIt begin()        { return mRows.begin(); }
    RowsIt end()          { return mRows.end();   }
    RowsCIt begin() const { return mRows.begin(); }
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
