#ifndef FLAGSDIALOG_H
#define FLAGSDIALOG_H
#pragma once

#include <QDialog>
#include "DataFlags.h"

namespace Ui {
class FlagsDialog;
}

class FlagsDialog : public QDialog
{
    Q_OBJECT

public:

    explicit FlagsDialog(QWidget *parent = 0);

    ~FlagsDialog();

    void setFalgs(const  DataFlags& flags);

    DataFlags getFlags();

    bool exec(DataFlags& flags);

public slots:

    void onOk();
    void onCancel();

private:

    Ui::FlagsDialog *ui;
    bool             mReturnStatus{ false };

};

#endif // FLAGSDIALOG_H
