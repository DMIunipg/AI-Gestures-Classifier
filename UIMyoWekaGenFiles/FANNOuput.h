#ifndef FANNOUPUT_H
#define FANNOUPUT_H
#pragma once

#include <cstdio>
#include <vector>
#include <ostream>
#include <fstream>
#include <sstream>
#include <math.h>



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
              const std::vector<std::string>& classes)
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
    void append(const std::string& className,const WekaRows< T , J , H , X , N >& rows)
    {

        //assert
        assert(isOpen());
        assert(EmgN == N);
        //find
        auto it=std::find(mClasses.begin(),mClasses.end(),className);
        assert(it != mClasses.end());
        size_t idClass= it - mClasses.begin();
        //count rows
        size_t nrows=rows.size()/mFlags.mReps;
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
    }

    void open(const std::string& path)
    {
        //close file
        if(isOpen()) close();
        //open file
        mFile.open(path);
    }

    //file data
    std::ofstream  mFile;
    std::vector < std::string > mClasses;
    std::vector < std::stringstream > mLines;
    //flags
    DataFlags mFlags;
};


#endif // FANNOUPUT

