#ifndef CLASSFORM_H
#define CLASSFORM_H

#include <QWidget>

namespace Ui {
class ClassForm;
}

class ClassForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClassForm(QWidget *parent = 0);

    ~ClassForm();

    void setOnDelete(std::function<void(void)> onDelete);

    void setOnSampling(std::function<void(void)> onSampling);

    QString getName() const;

    void setName(const QString& str);

public slots:


    void onDelete();

    void onSampling();


private:

    Ui::ClassForm *ui;
    std::function<void(void)> mOnDelete    { nullptr };
    std::function<void(void)> mOnSampling  { nullptr };
};

#endif // CLASSFORM_H
