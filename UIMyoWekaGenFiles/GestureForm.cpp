#include "GestureForm.h"
#include "ui_GestureForm.h"

GestureForm::GestureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GestureForm)
{
    ui->setupUi(this);
}

GestureForm::~GestureForm()
{
    delete ui;
}

void GestureForm::setOnDelete(std::function<void(void)> onDelete)
{
    mOnDelete=onDelete;
}

void GestureForm::setOnRecording(std::function<void(void)> onRecording)
{
    mOnRecording=onRecording;
}

QString GestureForm::getName() const
{
    return ui->mLEName->text();
}

void GestureForm::setName(const QString& str)
{
    ui->mLEName->setText(str);
}

void GestureForm::onDelete()
{
    if(mOnDelete) mOnDelete();
}

void GestureForm::onRecording()
{
    if(mOnRecording) mOnRecording();
}
