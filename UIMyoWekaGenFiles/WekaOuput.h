//
//  WekaOuput.h
//  MyoWekaGenFiles
//
//  Created by Daniele on 26/06/15.
//  Copyright (c) 2015 Daniele. All rights reserved.
//

#ifndef MYO_WEKAOUTPUT_H
#define MYO_WEKAOUTPUT_H
#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <assert.h>
#include "Utilities.h"
#include "DataFlags.h"
#include "WekaRows.h"

template < const size_t EmgN = 8 >
class WekaOuput
{
public:
    
    ~WekaOuput()
    {
        if(mFile) close();
    }
    
    void open(const std::string& path,
              const DataFlags& flags,
              const std::vector<std::string>& mWekaClass)
    {

        assert(mFlags.mGyroscope     ||
               mFlags.mAccelerometer ||
               mFlags.mQuaternion    ||
               mFlags.mEuler         ||
               mFlags.mEmg);
        //set flags
        mFlags = flags;
        //compute class string
        buildHeaderClass(mWekaClass);
        //open file
        open(path);
    }

    template < class T,
               class J,
               class H,
               class X,
               const size_t N = 8 >
    void append(const std::string& className,const WekaRows< T , J , H , X , N >& rows)
    {

        //assert
        assert(isOpen());
        assert(EmgN == N);
        //count rows
        size_t nrows=rows.size()/mFlags.mReps;
        //row
        std::string strRow;
        for(size_t i=0;i!=nrows;++i)
        {
            for(size_t j=0;j!=mFlags.mReps;++j)
            {
                auto& row = rows[i*mFlags.mReps+j];
                //append gyroscope
                if(mFlags.mGyroscope)
                {
                    strRow += std::to_string(row.getGyroscope().x()) +",";
                    strRow += std::to_string(row.getGyroscope().y()) +",";
                    strRow += std::to_string(row.getGyroscope().z()) +",";
                }
                //append accelerometer
                if(mFlags.mAccelerometer)
                {
                    strRow += std::to_string(row.getAccelerometer().x()) +",";
                    strRow += std::to_string(row.getAccelerometer().y()) +",";
                    strRow += std::to_string(row.getAccelerometer().z()) +",";
                }
                //append quaternion
                if(mFlags.mQuaternion)
                {
                    strRow += std::to_string(row.getQuaternion().x()) +",";
                    strRow += std::to_string(row.getQuaternion().y()) +",";
                    strRow += std::to_string(row.getQuaternion().z()) +",";
                    strRow += std::to_string(row.getQuaternion().w()) +",";
                }
                //append euler angle
                if(mFlags.mEuler)
                {
                    strRow += std::to_string(row.getEulerAngles().roll()) +",";
                    strRow += std::to_string(row.getEulerAngles().pitch()) +",";
                    strRow += std::to_string(row.getEulerAngles().yaw()) +",";
                }
                //append emg
                if(mFlags.mEmg)
                {
                    for(auto value:row.getEmg())
                        strRow += std::to_string(value)+",";
                }
            }
            //append class
            strRow += className + "\n";
        }
        //write
        std::fwrite(strRow.data(),strRow.size(),1,file());
    }
    
    bool isOpen() const
    {
        return (bool)mFile;
    }

private:


    void buildHeaderClass(const std::vector<std::string>& wekaClass)
    {
        assert(wekaClass.size());
        mWekaClass="";
        size_t i=0;
        for(i=0; i!=wekaClass.size()-1 ;++i)  mWekaClass += wekaClass[i] +",";
        mWekaClass+=wekaClass[i];
    }

    void createHeader(std::string& header) const
    {
        //start header
        header="@RELATION myo\n";
        //header attributes
        for(int i=0; i!=mFlags.mReps; ++i)
        {
            if (mFlags.mGyroscope)
            {
                header += "@ATTRIBUTE gyroscope"+std::to_string(i+1)+"_x NUMERIC\n";
                header += "@ATTRIBUTE gyroscope"+std::to_string(i+1)+"_y NUMERIC\n";
                header += "@ATTRIBUTE gyroscope"+std::to_string(i+1)+"_z NUMERIC\n";
            }
            if (mFlags.mAccelerometer)
            {
                header += "@ATTRIBUTE accelerometer"+std::to_string(i+1)+"_x NUMERIC\n";
                header += "@ATTRIBUTE accelerometer"+std::to_string(i+1)+"_y NUMERIC\n";
                header += "@ATTRIBUTE accelerometer"+std::to_string(i+1)+"_z NUMERIC\n";
            }
            if (mFlags.mQuaternion)
            {
                header += "@ATTRIBUTE quaternion"+std::to_string(i+1)+"_x NUMERIC\n";
                header += "@ATTRIBUTE quaternion"+std::to_string(i+1)+"_y NUMERIC\n";
                header += "@ATTRIBUTE quaternion"+std::to_string(i+1)+"_z NUMERIC\n";
                header += "@ATTRIBUTE quaternion"+std::to_string(i+1)+"_w NUMERIC\n";
            }
            if (mFlags.mEuler)
            {
                header += "@ATTRIBUTE roll"+std::to_string(i+1)+" NUMERIC\n";
                header += "@ATTRIBUTE pitch"+std::to_string(i+1)+" NUMERIC\n";
                header += "@ATTRIBUTE yaw"+std::to_string(i+1)+" NUMERIC\n";
            }
            for(size_t j=0;mFlags.mEmg && j!=EmgN;++j)
            {
                header += "@ATTRIBUTE emg"+ std::to_string(i+1)+'_'+std::to_string(j+1)+" NUMERIC\n";
            }
        }
        //class
        header+="@ATTRIBUTE class {"+mWekaClass+"}\n";
        //start data
        header+="@data\n";
    }

    void writeHeader()
    {
        //create header
        std::string header;
        createHeader(header);
        //write header
        std::fwrite(header.data(),header.size(),1,file());
    }

    void close()
    {
        assert(mFile);
        std::fclose(mFile);
        mFile=nullptr;
    }

    void open(const std::string& path)
    {
        //close file
        if(mFile) close();
        //open file
        mFile = std::fopen(path.c_str(),"wb");
        //write header
        writeHeader();
    }

    std::FILE* file()
    {
        return mFile;
    }

    //file data
    std::FILE*  mFile { nullptr };
    std::string mWekaClass;
    //flags
    DataFlags mFlags;
};

#endif
