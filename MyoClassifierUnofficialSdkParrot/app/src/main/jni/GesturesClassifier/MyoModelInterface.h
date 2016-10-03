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

/*!
 * \brief Myo Model Interface, interface used to represent a model of classification method
 */
class MyoModelInterface
{
public:
    
    /*!
     * \brief ~MyoModelInterface
     */
    virtual ~MyoModelInterface() {};
    
    /*!
     * \brief serialize
     * \param path
     */
    virtual void serialize(const std::string& path) = 0;
    
    /*!
     * \brief deserialize
     * \param path
     */
    virtual void deserialize(const std::string& path) = 0;
};

#endif /* MyoModelInterface_h */
