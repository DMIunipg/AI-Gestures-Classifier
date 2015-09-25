#ifndef GESTUREFORM_H
#define GESTUREFORM_H
#pragma once

#include <QWidget>
#include <functional>

namespace Ui {
class GestureForm;
}

class GestureForm : public QWidget
{
    Q_OBJECT

public:

    explicit GestureForm(QWidget *parent = 0);

    ~GestureForm();

    void setOnDelete(std::function<void(void)> onDelete);

    void setOnRecording(std::function<void(void)> onRecording);

    QString getName() const;

    void setName(const QString& str);

public slots:

    void onDelete();
    void onRecording();

private:
    Ui::GestureForm *ui;
    std::function<void(void)> mOnDelete    { nullptr };
    std::function<void(void)> mOnRecording { nullptr };
};

#endif // GESTUREFORM_H
