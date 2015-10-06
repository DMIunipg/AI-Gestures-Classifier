#include "SamplingList.h"
#include "ui_SamplingList.h"
#include "SampleForm.h"

SamplingList::SamplingList(QWidget *parent,
                           QLinkedList< MyoListener::TypeRows >* samples)
    :QDialog(parent)
    ,ui(new Ui::SamplingList)
{
    ui->setupUi(this);
    setCurrentSampleList(samples);
}

void SamplingList::setClassName(const QString& name)
{
    setWindowTitle(name);
}

void SamplingList::setOnAddSample(std::function<void(SampleForm &)> onAddSample)
{
    mOnAddSample=onAddSample;
}


void SamplingList::setCurrentSampleList(QLinkedList< MyoListener::TypeRows >* samples)
{
    assert(samples);
    //save pointer
    mSamples = samples;
    //delete all
    ui->mLWSamples->clear();
    //put all rows into list
    for(auto it = samples->begin();
             it != samples->end();
           ++it)
    {
        addASempleWidget(it);
    }
}

//row add event
void SamplingList::setOnRecording(std::function<void(SampleForm& sform)> onRecording)
{
    //recording
    mOnRecording=onRecording;
}

SampleForm& SamplingList::addASempleWidget(QLinkedList< MyoListener::TypeRows >::iterator it)
{
    assert(mSamples);
    //get rows
    MyoListener::TypeRows& rows = *it;
    //new item
    auto item=new QListWidgetItem();
    auto widget=new SampleForm(this,rows);
    //show time of sample
    widget->setTime(rows.size() ?  rows[rows.size()-1].getTime() : 0.0);
    //init size
    item->setSizeHint(widget->sizeHint());
    //add item
    ui->mLWSamples->addItem(item);
    ui->mLWSamples->setItemWidget(item,widget);
    //Set callbacks
    widget->setOnDelete(
    [this,it,item](SampleForm& sample)
    {
        //remove from ui
        delete item;
        //delete
        mSamples->erase(it);
    });
    widget->setOnRecording(
    [this](SampleForm& sample)
    {
        if(mOnRecording) mOnRecording(sample);
    });
    //
    return *widget;
}

void SamplingList::onAddSample()
{
    assert(mSamples);
    //put into rows list
    mSamples->append( MyoListener::TypeRows());
    //add widget
    auto& wsample=addASempleWidget(--mSamples->end());
    //callback
    if(mOnAddSample) mOnAddSample(wsample);
}


SamplingList::~SamplingList()
{
    delete ui;
}
