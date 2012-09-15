#include "qsseditorfactory.h"
#include "qsseditor.h"
#include "qsseditorconstants.h"
#include "qsseditorplugin.h"

#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

#include <coreplugin/icore.h>
#include <coreplugin/editormanager/editormanager.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QPushButton>

using namespace QssEditor::Internal;
using namespace QssEditor::Constants;

QssEditorFactory::QssEditorFactory(QObject *parent)
  : Core::IEditorFactory(parent)
{
    m_mimeTypes << QLatin1String(QSS_MIMETYPE);
}

Core::Id QssEditorFactory::id() const
{
    return C_QSSEDITOR_ID;
}

QString QssEditorFactory::displayName() const
{
    return qApp->translate("OpenWith::Editors", C_QSSEDITOR_DISPLAY_NAME);
}

Core::IEditor *QssEditorFactory::createEditor(QWidget *parent)
{
    QssEditor::QSSTextEditorWidget *rc = new QssEditor::QSSTextEditorWidget(parent);
    QssEditorPlugin::instance()->initializeEditor(rc);
    return rc->editor();
}

QStringList QssEditorFactory::mimeTypes() const
{
    return m_mimeTypes;
}
