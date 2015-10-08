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


/*!
 * \brief The FANNOutput class
 * \tparam EmgN the number of the emg sensors
 */
template < const size_t EmgN = 8 >
class FANNOutput
{
public:

    /*!
     * \brief ~FANNOutput
     */
    ~FANNOutput()
    {
        if(isOpen()) close();
    }

    /*!
     * \brief open
     * \param path
     * \param flags
     * \param classes
     */
    void open(const std::string& path,
              const DataFlags& flags,
              const QList< QString >& classes)
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
        //set class
        mClasses = classes;
        //open file
        open(path);
    }

    /*!
     * \brief topositive
     * \param t
     * \param topos
     * \return T
     */
    template < class T >
    T topositive(const T& t,bool topos)
    {
        return topos? (t+1.0)*0.5 : t;
    }

    /*!
     * \brief append
     * \param className
     * \param rows
     * \tparam T
     * \tparam J
     * \tparam H
     * \tparam X
     * \tparam N the number of the emg sensors
     */
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
        //alloc emgs
        auto listEmgs = new std::array< int8_t, N >[mFlags.mReps];
        //...
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
                    stream << mFlags.toNormalize( row.getTime(), maxTime ) << " ";
                }
                //append gyroscope
                if(mFlags.mGyroscope)
                {
                    auto gn = mFlags.apply( row.getGyroscope() );
                    stream << gn.x() <<" ";
                    stream << gn.y() <<" ";
                    stream << gn.z() <<" ";
                }
                //append accelerometer
                if(mFlags.mAccelerometer)
                {
                    auto an = mFlags.apply( row.getAccelerometer() );
                    stream <<  an.x() << " ";
                    stream <<  an.y() << " ";
                    stream <<  an.z() << " ";
                }
                //append quaternion
                if(mFlags.mQuaternion)
                {
                    auto qn = mFlags.apply( row.getQuaternion() );
                    stream << qn.x() << " ";
                    stream << qn.y() << " ";
                    stream << qn.z() << " ";
                    stream << qn.w() << " ";
                }
                //append euler angle
                if(mFlags.mPitch)
                   stream <<  mFlags.apply( row.getEulerAngles().pitch(), (M_PI*2.0) ) << " ";
                if(mFlags.mYaw)
                   stream <<  mFlags.apply( row.getEulerAngles().yaw(),   (M_PI*2.0) ) << " ";
                if(mFlags.mRoll)
                   stream <<  mFlags.apply( row.getEulerAngles().roll(),  (M_PI*2.0) ) << " ";
                //append emg
                if(mFlags.mEmg)
                {
                    for(auto value:listEmgs[j])
                    {
                        stream << std::to_string(mFlags.apply( (double)value , 128 )) << " ";
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

    /*!
     * \brief isOpen
     * \return true if file is open
     */
    bool isOpen() const
    {
        return mFile.is_open();
    }

private:

    /*!
     * \brief close
     */
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

    /*!
     * \brief writeMataData
     */
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

    /*!
     * \brief writeMapNames
     */
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

    /*!
     * \brief open
     * \param path
     */
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
    std::string mPath; //! the path of the file
    std::ofstream  mFile; //! file object
    QList < QString > mClasses; //! list of classes
    std::vector < std::stringstream > mLines; //! ouput lines
    //flags
    DataFlags mFlags; //! meta data info
};


#endif // FANNOUPUT

