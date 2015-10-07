#ifndef MYODIALOG_H
#define MYODIALOG_H

#include <QDialog>
#include <QVector>
#include <QBasicTimer>
#include "MyoManager.h"

namespace Ui {
class MyoDialog;
}

class MyoDialog : public QDialog
{
    Q_OBJECT

public:
    //myo dialog con.
    explicit MyoDialog(QWidget *parent = 0, bool disable=true);
    ~MyoDialog();
    //myo manager
    void setMyoManager(MyoManager* myoManager);
    //show pitch, yaw, roll
    void showEulers(double pitch,
                    double yaw,
                    double roll);
    //show emg
    void showEmg(const std::array< int8_t, 8 >& emg);
    //put video
    void setVideoToShow(const MyoListener::TypeRows& rows);

public slots:

    //module
    void onAbs();
    //play/pause event
    void onPlayPause(bool event);
    //change slaider
    void onSlaiderChange(int value);

private:
    //ui dialog
    Ui::MyoDialog *ui { nullptr };
    //graph temp values
    QVector<double> mXPlots;
    std::array< QVector<double>, 8 > mYPlots;
    //video info
    size_t                mCurrentRow{ 0 };
    QBasicTimer*          mVideoTime { nullptr };
    MyoListener::TypeRows mVideoRows;
    //video update
    void timerEvent(QTimerEvent *e);
    //utilities
    void absPlots();
    void notAbsPlots();
    void doPlots();
};

#endif // MYODIALOG_H
