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

    void execute(std::function<void(void)> onEnd)
    {
        mOnEnd = onEnd;
        QDialog::exec();
    }

public slots:

    void onEnd();

private:

    Ui::RecordingDialog *ui;
    std::function<void(void)> mOnEnd { nullptr };
};

#endif // RECORDINGDIALOG_H
