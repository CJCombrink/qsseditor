#ifndef QSSPARSER_H
#define QSSPARSER_H

#include "qsslexer.h"
#include <QColor>
#include <QList>

class QGradient;
class QLinearGradient;
class QRadialGradient;
class QConicalGradient;
namespace QssEditor {
namespace Internal {
class Parser
{
public:
    enum State {
        ObjectState,
        PseudoStatesState,
        SubControlState,
        AttributeNameState,
        AttributeBodyState
    };
    Parser();
    State state() const;
    void setState(State s);
    void parse(const QList<Token> &tokenList, enum State state);
    void parse(const Token &t, enum State state);
    void parseMore(const QList<Token> &tokenList);
    void parseMore(const Token &t);
    QGradient *parseGradient(const QList<Token> &tokenList);
    QColor parseColor(const QList<Token> &tokenList);

private:
    QLinearGradient *parseLinearGradient(const QList<Token> &tokenList);
    QRadialGradient *parseRadialGradient(const QList<Token> &tokenList);
    QConicalGradient *parseConicalGradient(const QList<Token> &tokenList);
    QList<Token> getColorTokens(const QList<Token> &tokenList, int &i);

    enum State m_state;
};
} // namespace Internal
} // namespace QssEditor
#endif // QSSPARSER_H
