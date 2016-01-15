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

    enum class ModelType
    {
        M_kNN,
        M_SVM,
        M_RBF_Network
    };

    struct SVMParams
    {
        QString mType;
        QString mKernel;
        double  mCacheSize;
        double  mCoef0;
        int     mDegree;
        double  mEps;
        double  mGamma;
        double  mNu;
        double  mP;
        bool    mProbability;
        bool    mShrinking;
    };

    struct RBFNetworkParams
    {
        int     mNumIterations;
        double  mLearningRate;
        bool mCalcAccuracyMse;
        bool mPrint;
    };

    explicit ModelForm(QWidget *parent = 0);

    ~ModelForm();

    //applay to form
    void applay(ModelType type);
    void applay(const SVMParams& params);
    void applay(const RBFNetworkParams& params);
    //get from form
    ModelType getType() const;
    SVMParams getSVMParams() const;
    RBFNetworkParams getRBFParams() const;
    //create cmd params
    static QStringList toStrListParams(ModelType& type);
    static QStringList toStrListParams(const SVMParams& params);
    static QStringList toStrListParams(const RBFNetworkParams& params);
    //execute
    bool execute(const QString& path);

public slots:

    void onApplay(bool event);
    void onSearchDir(bool event);

private:

    Ui::ModelForm *ui           { nullptr };
    QString        mModelPath   { ""      };
    QString        mDatasetPath { ""      };
};

#endif // MODELFORM_H
