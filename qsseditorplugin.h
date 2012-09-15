#ifndef QSSEDITORPLUGIN_H
#define QSSEDITORPLUGIN_H

#include "qsseditor_global.h"
#include "qsseditor.h"
#include <extensionsystem/iplugin.h>
#include <texteditor/texteditoractionhandler.h>

namespace QssEditor {
namespace Internal {

class QssEditorFactory;
class QssEditorPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "QssEditor.json")
    
public:
    QssEditorPlugin();
    ~QssEditorPlugin();
    
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

    static QssEditorPlugin *instance()
    { return m_instance; }

    void initializeEditor(QSSTextEditorWidget *editor);

private slots:
    void triggerAction();

private:
    static QssEditorPlugin *m_instance;
    QssEditorFactory *m_editor;
    TextEditor::TextEditorActionHandler *m_actionHandler;
};

} // namespace Internal
} // namespace QssEditor

#endif // QSSEDITORPLUGIN_H

