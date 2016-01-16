#include "ModelForm.h"
#include "ui_ModelForm.h"
#include <QProcess>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
/*
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
*/
static ModelForm::SVMParams defaultSVMParams =
{
    "NU_SVC",   //type
    "RBF",      //kernel
    100.0f,     //cache
    0.1f,       //coef0
    3,          //degree
    0.1f,       //eps
    0.2f,       //gamma
    0.3f,       //nu
    0.0f,       //p
    true,       //probability
    true        //shrinking
};

/*
    int     mNumIterations;
    double  mLearningRate;
    bool    mCalcAccuracyMse;
    bool    mPrint;
*/
static ModelForm::RBFNetworkParams  defaultRBFNetworkParams =
{
    50000,      //iterations
    0.5,        //learning rate
    true,       //calc accuracy mse
    false       //print debug info
};

ModelForm::ModelForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelForm)
{
    ui->setupUi(this);
    //default is kNN
    applay(ModelType::M_kNN);
    //applay default svm
    applay(defaultSVMParams);
    //applay default rbf network params
    applay(defaultRBFNetworkParams);
}

ModelForm::~ModelForm()
{
    delete ui;
}
//set directory
void ModelForm::onSearchDir(bool)
{
    //path
    mModelPath=QFileDialog::getSaveFileName(this,
                                            "Save",
                                            QDir(mDatasetPath).absolutePath(),
                                            "Model File");
    ui->mLEDir->setText(mModelPath);

}
//applay event
void ModelForm::onApplay(bool event)
{
    //if have a valid paths
    if(!mModelPath.size()) return;
    //this app dir
    QString  thizdir = QCoreApplication::applicationDirPath();
#if __APPLE__
    QString  dir = thizdir+"/../Resources";
#else
    const QString&  dir = thizdir;
#endif
    QString  name   = "GesturesClassifierApplication";
    QString  apppath= dir + "/" + name;
    //debug
    qDebug() << apppath;
    //params
    QStringList  params;
    //set ouput
    params << "-i" << mDatasetPath;
    params << "-o"<< mModelPath;
    //type
    ModelType type = getType();
    //type to args
    params << toStrListParams(type);
    //cases
    switch(type)
    {
        case ModelType::M_SVM:         params << toStrListParams(getSVMParams()); break;
        case ModelType::M_RBF_Network: params << toStrListParams(getRBFParams()); break;
        default: break;
    };
    //process
    QProcess gcapp;
    //execute
    gcapp.start(apppath,params);
    gcapp.waitForFinished();
    //print ouput
    ui->mTBConsole->setText(
                + "Console command:\n"
                + name
                + " "
                + params.join(' ')
                + "\n\n"
                + "Output:\n"
                + gcapp.readAllStandardOutput()
    );
}

void ModelForm::onKNN(bool event)
{
    if(event)
    {
        ui->mGBSVM->setEnabled(false);
        ui->mGBRBFNetwork->setEnabled(false);
    }
}

void ModelForm::onSVM(bool event)
{
    if(event)
    {
        ui->mGBSVM->setEnabled(true);
        ui->mGBRBFNetwork->setEnabled(false);
    }
}

void ModelForm::onRBFNetwork(bool event)
{
    if(event)
    {
        ui->mGBSVM->setEnabled(false);
        ui->mGBRBFNetwork->setEnabled(true);
    }
}

//execute
bool ModelForm::execute(const QString& path)
{
    //dataset path
    mDatasetPath = path;
    //execute
    return exec() == 0;
}

//applay to form
void ModelForm::applay(ModelForm::ModelType type)
{
    switch (type)
    {
        default:
        case ModelType::M_kNN:
            ui->mRBkNN->setChecked(true);
        break;
        case ModelType::M_SVM:
            ui->mRBSVM->setChecked(true);
        break;
        case ModelType::M_RBF_Network:
            ui->mRBRBFNetwork->setChecked(true);
        break;
    }
}

void ModelForm::applay(const ModelForm::SVMParams& params)
{
    ui->mCBType->setCurrentText(params.mType);
    ui->mCBKernel->setCurrentText(params.mKernel);
    ui->mDSBCacheSize->setValue(params.mCacheSize);
    ui->mDSBCoef0->setValue(params.mCoef0);
    ui->mSBDegree->setValue(params.mDegree);
    ui->mDSBEps->setValue(params.mEps);
    ui->mDSBGamma->setValue(params.mGamma);
    ui->mDSBNu->setValue(params.mNu);
    ui->mDSBP->setValue(params.mP);
    ui->mCBProbability->setCurrentText(params.mProbability ? "TRUE" : "FALSE");
    ui->mCBShrinking->setCurrentText(params.mShrinking ? "TRUE" : "FALSE");
}

void ModelForm::applay(const ModelForm::RBFNetworkParams& params)
{
    ui->mSBNumIterations->setValue(params.mNumIterations);
    ui->mDSBLearningRate->setValue(params.mLearningRate);
    ui->mCBAccuracyMse->setCurrentText(params.mCalcAccuracyMse ? "TRUE" : "FALSE");
    ui->mCBPrint->setCurrentText(params.mPrint ? "TRUE" : "FALSE");
}

//get from form
ModelForm::ModelType ModelForm::getType() const
{
    if(ui->mRBkNN->isChecked())        return ModelType::M_kNN;
    if(ui->mRBSVM->isChecked())        return ModelType::M_SVM;
    if(ui->mRBRBFNetwork->isChecked()) return ModelType::M_RBF_Network;
    //default
    return ModelType::M_kNN;
}
ModelForm::SVMParams ModelForm::getSVMParams() const
{
    ModelForm::SVMParams output;
    output.mType = ui->mCBType->currentText();
    output.mKernel = ui->mCBKernel->currentText();
    output.mCacheSize = ui->mDSBCacheSize->value();
    output.mCoef0 = ui->mDSBCoef0->value();
    output.mDegree = ui->mSBDegree->value();
    output.mEps = ui->mDSBEps->value();
    output.mGamma = ui->mDSBGamma->value();
    output.mNu = ui->mDSBNu->value();
    output.mP = ui->mDSBP->value();
    output.mProbability = (ui->mCBProbability->currentText() == "FALSE" ? false : true);
    output.mShrinking = (ui->mCBShrinking->currentText() == "FALSE" ? false : true);

    return output;
}
ModelForm::RBFNetworkParams ModelForm::getRBFParams() const
{
    ModelForm::RBFNetworkParams output;

    output.mNumIterations = ui->mSBNumIterations->value();
    output.mLearningRate = ui->mDSBLearningRate->value();
    output.mCalcAccuracyMse = (output.mCalcAccuracyMse = ui->mCBAccuracyMse->currentText() == "FALSE" ? false : true);
    output.mPrint = (output.mPrint = ui->mCBPrint->currentText() == "FALSE" ? false : true);

    return output;
}
//create cmd params
QStringList ModelForm::toStrListParams(ModelForm::ModelType& type)
{
    QStringList list;

    switch(type)
    {
        default:
        case ModelType::M_kNN:         list << "-c" << "kNN";        break;
        case ModelType::M_SVM:         list << "-c" << "SVM";        break;
        case ModelType::M_RBF_Network: list << "-c" << "RBFNetwork"; break;
    };

    return list;
}
QStringList ModelForm::toStrListParams(const ModelForm::SVMParams& params)
{
    QStringList list;
    list << "type" << params.mType;
    list << "kernel" << params.mKernel;
    list << "cache" << QString::number(params.mCacheSize);
    list << "coef0" << QString::number(params.mCoef0);
    list << "degree" << QString::number(params.mDegree);
    list << "eps" << QString::number(params.mEps);
    list << "gamma" << QString::number(params.mGamma);
    list << "nu" << QString::number(params.mNu);
    list << "p" << QString::number(params.mP);
    list << "probability" << QString(params.mProbability ? "true" : "false");
    list << "shrinking" << QString(params.mShrinking ? "true" : "false");

    return list;
}
QStringList ModelForm::toStrListParams(const ModelForm::RBFNetworkParams& params)
{
    QStringList list;

    list << "learning rate" << QString::number(params.mLearningRate);
    list << "iterations" << QString::number(params.mNumIterations);
    list << "accuracy mse" << QString(params.mCalcAccuracyMse ? "true" : "false");
    list << "print" << QString(params.mPrint ? "true" : "false");

    return list;
}
