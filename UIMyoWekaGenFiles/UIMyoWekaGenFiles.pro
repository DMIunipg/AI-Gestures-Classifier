#-------------------------------------------------
#
# Project created by QtCreator 2015-06-27T11:53:04
#
#-------------------------------------------------

QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = UIMyoWekaGenFiles
TEMPLATE = app
#c++11 flags
CONFIG += c++11

#dest
CONFIG(debug, debug|release) {
    DESTDIR = $${OUT_PWD}/debug
} else {
    DESTDIR = $${OUT_PWD}/release
}
#windows lib
win32 {
    !contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += -L$$PWD/../myo.win/lib/ -lmyo32
        LIBS += -L$$PWD/../myo.win/bin/myo32.dll
        #static libs
        #LIBS += -L$$PWD/../myo.mac/lib/ -l<lib>
    } else {
        LIBS += -L$$PWD/../myo.win/lib/ -lmyo64
        LIBS += -L$$PWD/../myo.win/bin/myo64.dll
        #static libs
        #LIBS += -L$$PWD/../myo.mac/lib/ -l<lib>
    }
    #OpenGL
    LIBS += OpenGL32.lib
    #include
    INCLUDEPATH += $$PWD/

    INCLUDEPATH += $$PWD/../myo.win/include/
    DEPENDPATH  += $$PWD/../myo.win/include/

    INCLUDEPATH += $$PWD/../myo.win/include/myo/
    DEPENDPATH  += $$PWD/../myo.win/include/myo/
    #path GesturesClassifierApplication
    GCAppPath = $$PWD/../GesturesClassifierWorkSpace/x64/Release
    #if not exists... compile...
    !exists($$GCAppPath) {
        #output
        message(compile $$GCAppPath)
        #compile external tools
        system(msbuild \
               ../GesturesClassifierWorkSpace/GesturesClassifierWorkSpace.sln \
               /p:Configuration=Release /p:Platform=x64)

    }
    #output
    message(copy $$shell_path($$GCAppPath/GesturesClassifierApplication.exe) to $$shell_path($${DESTDIR}))
    system(cmd /c xcopy $$shell_path($$GCAppPath/GesturesClassifierApplication.exe) $$shell_path($${DESTDIR}) /y)
    message(copy $$shell_path($$GCAppPath/GesturesClassifierExemple.exe) to $$shell_path($${DESTDIR}))
    system(cmd /c xcopy $$shell_path($$GCAppPath/GesturesClassifierExemple.exe) $$shell_path($${DESTDIR}) /y)
}
#mac os x libs
macx {
    #mac os x
    QMAKE_MAC_SDK = macosx10.11
    #add myo framework
    LIBS += -F$$PWD/../myo.mac/framework/ -framework myo
    #framework path
    QMAKE_RPATHDIR += @executable_path/../Frameworks
    #C++11 Lib Clang
    QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++
    QMAKE_CXXFLAGS += -Wunused-parameter
    #static libs
    #LIBS += -L$$PWD/../myo.mac/lib/ -l<lib>
    #dynamic
    #LIBS += -L$$PWD/../myo.mac/bin/ -lfann.2.2.0
    #LIBS += -L$$PWD/../myo.mac/bin/ -ldoublefann.2.2.0
    #LIBS += -L$$PWD/../myo.mac/bin/ -lfixedfann.2.2.0
    #LIBS += -L$$PWD/../myo.mac/bin/ -lfloatfann.2.2.0
    #dynamic path
    QMAKE_RPATHDIR += @executable_path/../Dylibs
    #include
    INCLUDEPATH += $$PWD/
    INCLUDEPATH += $$PWD/../myo.mac/include/
    DEPENDPATH  += $$PWD/../myo.mac/include/
    #hack
    INCLUDEPATH += $$PWD/../myo.win/include
    DEPENDPATH  += $$PWD/../myo.win/include
    INCLUDEPATH += $$PWD/../myo.win/include/myo/
    INCLUDEPATH += $$PWD/../myo.win/include/myo/
    #make bundle
    #framework
    APP_FM_FILES.files = $$PWD/../myo.mac/framework/myo.framework
    APP_FM_FILES.path  = Contents/Frameworks/
    QMAKE_BUNDLE_DATA += APP_FM_FILES
    #path GesturesClassifierApplication
    GCAppPath = $$PWD/../GesturesClassifierApplication/Build/Release/GesturesClassifierApplication
    #if not exists... compile...
    !exists($$GCAppPath) {
        #output
        message(compile $$GCAppPath)
        #compile external tools
        system(xcodebuild \
               -project ../GesturesClassifier/GesturesClassifier.xcodeproj \
               -scheme GesturesClassifier \
               -configuration Release)
        system(xcodebuild \
               -project ../GesturesClassifierApplication/GesturesClassifierApplication.xcodeproj \
               -scheme GesturesClassifierApplication \
               -configuration Release)
    }
    #resource
    APP_CL_FILES.files = $$GCAppPath
    APP_CL_FILES.path = Contents/Resources/
    QMAKE_BUNDLE_DATA += APP_CL_FILES

}

SOURCES += main.cpp\
        GenMyoWindow.cpp \
    MyoManager.cpp \
    RecordingDialog.cpp \
    FlagsDialog.cpp \
    MyoTime.cpp \
    GesturesBuilder.cpp \
    MyoDialog.cpp \
    MyoDrawFrame.cpp \
    qcustomplot/qcustomplot.cpp \
    SamplingList.cpp \
    ClassForm.cpp \
    SampleForm.cpp \
    ModelForm.cpp

HEADERS  += GenMyoWindow.h \
    Utilities.h \
    WekaOuput.h \
    MyoListener.h \
    MyoManager.h \
    RecordingDialog.h \
    FlagsDialog.h \
    DataFlags.h \
    WekaRows.h \
    MyoDataOuput.h \
    FANNOuput.h \
    MyoDataInput.h \
    MyoSerialize.h \
    MyoTime.h \
    GesturesBuilder.h \
    MyoDialog.h \
    MyoDrawFrame.h \
    qcustomplot/qcustomplot.h \
    SamplingList.h \
    ClassForm.h \
    SampleForm.h \
    MyoData.h \
    DataSetOutput.h \
    ModelForm.h

FORMS    += GenMyoWindow.ui \
    RecordingDialog.ui \
    FlagsDialog.ui \
    MyoDialog.ui \
    SamplingList.ui \
    ClassForm.ui \
    SampleForm.ui \
    ModelForm.ui

#redources
RESOURCES += \
    qdarkstyle/style.qrc \
    assets/assets.qrc
