#ifndef RECORDINGDIALOG_H
#define RECORDINGDIALOG_H
#pragma once

#include <QDialog>
#include <functional>

namespace Ui {
class RecordingDialog;
}

/*!
 * \brief The RecordingDialog class
 */
class RecordingDialog : public QDialog
{
    Q_OBJECT

public:

    /*!
     * \brief RecordingDialog
     * \param parent
     */
    explicit RecordingDialog(QWidget *parent = 0);

    /*!
     * \brief ~RecordingDialog
     */
    ~RecordingDialog();

    /*!
     * \brief execute
     * \param onEnd
     */
    void execute(std::function<void(void)> onEnd);

public slots:

    /*!
     * \brief onEnd
     */
    void onEnd();

    /*!
     * \brief updateTimeOfRec
     */
    void updateTimeOfRec();

private:

    Ui::RecordingDialog *ui;                        //! ui pointer
    double  mTime{ 0.0 };                           //! recoding time
    QTimer* mTimer{ nullptr };                      //! scheduler of update
    std::function<void(void)> mOnEnd { nullptr };   //! callback called when dialog will be closed
};

#endif // RECORDINGDIALOG_H
