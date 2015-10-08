#ifndef GENMYOWINDOW_H
#define GENMYOWINDOW_H
#pragma once

#include <QMainWindow>
#include <QMap>
#include <QListWidgetItem>
#include "MyoManager.h"
#include "MyoDialog.h"
#include "ClassForm.h"
#include "SampleForm.h"

namespace Ui {
class GenMyoWindow;
}

/*!
 * \brief The GenMyoWindow class
 */
class GenMyoWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*!
     * \brief GenMyoWindow
     * \param parent
     */
    explicit GenMyoWindow(QWidget *parent = 0);

    /*!
     * \brief ~GenMyoWindow
     */
    ~GenMyoWindow();

private slots:

    /*!
     * \brief onAddClass
     * \param cname class name
     */
    void onAddClass(const QString &cname="");

    /*!
     * \brief onNew
     */
    void onNew();

    /*!
     * \brief onOpen
     */
    void onOpen();

    /*!
     * \brief onSave
     */
    void onSave();

    /*!
     * \brief onSaveAs
     */
    void onSaveAs();

    /*!
     * \brief onExport
     */
    void onExport();

    /*!
     * \brief onExportAs
     */
    void onExportAs();

    /*!
     * \brief onShowInputs
     * \param show
     */
    void onShowInputs(bool show);

private:

    /*!
     * \brief save MUI
     */
    void save();
    /*!
     * \brief save WEKA file
     */
    void saveWEKA();

    /*!
     * \brief save FANN file
     */
    void saveFANN();

    /*!
     * \brief addClass
     * \param cname
     * \return
     */
    QListWidgetItem* addClass(const QString &cname="");

    /*!
     * \brief getFormClass
     * \param id
     * \return class form
     */
    ClassForm& getFormClass(int id);

    /*!
     * \brief getNameClass
     * \param id
     * \return class name
     */
    QString getNameClass(int id);

    /*!
     * \brief getList
     * \param id
     * \return list of samples
     */
    QLinkedList < MyoListener::TypeRows >& getList(int id);

    /*!
     * \brief getList
     * \param item
     * \return list of samples
     */
    QLinkedList < MyoListener::TypeRows >& getList(QListWidgetItem* item);

    Ui::GenMyoWindow *ui;   //! ui pointer
    MyoManager mMyoManager; //! myo listener manager
    MyoDialog mMyoDialog;   //! myo dialog (dialog to show the myo inputs)
    QMap< QListWidgetItem*, QLinkedList < MyoListener::TypeRows > > mWekaItems; //! map of the item with list of samples
    QString mPath;      //! last path where have saved myo datas
    QString mPathExport;//! last path where have exported myo datas
    DataFlags mFlags;   //! last flags for datas exporting
};

#endif // GENMYOWINDOW_H
