#include <climits>
#include "GesturesBuilder.h"

GesturesBuilder::GesturesBuilder(const DataFlags& flags)
{
    mFlags = flags;
}


void GesturesBuilder::append(const QString& nameClass,
                             const MyoListener::TypeRows& rows)
{
    if(!rows.size()) return;
    //temp
    MyoListener::TypeRows ouputRows;
    //delta time
    double deltaTime = 0;
    //for all rows
    for(auto& row : rows)
    {
        //time semple filter
        if(deltaTime <= row.getTime())
        {
            //append class
            ouputRows.append(row);
            //next semple
            deltaTime+=mFlags.mDeltaTime;
        }
        //time to break
        if(row.getTime() >=  mFlags.mTimePerGesture)
        {
            break;
        }
    }
    //append
    mContext[nameClass].append(ouputRows);
}

void GesturesBuilder::finalize(size_t& nrows, GestureOutput& oContext)  const
{
   if(!mContext.size()) return;
   //size
   size_t msize = (size_t)ULONG_MAX;
   //for all rows
   for(auto& rowsClass : mContext)
   for(auto& rowsSemple: rowsClass)
   {
       msize=std::min(msize,rowsSemple.size());
   }
   //put all into new context
   oContext = mContext;
   //resize
   for(auto& rowsClass : oContext)
   for(auto& rowsSemple: rowsClass)
   {
       rowsSemple.resize(msize);
   }
   //ouput by ref
   nrows = msize;
}
