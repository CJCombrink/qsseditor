#ifndef QSSEDITORFACTORY_H
#define QSSEDITORFACTORY_H

#include <coreplugin/editormanager/ieditorfactory.h>

#include <QStringList>

namespace QssEditor {
namespace Internal {

class QssEditorFactory : public Core::IEditorFactory
{
    Q_OBJECT

public:
    QssEditorFactory(QObject *parent);

    // IEditorFactory
    QStringList mimeTypes() const;
    Core::Id id() const;
    QString displayName() const;
    Core::IEditor *createEditor(QWidget *parent);

private:
    QStringList m_mimeTypes;
};

} // namespace Internal
} // namespace QssEditor

#endif // QSSEDITORFACTORY_H
