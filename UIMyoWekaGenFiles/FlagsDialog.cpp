#include "FlagsDialog.h"
#include "ui_FlagsDialog.h"

FlagsDialog::FlagsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlagsDialog)
{
    //ui setup
    ui->setupUi(this);
    //connections
    connect(ui->mRBSempleMode,
            SIGNAL(clicked(bool)),
            this,
            SLOT(onSempleMode(bool)));

    connect(ui->mRBGestureMode,
            SIGNAL(clicked(bool)),
            this,
            SLOT(onGestureMode(bool)));
}

FlagsDialog::~FlagsDialog()
{
    delete ui;
}



void FlagsDialog::setFalgs(const  DataFlags& flags)
{
    //value
    switch (flags.mMode)
    {
        case DataFlags::SEMPLE_MODE:
            onSempleMode(true);
        break;
        case DataFlags::GESTURE_MODE:
            onGestureMode(true);
        break;
        default: break;
    };
    ui->mSBReps->setValue(flags.mReps);
    ui->mDSBTimePerGesture->setValue(flags.mTimePerGesture);
    ui->mDSBDeltaTimeOfASemple->setValue(flags.mDeltaTime);
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
     out.mMode           = ui->mRBSempleMode->isChecked() ?
                           DataFlags::SEMPLE_MODE :
                           DataFlags::GESTURE_MODE;
     out.mReps           = ui->mSBReps->value();
     out.mTimePerGesture = ui->mDSBTimePerGesture->value();
     out.mDeltaTime      = ui->mDSBDeltaTimeOfASemple->value();
     out.mTime           = ui->mCBTime->isChecked();
     out.mGyroscope      = ui->mCBGyroscope->isChecked();
     out.mAccelerometer  = ui->mCBAccelerometer->isChecked();
     out.mQuaternion     = ui->mCBQuaternion->isChecked();
     out.mEuler          = ui->mCBEuler->isChecked();
     out.mEmg            = ui->mCBEmg->isChecked();
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

void FlagsDialog::onSempleMode(bool enabled)
{
    ui->mRBSempleMode->setChecked(enabled);
    ui->mGBSemplePerRow->setEnabled(enabled);
    ui->mRBGestureMode->setChecked(!enabled);
    ui->mGBGesturePerRow->setDisabled(enabled);
}

void FlagsDialog::onGestureMode(bool enabled)
{
    ui->mRBSempleMode->setChecked(!enabled);
    ui->mGBSemplePerRow->setDisabled(enabled);
    ui->mRBGestureMode->setChecked(enabled);
    ui->mGBGesturePerRow->setEnabled(enabled);
}
