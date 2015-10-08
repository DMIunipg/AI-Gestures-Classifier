#ifndef FLAGSDIALOG_H
#define FLAGSDIALOG_H
#pragma once

#include <QDialog>
#include "DataFlags.h"

namespace Ui {
class FlagsDialog;
}

/*!
 * \brief The FlagsDialog class
 */
class FlagsDialog : public QDialog
{
    Q_OBJECT

public:

    /*!
     * \brief FlagsDialog
     * \param parent
     */
    explicit FlagsDialog(QWidget *parent = 0);

    /*!
     * \brief ~FlagsDialog
     * \param parent
     */
    ~FlagsDialog();

    /*!
     * \brief setFalgs
     * \param flags
     */
    void setFalgs(const  DataFlags& flags);

    /*!
     * \brief getFlags
     * \return flags
     */
    DataFlags getFlags();

    /*!
     * \brief exec
     * \param flags
     * \return true if user press ok button
     */
    bool exec(DataFlags& flags);

public slots:

    /*!
     * \brief onOk
     */
    void onOk();

    /*!
     * \brief onCancel
     */
    void onCancel();

    /*!
     * \brief onSempleMode
     * \param enabled
     */
    void onSempleMode(bool enabled);

    /*!
     * \brief onGestureMode
     * \param enabled
     */
    void onGestureMode(bool enabled);

    /*!
     * \brief onNormalize
     * \param enable
     */
    void onNormalize(bool enable);

    /*!
     * \brief onEmg
     * \param enable
     */
    void onEmg(bool enable);

private:

    Ui::FlagsDialog *ui;                     //! ui pointer
    bool             mReturnStatus{ false }; //! save true if user press ok button


};

#endif // FLAGSDIALOG_H
