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

    connect(ui->mCBNormalize,
            SIGNAL(clicked(bool)),
            this,
            SLOT(onNormalize(bool)));
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
        case DataFlags::SAMPLE_MODE:
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
    ui->mCBPitch->setChecked(flags.mPitch);
    ui->mCBYaw->setChecked(flags.mYaw);
    ui->mCBRoll->setChecked(flags.mRoll);
    ui->mCBEmg->setChecked(flags.mEmg);
    ui->mCBNormalize->setChecked(flags.mNormalize);
    ui->mCBAllPositive->setChecked(flags.mPositive);
    ui->mCBEmgAbs->setChecked(flags.mEmgAbs);
    ui->mDSBEmgSmooth->setValue(flags.mEmgSmooth);
}

DataFlags FlagsDialog::getFlags()
{
     DataFlags out;
     out.mMode           = ui->mRBSempleMode->isChecked() ?
                           DataFlags::SAMPLE_MODE :
                           DataFlags::GESTURE_MODE;
     out.mReps           = ui->mSBReps->value();
     out.mTimePerGesture = ui->mDSBTimePerGesture->value();
     out.mDeltaTime      = ui->mDSBDeltaTimeOfASemple->value();
     out.mTime           = ui->mCBTime->isChecked();
     out.mGyroscope      = ui->mCBGyroscope->isChecked();
     out.mAccelerometer  = ui->mCBAccelerometer->isChecked();
     out.mQuaternion     = ui->mCBQuaternion->isChecked();
     out.mPitch          = ui->mCBPitch->isChecked();
     out.mYaw            = ui->mCBYaw->isChecked();
     out.mRoll           = ui->mCBRoll->isChecked();
     out.mEmg            = ui->mCBEmg->isChecked();
     out.mNormalize      = ui->mCBNormalize->isChecked();
     out.mPositive       = ui->mCBAllPositive->isChecked() && out.mNormalize ;
     out.mEmgAbs         = ui->mCBEmgAbs->isChecked() && out.mEmg;
     out.mEmgSmooth      = ui->mDSBEmgSmooth->value() * (double)out.mEmg;
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

void FlagsDialog::onNormalize(bool enable)
{
    ui->mCBAllPositive->setEnabled(enable);
}

void FlagsDialog::onEmg(bool enable)
{
    ui->mGBEmgFilters->setEnabled(enable);
}
