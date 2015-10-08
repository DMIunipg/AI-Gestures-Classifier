#ifndef MYODIALOG_H
#define MYODIALOG_H

#include <QDialog>
#include <QVector>
#include <QBasicTimer>
#include "MyoManager.h"

namespace Ui {
class MyoDialog;
}

/*!
 * \brief The MyoDialog class
 */
class MyoDialog : public QDialog
{
    Q_OBJECT

public:

    /*!
     * \brief MyoDialog
     * \param parent
     * \param disable
     */
    explicit MyoDialog(QWidget *parent = 0, bool disable=true);

    /*!
     * \brief ~MyoDialog
     */
    ~MyoDialog();

    /*!
     * \brief setMyoManager
     * \param myoManager
     */
    void setMyoManager(MyoManager* myoManager);

    /*!
     * \brief showEulers
     * \param pitch
     * \param yaw
     * \param roll
     */
    void showEulers(double pitch,
                    double yaw,
                    double roll);

    /*!
     * \brief showEmg
     * \param emg
     */
    void showEmg(const std::array< int8_t, 8 >& emg);

    /*!
     * \brief setVideoToShow
     * \param rows
     */
    void setVideoToShow(const MyoListener::TypeRows& rows);

public slots:

    /*!
     * \brief onAbs
     */
    void onAbs();

    /*!
     * \brief onPlayPause
     * \param event
     */
    void onPlayPause(bool event);

    /*!
     * \brief onSlaiderChange
     * \param value
     */
    void onSlaiderChange(int value);

private:

    Ui::MyoDialog *ui { nullptr };               //! ui pointer
    QVector<double> mXPlots;                     //! vector of values of x axis
    std::array< QVector<double>, 8 > mYPlots;    //! vectors of values of all y axis
    //video info
    size_t                mCurrentRow{ 0 };      //! id of the current data
    QBasicTimer*          mVideoTime { nullptr };//! timer of the video
    MyoListener::TypeRows mVideoRows;            //! list of rows (will be showed)

    /*!
     * \brief timerEvent
     * \param e
     */
    void timerEvent(QTimerEvent *e);

    /*!
     * \brief absPlots
     */
    void absPlots();

    /*!
     * \brief notAbsPlots
     */
    void notAbsPlots();

    /*!
     * \brief doPlots
     */
    void doPlots();
};

#endif // MYODIALOG_H
