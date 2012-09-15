#ifndef GRADIENTTIPWIDGET_H
#define GRADIENTTIPWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QGradient>

namespace QssEditor {
namespace Internal {
class GradientTipWidget;
class GradientTipWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GradientTipWidget(QGradient *gradient, QWidget *parent = 0);
    ~GradientTipWidget();
    QSize sizeHint() const;

signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *);

private:
    QGradient m_gradient;
    QPixmap m_tilePixMap;
};

}
}

#endif // GRADIENTTIPWIDGET_H
