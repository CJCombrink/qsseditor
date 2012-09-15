#include "gradienttipwidget.h"
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QBrush>
#include <QRect>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QConicalGradient>
#include <QDebug>
using namespace QssEditor;
using namespace QssEditor::Internal;

namespace {
QPixmap tilePixMap(int size)
{
    const int checkerbordSize= size;
    QPixmap tilePixmap(checkerbordSize * 2, checkerbordSize * 2);
    tilePixmap.fill(Qt::white);
    QPainter tilePainter(&tilePixmap);
    QColor color(220, 220, 220);
    tilePainter.fillRect(0, 0, checkerbordSize, checkerbordSize, color);
    tilePainter.fillRect(checkerbordSize, checkerbordSize, checkerbordSize, checkerbordSize, color);
    return tilePixmap;
}
} // Anonymous namespace

GradientTipWidget::GradientTipWidget(QGradient *gradient, QWidget *parent) :
    QWidget(parent)
{
    m_tilePixMap = tilePixMap(10);
    m_gradient = gradient ? *gradient : QLinearGradient();
}

GradientTipWidget::~GradientTipWidget()
{
}

QSize GradientTipWidget::sizeHint() const
{
    return QSize(40, 40);
}

void GradientTipWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawTiledPixmap(rect(), m_tilePixMap);
    painter.scale(width(), height());
    painter.setBrush(m_gradient);
    painter.drawRect(rect());
}
