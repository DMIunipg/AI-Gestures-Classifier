#include "MyoDialog.h"
#include "SampleForm.h"
#include "ui_SampleForm.h"

SampleForm::SampleForm(QWidget *parent, MyoListener::TypeRows &rows)
  : QWidget(parent)
  , ui(new Ui::SampleForm)
  , mRefRows(rows)
{
    ui->setupUi(this);
}

SampleForm::~SampleForm()
{
    delete ui;
}

void SampleForm::setOnDelete(std::function<void(SampleForm& sform)> onDelete)
{
    mOnDelete=onDelete;
}

void SampleForm::setOnRecording(std::function<void(SampleForm& sform)> onRecording)
{
    mOnRecording=onRecording;
}

void SampleForm::onShowVideo()
{
    MyoDialog mdialog(this,false);
    mdialog.setVideoToShow(getRows());
    mdialog.exec();
}

double SampleForm::getTime() const
{
    return ui->mLTime->text().toDouble();
}

void SampleForm::setTime(double time)
{
    ui->mLTime->setText(QString::number(time,'f',3));
}

void SampleForm::updateTime()
{
    if(mRefRows.size())
        setTime(mRefRows[mRefRows.size()-1].getTime());
    else
        setTime(0);
}

MyoListener::TypeRows& SampleForm::getRows()
{
    return mRefRows;
}

const MyoListener::TypeRows& SampleForm::getRows() const
{
    return mRefRows;
}


void SampleForm::onDelete()
{
    if(mOnDelete) mOnDelete(*this);
}

void SampleForm::onRecording()
{
    if(mOnRecording) mOnRecording(*this);
}
