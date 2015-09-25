#ifndef MYODATAINPUT_H
#define MYODATAINPUT_H
#pragma once

#include <cstdio>
#include <vector>
#include <algorithm>
#include "Utilities.h"
#include "MyoSerialize.h"

template < class T,
           class J,
           class H,
           class X,
           const size_t EmgN = 8 >
class MyoDataInput
{
public:

    bool read(const std::string& path,
              std::function< void(int idClass,
                                  const std::string& className,
                                  const myo::RawDatas< T , J , H, X , EmgN >& row) > callback) const
    {
        //open file
        std::FILE* file=std::fopen(path.c_str(),"rb");
        //read
        if(file)
        {
            //read count rows
            size_t size = 0;
            deserialize(size,file);
            //read all
            for(size_t id=0; id!=size; ++id)
            {
                //read name
                std::string str;
                deserialize(str,file);
                //read rows
                WekaRows< T , J , H , X , EmgN > rows;
                deserialize(rows,file);
                //call the callback
                for(int ir = 0; ir!=rows.size() ;++ir)
                {
                    callback((int)id,str,rows[ir]);
                }
            }

        }
        return false;
    }

};

#endif // MYODATAINPUT_H

