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
#include <memory>
#include <MyoThread.h>
#include <DataFlags.h>
#include <MyoClassifierInterface.h>


/*!
 * \brief Myo Model SVM
 */
class MyoModelSVM;

/*!
 * \brief Myo Classifier SVM, SVM implementation
 */
class MyoClassifierSVM : public MyoClassifierInterface
{
    
    double mProbabilityFilter {  0.0 };
    double mLastClass         { -1.0 };
    std::shared_ptr<MyoModelSVM> mModel;

public:
    
    /*!
     * \brief ~MyoClassifierSVM
     */
    virtual ~MyoClassifierSVM();
    
    /*!
     * \brief createModel
     * \param dataset
     * \return model
     */
    virtual MyoModelInterface* createModel(const DataSetReader& ds);
    
    /*!
     * \brief createModel
     * \param dataset
     * \param arguments
     * \return Myo Model built
     */
    virtual MyoModelInterface* createModel(const DataSetReader& ds,const std::string& args);
    
    /*!
     * \brief modelFromfile
     * \param path
     * \return model
     */
    virtual MyoModelInterface* modelFromfile(const std::string& path);
    
    /*!
     * \brief setModel
     * \param model
     */
    virtual void setModel(MyoModelInterface* model);
    
    /*!
     * \brief setProbabilityFilter, set the minimum probability required to call the callback
     * \param probability
     */
    virtual void setProbabilityFilter(double probability =0.0);
    
    
    /*!
     * \brief classification
     * \param myo thread
     * \param callback
     */
    virtual void classification(MyoThread& myo,const std::function< void (const std::string& className) >);
};


#endif /* SVMImp_hpp */
