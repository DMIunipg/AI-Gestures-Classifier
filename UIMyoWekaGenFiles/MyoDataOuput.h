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

/*!
* \brief Myo Data Ouput
* \tparam EmgN the number of the emg sensors
*/
template < const size_t EmgN = 8 >
class MyoDataOuput
{
public:

    /*!
     * \brief ~MyoDataOuput
     */

    ~MyoDataOuput()
    {
        if(mFile) close();
    }

    /*!
     * \brief open
     * \param path
     * \param nclass
     */
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

    /*!
     * \brief append
     * \param className
     * \param listRows
     * \tparam T type of emg values
     * \tparam J type of components of gyroscope
     * \tparam H type of components of accelerometer
     * \tparam X type of components of quaternion
     * \tparam EmgN the number of the emg sensors
     */
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

    /*!
     * \brief isOpen
     * \return true if file is open
     */
    bool isOpen() const
    {
        return (bool)mFile;
    }

    /*!
     * \brief close
     */
    void close()
    {
        assert(mFile);
        //close
        std::fclose(mFile);
        //safe close
        mFile=nullptr;
    }

private:

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
    }

    /*!
     * \brief file
     * \return
     */
    std::FILE* file()
    {
        return mFile;
    }


    int mNClass { 0 };              //! number of classes
    std::FILE*  mFile { nullptr };  //! file pointer
};

#endif // MYODATAOUPUT

