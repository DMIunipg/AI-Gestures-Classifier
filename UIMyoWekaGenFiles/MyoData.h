#ifndef MYODATA_H
#define MYODATA_H

/*!
 * \brief Myo Data, namespace where are put all application constants
 */
namespace MyoData
{
    static const unsigned short  byte      = sizeof(size_t); //! 64bit or 34bit version
    static const unsigned short  version   = 2;              //! file ouput version
    static const unsigned int    update    = 50;             //! myo listener update in frame per seconds
    static const unsigned int    msupadate = 1000/update;    //! myo listener update in ms
};

#endif // MYODATA_H

