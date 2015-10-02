#include <QTimer>
#include "RecordingDialog.h"
#include "ui_RecordingDialog.h"
#include "MyoTime.h"

RecordingDialog::RecordingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordingDialog)
{
    ui->setupUi(this);
}

RecordingDialog::~RecordingDialog()
{
    delete ui;
}

void RecordingDialog::onEnd()
{
    if(mOnEnd) mOnEnd();
    if(mTimer){ delete mTimer; mTimer=nullptr; }
    close();
}

void RecordingDialog::updateTimeOfRec()
{
    ui->mLBRTime->setText(QString::number((myo::GetTime()-mTime)/1000,'f',3));
}

void RecordingDialog::execute(std::function<void(void)> onEnd)
{
    mTime  = myo::GetTime();
    mOnEnd = onEnd;
    //delete old time
    if(mTime) delete mTimer;
    //schedul time
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateTimeOfRec()));
    mTimer->start(16);
    //exec dialog
    QDialog::exec();
}

