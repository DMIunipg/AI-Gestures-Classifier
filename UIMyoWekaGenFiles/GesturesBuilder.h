#ifndef GESTURESBUILDER_H
#define GESTURESBUILDER_H

#include <QString>
#include <QMap>
#include <QVector>
#include "MyoListener.h"

class GesturesBuilder
{
public:
    //ouput
    using GestureOutput = QMap< QString, QList < MyoListener::TypeRows > >;
    //build
    GesturesBuilder(const DataFlags& flags);
    //append a row
    void append(const QString& className,const MyoListener::TypeRows& rows);
    //get rows
    void finalize(size_t& nrows,GestureOutput& gout) const;

private:

    DataFlags mFlags;
    GestureOutput mContext;

};

#endif // GESTURESBUILDER_H
