#ifndef GENMYOWINDOW_H
#define GENMYOWINDOW_H
#pragma once

#include <QMainWindow>
#include <QMap>
#include <QListWidgetItem>
#include "MyoManager.h"

namespace Ui {
class GenMyoWindow;
}

class GenMyoWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit GenMyoWindow(QWidget *parent = 0);

    ~GenMyoWindow();

private slots:

    void onAddGesture();
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onExport();
    void onExportAs();

private:
    //save
    void open(const QString& str);
    void save();
    void saveWEKA();
    void saveFANN();
    //Gui objects
    Ui::GenMyoWindow *ui;
    //Myo manager
    MyoManager mMyoManager;
    //item/rows map
    QMap< QListWidgetItem*, MyoListener::TypeRows > mWekaItems;
    //get dir
    QString mPath;
    //get dir
    QString mPathExport;
    //get flags
    DataFlags mFlags;
};

#endif // GENMYOWINDOW_H
