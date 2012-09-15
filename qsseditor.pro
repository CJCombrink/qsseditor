TARGET = QssEditor
TEMPLATE = lib

DEFINES += QSSEDITOR_LIBRARY

# QssEditor files

SOURCES += qsseditorplugin.cpp \
    qsseditorfactory.cpp \
    qsseditor.cpp \
    qsseditoreditable.cpp \
    qssindenter.cpp \
    qssautocompleter.cpp \
    qsshighlighter.cpp \
    qsscompletionassist.cpp \
    qsseditorutils.cpp \
    qsshoverhandler.cpp \
    gradienttipwidget.cpp

HEADERS += qsseditorplugin.h\
    qsseditor_global.h\
    qsseditorconstants.h \
    qsseditorfactory.h \
    qsseditor.h \
    qsseditoreditable.h \
    qssindenter.h \
    qssautocompleter.h \
    qsshighlighter.h \
    qsscompletionassist.h \
    qsseditorutils.h \
    qsshoverhandler.h \
    gradienttipwidget.h

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=E:/Projects/qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE) {
    CONFIG(release, debug|release):IDE_BUILD_TREE=E:/Projects/qt-creator-build-release
    else:CONFIG(debug, debug|release):IDE_BUILD_TREE=E:/Projects/qt-creator-build-debug
}

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\Nokia\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/Nokia/qtcreator" or "~/.local/share/data/Nokia/qtcreator" on Linux
##    "~/Library/Application Support/Nokia/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

PROVIDER = WangHoi

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
include(qsseditor_dependencies.pri)

RESOURCES += \
    qsseditor.qrc

OTHER_FILES += \
    QssEditor.mimetypes.xml \
    qsseditor_dependencies.pri

HEADERS += \
    qssparser.h \
    qsslexer.h

SOURCES += \
    qssparser.cpp \
    qsslexer.cpp

