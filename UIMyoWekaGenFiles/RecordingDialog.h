#ifndef RECORDINGDIALOG_H
#define RECORDINGDIALOG_H
#pragma once

#include <QDialog>
#include <functional>

namespace Ui {
class RecordingDialog;
}

class RecordingDialog : public QDialog
{
    Q_OBJECT

public:

    explicit RecordingDialog(QWidget *parent = 0);

    ~RecordingDialog();

    void execute(std::function<void(void)> onEnd);

public slots:

    void onEnd();
    void updateTimeOfRec();

private:

    Ui::RecordingDialog *ui;
    double  mTime{ 0.0 };
    QTimer* mTimer{ nullptr };
    std::function<void(void)> mOnEnd { nullptr };
};

#endif // RECORDINGDIALOG_H
