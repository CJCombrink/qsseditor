#include "qssparser.h"
#include <QLinearGradient>
#include <QRadialGradient>
#include <QConicalGradient>
#include <QDebug>
using namespace QssEditor::Internal;

Parser::Parser() :
    m_state(Parser::ObjectState)
{
}

Parser::State Parser::state() const
{
    return m_state;
}

void Parser::setState(Parser::State s)
{
    m_state = s;
}

void Parser::parse(const QList<Token> &tokenList, enum State state)
{
    m_state = state;
    Q_FOREACH (const Token &t, tokenList)
        parse(t, m_state);
}

void Parser::parse(const Token &t, enum State state)
{
    m_state = state;
    switch (m_state) {
    case ObjectState:
        if (t.kind == Token::Colon)
            m_state = PseudoStatesState;
        else if (t.kind == Token::DoubleColon)
            m_state = SubControlState;
        else if (t.kind == Token::LeftBrace)
            m_state = AttributeNameState;
        break;
    case PseudoStatesState:
        if (t.kind == Token::Colon)
            m_state = PseudoStatesState;
        else if (t.kind == Token::DoubleColon)
            m_state = SubControlState;
        else if (t.kind == Token::LeftBrace)
            m_state = AttributeNameState;
        else
            m_state = ObjectState;
        break;
    case SubControlState:
        if (t.kind == Token::Colon)
            m_state = PseudoStatesState;
        else if (t.kind == Token::DoubleColon)
            m_state = SubControlState;
        else if (t.kind == Token::LeftBrace)
            m_state = AttributeNameState;
        else
            m_state = ObjectState;
        break;
    case AttributeNameState:
        if (t.kind == Token::RightBrace)
            m_state = ObjectState;
        else if (t.kind == Token::Colon)
            m_state = AttributeBodyState;
        break;
    case AttributeBodyState:
        if (t.kind == Token::Semicolon)
            m_state = AttributeNameState;
        break;
    default:
        Q_ASSERT(0);
    }
}

void Parser::parseMore(const QList<Token> &tokenList)
{
    parse(tokenList, m_state);
}

void Parser::parseMore(const Token &t)
{
    parse(t, m_state);
}

QGradient *Parser::parseGradient(const QList<Token> &tokenList)
{
    if (tokenList.count() < 3)
        return 0;
    if (tokenList[0].isNot(Token::Identifier))
        return 0;
    if (tokenList[1].isNot(Token::LeftParenthesis))
        return 0;
    if (tokenList.last().isNot(Token::RightParenthesis))
        return 0;
    Token t = tokenList.first();
    if (t.text == "qlineargradient")
        return parseLinearGradient(tokenList);
    else if (t.text == "qradialgradient")
        return parseRadialGradient(tokenList);
    else if (t.text == "qconicalgradient")
        return parseConicalGradient(tokenList);

    return 0;
}

QColor Parser::parseColor(const QList<Token> &tokenList)
{
    if (tokenList.count() == 0)
        return QColor();
    QColor color;
    Token token;
    Token nextToken;
    Token num1;
    Token num2;
    Token num3;
    Token num4;
    int n4;
    token = tokenList.at(0);
    if (2 <= tokenList.count())
        nextToken = tokenList.at(1);
    else
        nextToken = Token();
    if (3 <= tokenList.count())
        num1 = tokenList.at(2);
    if (5 <= tokenList.count())
        num2 = tokenList.at(4);
    if (7 <= tokenList.count())
        num3 = tokenList.at(6);
    if (9 <= tokenList.count())
        num4 = tokenList.at(8);
    if (token.is(Token::Identifier)) {
        if (QColor::isValidColor(token.text))
            color.setNamedColor(token.text);
        else if (token.text == "rgb") {
            if (num1.is(Token::Number) && num2.is(Token::Number) && num3.is(Token::Number))
                color.setRgb(num1.text.toInt(), num2.text.toInt(), num3.text.toInt());
        } else if (token.text == "rgba") {
            if (num1.is(Token::Number) && num2.is(Token::Number) && num3.is(Token::Number) && num4.is(Token::Number)) {
                if (num4.text.endsWith('%'))
                    n4 = num4.text.left(num4.text.length() - 1).toInt();
                else
                    n4 = num4.text.toInt();
                color.setRgb(num1.text.toInt(), num2.text.toInt(), num3.text.toInt(), n4);
            }
        } else if (token.text == "hsv") {
            if (num1.is(Token::Number) && num2.is(Token::Number) && num3.is(Token::Number))
                color.setHsv(num1.text.toInt(), num2.text.toInt(), num3.text.toInt());
        } else if (token.text == "hsva") {
            if (num1.is(Token::Number) && num2.is(Token::Number) && num3.is(Token::Number) && num4.is(Token::Number)) {
                if (num4.text.endsWith('%'))
                    n4 = num4.text.left(num4.text.length() - 1).toInt();
                else
                    n4 = num4.text.toInt();
                color.setHsv(num1.text.toInt(), num2.text.toInt(), num3.text.toInt(), n4);
            }
        }
    } else if (token.is(Token::Sharp)
               && (nextToken.is(Token::Number) || nextToken.is(Token::Identifier))) {
        color.setNamedColor("#" + nextToken.text);
    }
    return color;
}

QLinearGradient *Parser::parseLinearGradient(const QList<Token> &tokenList)
{
    QLinearGradient *grad = new QLinearGradient();

    double x1 = 0.0;
    double y1 = 0.0;
    double x2 = 0.0;
    double y2 = 0.0;
    QGradientStops stops;
    Token t;
    QList<Token> colorTokens;
    QColor color;
    // skip 'qlineargradient' '('
    int i = 2;
    while (i < tokenList.count()) {
        t = tokenList[i];
//        qDebug() << "linear:" << t.text;
        if (t.is(Token::Identifier)) {
            if (t.text == "x1") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    x1 = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "y1") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    y1 = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "x2") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    x2 = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "y2") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    y2 = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "stop") {
                // skip 'stop' ':'
                i += 2;
                if (i >= tokenList.count())
                    goto _LerrOut;
                t = tokenList[i];
                if (t.isNot(Token::Number))
                    goto _LerrOut;
                i++;
                colorTokens = getColorTokens(tokenList, i);
//                qDebug() << colorTokens.count();
                if (colorTokens.isEmpty())
                    goto _LerrOut;
                color = parseColor(colorTokens);
                if (!color.isValid())
                    goto _LerrOut;
                stops << QGradientStop(t.text.toDouble(), color);
            } else {
                i++;
            }
        } else {
            // just skip
            i++;
        }
    }
    grad->setStart(x1, y1);
    grad->setFinalStop(x2, y2);
    grad->setStops(stops);
    return grad;
_LerrOut:
    delete grad;
    return 0;
}

QRadialGradient *Parser::parseRadialGradient(const QList<Token> &tokenList)
{
    QRadialGradient *grad = new QRadialGradient();

    double cx = 0.0;
    double cy = 0.0;
    double radius = 0.0;
    double fx = 0.0;
    double fy = 0.0;
    QGradientStops stops;
    Token t;
    QList<Token> colorTokens;
    QColor color;
    // skip 'qlineargradient' '('
    int i = 2;
    while (i < tokenList.count()) {
        t = tokenList[i];
//        qDebug() << "linear:" << t.text;
        if (t.is(Token::Identifier)) {
            if (t.text == "cx") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    cx = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "cy") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    cy = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "fx") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    fx = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "fy") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    fy = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "radius") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    radius = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "stop") {
                // skip 'stop' ':'
                i += 2;
                if (i >= tokenList.count())
                    goto _LerrOut;
                t = tokenList[i];
                if (t.isNot(Token::Number))
                    goto _LerrOut;
                i++;
                colorTokens = getColorTokens(tokenList, i);
//                qDebug() << colorTokens.count();
                if (colorTokens.isEmpty())
                    goto _LerrOut;
                color = parseColor(colorTokens);
                if (!color.isValid())
                    goto _LerrOut;
                stops << QGradientStop(t.text.toDouble(), color);
            } else {
                i++;
            }
        } else {
            // just skip
            i++;
        }
    }
    grad->setCenter(cx, cy);
    grad->setFocalPoint(fx, fy);
    grad->setCenterRadius(radius);
    grad->setStops(stops);
    return grad;
_LerrOut:
    delete grad;
    return 0;
}

QConicalGradient *Parser::parseConicalGradient(const QList<Token> &tokenList)
{
    QConicalGradient *grad = new QConicalGradient();

    double cx = 0.0;
    double cy = 0.0;
    double angle = 0.0;
    QGradientStops stops;
    Token t;
    QList<Token> colorTokens;
    QColor color;
    // skip 'qlineargradient' '('
    int i = 2;
    while (i < tokenList.count()) {
        t = tokenList[i];
//        qDebug() << "linear:" << t.text;
        if (t.is(Token::Identifier)) {
            if (t.text == "cx") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    cx = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "cy") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    cy = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "angle") {
                if (i + 2 < tokenList.count()) {
                    t = tokenList[i + 2];
                    angle = t.text.toDouble();
                    i += 3;
                } else
                    goto _LerrOut;
            } else if (t.text == "stop") {
                // skip 'stop' ':'
                i += 2;
                if (i >= tokenList.count())
                    goto _LerrOut;
                t = tokenList[i];
                if (t.isNot(Token::Number))
                    goto _LerrOut;
                i++;
                colorTokens = getColorTokens(tokenList, i);
//                qDebug() << colorTokens.count();
                if (colorTokens.isEmpty())
                    goto _LerrOut;
                color = parseColor(colorTokens);
                if (!color.isValid())
                    goto _LerrOut;
                stops << QGradientStop(t.text.toDouble(), color);
            } else {
                i++;
            }
        } else {
            // just skip
            i++;
        }
    }
    grad->setCenter(cx, cy);
    grad->setAngle(angle);
    grad->setStops(stops);
    return grad;
_LerrOut:
    delete grad;
    return 0;

}

QList<Token> Parser::getColorTokens(const QList<Token> &tokenList, int &i)
{
    QList<Token> colorTokens;
    Token token, nextToken;
    if (i < tokenList.count()) {
        token = tokenList.at(i);
        if (i + 1 < tokenList.count())
            nextToken = tokenList.at(i + 1);
        else
            nextToken = Token();
        if (token.is(Token::Identifier) && QColor::isValidColor(token.text)) {
            colorTokens << token;
            i++;
        } else if (token.is(Token::Sharp)
                   && (nextToken.is(Token::Number) || nextToken.is(Token::Identifier))
                   && QColor::isValidColor("#" + nextToken.text)) {
            colorTokens << token << nextToken;
            i += 2;
        } else if (token.is(Token::Identifier) &&
                   (token.text == "rgb" || token.text == "rgba" || token.text == "hsv" || token.text == "hsva") &&
                   nextToken.is(Token::LeftParenthesis)) {
            colorTokens << token << nextToken;
            i += 2;
            while (i < tokenList.count()) {
                colorTokens << tokenList.at(i);
                if (tokenList.at(i).is(Token::RightParenthesis))
                    break;
                i++;
            }
        }
    }
    return colorTokens;
}
