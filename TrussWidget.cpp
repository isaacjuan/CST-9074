#include "TrussWidget.h"
#include <QPainter>
#include <QPaintEvent>

TrussWidget::TrussWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400, 200);
}

void TrussWidget::setPieces(const QVector<TrussPiece> &pieces)
{
    m_pieces = pieces;
    update();
}

void TrussWidget::setInfo(const TrussInfo &info)
{
    m_info = info;
    update();
}

void TrussWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    TrussRenderer::paint(painter, size(), m_pieces, m_info);
}
