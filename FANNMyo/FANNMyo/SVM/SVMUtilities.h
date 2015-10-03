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
#include "svm.h"

svm_problem* svm_deserialize(const std::string& path);
void svm_unnormalize_classes_names(svm_problem* dataset);
double svm_normalize_class_name(double classname);
void svm_free(svm_problem* dataset);
void svm_do_cross_validation(const svm_parameter& param, const svm_problem& prob,int nr_fold);

#endif /* SVMSerialize_hpp */
