#ifndef MYODATAOUPUT_H
#define MYODATAOUPUT_H
#pragma once

#include <cstdio>
#include <vector>
#include <algorithm>
#include <QLinkedList>
#include "MyoSerialize.h"

namespace MyoData
{
    static const short byte    = sizeof(size_t);
    static const short version = 1.0;
};

template < const size_t EmgN = 8 >
class MyoDataOuput
{
public:

    ~MyoDataOuput()
    {
        if(mFile) close();
    }

    void open(const std::string& path,size_t nclass)
    {
        //open file
        open(path);
        //8/6 (byte machine)
        serialize(MyoData::byte,file());
        //version
        serialize(MyoData::version,file());
        //write count of class
        serialize(nclass,file());
        //classes
        mNClass = nclass;
    }


    template < class T,
               class J,
               class H,
               class X,
               const size_t N = 8 >
    void append(const std::string& className,
                QLinkedList< WekaRows< T , J , H , X , N > >& listRows)
    {
        //assert
        assert(isOpen());
        assert(EmgN == N);
        //write name
        serialize(className,file());
        //write number of samples
        serialize(listRows.size(),file());
        //serialize all rows
        for(auto& wrows:listRows) serialize(wrows,file());
    }

    bool isOpen() const
    {
        return (bool)mFile;
    }

    void close()
    {
        assert(mFile);
        //close
        std::fclose(mFile);
        //safe close
        mFile=nullptr;
    }

private:

    void open(const std::string& path)
    {
        //close file
        if(mFile) close();
        //open file
        mFile = std::fopen(path.c_str(),"wb");
    }

    std::FILE* file()
    {
        return mFile;
    }

    //file data
    int mNClass { 0 };
    std::FILE*  mFile { nullptr };
};

#endif // MYODATAOUPUT

