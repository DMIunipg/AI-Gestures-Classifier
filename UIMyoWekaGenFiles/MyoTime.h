#ifndef MYOTIME_H
#define MYOTIME_H
#pragma once

#include <inttypes.h>

namespace myo
{
    /*!
     * \brief GetTimeTicks
     * \return  cpu tricks
     */
    extern uint64_t GetTimeTicks();

    /*!
     * \brief GetTime
     * \return current time in seconds
     */
    extern double GetTime();
};

#endif // MYOTIME_H

