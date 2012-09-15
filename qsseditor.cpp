#include "qsseditor.h"
#include "qsseditorconstants.h"
#include "qsseditoreditable.h"
#include "qssautocompleter.h"
#include "qssindenter.h"
#include "qsseditorplugin.h"
#include "qsshighlighter.h"

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/id.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/basetextdocument.h>
#include <texteditor/fontsettings.h>
#include <texteditor/tabsettings.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/texteditorsettings.h>
#include <texteditor/syntaxhighlighter.h>
#include <texteditor/refactoroverlay.h>
#include <texteditor/tooltip/tooltip.h>

#include <QFileInfo>
#include <QSignalMapper>
#include <QTimer>
#include <QDebug>
#include <QVector>
#include <QMenu>
#include <QComboBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QToolBar>
#include <QTreeView>

using namespace QssEditor;
using namespace QssEditor::Internal;

Document::Document()
{

}

Document::~Document()
{
}

QSSTextEditorWidget::QSSTextEditorWidget(QWidget *parent) :
    TextEditor::BaseTextEditorWidget(parent)
{
    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);
    setAutoCompleter(new QSSAutoCompleter());
    setIndenter(new QSSIndenter());
    baseTextDocument()->setSyntaxHighlighter(new Highlighter());
}

QSSTextEditorWidget::~QSSTextEditorWidget()
{
}

bool QssEditorEditable::open(QString *errorString, const QString &fileName, const QString &realFileName)
{
    editorWidget()->setMimeType(Core::ICore::mimeDatabase()->findByFile(QFileInfo(fileName)).type());
    bool b = TextEditor::BaseTextEditor::open(errorString, fileName, realFileName);
    return b;
}

void QSSTextEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
    TextEditor::BaseTextEditorWidget::setFontSettings(fs);
    Highlighter *highlighter = qobject_cast<Highlighter*>(baseTextDocument()->syntaxHighlighter());
    if (!highlighter)
        return;

    static QVector<TextEditor::TextStyle> categories;
    if (categories.isEmpty()) {
        categories << TextEditor::C_TYPE    // object
                << TextEditor::C_TYPE       // pseudo state
                << TextEditor::C_TYPE       // sub control
                << TextEditor::C_KEYWORD    // attribute name
                << TextEditor::C_LOCAL      // identifier
                << TextEditor::C_STRING     // string
                << TextEditor::C_NUMBER     // number
                << TextEditor::C_COMMENT    // comment
                << TextEditor::C_VISUAL_WHITESPACE;
    }

    const QVector<QTextCharFormat> formats = fs.toTextCharFormats(categories);
    highlighter->setFormats(formats.constBegin(), formats.constEnd());
    highlighter->rehighlight();
}

TextEditor::BaseTextEditor *QSSTextEditorWidget::createEditor()
{
    QssEditorEditable *editable = new QssEditorEditable(this);
    return editable;
}

bool QSSTextEditorWidget::event(QEvent *e)
{
    return BaseTextEditorWidget::event(e);
}
