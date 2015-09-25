#include "FlagsDialog.h"
#include "ui_FlagsDialog.h"

FlagsDialog::FlagsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlagsDialog)
{
    ui->setupUi(this);
}

FlagsDialog::~FlagsDialog()
{
    delete ui;
}



void FlagsDialog::setFalgs(const  DataFlags& flags)
{
    ui->mSBReps->setValue(flags.mReps);
    ui->mCBTime->setChecked(flags.mTime);
    ui->mCBGyroscope->setChecked(flags.mGyroscope);
    ui->mCBAccelerometer->setChecked(flags.mAccelerometer);
    ui->mCBQuaternion->setChecked(flags.mQuaternion);
    ui->mCBEuler->setChecked(flags.mEuler);
    ui->mCBEmg->setChecked(flags.mEmg);
}

DataFlags FlagsDialog::getFlags()
{
     DataFlags out;
     out.mReps          = ui->mSBReps->value();
     out.mTime          = ui->mCBTime->isChecked();
     out.mGyroscope     = ui->mCBGyroscope->isChecked();
     out.mAccelerometer = ui->mCBAccelerometer->isChecked();
     out.mQuaternion    = ui->mCBQuaternion->isChecked();
     out.mEuler         = ui->mCBEuler->isChecked();
     out.mEmg           = ui->mCBEmg->isChecked();
     return out;
}

bool FlagsDialog::exec(DataFlags &flags)
{
    //init
    setFalgs(flags);
    mReturnStatus=false;
    //exec
    QDialog::exec();
    //if success
    if(mReturnStatus)
        flags=getFlags();
    //return
    return mReturnStatus;
}

void FlagsDialog::onOk()
{
    mReturnStatus=true;
    close();
}

void FlagsDialog::onCancel()
{
    mReturnStatus=false;
    close();
}
