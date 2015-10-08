#ifndef GESTURESBUILDER_H
#define GESTURESBUILDER_H

#include <QString>
#include <QMap>
#include <QVector>
#include "MyoListener.h"

/*!
 * \brief The GesturesBuilder class
 */
class GesturesBuilder
{
public:

    /*!
     * \brief GestureOutput, map of samples by classes
     */
    using GestureOutput = QMap< QString, QList < MyoListener::TypeRows > >;

    /*!
     * \brief GesturesBuilder
     * \param flags
     */
    GesturesBuilder(const DataFlags& flags);

    /*!
     * \brief append
     * \param className
     * \param rows
     */
    void append(const QString& className,const MyoListener::TypeRows& rows);

    /*!
     * \brief finalize
     * \param nrows
     * \param gout
     */
    void finalize(size_t& nrows,GestureOutput& gout) const;

private:

    DataFlags mFlags;         //! flags used for bilding datas
    GestureOutput mContext;   //! datas context builts

};

#endif // GESTURESBUILDER_H
