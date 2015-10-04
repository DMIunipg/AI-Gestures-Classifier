//
//  kNNImp.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 04/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef kNNImp_h
#define kNNImp_h

#include <string>
#include "MyoThread.h"
#include "DataFlags.h"

namespace myoknn
{
    void create_model(const std::string& path);
    void* myo_classification(const std::string& path,MyoThread& myo,DataFlags&  flags,ClassesNames& cnames);
    void  myo_classification_free(void*);
};

#endif /* kNNImp_hpp */
