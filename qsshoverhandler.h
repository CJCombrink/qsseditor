#ifndef QSSHOVERHANDLER_H
#define QSSHOVERHANDLER_H

#include "qsseditor_global.h"
#include "qsslexer.h"
#include "qssparser.h"
#include <texteditor/itexteditor.h>
#include <texteditor/basehoverhandler.h>
#include <QColor>
#include <QString>

class QGradient;
namespace QssEditor {
namespace Internal {
class GradientTipWidget;
class QSSEDITOR_EXPORT QssHoverHandler : public TextEditor::BaseHoverHandler
{
public:
    QssHoverHandler(QObject *parent = 0);
    ~QssHoverHandler();
    bool acceptEditor(Core::IEditor *editor);
    void identifyMatch(TextEditor::ITextEditor *editor, int pos);
    void operateTooltip(TextEditor::ITextEditor *editor, const QPoint &point);

private:
    int findStartOfName(int pos) const;
    bool matchColor(Parser::State parserState,
                    const Token &prevToken,
                    const QString &prevString,
                    const Token &currentToken,
                    const QString &currentString);
    bool matchGradient(Parser::State parserState,
                    const Token &prevToken,
                    const QString &prevString,
                    const Token &currentToken,
                    const QString &currentString);
    bool mergeGradientTokens(QList<Token> &gradientList, const QList<Token> &tokenList);

    void purgeTooltips();

    TextEditor::ITextEditor *m_editor;
    int m_pos;
    QColor m_color;
    QString m_text;
    QGradient *m_gradient;
    QList<GradientTipWidget *> m_tooltipWidgets;
};
} // namespace Internal
} // namespace QssEditor

#endif // QSSHOVERHANDLER_H
