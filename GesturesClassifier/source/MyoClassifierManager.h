//
//  MyoClassifierManager.hpp
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 10/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef MyoClassifierManager_h
#define MyoClassifierManager_h

#include <string>
#include <functional>
#include <MyoModelInterface.h>

/*!
 * \brief Classifier enum, type of classification method
 */
enum Classifier
{
    CLA_SVM,
    CLA_kNN,
    CLA_RBFNETWORK
};

class MyoThread;
class MyoClassifierInterface;

/*!
 * \brief Myo Classifier Manager, class to manage the classification
 */
class MyoClassifierManager
{
    
    Classifier mClassifier;             //! current type classification method
    MyoClassifierInterface* mInterface; //! pointer to instance of classification method
    MyoThread* mMyoThread;              //! pointer to myo thread
    
public:
    
    /*!
     * \brief MyoClassifierManager
     * \param type, type of classification method
     */
    MyoClassifierManager(Classifier type);
    
    /*!
     * \brief ~MyoClassifierManager
     */
    ~MyoClassifierManager();
    
    /*!
     * \brief buildModel
     * \param dataset
     * \return Myo Model built
     */
    MyoModelInterface* buildModel(const std::string& dataset);
    
    /*!
     * \brief loadModel
     * \param model
     * \return Myo Model loaded
     */
    MyoModelInterface* loadModel(const std::string& model);
    
    /*!
     * \brief setProbabilityFilter, set the minimum probability required to call the callback
     * \param probability
     */
    void setProbabilityFilter(double probability =0.0);
    
    /*! 
     * \brief classification, start the classification (N.B. work in separate thread)
     * \param callback
     */
    void classification(const std::function< void (const std::string& className) >);

};

#endif /* MyoClassifierManager.h */
