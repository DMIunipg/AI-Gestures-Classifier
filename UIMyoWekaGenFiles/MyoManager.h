#ifndef MYOMANAGER_H
#define MYOMANAGER_H
#pragma once


#include <ctime>
#include <QMutex>
#include <QThread>
#include "MyoListener.h"
#include "MyoData.h"

/*!
 * \brief The MyoManager class
 */
class MyoManager : protected QThread
{
public:

    /*!
     * \brief MyoManager
     */
    MyoManager();

    /*!
     * \brief ~MyoManager
     */
    ~MyoManager();

    /*!
     * \brief startRecording
     */
    void startRecording();

    /*!
     * \brief startRecording
     * \param n
     */
    void startRecording(size_t n,
                        std::function<void(QMutex&,MyoListener::TypeRows&)>);

    /*!
     * \brief endRecording
     * \return datas registered
     */
    MyoListener::TypeRows endRecording();

    /*!
     * \brief getCurrentRow
     * \return last raw data registered
     */
    MyoListener::TypeRaw getCurrentRow();

    /*!
     * \return true if a myo is connected
     */
    bool isConnected() const;

    /*!
     * \return true if myo thread is running
     */
    bool isRunning() const;

    /*!
     * \brief run
     */
    void run();

    /*!
     * \brief start
     * \param priority
     */
    void start(Priority priority = InheritPriority)
    {
        mLoop = true;
        QThread::start(priority);
    }

    /*!
     * \brief close
     */
    void close()
    {
        mLoop = false;
        QThread::wait();
    }

    /*!
     * \brief terminate
     */
    void terminate()
    {
        mLoop = false;
        QThread::terminate();
    }

    /*!
     * \brief quit
     */
    void quit()
    {
        mLoop = false;
        QThread::quit();
    }

protected:

     myo::Hub*             mMyoHub   { nullptr }; //! myo hub pointer
     myo::Myo*             mMyo      { nullptr }; //! myo device pointer
     MyoListener           mListener;             //! myo listener instance
     bool                  mLoop{ false };        //! loop flag
     QMutex                mMutex;                //! mutex

     /*!
      * \brief The State enum
      */
     enum State
     {
        NO_REC,
        LIST_REC,
        CALLBACK_REC
     };
     State                 mRecording { NO_REC };        //! current state
     MyoListener::TypeRows mDatas;                       //! last raw data
     double                mTime{ 0 };                   //! time of recording
     size_t                                               mSizeData{ 0 };      //! size of data
     size_t                                               mSizeNext{ 0 };      //! next size
     std::function<void(QMutex&,MyoListener::TypeRows&)>  mCallback{ nullptr };//! callback to pass the raw data


};

#endif // MYOMANAGER_H
