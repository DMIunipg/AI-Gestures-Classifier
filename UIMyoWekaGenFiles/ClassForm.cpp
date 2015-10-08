#include "ClassForm.h"
#include "ui_ClassForm.h"

ClassForm::ClassForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassForm)
{
    ui->setupUi(this);
}


void ClassForm::setOnDelete(std::function<void(void)> onDelete)
{
    mOnDelete = onDelete;
}

void ClassForm::setOnSampling(std::function<void(void)> onSampling)
{
    mOnSampling = onSampling;
}


void ClassForm::onDelete()
{
    if(mOnDelete) mOnDelete();
}

void ClassForm::onSampling()
{
    if(mOnSampling) mOnSampling();
}

QString ClassForm::getName() const
{
    return ui->mLEName->text();
}

void ClassForm::setName(const QString& cname)
{
    ui->mLEName->setText(cname);
}


ClassForm::~ClassForm()
{
    delete ui;
}
