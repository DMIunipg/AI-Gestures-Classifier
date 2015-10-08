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

/*!
 * \brief The SamplingList class
 */
class SamplingList : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief SamplingList
     * \param parent
     * \param mSamples
     */
    explicit SamplingList(QWidget *parent = nullptr,
                          QLinkedList< MyoListener::TypeRows >* mSamples = nullptr);

    /*!
     * \brief ~SamplingList
     */
    ~SamplingList();

    /*!
     * \brief setClassName
     * \param name
     */
    void setClassName(const QString& name);

    /*!
     * \brief setOnAddSample
     */
    void setOnAddSample(std::function< void(SampleForm&) >);

    /*!
     * \brief setCurrentSampleList
     * \param samples
     */
    void setCurrentSampleList(QLinkedList<MyoListener::TypeRows> *samples);

    /*!
     * \brief setOnRecording
     * \param onRecording
     */
    void setOnRecording(std::function<void(SampleForm& sform)> onRecording);


public slots:

    /*!
     * \brief onAddSample
     */
    void onAddSample();

private:

    Ui::SamplingList *ui; //! ui pointer
    std::function< void(SampleForm&) > mOnAddSample{ nullptr }; //! callback called when a sample is added
    std::function<void(SampleForm&)> mOnRecording{ nullptr };  //! callback called when start samples recording
    QLinkedList< MyoListener::TypeRows >* mSamples{ nullptr }; //! list of samples

    /*!
     * \brief addASempleWidget
     * \param it
     * \return return ui item of a sample
     */
    SampleForm& addASempleWidget(QLinkedList< MyoListener::TypeRows >::iterator it);

};

#endif // SAMPLINGLIST_H
