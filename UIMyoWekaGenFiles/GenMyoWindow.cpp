#include "GenMyoWindow.h"
#include "RecordingDialog.h"
#include "FlagsDialog.h"
#include "MyoDataOuput.h"
#include "MyoDataInput.h"
#include "GesturesBuilder.h"
#include "SamplingList.h"
#include "ModelForm.h"
#include "ui_GenMyoWindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QDialogButtonBox>

GenMyoWindow::GenMyoWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::GenMyoWindow)
  , mMyoDialog(this)
{
    ui->setupUi(this);
    //start path
    mPath = QDir::currentPath();
    //start path
    mPathExport = QDir::currentPath();
    //start
    mMyoManager.start();
    //set manager to myo dialog
    mMyoDialog.setMyoManager(&mMyoManager);
}

GenMyoWindow::~GenMyoWindow()
{
    //close window
    mMyoDialog.close();
    //close
    mMyoManager.close();
    //delete
    delete ui;
}


void GenMyoWindow::onAddClass(const QString& cname)
{
    addClass(cname);
}


//add a class
QListWidgetItem* GenMyoWindow::addClass(const QString &cname)
{
    //alloc item
    auto item=new QListWidgetItem();
    auto widget=new ClassForm(this);
    //set name
    widget->setName(cname);
    //init size
    item->setSizeHint(widget->sizeHint());
    //add item
    ui->mLWClasses->addItem(item);
    ui->mLWClasses->setItemWidget(item,widget);
    //put item into list
    auto* rows=&mWekaItems[item];
    //on delete
    widget->setOnDelete(
    [this,item]()
    {
        //remove from list
        mWekaItems.remove(item);
        //delete item
        delete item;
    });
    //on sempling
    widget->setOnSampling(
    [this,rows,widget]()
    {
        //show sampling list
        SamplingList dialog(this,rows);
        dialog.setClassName(widget->getName());
        //on record a sapling
        dialog.setOnRecording(
        [this](SampleForm& sample)
        {
            //start rec
            mMyoManager.startRecording();
            //sho dialog
            RecordingDialog dialog(this);
            //at end save samples
            dialog.execute(
            [this,&sample]()
            {
                sample.getRows() = mMyoManager.endRecording();
                sample.updateTime();
            });
        });
        //exec dialog
        dialog.exec();
    });
    //return last item edded
    return item;
}

void GenMyoWindow::onNew()
{
    //clear data
    mWekaItems.clear();
    ui->mLWClasses->clear();
}

void GenMyoWindow::onOpen()
{
    //path
    QString sPath=QFileDialog::getOpenFileName(this,
                                               "Open",
                                               mPath,
                                               "UIMyo Files *.uim;;"
                                               "Legacy Myo Files *.myodata");
    if( sPath.length() )
    {
        //save path
        mPath=sPath;
        //file type
        bool notLegacy = !mPath.endsWith(".myodata");
        //clear data
        mWekaItems.clear();
        ui->mLWClasses->clear();
        //load
        MyoListener::TypeInput myoLoader;
        //read new file
        if(notLegacy)
        {
            myoLoader.read(
            //file map
            mPath.toStdString(),
            mFlags,
            [this](const std::string& name,MyoListener::TypeInput::ListSamples& lsample)
            {
                //add item
                auto* item    = addClass(QString::fromStdString(name));
                //put all samples
                getList(item) = lsample;
            });
        }
        //read old file
        else
        {
            myoLoader.readOldFile(
            //file map
            mPath.toStdString(),
            //read
            [this](int idClass,
                   const std::string& className,
                   const MyoListener::TypeInput::Sample& rows)
            {
                //add class
                if(ui->mLWClasses->count() <= idClass)
                    onAddClass(QString::fromStdString(className));
                //get item
                auto item=ui->mLWClasses->item(idClass);
                //put row into row list
                mWekaItems[item].append(rows);
            });
        }
    }
}

void GenMyoWindow::onSave()
{
    if(!mWekaItems.empty())
    {
        //save
        if( mPath.length() )
            save();
        else
            onSaveAs();
    }
}

void GenMyoWindow::onSaveAs()
{
    if(!mWekaItems.empty())
    {
        //path
        QString sPath=QFileDialog::getSaveFileName(this,
                                                  "Save",
                                                   mPath,
                                                   "UIMyo Files *.uim");

        if( sPath.length() )
        {
            mPath=sPath;
            save();
        }
    }
}

void GenMyoWindow::onExport()
{
    if(!mWekaItems.empty())
    {
        //get flags
        FlagsDialog flagsDialog(this);
        if(!flagsDialog.exec(mFlags)) return;
        //save
        if( mPathExport.length() )
        {
            if( mPathExport.endsWith(".arff") ) saveWEKA();
            else if( mPathExport.endsWith(".trening") ) saveFANN();
        }
        else
            onExportAs();
    }
}

void GenMyoWindow::onExportAs()
{
    if(!mWekaItems.empty())
    {
        //path
        QString sPath=QFileDialog::getSaveFileName(this,
                                                  "Export",
                                                   mPath,
                                                   "DataSet Files *.ds;;"
                                                   "Weka Files *.arff;;"
                                                   "FANN Files *.fann");

        if( sPath.length() &&
           (sPath.endsWith(".ds") ||
            sPath.endsWith(".arff") ||
            sPath.endsWith(".fann")) )
        {
            //save path
            mPath=sPath;
            //get flags
            FlagsDialog flagsDialog(this);
            if(!flagsDialog.exec(mFlags)) return;
            //save file
            if(mPath.endsWith(".ds")) saveDATASET();
            else if(mPath.endsWith(".arff")) saveWEKA();
            else if(mPath.endsWith(".fann")) saveFANN();
        }
    }
}

void GenMyoWindow::onCreateModel()
{
    //path
    QString sPath=QFileDialog::getOpenFileName(this,
                                               "Open",
                                               mPath,
                                               "DataSet Files *.ds");
    //model form
    ModelForm mform(this);
    //execute
    mform.execute(sPath);
}


void GenMyoWindow::save()
{
    if(!mWekaItems.empty())
    {
        //get all items
        size_t nItems=ui->mLWClasses->count();
        //get path
        std::string lPath=mPath.toStdString().c_str();
        assert(lPath.length());
        //file
        MyoListener::TypeOuput ouput;
        ouput.open(lPath,mFlags,nItems);
        //seva all
        for(size_t i=0;i!=nItems;++i)
        {
            ouput.append(getNameClass(i).toStdString(),getList(i));
        }
    }
}

void GenMyoWindow::saveDATASET()
{
    if(!mWekaItems.empty())
    {
        //get all items
        size_t nclass=ui->mLWClasses->count();
        //path
        auto lPath=mPath.toStdString();
        assert(lPath.length());
        //ouput serialize
        MyoListener::TypeOuputDataSet ouput;
        //types
        switch (mFlags.mMode)
        {
            case DataFlags::SAMPLE_MODE:
            {
                //vector of names
                QList< QString > classNames;
                //get all class
                for(size_t i=0;i!=nclass;++i)
                    classNames.push_back(getNameClass(i));
                //file
                ouput.open(lPath, mFlags, classNames);
                //seva all
                for(size_t i=0;i!=nclass;++i)
                {
                    auto  name  = getNameClass(i);
                    auto& lrows = getList(i);
                    for(auto& rows:lrows) ouput.append(name,rows);
                }
            }
            break;
            case DataFlags::GESTURE_MODE:
            {
                GesturesBuilder gbuilder(mFlags);
                //put all
                for(size_t i=0;i!=nclass;++i)
                {
                    auto  name  = getNameClass(i);
                    auto& lrows = getList(i);
                    for(auto& rows:lrows) gbuilder.append(name,rows);
                }
                //nreps
                size_t nreps = 1;
                //get ouput
                GesturesBuilder::GestureOutput ouputItems;
                gbuilder.finalize(nreps,ouputItems);
                //////////////////////////////////////////////////////////////
                //type of ouput
                DataFlags flags( mFlags );
                //set nreps
                flags.mReps = nreps;
                //get keys
                QList< QString > keys = ouputItems.keys();
                //ouput
                ouput.open(lPath, flags, keys);
                //write
                for(auto& name:keys)
                {
                    //list of input
                    auto& inputs=ouputItems.value(name);
                    //put all into file
                    for(auto& rows:inputs)
                    {
                        ouput.append(name, rows);
                    }
                }
                //////////////////////////////////////////////////////////////
            }
            default:
            break;
        }
    }
}

void GenMyoWindow::saveWEKA()
{
    if(!mWekaItems.empty())
    {
        //get all items
        size_t nclass=ui->mLWClasses->count();
        //ouput
        MyoListener::TypeOuputWeka ouput;
        //path
        std::string lPath=mPath.toStdString().c_str();
        assert(lPath.length());
        //cases
        switch (mFlags.mMode)
        {
            case DataFlags::SAMPLE_MODE:
            {
                //vector of names
                QList< QString > classNames;
                //get all class
                for(size_t i=0;i!=nclass;++i)
                    classNames.push_back(getNameClass(i));
                //file
                ouput.open(lPath, mFlags, classNames);
                //seva all
                for(size_t i=0;i!=nclass;++i)
                {
                    auto  name  = getNameClass(i).toStdString();
                    auto& lrows = getList(i);
                    for(auto& rows:lrows) ouput.append(name,rows);
                }
            }
            break;
            case DataFlags::GESTURE_MODE:
            {
                GesturesBuilder gbuilder(mFlags);
                //put all
                for(size_t i=0;i!=nclass;++i)
                {
                    auto  name  = getNameClass(i);
                    auto& lrows = getList(i);
                    for(auto& rows:lrows) gbuilder.append(name,rows);
                }
                //nreps
                size_t nreps = 1;
                //get ouput
                GesturesBuilder::GestureOutput ouputItems;
                gbuilder.finalize(nreps,ouputItems);
                //////////////////////////////////////////////////////////////
                //type of ouput
                DataFlags flags( mFlags );
                //set nreps
                flags.mReps = nreps;
                //get keys
                QList< QString > keys = ouputItems.keys();
                //ouput
                ouput.open(lPath, flags, keys);
                //write
                for(auto& name:keys)
                {
                    //list of input
                    auto& inputs=ouputItems.value(name);
                    //put all into file
                    for(auto& rows:inputs)
                    {
                        ouput.append(name.toStdString(), rows);
                    }
                }
                //////////////////////////////////////////////////////////////
            }
            default:
            break;
        }
    }
}

void GenMyoWindow::saveFANN()
{
    if(!mWekaItems.empty())
    {
        //get all items
        size_t nclass=ui->mLWClasses->count();
        //path
        auto lPath=mPath.toStdString();
        assert(lPath.length());
        //ouput serialize
        MyoListener::TypeOuputFANN ouput;
        //types
        switch (mFlags.mMode)
        {
            case DataFlags::SAMPLE_MODE:
            {
                //vector of names
                QList< QString > classNames;
                //get all class
                for(size_t i=0;i!=nclass;++i)
                    classNames.push_back(getNameClass(i));
                //file
                ouput.open(lPath, mFlags, classNames);
                //seva all
                for(size_t i=0;i!=nclass;++i)
                {
                    auto  name  = getNameClass(i);
                    auto& lrows = getList(i);
                    for(auto& rows:lrows) ouput.append(name,rows);
                }
            }
            break;
            case DataFlags::GESTURE_MODE:
            {
                GesturesBuilder gbuilder(mFlags);
                //put all
                for(size_t i=0;i!=nclass;++i)
                {
                    auto  name  = getNameClass(i);
                    auto& lrows = getList(i);
                    for(auto& rows:lrows) gbuilder.append(name,rows);
                }
                //nreps
                size_t nreps = 1;
                //get ouput
                GesturesBuilder::GestureOutput ouputItems;
                gbuilder.finalize(nreps,ouputItems);
                //////////////////////////////////////////////////////////////
                //type of ouput
                DataFlags flags( mFlags );
                //set nreps
                flags.mReps = nreps;
                //get keys
                QList< QString > keys = ouputItems.keys();
                //ouput
                ouput.open(lPath, flags, keys);
                //write
                for(auto& name:keys)
                {
                    //list of input
                    auto& inputs=ouputItems.value(name);
                    //put all into file
                    for(auto& rows:inputs)
                    {
                        ouput.append(name, rows);
                    }
                }
                //////////////////////////////////////////////////////////////
            }
            default:
            break;
        }
    }
}

//utilis
ClassForm& GenMyoWindow::getFormClass(int i)
{
    auto  item    = ui->mLWClasses->item(i);
    return *dynamic_cast<ClassForm*>(ui->mLWClasses->itemWidget(item));
}

QString GenMyoWindow::getNameClass(int i)
{
    return getFormClass(i).getName();
}

QLinkedList < MyoListener::TypeRows >& GenMyoWindow::getList(int i)
{
    auto  item = ui->mLWClasses->item(i);
    return getList(item);
}

QLinkedList < MyoListener::TypeRows >& GenMyoWindow::getList(QListWidgetItem* item)
{
    return *mWekaItems.find(item);
}

void GenMyoWindow::onShowInputs(bool show)
{
    mMyoDialog.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);
    mMyoDialog.setMyoManager(&mMyoManager);
    mMyoDialog.show();
    mMyoDialog.setFocus();
    mMyoDialog.activateWindow();
    mMyoDialog.raise();
}
