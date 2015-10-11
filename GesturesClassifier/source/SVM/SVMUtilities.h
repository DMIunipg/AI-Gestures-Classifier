//
//  SVMSerialize.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 29/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef SVMUtilities_h
#define SVMUtilities_h
#pragma once
#include <string>
#include <cstdio>
#include <svm.h>

void svm_do_cross_validation(const svm_parameter& param, const svm_problem& prob,int nr_fold);

#endif /* SVMSerialize_hpp */
