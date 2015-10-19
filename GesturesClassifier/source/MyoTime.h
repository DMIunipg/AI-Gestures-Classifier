#ifndef MYOTIME_H
#define MYOTIME_H
#pragma once

#include <inttypes.h>

namespace myo
{
    /*!
     * \brief GetTimeTicks
     * \return tricks
     */
    extern uint64_t GetTimeTicks();
    
    /*!
     * \brief GetTime
     * \return time in seconds
     */
    extern double GetTime();
};

#endif // MYOTIME_H

