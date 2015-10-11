//
//  FANNImp.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 29/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef FANNImp_h
#define FANNImp_h

#include <string>
#include "MyoThread.h"
#include "DataFlags.h"

namespace myofann
{
    void create_model(const std::string& path);
    void* myo_classification(const std::string& path,MyoThread& myo,DataFlags&  flags,ClassesNames& cnames);
    void  myo_classification_free(void*);
};


#endif /* FANNImp_hpp */
