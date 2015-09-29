#ifndef FANNOUPUT_H
#define FANNOUPUT_H
#pragma once

#include <cstdio>
#include <vector>
#include <ostream>
#include <fstream>
#include <sstream>
#include <QList>
#include <QString>
#include <math.h>
#include "DataFlags.h"



template < const size_t EmgN = 8 >
class FANNOutput
{
public:

    ~FANNOutput()
    {
        if(isOpen()) close();
    }


    void open(const std::string& path,
              const DataFlags& flags,
              const QList< QString >& classes)
    {

        assert(mFlags.mGyroscope     ||
               mFlags.mAccelerometer ||
               mFlags.mQuaternion    ||
               mFlags.mEuler         ||
               mFlags.mEmg);
        //set flags
        mFlags = flags;
        //set class
        mClasses = classes;
        //open file
        open(path);
    }
    template < class T,
               class J,
               class H,
               class X,
               const size_t N = 8 >
    void append(const QString& className,const WekaRows< T , J , H , X , N >& rows)
    {

        //assert
        assert(isOpen());
        assert(EmgN == N);
        //find class's id
        int idClass= mClasses.indexOf(className);
        assert(idClass != -1);
        //count rows
        size_t nrows=rows.size()/mFlags.mReps;
        //get max time
        double maxTime = 0.0;
        //for each
        for(size_t i=0;i!=rows.size();++i)
        {
            maxTime=std::max(maxTime,rows[i].getTime());
        }
        //
        for(size_t i=0;i!=nrows;++i)
        {
            //stream row
            mLines.push_back(std::move(std::stringstream()));
            std::stringstream& stream=mLines[mLines.size()-1];
            //put all values
            for(size_t j=0;j!=mFlags.mReps;++j)
            {
                auto& row = rows[i*mFlags.mReps+j];
                //append time
                if(mFlags.mTime)
                {
                    stream << row.getTime()/maxTime << " ";
                }
                //append gyroscope
                if(mFlags.mGyroscope)
                {
                    auto gn = row.getGyroscope().normalized();
                    stream << gn.x() <<" ";
                    stream << gn.y() <<" ";
                    stream << gn.z() <<" ";
                }
                //append accelerometer
                if(mFlags.mAccelerometer)
                {
                    auto an = row.getAccelerometer().normalized();
                    stream <<  an.x() << " ";
                    stream <<  an.y() << " ";
                    stream <<  an.z() << " ";
                }
                //append quaternion
                if(mFlags.mQuaternion)
                {
                    auto qn = row.getQuaternion().normalized();
                    stream << qn.x() << " ";
                    stream << qn.y() << " ";
                    stream << qn.z() << " ";
                    stream << qn.w() << " ";
                }
                //append euler angle
                if(mFlags.mEuler)
                {
                    stream <<  row.getEulerAngles().roll() / M_PI*2.0 << " ";
                    stream <<  row.getEulerAngles().pitch()/ M_PI*2.0 << " ";
                    stream <<  row.getEulerAngles().yaw()  / M_PI*2.0  << " ";
                }
                //append emg
                if(mFlags.mEmg)
                {
                    for(int value:row.getEmg())
                    {
                        stream << std::to_string((double)value/128) << " ";
                    }
                }
            }
            //max -1 +1 normal class
            double size = ((double)mClasses.size()-1);
            //class row
            mLines.push_back(std::move(std::stringstream()));
            mLines[mLines.size()-1] << ((double)idClass / size) * 2.0 - 1.0;

        }
    }

    bool isOpen() const
    {
        return mFile.is_open();
    }

private:

    void close()
    {
        //header
        std::stringstream header;
        header << mLines.size() / 2 << " " << mFlags.lineSize<EmgN>() << " 1";
        //save
        mFile << header.str() << "\n";
        for(auto& line : mLines) mFile << line.str() << "\n";
        //close
        mFile.close();
        //write meta data
        writeMataData();
        //write map name
        writeMapNames();
    }

    void writeMataData() const
    {
        std::string mtPath = mPath+".meta";
        //create file
        FILE* metaFile = std::fopen(mtPath.c_str(),"w");
        //write
        mFlags.serialize(metaFile);
        //close
        std::fclose(metaFile);
    }

    void writeMapNames() const
    {
        //path
        std::string mcPath = mPath+".classes";
        //file ouput
        std::ofstream file;
        //open file
        file.open(mcPath);
        //is if open...
        if(file.is_open())
        {
            //max -1 +1 normal class
            double size = ((double)mClasses.size()-1);
            //write number of classes
            file << std::to_string(mClasses.size()) << "\n";
            //write names
            for(size_t i=0;i!=mClasses.size();++i)
            {
                double id=((double)i / size) * 2.0 - 1.0;
                file << std::to_string(id) << ", ";
                file << mClasses[i].toStdString()  << "\n";
            }
        }
    }

    void open(const std::string& path)
    {
        //close file
        if(isOpen()) close();
        //open file
        mFile.open(path);
        //save path
        mPath = path;
    }

    //file data
    std::string mPath;
    std::ofstream  mFile;
    QList < QString > mClasses;
    std::vector < std::stringstream > mLines;
    //flags
    DataFlags mFlags;
};


#endif // FANNOUPUT

