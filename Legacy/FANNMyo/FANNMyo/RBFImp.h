//
//  RBFImp.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 02/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RBFImp_h
#define RBFImp_h

#include <string>
#include "MyoThread.h"
#include "DataFlags.h"

namespace myorbf
{
    void create_model(const std::string& path);
    void* myo_classification(const std::string& path,MyoThread& myo,DataFlags&  flags,ClassesNames& cnames);
    void  myo_classification_free(void*);
};

#endif /* RBFImp_hpp */
