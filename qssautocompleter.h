#ifndef QSSAUTOCOMPLETER_H
#define QSSAUTOCOMPLETER_H

#include <texteditor/autocompleter.h>

class QSSAutoCompleter : public TextEditor::AutoCompleter
{
public:
    QSSAutoCompleter();
    bool contextAllowsElectricCharacters(const QTextCursor &cursor) const;
};

#endif // QSSAUTOCOMPLETER_H
