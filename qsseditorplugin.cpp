#include "qsseditorplugin.h"
#include "qsseditorconstants.h"
#include "qsseditor.h"
#include "qsseditorfactory.h"
#include "qsscompletionassist.h"
#include "qsshoverhandler.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/mimedatabase.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <texteditor/basetexteditor.h>
#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditorsettings.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

using namespace QssEditor;
using namespace QssEditor::Internal;

QssEditorPlugin *QssEditorPlugin::m_instance = 0;

QssEditorPlugin::QssEditorPlugin() :
    ExtensionSystem::IPlugin(),
    m_editor(0),
    m_actionHandler(0)
{
    // Create your members
    m_instance = this;
}

QssEditorPlugin::~QssEditorPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
    removeObject(m_editor);
    m_instance = 0;
}

bool QssEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize method, a plugin can be sure that the plugins it
    // depends on have initialized their members.
    
    Q_UNUSED(arguments)

    if (!Core::ICore::mimeDatabase()->addMimeTypes(QLatin1String(":/qsseditor/QssEditor.mimetypes.xml"), errorString))
        return false;

    m_editor = new QssEditorFactory(this);
    addObject(m_editor);

    addAutoReleasedObject(new QssHoverHandler);
    addAutoReleasedObject(new QssCompletionAssistProvider);

    m_actionHandler = new TextEditor::TextEditorActionHandler(QssEditor::Constants::C_QSSEDITOR_ID,
                                                              TextEditor::TextEditorActionHandler::Format);
    m_actionHandler->initializeActions();

    QAction *action = new QAction(tr("QssEditor action"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+A")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));
    
    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("QssEditor"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);
    
    return true;
}

void QssEditorPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized method, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag QssEditorPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void QssEditorPlugin::initializeEditor(QSSTextEditorWidget *editor)
{
    m_actionHandler->setupActions(editor);

    TextEditor::TextEditorSettings::instance()->initializeEditor(editor);
}

void QssEditorPlugin::triggerAction()
{
    QMessageBox::information(Core::ICore::mainWindow(),
                             tr("Action triggered"),
                             tr("This is an action from QssEditor."));
}

Q_EXPORT_PLUGIN2(QssEditor, QssEditorPlugin)

