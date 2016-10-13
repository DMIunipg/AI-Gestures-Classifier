#ifndef MODELFORM_H
#define MODELFORM_H

#include <QStringList>
#include <QDialog>

namespace Ui {
class ModelForm;
}

class ModelForm : public QDialog
{
    Q_OBJECT

public:

    /*!
     * \brief The ModelType enum
     */
    enum class ModelType
    {
        M_kNN,
        M_SVM,
        M_RBF_Network
    };

    /*!
     * \brief The kNNParams struct
     */
    struct kNNParams
    {
        QString mType;       //! type regression
        QString mWeight;     //! weight
        bool    mFFN;        //! Use FFN for EMGs values
        int     mRtoc;       //! k Near to classify
    };

    /*!
     * \brief The SVMParams struct
     */
    struct SVMParams
    {
        QString mType;       //! type regression
        QString mKernel;     //! kernel
        double  mCacheSize;  //! cache size
        double  mCoef0;      //! for poly/sigmoid
        int     mDegree;     //! poly degree
        double  mEps;        //! stopping criteria
        double  mGamma;      //! for poly/rbf/sigmoid
        double  mNu;         //! for NU_SVC, ONE_CLASS, and NU_SVR
        double  mP;          //! for EPSILON_SVR
        double  mConst;      //! for C_SVC
        bool    mFFN;        //! Use FFN for EMGs values
        bool    mProbability;//! do probability estimates
        bool    mShrinking;  //! use the shrinking heuristics
    };

    /*!
     * \brief The RBFNetworkParams struct
     */
    struct RBFNetworkParams
    {
        int     mNumIterations; //! number of rbf iterations
        double  mLearningRate;  //! learning rate
        bool mCalcAccuracyMse;  //! compute mse
        bool mPrint;            //! print model build info
    };



    explicit ModelForm(QWidget *parent = 0);

    ~ModelForm();

    /*!
     * \brief apply
     * \param method type
     */
    void apply(ModelType type);

    /*!
     * \brief apply
     * \param knn params
     */
    void apply(const kNNParams& params);


    /*!
     * \brief apply
     * \param svm params
     */
    void apply(const SVMParams& params);

    /*!
     * \brief apply
     * \param rbf network params
     */
    void apply(const RBFNetworkParams& params);

    /*!
     * \brief getType
     * \return method type
     */
    ModelType getType() const;

    /*!
     * \brief getkNNParams
     * \return knn params
     */
    kNNParams getkNNParams() const;

    /*!
     * \brief getSVMParams
     * \return svm params
     */
    SVMParams getSVMParams() const;

    /*!
     * \brief getRBFParams
     * \return rbf network params
     */
    RBFNetworkParams getRBFParams() const;

    /*!
     * \brief toStrListParams
     * \param type
     * \return params list
     */
    static QStringList toStrListParams(ModelType& type);

    /*!
     * \brief toStrListParams
     * \param knn params
     * \return knn params list
     */
    static QStringList toStrListParams(const kNNParams& params);

    /*!
     * \brief toStrListParams
     * \param svm params
     * \return svm params list
     */
    static QStringList toStrListParams(const SVMParams& params);

    /*!
     * \brief toStrListParams
     * \param rbf network params
     * \return rbf network params list
     */
    static QStringList toStrListParams(const RBFNetworkParams& params);

    /*!
     * \brief execute
     * \param path
     * \return true if successful to execute
     */
    bool execute(const QString& path);

public slots:

    /*!
     * \brief onapply
     * \param event
     */
    void onApply(bool event);

    /*!
     * \brief onTestModel
     * \param event
     */
    void onTestModel(bool event);

    /*!
     * \brief onSearchDir
     * \param event
     */
    void onSearchDir(bool event);

    /*!
     * \brief onKNN
     * \param event
     */
    void onKNN(bool event);

    /*!
     * \brief onSVM
     * \param event
     */
    void onSVM(bool event);

    /*!
     * \brief onRBFNetwork
     * \param event
     */
    void onRBFNetwork(bool event);

private:

    Ui::ModelForm *ui           { nullptr };    //! ui ptr
    QString        mModelPath   { ""      };    //! path of model to save
    QString        mDatasetPath { ""      };    //! path of dataset to load

};

#endif // MODELFORM_H
