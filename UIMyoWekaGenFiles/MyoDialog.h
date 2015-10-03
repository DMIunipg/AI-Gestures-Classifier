#ifndef MYODIALOG_H
#define MYODIALOG_H

#include <QDialog>
#include <QVector>
#include "MyoManager.h"

namespace Ui {
class MyoDialog;
}

class MyoDialog : public QDialog
{
    Q_OBJECT

public:
    //myo dialog con.
    explicit MyoDialog(QWidget *parent = 0);
    ~MyoDialog();
    //myo manager
    void setMyoManager(MyoManager* myoManager);
    //show pitch, yaw, roll
    void showEulers(double pitch,
                    double yaw,
                    double roll);
    //show emg
    void showEmg(const std::array< int8_t, 8 >& emg);

public slots:

    void onAbs();

private:
    //ui dialog
    Ui::MyoDialog *ui;
    //graph temp values
    QVector<double> mXPlots;
    std::array< QVector<double>, 8 > mYPlots;
    //utilities
    void absPlots();
    void notAbsPlots();
    void doPlots();
};

#endif // MYODIALOG_H
