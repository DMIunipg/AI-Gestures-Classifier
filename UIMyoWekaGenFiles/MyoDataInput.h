#ifndef MYODATAINPUT_H
#define MYODATAINPUT_H
#pragma once

#include <cstdio>
#include <vector>
#include <algorithm>
#include <QMap>
#include <QLinkedList>
#include "Utilities.h"
#include "MyoSerialize.h"
#include "MyoData.h"
#include "DataFlags.h"

/*!
 * \brief The MyoDataInput class
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
           const size_t EmgN = 8 >
class MyoDataInput
{
public:

    using Sample      = WekaRows< T , J , H , X , EmgN >;
    using ListSamples = QLinkedList< Sample >;
    using Callback    = std::function< void( const std::string& , ListSamples& ) >;
    using LGCallback  = std::function< void( int idClass, const std::string& className, const Sample& ) >;

    /*!
     * \brief read
     * \param path
     * \param callback
     * \return success to write file
     */
    bool read(const std::string& path,
              DataFlags& flags,
              Callback callback)
    {
        //open file
        std::FILE* file=std::fopen(path.c_str(),"rb");
        //read
        if(file)
        {
            unsigned short  byte     = 0;
            unsigned short  version  = 0;
            size_t nclass            = 0;
            unsigned int    msupdate = 0;
            //deserialize
            deserialize(byte,file);
            deserialize(version,file);
            deserialize(nclass,file);
            //if version 2.0
            if(version == 2)
            {
                //get update time
                deserialize(msupdate,file);
                //get flags info
                flags.deserialize(file);
            }
            //for all class
            for(size_t id=0; id!=nclass; ++id)
            {
                //get name
                std::string className;
                deserialize(className,file);
                //get sample list
                int sizeList = 0;
                deserialize(sizeList,file);
                //alloc list
                ListSamples list;
                //read
                for(int i=0;i!=sizeList;++i)
                {
                    Sample sample;
                    deserialize(sample,file);
                    list.append(sample);
                }
                //callback
                callback(className,list);
            }
        }

    }

    /*!
     * \brief readOldFile
     * \param path
     * \param callback
     * \return success to write file
     */
    bool readOldFile(const std::string& path, LGCallback callback) const
    {
        //open file
        std::FILE* file=std::fopen(path.c_str(),"rb");
        //class id
        QMap< std::string , size_t > mapClasses;
        //read
        if(file)
        {
            //read count rows
            size_t size  = 0;
            size_t idGen = 0;
            deserialize(size,file);
            //read all
            for(size_t id=0; id!=size; ++id)
            {
                //read name class
                std::string str;
                deserialize(str,file);
                //get id from name
                auto idClass = mapClasses.find(str);
                //if not found
                if(idClass == mapClasses.end())
                {
                    //gen a new id
                    idClass = mapClasses.insert(str,idGen++);
                }
                //read rows
                WekaRows< T , J , H , X , EmgN > rows;
                deserialize(rows,file);
                //call the callback
                callback(idClass.value(),str,rows);
            }

        }
        return false;
    }

};

#endif // MYODATAINPUT_H

