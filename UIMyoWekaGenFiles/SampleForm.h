#ifndef SAMPLEFORM_H
#define SAMPLEFORM_H
#pragma once

#include <QWidget>
#include <functional>
#include <MyoListener.h>

namespace Ui {
class SampleForm;
}

class SampleForm : public QWidget
{
    Q_OBJECT

public:

    explicit SampleForm(QWidget *parent,MyoListener::TypeRows& rows);

    ~SampleForm();

    void setOnDelete(std::function<void(SampleForm& sform)> onDelete);

    void setOnRecording(std::function<void(SampleForm& sform)> onRecording);

    double getTime() const;

    void setTime(double time);

    void updateTime();

    MyoListener::TypeRows& getRows();

    const MyoListener::TypeRows& getRows() const;

public slots:

    void onDelete();
    void onRecording();
    void onShowVideo();

private:
    Ui::SampleForm* ui;
    MyoListener::TypeRows&    mRefRows;
    std::function<void(SampleForm& sform)> mOnDelete    { nullptr };
    std::function<void(SampleForm& sform)> mOnRecording { nullptr };
};

#endif // SAMPLEFORM_H
