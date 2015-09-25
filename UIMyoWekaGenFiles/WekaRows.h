#ifndef WEKAROWS_H
#define WEKAROWS_H
#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <assert.h>
#include "Utilities.h"

template < class T,
           class J,
           class H,
           class X,
           const size_t N = 8 >
class WekaRows
{
public:
    using Row  = typename myo::RawDatas< T , J , H, X , N >;
    using List = typename std::vector< myo::RawDatas< T , J , H, X , N > >;

    void append(const Row& raw)
    {
        mList.push_back(std::move(raw));
    }

    size_t size() const
    {
        return mList.size();
    }

    void clear()
    {
        mList.clear();
    }

    Row& operator[](size_t i)
    {
        assert(i<=size());
        return mList[i];
    }

    const Row& operator[](size_t i) const
    {
        assert(i<=size());
        return mList[i];
    }

private:

    List mList;

};

#endif // WEKAROWS_H

