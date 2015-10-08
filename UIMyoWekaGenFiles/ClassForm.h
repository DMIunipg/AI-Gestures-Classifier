#ifndef CLASSFORM_H
#define CLASSFORM_H

#include <QWidget>

namespace Ui {
class ClassForm;
}

/*!
 * \brief The ClassForm class
 */
class ClassForm : public QWidget
{
    Q_OBJECT

public:

    /*!
     * \brief ClassForm
     * \param parent
     */
    explicit ClassForm(QWidget *parent = 0);

    /*!
     * \brief ~ClassForm
     */
    ~ClassForm();

    /*!
     * \brief setOnDelete
     * \param onDelete
     */
    void setOnDelete(std::function<void(void)> onDelete);

    /*!
     * \brief setOnSampling
     * \param onSampling
     */
    void setOnSampling(std::function<void(void)> onSampling);

    /*!
     * \brief getName
     * \return class name
     */
    QString getName() const;

    /*!
     * \brief setName
     * \param cname set name of a class
     */
    void setName(const QString& cname);

public slots:

    /*!
     * \brief onDelete
     */
    void onDelete();

    /*!
     * \brief onSampling
     */
    void onSampling();


private:

    Ui::ClassForm *ui;                                  //! ui pointer
    std::function<void(void)> mOnDelete    { nullptr }; //! callback called when is pressed the delete button
    std::function<void(void)> mOnSampling  { nullptr }; //! callback called when is pressed the sampling button
};

#endif // CLASSFORM_H
