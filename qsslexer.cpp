#include "qsslexer.h"
#include <QMetaEnum>
#include <QMap>

using namespace QssEditor::Internal;

namespace QssEditor {
namespace Internal {
QString QSS_OBJECTS[] = {
    QLatin1String("QAbstractScrollArea"),
    QLatin1String("QColumnView"),
    QLatin1String("QComboBox"),
    QLatin1String("QDateEdit"),
    QLatin1String("QDateTimeEdit"),
    QLatin1String("QDialog"),
    QLatin1String("QDialogButtonBox"),
    QLatin1String("QDockWidget"),
    QLatin1String("QDoubleSpinBox"),
    QLatin1String("QFrame"),
    QLatin1String("QGroupBox"),
    QLatin1String("QHeaderView"),
    QLatin1String("QLineEdit"),
    QLatin1String("QListView"),
    QLatin1String("QListWidget"),
    QLatin1String("QMainWindow"),
    QLatin1String("QMenu"),
    QLatin1String("QMenuBar"),
    QLatin1String("QMessageBox"),
    QLatin1String("QProgressBar"),
    QLatin1String("QPushButton"),
    QLatin1String("QRadioButton"),
    QLatin1String("QScrollBar"),
    QLatin1String("QSizeGrip"),
    QLatin1String("QSlider"),
    QLatin1String("QSpinBox"),
    QLatin1String("QSplitter"),
    QLatin1String("QStatusBar"),
    QLatin1String("QTabBar"),
    QLatin1String("QTabWidget"),
    QLatin1String("QTableView"),
    QLatin1String("QTableWidget"),
    QLatin1String("QTextEdit"),
    QLatin1String("QTimeEdit"),
    QLatin1String("QToolBar"),
    QLatin1String("QToolButton"),
    QLatin1String("QToolBox"),
    QLatin1String("QToolTip"),
    QLatin1String("QTreeView"),
    QLatin1String("QTreeWidget"),
    QLatin1String("QWidget")
};

QString QSS_ATTRIBUTE_NAMES[] = {
    QLatin1String("alternate-background-color"),
    QLatin1String("background"),
    QLatin1String("background-color"),
    QLatin1String("background-image"),
    QLatin1String("background-repeat"),
    QLatin1String("background-position"),
    QLatin1String("background-attachment"),
    QLatin1String("background-clip"),
    QLatin1String("background-origin"),
    QLatin1String("border"),
    QLatin1String("border-top"),
    QLatin1String("border-right"),
    QLatin1String("border-bottom"),
    QLatin1String("border-left"),
    QLatin1String("border-color"),
    QLatin1String("border-top-color"),
    QLatin1String("border-right-color"),
    QLatin1String("border-bottom-color"),
    QLatin1String("border-left-color"),
    QLatin1String("border-image"),
    QLatin1String("border-radius"),
    QLatin1String("border-top-left-radius"),
    QLatin1String("border-top-right-radius"),
    QLatin1String("border-bottom-right-radius"),
    QLatin1String("border-bottom-left-radius"),
    QLatin1String("border-style"),
    QLatin1String("border-top-style"),
    QLatin1String("border-right-style"),
    QLatin1String("border-bottom-style"),
    QLatin1String("border-left-style"),
    QLatin1String("border-width"),
    QLatin1String("border-top-width"),
    QLatin1String("border-right-width"),
    QLatin1String("border-bottom-width"),
    QLatin1String("border-left-width"),
    QLatin1String("bottom"),
    QLatin1String("button-layout"),
    QLatin1String("color"),
    QLatin1String("dialogbuttonbox-buttons-have-icons"),
    QLatin1String("font"),
    QLatin1String("font-family"),
    QLatin1String("font-size"),
    QLatin1String("font-style"),
    QLatin1String("font-weight"),
    QLatin1String("gridline-color"),
    QLatin1String("height"),
    QLatin1String("icon-size"),
    QLatin1String("image"),
    QLatin1String("image-position"),
    QLatin1String("left"),
    QLatin1String("lineedit-password-character"),
    QLatin1String("margin"),
    QLatin1String("margin-top"),
    QLatin1String("margin-right"),
    QLatin1String("margin-bottom"),
    QLatin1String("margin-left"),
    QLatin1String("max-height"),
    QLatin1String("max-width"),
    QLatin1String("messagebox-text-interaction-flags"),
    QLatin1String("min-height"),
    QLatin1String("min-width"),
    QLatin1String("opacity"),
    QLatin1String("padding"),
    QLatin1String("padding-top"),
    QLatin1String("padding-right"),
    QLatin1String("padding-bottom"),
    QLatin1String("padding-left"),
    QLatin1String("paint-alternating-row-colors-for-empty-area"),
    QLatin1String("position"),
    QLatin1String("right"),
    QLatin1String("selection-background-color"),
    QLatin1String("selection-color"),
    QLatin1String("show-decoration-selected"),
    QLatin1String("spacing"),
    QLatin1String("subcontrol-origin"),
    QLatin1String("subcontrol-position"),
    QLatin1String("text-align"),
    QLatin1String("text-decoration"),
    QLatin1String("top"),
    QLatin1String("width"),
    // icons
    QLatin1String("backward-icon"),
    QLatin1String("cd-icon"),
    QLatin1String("computer-icon"),
    QLatin1String("desktop-icon"),
    QLatin1String("dialog-apply-icon"),
    QLatin1String("dialog-cancel-icon"),
    QLatin1String("dialog-close-icon"),
    QLatin1String("dialog-discard-icon"),
    QLatin1String("dialog-help-icon"),
    QLatin1String("dialog-no-icon"),
    QLatin1String("dialog-ok-icon"),
    QLatin1String("dialog-open-icon"),
    QLatin1String("dialog-reset-icon"),
    QLatin1String("dialog-save-icon"),
    QLatin1String("dialog-yes-icon"),
    QLatin1String("directory-close-icon"),
    QLatin1String("directory-icon"),
    QLatin1String("directory-link-icon"),
    QLatin1String("directory-open-icon"),
    QLatin1String("dockwidget-close-icon"),
    QLatin1String("downarrow-icon"),
    QLatin1String("dvd-icon"),
    QLatin1String("file-icon"),
    QLatin1String("file-link-icon"),
    QLatin1String("filedialog-contentsview-icon"),
    QLatin1String("filedialog-detailedview-icon"),
    QLatin1String("filedialog-end-icon"),
    QLatin1String("filedialog-infoview-icon"),
    QLatin1String("filedialog-listview-icon"),
    QLatin1String("filedialog-new-directory-icon"),
    QLatin1String("filedialog-parent-directory-icon"),
    QLatin1String("filedialog-start-icon"),
    QLatin1String("floppy-icon"),
    QLatin1String("forward-icon"),
    QLatin1String("harddisk-icon"),
    QLatin1String("home-icon"),
    QLatin1String("leftarrow-icon"),
    QLatin1String("messagebox-critical-icon"),
    QLatin1String("messagebox-information-icon"),
    QLatin1String("messagebox-question-icon"),
    QLatin1String("messagebox-warning-icon"),
    QLatin1String("network-icon"),
    QLatin1String("rightarrow-icon"),
    QLatin1String("titlebar-contexthelp-icon"),
    QLatin1String("titlebar-maximize-icon"),
    QLatin1String("titlebar-menu-icon"),
    QLatin1String("titlebar-minimize-icon"),
    QLatin1String("titlebar-normal-icon"),
    QLatin1String("titlebar-shade-icon"),
    QLatin1String("titlebar-unshade-icon"),
    QLatin1String("trash-icon"),
    QLatin1String("uparrow-icon")
};

QString QSS_ATTRIBUTE_CTORS[] = {
    QLatin1String("qlineargradient"),
    QLatin1String("qradialgradient"),
    QLatin1String("qconicalgradient"),
    QLatin1String("url")
};

QString QSS_ATTRIBUTE_KEYWORDS[] = {
    QLatin1String("x1"),
    QLatin1String("y1"),
    QLatin1String("x2"),
    QLatin1String("y2"),
    QLatin1String("stop"),
    QLatin1String("top"),
    QLatin1String("right"),
    QLatin1String("bottom"),
    QLatin1String("left"),
    QLatin1String("scroll"),
    QLatin1String("fixed"),
    QLatin1String("none"),
    QLatin1String("stretch"),
    QLatin1String("repeat"),
    QLatin1String("dashed"),
    QLatin1String("dot-dash"),
    QLatin1String("dot-dot-dash"),
    QLatin1String("dotted"),
    QLatin1String("double"),
    QLatin1String("groove"),
    QLatin1String("inset"),
    QLatin1String("outset"),
    QLatin1String("ridge"),
    QLatin1String("solid"),
    QLatin1String("none"),
    QLatin1String("rgb"),
    QLatin1String("rgba"),
    QLatin1String("hsv"),
    QLatin1String("hsva"),
    QLatin1String("normal"),
    QLatin1String("italic"),
    QLatin1String("oblique"),
    QLatin1String("disabled"),
    QLatin1String("active"),
    QLatin1String("selected"),
    QLatin1String("margin"),
    QLatin1String("border"),
    QLatin1String("padding"),
    QLatin1String("content"),
    QLatin1String("alternate-base"),
    QLatin1String("base"),
    QLatin1String("bright-text"),
    QLatin1String("button"),
    QLatin1String("button-text"),
    QLatin1String("dark"),
    QLatin1String("highlight"),
    QLatin1String("highlighted-text"),
    QLatin1String("light"),
    QLatin1String("link"),
    QLatin1String("link-visited"),
    QLatin1String("mid"),
    QLatin1String("midlight"),
    QLatin1String("shadow"),
    QLatin1String("text"),
    QLatin1String("window"),
    QLatin1String("window-text"),
    QLatin1String("repeat"),
    QLatin1String("repeat-x"),
    QLatin1String("repeat-y"),
    QLatin1String("no-repeat")
};

QString QSS_PSEUDO_STATES[] = {
    QLatin1String("active"),
    QLatin1String("adjoins-item"),
    QLatin1String("alternate"),
    QLatin1String("bottom"),
    QLatin1String("checkable"),
    QLatin1String("closable"),
    QLatin1String("closed"),
    QLatin1String("default"),
    QLatin1String("disabled"),
    QLatin1String("editable"),
    QLatin1String("edit-focus"),
    QLatin1String("enabled"),
    QLatin1String("exclusive"),
    QLatin1String("first"),
    QLatin1String("flat"),
    QLatin1String("floatable"),
    QLatin1String("focus"),
    QLatin1String("has-children"),
    QLatin1String("has-siblings"),
    QLatin1String("horizontal"),
    QLatin1String("hover"),
    QLatin1String("indeterminate"),
    QLatin1String("last"),
    QLatin1String("left"),
    QLatin1String("maximized"),
    QLatin1String("middle"),
    QLatin1String("minimized"),
    QLatin1String("movable"),
    QLatin1String("no-frame"),
    QLatin1String("non-exclusive"),
    QLatin1String("off"),
    QLatin1String("on"),
    QLatin1String("only-one"),
    QLatin1String("open"),
    QLatin1String("next-selected"),
    QLatin1String("pressed"),
    QLatin1String("previous-selected"),
    QLatin1String("read-only"),
    QLatin1String("right"),
    QLatin1String("selected"),
    QLatin1String("top"),
    QLatin1String("unchecked"),
    QLatin1String("vertical"),
    QLatin1String("window")
};

QString QSS_SUBCONTROLS[] = {
    QLatin1String("add-line"),
    QLatin1String("add-page"),
    QLatin1String("branch"),
    QLatin1String("chunk"),
    QLatin1String("close-button"),
    QLatin1String("corner"),
    QLatin1String("down-arrow"),
    QLatin1String("down-button"),
    QLatin1String("drop-down"),
    QLatin1String("float-button"),
    QLatin1String("groove"),
    QLatin1String("indicator"),
    QLatin1String("handle"),
    QLatin1String("icon"),
    QLatin1String("item"),
    QLatin1String("left-arrow"),
    QLatin1String("left-corner"),
    QLatin1String("menu-arrow"),
    QLatin1String("menu-button"),
    QLatin1String("menu-indicator"),
    QLatin1String("right-arrow"),
    QLatin1String("pane"),
    QLatin1String("right-corner"),
    QLatin1String("scroller"),
    QLatin1String("section"),
    QLatin1String("separator"),
    QLatin1String("sub-line"),
    QLatin1String("sub-page"),
    QLatin1String("tab"),
    QLatin1String("tab-bar"),
    QLatin1String("tear"),
    QLatin1String("tearoff"),
    QLatin1String("text"),
    QLatin1String("title"),
    QLatin1String("up-arrow"),
    QLatin1String("up-button")
};
} // namespace Internal
} // namespace QssEditor


namespace {
template <typename _Tp, int N>
const _Tp *begin(const _Tp (&a)[N])
{
    return a;
}

template <typename _Tp, int N>
const _Tp *end(const _Tp (&a)[N])
{
    return a + N;
}
} // end of anonymous namespace

QString Token::kindName() const
{
    QMetaEnum metaEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Kind"));
    return QString::fromLatin1(metaEnum.valueToKey(kind));
}

Lexer::Lexer() :
    m_state(Lexer::Normal),
    m_scanComments(false)
{
}

QList<Token> Lexer::scan(const QString &text, Lexer::States startState)
{
    m_state = startState;
    QList<Token> tokens;
    int index = 0;
    int start;
    if (multilineState() == Lexer::MultiLineComment) {
        start = -1;
        while ((index < text.length()) && text.at(index).isSpace())
            index++;
        if (index < text.length())
            start = index;
        while (index < text.length()) {
            QChar ch = text.at(index);
            QChar la;
            if (index + 1 < text.length())
                la = text.at(index + 1);
            if ((ch == '*') && (la == '/')) {
                m_state = Lexer::Normal;
                index += 2;
                break;
            }
            index++;
        }
        if ((start != -1) && m_scanComments)
            tokens << Token(start, index - start, Token::Comment, text.mid(start, index - start));
    } else {

    }
    if (m_state != Lexer::Normal)
        return tokens;
    while (index < text.length()) {
        QChar ch = text.at(index);
        QChar la;
        if (index + 1 < text.length())
            la = text.at(index + 1);
        QString id;
        switch (ch.unicode()) {
        case '/':
            if (la == '*') {
                setMultilineState(Lexer::MultiLineComment);
                start = index;
                index += 2;
                while (index < text.length()) {
                    ch = text.at(index);
                    if (index + 1 < text.length())
                        la = text.at(index + 1);
                    if ((ch == '*') && (la == '/')) {
                        m_state = Lexer::Normal;
                        index += 2;
                        break;
                    }
                    index++;
                }
                if (m_scanComments)
                    tokens << Token(start, index - start, Token::Comment, text.mid(start, index - start));
            } else {
                tokens << Token(index, 1, Token::Delimiter, text.mid(index, 1));
                index++;
            }
            break;
        case ',':
            tokens << Token(index, 1, Token::Comma, ",");
            index++;
            break;
        case '.':
            if (la.isDigit()) {
                start = index;
                do {
                    ++index;
                } while (index < text.length() && text.at(index).isLetterOrNumber());
                tokens.append(Token(start, index - start, Token::Number, text.mid(start, index - start)));
                break;
            } else {
                tokens << Token(index, 1, Token::Dot, ",");
                index++;
            }
            break;
        case ':':
            if (la == ':') {
                tokens << Token(index, 2, Token::DoubleColon, "::");
                index += 2;
            } else {
                tokens << Token(index, 1, Token::Colon, ":");
                index++;
            }
            break;
        case ';':
            tokens << Token(index, 1, Token::Semicolon, ";");
            index++;
            break;
        case '(':
            tokens << Token(index, 1, Token::LeftParenthesis, "(");
            index++;
            break;
        case ')':
            tokens << Token(index, 1, Token::RightParenthesis, ")");
            index++;
            break;
        case '{':
            tokens << Token(index, 1, Token::LeftBrace, "{");
            index++;
            break;
        case '}':
            tokens << Token(index, 1, Token::RightBrace, "}");
            index++;
            break;
        case '"':
            start = index;
            index++;
            while (index < text.length()) {
                ch = text.at(index);
                if (ch == '"') {
                    break;
                } else if ((ch == '\'') && (index + 1 < text.length())) {
                    index += 2;
                } else {
                    index++;
                }
            }
            if (index < text.length()) {
                index++;
            } else {
                setMultilineState(Lexer::MultiLineStringDQuote);
            }
            tokens << Token(start, index - start, Token::String, text.mid(start, index - start));
            break;
        case '#':
            tokens << Token(index, 1, Token::Sharp, "#");
            index++;
            break;
        default:
            if (ch.isSpace()) {
                do {
                    index++;
                } while (index < text.length() && text.at(index).isSpace());
            } else if (ch.isLetter()) {
                start = index;
                do {
                    index++;
                } while ((index < text.length()) && isIdentifierBody(text.at(index)));
                tokens << Token(start, index - start, Token::Identifier, text.mid(start, index - start));
            } else if (ch.isDigit()) {
                start = index;
                do {
                    index++;
                } while ((index < text.length()) && isNumber(text.at(index)));
                tokens << Token(start, index - start, Token::Number, text.mid(start, index - start));
            } else {
                tokens << Token(index, 1, Token::Delimiter, text.mid(index, 1));
                index++;
            }
        }
    }
    return tokens;
}

QList<Token> Lexer::scanMore(const QString &text)
{
    return scan(text, m_state);
}

bool Lexer::scanComments()
{
    return m_scanComments;
}

void Lexer::setScanComments(bool sc)
{
    m_scanComments = sc;
}

Lexer::States Lexer::state() const
{
    return m_state;
}

void Lexer::setState(Lexer::States s)
{
    m_state = s;
}

int Lexer::multilineState() const
{
    return m_state & Lexer::MultiLineMask;
}

void Lexer::setMultilineState(Lexer::States state)
{
    m_state &= !Lexer::MultiLineMask;
    m_state |= state & Lexer::MultiLineMask;
}

bool Lexer::isObject(const QString &text) const
{
    return objects().contains(text);
}

QStringList Lexer::objects()
{
    static QStringList wordList;
    if (wordList.isEmpty()) {
        for (const QString *word = begin(QSS_OBJECTS); word != end(QSS_OBJECTS); word++)
            wordList << *word;
    }
    return wordList;
}

bool Lexer::isPseudoState(const QString &text) const
{
    return pseudoStates().contains(text);
}

QStringList Lexer::pseudoStates()
{
    static QStringList wordList;
    if (wordList.isEmpty()) {
        for (const QString *word = begin(QSS_PSEUDO_STATES); word != end(QSS_PSEUDO_STATES); word++)
            wordList << *word;
    }
    return wordList;
}

bool Lexer::isSubControl(const QString &text) const
{
    return subControls().contains(text);
}

QStringList Lexer::subControls()
{
    static QStringList wordList;
    if (wordList.isEmpty()) {
        for (const QString *word = begin(QSS_SUBCONTROLS); word != end(QSS_SUBCONTROLS); word++)
            wordList << *word;
    }
    return wordList;
}

bool Lexer::isAttributeName(const QString &text) const
{
    return attributeNames().contains(text);
}

QStringList Lexer::attributeNames()
{
    static QStringList wordList;
    if (wordList.isEmpty()) {
        for (const QString *word = begin(QSS_ATTRIBUTE_NAMES); word != end(QSS_ATTRIBUTE_NAMES); word++)
            wordList << *word;
    }
    return wordList;
}

bool Lexer::isAttributeCtor(const QString &text) const
{
    return attributeCtors().contains(text);
}

QStringList Lexer::attributeCtors()
{
    static QStringList wordList;
    if (wordList.isEmpty()) {
        for (const QString *word = begin(QSS_ATTRIBUTE_CTORS); word != end(QSS_ATTRIBUTE_CTORS); word++)
            wordList << *word;
    }
    return wordList;
}

bool Lexer::isAttributeKeyword(const QString &text) const
{
    return attributeKeywords().contains(text);
}

QStringList Lexer::attributeKeywords()
{
    static QStringList wordList;
    if (wordList.isEmpty()) {
        for (const QString *word = begin(QSS_ATTRIBUTE_KEYWORDS); word != end(QSS_ATTRIBUTE_KEYWORDS); word++)
            wordList << *word;
    }
    return wordList;
}

/*
enum QSSProperty Lexer::propertyValue(const QString &text)
{
    return (enum QSSProperty)(qFind(begin(QSS_PROPERTIES), end(QSS_PROPERTIES), text) - QSS_PROPERTIES);
}

QString Lexer::propertyName(enum QSSProperty p)
{
    return QSS_PROPERTIES[(int)p];
}
*/

bool Lexer::isIdentifierBody(const QChar &c)
{
    return (c.isLetterOrNumber() || c == '_' || c == '-');
}

bool Lexer::isNumber(const QChar &c)
{
    return (c.isLetterOrNumber() || c == '.' || c == '%');
}
