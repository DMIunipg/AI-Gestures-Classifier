//
//  SVMImp.hpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 29/09/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef SVMImp_h
#define SVMImp_h


#include <string>
#include "MyoThread.h"
#include "DataFlags.h"

namespace myosvm
{
    void create_model(const std::string& path);
    void* myo_classification(const std::string& path,MyoThread& myo,DataFlags&  flags);
    void  myo_classification_free(void*);
};


#endif /* SVMImp_hpp */
