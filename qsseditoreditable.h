#ifndef QSSEDITOREDITABLE_H
#define QSSEDITOREDITABLE_H

#include <texteditor/basetexteditor.h>

namespace QssEditor {
class QSSTextEditorWidget;

namespace Internal {

class QssEditorEditable : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    explicit QssEditorEditable(QSSTextEditorWidget *);

    bool duplicateSupported() const { return true; }
    Core::IEditor *duplicate(QWidget *parent);
    Core::Id id() const;
    bool isTemporary() const { return false; }
    bool open(QString *errorString, const QString &fileName, const QString &realFileName);
    Core::Id preferredModeType() const;
};

} // namespace Internal
} // namespace QssEditor

#endif // QSSEDITOREDITABLE_H
