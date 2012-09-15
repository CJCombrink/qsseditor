#ifndef QSSCOMPLETIONASSIST_H
#define QSSCOMPLETIONASSIST_H

#include <texteditor/codeassist/basicproposalitem.h>
#include <texteditor/codeassist/basicproposalitemlistmodel.h>
#include <texteditor/codeassist/completionassistprovider.h>
#include <texteditor/codeassist/iassistprocessor.h>

#include <QIcon>

namespace QssEditor {
namespace Internal {

class QssAssistProposalItem : public TextEditor::BasicProposalItem
{
public:
    QssAssistProposalItem();
    virtual ~QssAssistProposalItem();

    virtual bool prematurelyApplies(const QChar &c) const;
    virtual void applyContextualContent(TextEditor::BaseTextEditor *editor,
                                        int basePosition) const;
};


class QssCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
public:
    QssCompletionAssistProvider();
    virtual ~QssCompletionAssistProvider();

    virtual bool supportsEditor(const Core::Id &editorId) const;
    virtual TextEditor::IAssistProcessor *createProcessor() const;

    virtual bool isAsynchronous() const;
    virtual int activationCharSequenceLength() const;
    virtual bool isActivationCharSequence(const QString &sequence) const;
    virtual bool isContinuationChar(const QChar &c) const;
};


class QssCompletionAssistProcessor : public TextEditor::IAssistProcessor
{
public:
    QssCompletionAssistProcessor();
    virtual ~QssCompletionAssistProcessor();

    virtual TextEditor::IAssistProposal *perform(const TextEditor::IAssistInterface *interface);

private:
    bool acceptsIdleEditor();
    int findStartOfName(int pos = -1) const;
    bool isInComment() const;

    int m_startPosition;
    QScopedPointer<const TextEditor::IAssistInterface> m_interface;
};


class QssAssistProposalModel : public TextEditor::BasicProposalItemListModel
{
public:
    QssAssistProposalModel(const QList<TextEditor::BasicProposalItem *> &items);
    virtual ~QssAssistProposalModel();

    virtual bool isSortable(const QString &prefix) const;
};

} // Internal
} // QssEditor

#endif // QSSCOMPLETIONASSIST_H
