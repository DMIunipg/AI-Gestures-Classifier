#ifndef MYOSERIALIZE_H
#define MYOSERIALIZE_H
#pragma once
#include <cstdio>
#include <string>
#include "Utilities.h"
#include "WekaRows.h"

//default serialize
template < typename T >
void serialize(const T& value, FILE* file )
{
    std::fwrite(&value, sizeof(T) ,1,file);
}

inline void serialize(const std::string& str, FILE* file)
{
    //write size
    size_t size = str.size();
    std::fwrite(&size,sizeof(size_t),1,file);
    //write buffer
    std::fwrite(str.data(),sizeof(str[0]),str.size(),file);
}

inline void serialize(const std::vector< std::string >& vstrs, FILE* file )
{
    //write size
    size_t size = vstrs.size();
    std::fwrite(&size,sizeof(size_t),1,file);
    //read all
    for(const std::string& str: vstrs) serialize(str, file);
}

template < class T,
           class J,
           class H,
           class X,
           const size_t N = 8 >
void serialize(const myo::RawDatas< T, J, H, X, N >& row, FILE* file)
{
    row.serialize(file);
}

template < class T,
           class J,
           class H,
           class X,
           const size_t N = 8 >
void serialize(const WekaRows< T , J , H , X , N >& rows, FILE* file)
{
    //write size
    size_t size = rows.size();
    std::fwrite(&size,sizeof(size_t),1,file);
    //write all
    for(size_t i=0;i!=size;++i) serialize(rows[i],file);
}


//default deserialize
template < typename T >
void deserialize( T& value, FILE* file )
{
    std::fread(&value, sizeof(T) ,1,file);
}

inline void deserialize(std::string& str, FILE* file)
{
    //get size
    size_t size = 0;
    std::fread(&size,sizeof(size_t),1,file);
    //alloc
    str.resize( size , ' ' );
    //read buffer
    std::fread((void*)str.data(),sizeof(str[0]),size,file);
}

inline void deserialize(std::vector< std::string >& vstrs, FILE* file )
{
    //write size
    size_t size = vstrs.size();
    std::fread(&size,sizeof(size_t),1,file);
    //clear all
    vstrs.clear();
    //read all
    for(size_t i=0; i!=size; ++i)
    {
        std::string tStr;
        deserialize(tStr, file);
        vstrs.push_back(std::move(tStr));
    }
}

template < class T,
           class J,
           class H,
           class X,
           const size_t N = 8 >
void deserialize(myo::RawDatas< T, J, H, X, N >& row, FILE* file)
{
    row.deserialize(file);
}

template < class T,
           class J,
           class H,
           class X,
           const size_t N = 8 >
void deserialize(WekaRows< T , J , H , X , N >& rows, FILE* file)
{
    //clear
    rows.clear();
    //read size
    size_t size = 0;
    std::fread(&size,sizeof(size_t),1,file);
    //read all
    for(size_t i=0;i!=size;++i)
    {
        //read row
        myo::RawDatas< T, J, H, X, N > row;
        deserialize(row,file);
        //append
        rows.append(row);
    }
}


#endif // MYOSERIALIZE_H

