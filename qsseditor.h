#ifndef QSSEDITOR_H
#define QSSEDITOR_H

#include "qsseditor_global.h"

#include <texteditor/basetexteditor.h>

#include <QSharedPointer>
#include <QSet>

QT_BEGIN_NAMESPACE
class QComboBox;
class QTimer;
QT_END_NAMESPACE

namespace Core {
class ICore;
}

namespace QssEditor {
namespace Internal {
class QssEditorEditable;
}

class QSSEDITOR_EXPORT Document
{
public:
    typedef QSharedPointer<Document> Ptr;

    Document();
    ~Document();

private:
    friend class QSSTextEditorWidget;
};

class QSSEDITOR_EXPORT QSSTextEditorWidget : public TextEditor::BaseTextEditorWidget
{
    Q_OBJECT

public:
    QSSTextEditorWidget(QWidget *parent = 0);
    ~QSSTextEditorWidget();

public slots:
    virtual void setFontSettings(const TextEditor::FontSettings &);

protected:
    bool event(QEvent *e);
    TextEditor::BaseTextEditor *createEditor();

private:
};

} // namespace QssEditor

#endif // QSSEDITOR_H
