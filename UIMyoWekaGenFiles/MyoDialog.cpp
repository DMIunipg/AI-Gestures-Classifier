#include "MyoDialog.h"
#include "ui_MyoDialog.h"

static void UiPlotDefault(QCustomPlot* cplot)
{
    cplot->setBackground(QColor{"#302F2F"});
    cplot->addGraph();
    cplot->xAxis->setAutoTickLabels(false);
    cplot->yAxis->setAutoTickLabels(false);
    cplot->xAxis->setTickLabels(false);
    cplot->yAxis->setTickLabels(false);
    cplot->xAxis->setVisible(false);
    cplot->yAxis->setVisible(false);
    cplot->xAxis->setRange(0,100);
    cplot->yAxis->setRange(-128,127);
    cplot->axisRect()->setAutoMargins(QCP::msNone);
    cplot->axisRect()->setMargins(QMargins(0,0,0,0));
    //set default pen
    QPen pen;
    pen.setColor(QColor(38, 38, 255, 255));
    pen.setWidthF(2);
    cplot->graph(0)->setPen(pen);
}


//utilities
void MyoDialog::absPlots()
{
    ui->mCPTop->yAxis->setRange(-1,129);
    ui->mCPTopLeft->yAxis->setRange(-1,129);
    ui->mCPTopRight->yAxis->setRange(-1,129);
    ui->mCPMidLeft->yAxis->setRange(-1,129);
    ui->mCPMidRight->yAxis->setRange(-1,129);
    ui->mCPDownLeft->yAxis->setRange(-1,129);
    ui->mCPDownRight->yAxis->setRange(-1,129);
    ui->mCPDown->yAxis->setRange(-1,129);
    //put all value more then 0
    for(auto& vY:mYPlots)
    {
        for(auto& y:vY) y=std::abs(y);
    }
}
void MyoDialog::notAbsPlots()
{
    ui->mCPTop->yAxis->setRange(-128,127);
    ui->mCPTopLeft->yAxis->setRange(-128,127);
    ui->mCPTopRight->yAxis->setRange(-128,127);
    ui->mCPMidLeft->yAxis->setRange(-128,127);
    ui->mCPMidRight->yAxis->setRange(-128,127);
    ui->mCPDownLeft->yAxis->setRange(-128,127);
    ui->mCPDownRight->yAxis->setRange(-128,127);
    ui->mCPDown->yAxis->setRange(-128,127);
}
void MyoDialog::doPlots()
{
    //draw
    ui->mCPTop->replot();
    ui->mCPTopLeft->replot();
    ui->mCPTopRight->replot();
    ui->mCPMidLeft->replot();
    ui->mCPMidRight->replot();
    ui->mCPDownLeft->replot();
    ui->mCPDownRight->replot();
    ui->mCPDown->replot();
}
void MyoDialog::onAbs()
{
    if(ui->mCBAbs->isChecked())
    {
        absPlots();
    }
    else
    {
        notAbsPlots();
    }
}

MyoDialog::MyoDialog(QWidget *parent,
                     bool disable)
:QDialog(parent)
,ui(new Ui::MyoDialog)
{
    ui->setupUi(this);
    //disable video form
    if(disable)
    {
        //disable video form
        delete ui->mSTime;
        delete ui->mLTime;
        delete ui->mLTimeInfo;
        delete ui->mGBVideo;
        ui->mSTime = nullptr;
        ui->mLTime = nullptr;
        ui->mLTimeInfo = nullptr;
        ui->mGBVideo   = nullptr;
    }
    else
    {
        //disable button
        delete ui->mPBTTConnect;
        ui->mPBTTConnect = nullptr;
    }
    //init plotters
    UiPlotDefault(ui->mCPTop);
    UiPlotDefault(ui->mCPTopLeft);
    UiPlotDefault(ui->mCPTopRight);
    UiPlotDefault(ui->mCPMidLeft);
    UiPlotDefault(ui->mCPMidRight);
    UiPlotDefault(ui->mCPDownLeft);
    UiPlotDefault(ui->mCPDownRight);
    UiPlotDefault(ui->mCPDown);
    // generate some data:
    mXPlots.resize(100);
    for(int i=0;i!=mXPlots.size();++i)
    {
        mXPlots[i] = (double)i;
    }
    //default all to 0
    for (auto& vY:mYPlots)
    {
        vY.resize(100);
        vY.fill(0.0);
    }
    // create graph and assign data to it:
    ui->mCPTop->graph(0)->setData(mXPlots, mYPlots[myo::EMG_TOP]);
    ui->mCPTopLeft->graph(0)->setData(mXPlots, mYPlots[myo::EMG_TOP_LEFT]);
    ui->mCPTopRight->graph(0)->setData(mXPlots, mYPlots[myo::EMG_TOP_RIGHT]);
    ui->mCPMidLeft->graph(0)->setData(mXPlots, mYPlots[myo::EMG_MID_LEFT]);
    ui->mCPMidRight->graph(0)->setData(mXPlots, mYPlots[myo::EMG_MID_RIGHT]);
    ui->mCPDownLeft->graph(0)->setData(mXPlots, mYPlots[myo::EMG_DOWN_LEFT]);
    ui->mCPDownRight->graph(0)->setData(mXPlots, mYPlots[myo::EMG_DOWN_RIGHT]);
    ui->mCPDown->graph(0)->setData(mXPlots, mYPlots[myo::EMG_DOWN]);

}

void MyoDialog::setMyoManager(MyoManager* myoManager)
{
    ui->mMyoDrawFrame->setMyoManager(myoManager);
    //get connection status
    if(ui->mPBTTConnect)
    {
        connect(myoManager,SIGNAL(connectionStatusChange(bool)),
                this,      SLOT(setIsConnected(bool)));
    }
}

static void UiTextColor(QLabel* label,
                        const std::array< int8_t, 8 > &emg,
                        int id)
{

    int value  = std::abs(emg[id]);
        value *= value;
        value  = std::min(value,255);
    QString style("QLabel { color: rgba("+
                  QString::number(value)+
                  ",0,0,255); }");
    label->setStyleSheet(style);
    label->setText(QString::number(emg[id]));

}

static void UiStrength(QLabel* label,
                       const std::array< int8_t, 8 > &emgs)
{
    int value = 0;
    for(int8_t emg : emgs) value += std::abs(emg);
    value /= emgs.size();
    value *= value;
    value  = std::min(value,255);

    QString style("QLabel { color: rgba("+
                  QString::number(value)+
                  ",0,0,255); }");
    label->setStyleSheet(style);
}

static double lerp(double left,double right,double l)
{
    return left*l+(right*(1.0-l));
}

#if 0
static QVector<double> invSmoth(const QVector<double>& qvec,double smoth)
{
   //ouput vector
   QVector<double> output;
   output.resize(qvec.size());
   //last element
   size_t last = qvec.size()-1;
   //last value is equal
   output[last] = qvec[last];
   //smoth
   for(int i=last;i!=0;--i)
   {
      output[i-1] = lerp(qvec[i-1], output[i], smoth);
   }
   //ouput
   return output;
}
#else
static const QVector<double>& invSmoth(const QVector<double>& qvec,double smoth)
{
    return qvec;
}
#endif

void MyoDialog::showEmg(const std::array< int8_t, 8 > &emg)
{
    UiTextColor(ui->mLTop,emg,myo::EMG_TOP);

    UiTextColor(ui->mLTopLeft,emg,myo::EMG_TOP_LEFT);
    UiTextColor(ui->mLTopRight,emg,myo::EMG_TOP_RIGHT);

    UiTextColor(ui->mLMidLeft,emg,myo::EMG_MID_LEFT);
    UiTextColor(ui->mLMidRight,emg,myo::EMG_MID_RIGHT);

    UiTextColor(ui->mLDownLeft,emg,myo::EMG_DOWN_LEFT);
    UiTextColor(ui->mLDownRight,emg,myo::EMG_DOWN_RIGHT);

    UiTextColor(ui->mLDown,emg,myo::EMG_DOWN);

    UiStrength(ui->mLStrength,emg);
    //smoth
    double emgSmooth = ui->mDSBSmooth->value();
    bool   emgAbs    = ui->mCBAbs->isChecked();
    //put all values into graph
    for(int i=0;i!=8;++i)
    {
        //copy old vector
        auto oldY=mYPlots[i];
        //lerp
        double value = emgAbs ? std::abs(emg[i]) : emg[i];
        //put the new value into the vector
        mYPlots[i][0]= lerp(oldY[0],value,emgSmooth);
        //put all value shifted by 1
        for(int j=1;j!=mYPlots[i].size();++j)
        {
            mYPlots[i][j] = oldY[j-1];
        }
    }
    //put values
    ui->mCPTop->graph(0)->setData(mXPlots,      invSmoth(mYPlots[myo::EMG_TOP],emgSmooth));
    ui->mCPTopLeft->graph(0)->setData(mXPlots,  invSmoth(mYPlots[myo::EMG_TOP_LEFT],emgSmooth));
    ui->mCPTopRight->graph(0)->setData(mXPlots, invSmoth(mYPlots[myo::EMG_TOP_RIGHT],emgSmooth));
    ui->mCPMidLeft->graph(0)->setData(mXPlots,  invSmoth(mYPlots[myo::EMG_MID_LEFT],emgSmooth));
    ui->mCPMidRight->graph(0)->setData(mXPlots, invSmoth(mYPlots[myo::EMG_MID_RIGHT],emgSmooth));
    ui->mCPDownLeft->graph(0)->setData(mXPlots, invSmoth(mYPlots[myo::EMG_DOWN_LEFT],emgSmooth));
    ui->mCPDownRight->graph(0)->setData(mXPlots,invSmoth( mYPlots[myo::EMG_DOWN_RIGHT],emgSmooth));
    ui->mCPDown->graph(0)->setData(mXPlots,     invSmoth(mYPlots[myo::EMG_DOWN],emgSmooth));
    //draw
    doPlots();
}

void MyoDialog::showEulers(double pitch,
                           double yaw,
                           double roll)
{
    ui->mLPitch->setText(QString::number((int)pitch));
    ui->mLYaw->setText(QString::number((int)yaw));
    ui->mLRoll->setText(QString::number((int)roll));
}

void MyoDialog::setVideoToShow(const MyoListener::TypeRows& rows)
{
    //current row if first
    mCurrentRow = 0;
    //save list of rows
    mVideoRows  = rows;
    //set range
    ui->mSTime->setRange(0,rows.size());
}

//set connection status
void MyoDialog::setIsConnected(bool isConnected)
{
    if(isConnected)
    {
        ui->mPBTTConnect->setText("Connected");
        ui->mPBTTConnect->setDisabled(true);
    }
    else
    {
        ui->mPBTTConnect->setText("Try to connect");
        ui->mPBTTConnect->setDisabled(false);
    }
}

//connect event
void MyoDialog::onTryToConnect(bool event)
{
    //manager
    MyoManager* mmanager = ui->mMyoDrawFrame->getMyoManager();
    //try to connect
    if(mmanager && !mmanager->isConnected() && !mmanager->isRunning())
    {
        mmanager->start();
    }
}

//play/pause event
void MyoDialog::onPlayPause(bool play)
{
    if(play)
    {
        mVideoTime = new QBasicTimer();
        mVideoTime->start(MyoData::msupadate,this);
    }
    else if (mVideoTime)
    {
        mVideoTime->stop();
        delete mVideoTime;
        mVideoTime = nullptr;
    }
}

//change slaider
void MyoDialog::onSlaiderChange(int value)
{
    if(!mVideoRows.size()) return ;
    //set value
    mCurrentRow = std::min((size_t)value,mVideoRows.size());
    //show
    timerEvent(nullptr);
    //return to the last value
    --mCurrentRow;
}

//update video
void MyoDialog::timerEvent(QTimerEvent *etime)
{
    if(!mVideoRows.size()) return ;
    //show frame
    const MyoListener::TypeRaw& raw=mVideoRows[mCurrentRow];
    showEmg(raw.getEmg());
    //set angles
    auto eulers=raw.getEulerAngles();
    showEulers(eulers.mPitch  * 180.0 / M_PI  ,
               eulers.mYaw  * 180.0 / M_PI ,
               eulers.mRoll * 180.0 / M_PI );
    //show time
    ui->mLTime->setText(QString::number(raw.getTime(),'f',3));
    //show rotation
    ui->mMyoDrawFrame->setDefaultRotation(raw.getQuaternion());
    //redraw
    repaint();
    //next frame
    mCurrentRow = (mCurrentRow+1)  % mVideoRows.size();
    //set slider position
    if(etime)
    {
        ui->mSTime->blockSignals(true);
        ui->mSTime->setValue(mCurrentRow);
        ui->mSTime->blockSignals(false);
    }
}

MyoDialog::~MyoDialog()
{
    //delete update video
    if(mVideoTime)
    {
        mVideoTime->stop();
        delete mVideoTime;
    }
    //delete ui
    delete ui;
}
