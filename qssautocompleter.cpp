#include "qssautocompleter.h"
#include <QTextBlock>
#include <QTextCursor>

QSSAutoCompleter::QSSAutoCompleter() :
    TextEditor::AutoCompleter()
{
}

bool QSSAutoCompleter::contextAllowsElectricCharacters(const QTextCursor &cursor) const
{
    return true;
}

