#include "qsseditoreditable.h"
#include "qsseditor.h"
#include "qsseditorplugin.h"
#include "qsseditorconstants.h"

#include <texteditor/texteditorconstants.h>

#include <coreplugin/mimedatabase.h>
#include <coreplugin/icore.h>
#include <coreplugin/designmode.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/coreconstants.h>

namespace QssEditor {
namespace Internal {

QssEditorEditable::QssEditorEditable(QSSTextEditorWidget *editor)
    : BaseTextEditor(editor)
{
    setContext(Core::Context(QssEditor::Constants::C_QSSEDITOR_ID,
                             TextEditor::Constants::C_TEXTEDITOR));
}

Core::IEditor *QssEditorEditable::duplicate(QWidget *parent)
{
    QSSTextEditorWidget *newEditor = new QSSTextEditorWidget(parent);
    newEditor->duplicateFrom(editorWidget());
    QssEditorPlugin::instance()->initializeEditor(newEditor);
    return newEditor->editor();
}

Core::Id QssEditorEditable::id() const
{
    return QssEditor::Constants::C_QSSEDITOR_ID;
}

Core::Id QssEditorEditable::preferredModeType() const
{
    return Core::Id();
}

} // namespace Internal
} // namespace QssEditor
