//
//  Datapoint.h
//  FANNMyo
//
//  Created by Gabriele Di Bari on 02/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef Datapoint_h
#define Datapoint_h
#pragma once

#include <vector>

typedef std::vector<double> datapoint;

struct Dataset
{
    size_t                   mCCount {0};
    std::vector< datapoint > mDatas;
    std::vector< int >       mLabels;
};

#endif /* Datapoint_h */
