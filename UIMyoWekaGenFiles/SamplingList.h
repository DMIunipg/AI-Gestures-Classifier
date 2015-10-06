#ifndef SAMPLINGLIST_H
#define SAMPLINGLIST_H

#include <QDialog>
#include <functional>
#include <QLinkedList>
#include <MyoListener.h>
#include "SampleForm.h"

namespace Ui {
class SamplingList;
}

class SamplingList : public QDialog
{
    Q_OBJECT

public:
    //..
    explicit SamplingList(QWidget *parent = nullptr,
                          QLinkedList< MyoListener::TypeRows >* mSamples = nullptr);
    //..
    ~SamplingList();
    //set class name
    void setClassName(const QString& name);
    //callback on add sample
    void setOnAddSample(std::function< void(SampleForm&) >);
    //set current sample list
    void setCurrentSampleList(QLinkedList<MyoListener::TypeRows> *samples);
    //row add event
    void setOnRecording(std::function<void(SampleForm& sform)> onRecording);


public slots:

    void onAddSample();

private:

    Ui::SamplingList *ui;
    //callback on add a sampling
    std::function< void(SampleForm&) > mOnAddSample{ nullptr };
    //samples list
    QLinkedList< MyoListener::TypeRows >*     mSamples{ nullptr };
    //utility function
    SampleForm& addASempleWidget(QLinkedList< MyoListener::TypeRows >::iterator it);
    //default callback
    std::function<void(SampleForm&)> mOnRecording;
};

#endif // SAMPLINGLIST_H
