#ifndef WEKAROWS_H
#define WEKAROWS_H
#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <assert.h>
#include "Utilities.h"

/*!
 * \brief Weka Rows
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
class WekaRows
{
public:

    /*!
     * \brief Row type
     */
    using Row     = typename myo::RawDatas< T , J , H, X , N >;

    /*!
     * \brief List of rows type
     */
    using List    = typename std::vector< myo::RawDatas< T , J , H, X , N > >;

    /*!
     * \brief List Iterator
     */
    using ListIt  = typename std::vector< myo::RawDatas< T , J , H, X , N > >::iterator;

    /*!
     * \brief Constant List Iterator
     */
    using ListCIt = typename std::vector< myo::RawDatas< T , J , H, X , N > >::const_iterator;

    /*!
     * \brief append
     * \param raw
     */
    void append(const Row& raw)
    {
        mList.push_back(std::move(raw));
    }

    /*!
     * \brief size
     * \return count of the rows
     */
    size_t size() const
    {
        return mList.size();
    }

    /*!
     * \brief clear
     */
    void clear()
    {
        mList.clear();
    }

    /*!
     * \brief operator []
     * \param i
     * \return row
     */
    Row& operator[](size_t i)
    {
        assert(i<=size());
        return mList[i];
    }

    /*!
     * \brief operator []
     * \param i
     * \return row
     */
    const Row& operator[](size_t i) const
    {
        assert(i<=size());
        return mList[i];
    }

    /*!
     * \brief resize
     * \param size
     */
    void resize(size_t size)
    {
        mList.resize(size);
    }

    /*!
     * \brief begin
     * \return iterator
     */
    ListIt begin()
    {
        return mList.begin();
    }

    /*!
     * \brief begin
     * \return iterator
     */
    ListCIt begin() const
    {
        return mList.begin();
    }

    /*!
     * \brief end
     * \return iterator
     */
    ListIt end()
    {
        return mList.end();
    }

    /*!
     * \brief end
     * \return iterator
     */
    ListCIt end() const
    {
        return mList.end();
    }

private:

    List mList; //! list of rows

};

#endif // WEKAROWS_H

