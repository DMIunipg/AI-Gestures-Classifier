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
#include <QString>
#include <QList>
#include <assert.h>
#include "Utilities.h"
#include "DataFlags.h"
#include "WekaRows.h"
/*!
 * \brief Weka Ouput
 * \tparam EmgN the number of the emg sensors
 */
template < const size_t EmgN = 8 >
class WekaOuput
{
public:

    /*!
     * \brief ~WekaOuput
     */
    ~WekaOuput()
    {
        if(mFile) close();
    }

    /*!
     * \brief open
     * \param path
     * \param flags
     * \param mWekaClass
     */
    void open(const std::string& path,
              const DataFlags& flags,
              const std::vector<std::string>& mWekaClass)
    {

        assert(mFlags.mGyroscope     ||
               mFlags.mAccelerometer ||
               mFlags.mQuaternion    ||
               mFlags.mPitch         ||
               mFlags.mYaw           ||
               mFlags.mRoll          ||
               mFlags.mEmg);
        //set flags
        mFlags = flags;
        //compute class string
        buildHeaderClass(mWekaClass);
        //open file
        open(path);
    }

    /*!
     * \brief open
     * \param path
     * \param flags
     * \param mWekaClass
     */
    void open(const std::string& path,
              const DataFlags& flags,
              const QList< QString >& mWekaClass)
    {

        assert(mFlags.mGyroscope     ||
               mFlags.mAccelerometer ||
               mFlags.mQuaternion    ||
               mFlags.mPitch         ||
               mFlags.mYaw           ||
               mFlags.mRoll          ||
               mFlags.mEmg);
        //set flags
        mFlags = flags;
        //compute class string
        buildHeaderClass(mWekaClass);
        //open file
        open(path);
    }

    /*!
     * \brief append
     * \param className
     * \param rows
     * \tparam T type of emg values
     * \tparam J type of components of gyroscope
     * \tparam H type of components of accelerometer
     * \tparam X type of components of quaternion
     * \tparam N the number of the emg sensors
     */
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
        //get max time
        double maxTime = 0.0;
        //for each
        for(size_t i=0;i!=rows.size();++i)
        {
            maxTime=std::max(maxTime,rows[i].getTime());
        }
        //alloc emgs
        auto listEmgs = new std::array< int8_t, N >[mFlags.mReps];
        //row
        std::string strRow;
        for(size_t i=0;i!=nrows;++i)
        {
            if(mFlags.mEmg)
            {
                //copy emg
                for(size_t j=0;j!=mFlags.mReps;++j)
                {
                   auto& row   = rows[i*mFlags.mReps+j];
                   listEmgs[j] = row.getEmg();
                }
                //applay filters
                mFlags.applayEmgFilter(listEmgs,mFlags.mReps);
            }
            //put values
            for(size_t j=0;j!=mFlags.mReps;++j)
            {
                auto& row = rows[i*mFlags.mReps+j];
                //append time
                if(mFlags.mTime)
                    strRow += std::to_string(mFlags.toNormalize(row.getTime(),maxTime))+",";
                //append gyroscope
                if(mFlags.mGyroscope)
                {
                    auto gs = mFlags.apply( row.getGyroscope() );
                    strRow += std::to_string(gs.x()) +",";
                    strRow += std::to_string(gs.y()) +",";
                    strRow += std::to_string(gs.z()) +",";
                }
                //append accelerometer
                if(mFlags.mAccelerometer)
                {
                    auto ac = mFlags.apply( row.getAccelerometer() );
                    strRow += std::to_string(ac.x()) +",";
                    strRow += std::to_string(ac.y()) +",";
                    strRow += std::to_string(ac.z()) +",";
                }
                //append quaternion
                if(mFlags.mQuaternion)
                {
                    auto qu = mFlags.apply( row.getQuaternion() );
                    strRow += std::to_string(qu.x()) +",";
                    strRow += std::to_string(qu.y()) +",";
                    strRow += std::to_string(qu.z()) +",";
                    strRow += std::to_string(qu.w()) +",";
                }
                //append euler angle
                if(mFlags.mPitch)
                    strRow += std::to_string(mFlags.apply(row.getEulerAngles().pitch(), M_PI*2.0)) +",";
                if(mFlags.mYaw)
                    strRow += std::to_string(mFlags.apply(row.getEulerAngles().yaw(), M_PI*2.0)) +",";
                if(mFlags.mRoll)
                    strRow += std::to_string(mFlags.apply(row.getEulerAngles().roll(), M_PI*2.0)) +",";
                //append emg
                if(mFlags.mEmg)
                {
                    for(auto value:listEmgs[j])
                        strRow += std::to_string(mFlags.apply((double)value,128))+",";
                }
            }
            //append class
            strRow += className + "\n";
        }
        //dealloc
        delete [] listEmgs;
        //write
        std::fwrite(strRow.data(),strRow.size(),1,file());
    }

    /*!
     * \brief isOpen
     * \return true if file is open
     */
    bool isOpen() const
    {
        return (bool)mFile;
    }

private:


    /*!
     * \brief buildHeaderClass
     * \param wekaClass
     */
    void buildHeaderClass(const std::vector<std::string>& wekaClass)
    {
        assert(wekaClass.size());
        mWekaClass="";
        size_t i=0;
        for(i=0; i!=wekaClass.size()-1 ;++i)  mWekaClass += wekaClass[i] +",";
        mWekaClass+=wekaClass[i];
    }

    /*!
     * \brief buildHeaderClass
     * \param wekaClass
     */
    void buildHeaderClass(const QList< QString >& wekaClass)
    {
        assert(wekaClass.size());
        //count
        size_t i = 0;
        //first
        mWekaClass = wekaClass.begin()->toStdString();
        //next
        for(auto it= ++wekaClass.begin(); it!=wekaClass.end();++it )
        {
            mWekaClass += "," ;
            mWekaClass += it->toStdString() ;
        }
    }

    /*!
     * \brief createHeader
     * \param header
     */
    void createHeader(std::string& header) const
    {
        //start header
        header="@RELATION myo\n";
        //header attributes
        for(int i=0; i!=mFlags.mReps; ++i)
        {
            if(mFlags.mTime)
                header += "@ATTRIBUTE time"+std::to_string(i+1)+" NUMERIC\n";

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

            if(mFlags.mPitch)
                header += "@ATTRIBUTE pitch"+std::to_string(i+1)+" NUMERIC\n";
            if(mFlags.mYaw)
                header += "@ATTRIBUTE yaw"+std::to_string(i+1)+" NUMERIC\n";
            if(mFlags.mRoll)
                header += "@ATTRIBUTE roll"+std::to_string(i+1)+" NUMERIC\n";

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

    /*!
     * \brief writeHeader
     */
    void writeHeader()
    {
        //create header
        std::string header;
        createHeader(header);
        //write header
        std::fwrite(header.data(),header.size(),1,file());
    }

    /*!
     * \brief close
     */
    void close()
    {
        assert(mFile);
        std::fclose(mFile);
        mFile=nullptr;
    }

    /*!
     * \brief open
     * \param path
     */
    void open(const std::string& path)
    {
        //close file
        if(mFile) close();
        //open file
        mFile = std::fopen(path.c_str(),"wb");
        //write header
        writeHeader();
    }

    /*!
     * \brief file
     * \return file pointer
     */
    std::FILE* file()
    {
        return mFile;
    }

    //file data
    std::FILE*  mFile { nullptr }; //! file pointer
    std::string mWekaClass;        //! string of classes in weka format
    DataFlags mFlags;              //! meta data info
};

#endif
