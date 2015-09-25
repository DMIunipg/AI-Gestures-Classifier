#include "RecordingDialog.h"
#include "ui_RecordingDialog.h"

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
    close();
}
