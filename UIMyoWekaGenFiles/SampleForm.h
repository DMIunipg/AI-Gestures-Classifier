#ifndef SAMPLEFORM_H
#define SAMPLEFORM_H
#pragma once

#include <QWidget>
#include <functional>
#include <MyoListener.h>

namespace Ui {
class SampleForm;
}

/*!
 * \brief The SampleForm class
 */
class SampleForm : public QWidget
{
    Q_OBJECT

public:

    /*!
     * \brief SampleForm
     * \param parent
     * \param rows
     */
    explicit SampleForm(QWidget *parent,MyoListener::TypeRows& rows);

    /*!
     * \brief ~SampleForm
     */
    ~SampleForm();

    /*!
     * \brief setOnDelete
     * \param onDelete
     */
    void setOnDelete(std::function<void(SampleForm& sform)> onDelete);

    /*!
     * \brief setOnRecording
     * \param onRecording
     */
    void setOnRecording(std::function<void(SampleForm& sform)> onRecording);

    /*!
     * \brief getTime
     * \return time of registration
     */
    double getTime() const;

    /*!
     * \brief setTime
     * \param time
     */
    void setTime(double time);

    /*!
     * \brief updateTime
     */
    void updateTime();

    /*!
     * \brief getRows
     * \return samples
     */
    MyoListener::TypeRows& getRows();

    /*!
     * \brief getRows
     * \return samples
     */
    const MyoListener::TypeRows& getRows() const;

public slots:

    /*!
     * \brief onDelete
     */
    void onDelete();

    /*!
     * \brief onRecording
     */
    void onRecording();

    /*!
     * \brief onShowVideo
     */
    void onShowVideo();

private:
    Ui::SampleForm* ui;                                              //! ui pointer
    MyoListener::TypeRows&    mRefRows;                              //! reference to list of samples
    std::function<void(SampleForm& sform)> mOnDelete    { nullptr }; //! callback called on delete a samples row
    std::function<void(SampleForm& sform)> mOnRecording { nullptr }; //! callback called when start the samples recording
};

#endif // SAMPLEFORM_H
