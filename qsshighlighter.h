#ifndef QSSHIGHLIGHTER_H
#define QSSHIGHLIGHTER_H

#include <texteditor/syntaxhighlighter.h>
#include <QtAlgorithms>
#include <QTextCharFormat>

namespace QssEditor {
namespace Internal {

class Highlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT
public:
    enum Format {
        ObjectFormat,
        PseudoStateFormat,
        SubControlFormat,
        AttributeNameFormat,
        IdentifilerFormat,
        StringFormat,
        NumberFormat,
        CommentFormat,
        VisualWhiteSpaceFormat,
        NumFormats
    };
    Highlighter(QTextDocument *document = 0);

    void highlightBlock(const QString &text);
    // Set formats from a sequence of type QTextCharFormat
    template <class InputIterator>
    void setFormats(InputIterator begin, InputIterator end) {
        qCopy(begin, end, m_formats);
    }

private:
    QTextCharFormat m_formats[NumFormats];
};

} // namespace Internal
} // namespace QssEditor

#endif // QSSHIGHLIGHTER_H
