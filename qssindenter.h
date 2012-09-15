#ifndef QSSINDENTER_H
#define QSSINDENTER_H

#include <texteditor/indenter.h>

namespace QssEditor {
namespace Internal {
class QSSIndenter : public TextEditor::Indenter
{
public:
    QSSIndenter();

    bool isElectricCharacter(const QChar &ch) const;
    void indentBlock(QTextDocument *doc, const QTextBlock &block, const QChar &typedChar, const TextEditor::TabSettings &tabSettings);
    void setCodeStylePreferences(TextEditor::ICodeStylePreferences *preferences);
    void invalidateCache(QTextDocument *doc);

private:
    int getIndentDepth(QTextDocument *doc, const QTextBlock &block, const QChar &typedChar);
};
} // namespace Internal
} // namespace QssEditor

#endif // QSSINDENTER_H
