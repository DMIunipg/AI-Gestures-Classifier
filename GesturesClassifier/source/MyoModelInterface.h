//
//  MyoModelInterface.h
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 11/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef MyoModelInterface_h
#define MyoModelInterface_h

#include <string>

class MyoModelInterface
{
public:
    
    virtual ~MyoModelInterface() {};
    virtual void serialize(const std::string& path) = 0;
    virtual void deserialize(const std::string& path) = 0;
};

#endif /* MyoModelInterface_h */
