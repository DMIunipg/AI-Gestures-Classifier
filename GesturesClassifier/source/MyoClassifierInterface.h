//
//  MyoClassifierInterface.h
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 10/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef MyoClassifierInterface_h
#define MyoClassifierInterface_h

#include <string>
#include <cstdio>
#include <functional>
#include <MyoThread.h>
#include <DataSetReader.h>
#include <MyoModelInterface.h>


/*!
 * \brief Myo Classifier Interface, interface used to implement a type of classification
 */
class MyoClassifierInterface
{
    
public:
    
    /*!
     * \brief Myo Classifier Interface
     */
    virtual ~MyoClassifierInterface() {};
    
    /*!
     * \brief createModel
     * \param ds
     * \return Myo Model built
     */
    virtual MyoModelInterface* createModel(const DataSetReader& ds) = 0;
    
    /*!
     * \brief modelFromfile
     * \param path
     * \return loaded Myo Model
     */
    virtual MyoModelInterface* modelFromfile(const std::string& path) = 0;
    
    /*!
     * \brief setModel
     * \param model
     */
    virtual void setModel(MyoModelInterface* model) = 0;
    
    /*!
     * \brief setProbabilityFilter, set the minimum probability required to call the callback
     * \param probability
     */
    virtual void setProbabilityFilter(double probability =0.0) = 0;
    
    /*!
     * \brief classification
     * \param myo thread
     * \param callback
     */
    virtual void classification(MyoThread& myo,const std::function< void (const std::string& className) > callback) = 0;

};

#endif /* MyoClassifierInterface_h */
