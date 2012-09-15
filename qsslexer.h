#ifndef QSSLEXER_H
#define QSSLEXER_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QFlags>
#include <QDebug>

namespace QssEditor {
namespace Internal {
enum QSSProperty {
    AlternateBackgroundColorProperty,
    BackgroundProperty,
    BackgroundColorProperty,
    BackgroundImageProperty,
    BackgroundRepeatProperty,
    BackgroundPositionProperty,
    BackgroundAttachmentProperty,
    BackgroundClipProperty,
    BackgroundOriginProperty,
    BorderProperty,
    BorderTopProperty,
    BorderRightProperty,
    BorderBottomProperty,
    BorderLeftProperty,
    BorderColorProperty,
    BorderTopColorProperty,
    BorderRightColorProperty,
    BorderBottomColorProperty,
    BorderLeftColorProperty,
    BorderImageProperty,
    BorderRadiusProperty,
    BorderTopLeftRadiusProperty,
    BorderTopRightRadiusProperty,
    BorderBottomRightRadiusProperty,
    BorderBottomLeftRadiusProperty,
    BorderStyleProperty,
    BorderTopStyleProperty,
    BorderRightStyleProperty,
    BorderBottomStyleProperty,
    BorderLeftStyleProperty,
    BorderWidthProperty,
    BorderTopWidthProperty,
    BorderRightWidthProperty,
    BorderBottomWidthProperty,
    BorderLeftWidthProperty,
    BottomProperty,
    ButtonLayoutProperty,
    ColorProperty,
    DialogbuttonboxButtonsHaveIconsProperty,
    FontProperty,
    FontFamilyProperty,
    FontSizeProperty,
    FontStyleProperty,
    FontWeightProperty,
    GridlineColorProperty,
    HeightProperty,
    IconSizeProperty,
    ImageProperty,
    ImagePositionProperty,
    LeftProperty,
    LineeditPasswordCharacterProperty,
    MarginProperty,
    MarginTopProperty,
    MarginRightProperty,
    MarginBottomProperty,
    MarginLeftProperty,
    MaxHeightProperty,
    MaxWidthProperty,
    MessageboxTextInteractionFlagsProperty,
    MinHeightProperty,
    MinWidthProperty,
    OpacityProperty,
    PaddingProperty,
    PaddingTopProperty,
    PaddingRightProperty,
    PaddingBottomProperty,
    PaddingLeftProperty,
    PaintAlternatingRowColorsForEmptyAreaProperty,
    PositionProperty,
    RightProperty,
    SelectionBackgroundColorProperty,
    SelectionColorProperty,
    ShowDecorationSelectedProperty,
    SpacingProperty,
    SubcontrolOriginProperty,
    SubcontrolPositionProperty,
    TextAlignProperty,
    TextDecorationProperty,
    TopProperty,
    WidthProperty,
    // Icons
    BackwardIconProperty,
    CdIconProperty,
    ComputerIconProperty,
    DesktopIconProperty,
    DialogApplyIconProperty,
    DialogCancelIconProperty,
    DialogCloseIconProperty,
    DialogDiscardIconProperty,
    DialogHelpIconProperty,
    DialogNoIconProperty,
    DialogOkIconProperty,
    DialogOpenIconProperty,
    DialogResetIconProperty,
    DialogSaveIconProperty,
    DialogYesIconProperty,
    DirectoryCloseIconProperty,
    DirectoryIconProperty,
    DirectoryLinkIconProperty,
    DirectoryOpenIconProperty,
    DockwidgetCloseIconProperty,
    DownarrowIconProperty,
    DvdIconProperty,
    FileIconProperty,
    FileLinkIconProperty,
    FiledialogContentsviewIconProperty,
    FiledialogDetailedviewIconProperty,
    FiledialogEndIconProperty,
    FiledialogInfoviewIconProperty,
    FiledialogListviewIconProperty,
    FiledialogNewDirectoryIconProperty,
    FiledialogParentDirectoryIconProperty,
    FiledialogStartIconProperty,
    FloppyIconProperty,
    ForwardIconProperty,
    HarddiskIconProperty,
    HomeIconProperty,
    LeftarrowIconProperty,
    MessageboxCriticalIconProperty,
    MessageboxInformationIconProperty,
    MessageboxQuestionIconProperty,
    MessageboxWarningIconProperty,
    NetworkIconProperty,
    RightarrowIconProperty,
    TitlebarContexthelpIconProperty,
    TitlebarMaximizeIconProperty,
    TitlebarMenuIconProperty,
    TitlebarMinimizeIconProperty,
    TitlebarNormalIconProperty,
    TitlebarShadeIconProperty,
    TitlebarUnshadeIconProperty,
    TrashIconProperty,
    UparrowIconProperty
};

/*
enum QSSPropertyType {
    AlignmentType,
    AttachmentType,
    BackgroundType,
    BooleanType,
    BorderType,
    BorderImageType,
    BorderStyleType,
    BoxColorsType,
    BoxLengthsType,
    BrushType,
    ColorType,
    FontType,
    FontSizeType,
    FontStyleType,
    FontWeightType,
    GradientType,
    IconType,
    LengthType,
    NumberType,
    OriginType,
    PaletteRoleType,
    RadiusType,
    RepeatType,
    StringType,
    UrlType
};
*/

class Token
{
    Q_GADGET
public:
    enum Kind {
        EndOfFile,
        Identifier,
        Object,
        PseudoState,
        SubControl,
        Attribute,
        String,
        Number,
        Comment,
        LeftParenthesis,
        RightParenthesis,
        LeftBrace,
        RightBrace,
        Semicolon,
        Colon,
        DoubleColon,
        Comma,
        Dot,
        Sharp,
        Delimiter
    };
    Q_ENUMS(Kind)

    inline Token(): offset(0), length(0), kind(EndOfFile), text(QString()) {}
    inline Token(int o, int l, Kind k, const QString &t): offset(o), length(l), kind(k), text(t) {}
    inline int begin() const { return offset; }
    inline int end() const { return offset + length; }
    inline bool is(int k) const { return k == kind; }
    inline bool isNot(int k) const { return k != kind; }
    QString kindName() const;
    bool operator ==(const Token &rhs) const { return ((offset == rhs.offset) && (length == rhs.length) && (kind == rhs.kind)); }
    bool operator !=(const Token &rhs) const { return offset != rhs.offset || length != rhs.length || kind != rhs.kind; }

public:
    int offset;
    int length;
    Kind kind;
    QString text;
};

class Lexer
{
    Q_GADGET
public:
    enum State {
        Normal = 0,
        MultiLineComment = 1,
        MultiLineStringDQuote = 2,
        MultiLineMask = 3
    };
    Q_DECLARE_FLAGS(States, State)
    Lexer();
    QList<Token> scan(const QString &text, Lexer::States startState);
    QList<Token> scanMore(const QString &text);

    bool scanComments();
    void setScanComments(bool sc);

    States state() const;
    void setState(States state);
    int multilineState() const;
    void setMultilineState(States state);

    bool isObject(const QString &text) const;
    static QStringList objects();

    bool isPseudoState(const QString &text) const;
    static QStringList pseudoStates();

    bool isSubControl(const QString &text) const;
    static QStringList subControls();

    bool isAttributeName(const QString &text) const;
    static QStringList attributeNames();

    bool isAttributeCtor(const QString &text) const;
    static QStringList attributeCtors();

    bool isAttributeKeyword(const QString &text) const;
    static QStringList attributeKeywords();

    /*
    static enum QSSProperty propertyValue(const QString &text);
    static QString propertyName(enum QSSProperty p);
    */

private:
    bool isIdentifierBody(const QChar &c);
    bool isNumber(const QChar &c);

    States m_state;
    bool m_scanComments;
};

} // namespace Internal
} // namespace QssEditor
Q_DECLARE_OPERATORS_FOR_FLAGS(QssEditor::Internal::Lexer::States)
#endif // QSSLEXER_H
