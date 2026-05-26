#ifndef TRUSSRENDERER_H
#define TRUSSRENDERER_H

#include <QVector>
#include <QPointF>
#include <QString>
#include <QPainter>
#include <QSize>

struct TrussPiece {
    QString type;
    QString label;
    QVector<QPointF> negative;
    QVector<QPointF> positive;
};

struct TrussInfo {
    double span = 120.0;
    double height = 24.0;
    double leftHeelHeight = 16.75;
    double rightHeelHeight = 12.75;
};

namespace TrussRenderer {

void paint(QPainter &painter, const QSize &size,
           const QVector<TrussPiece> &pieces, const TrussInfo &info);

} // namespace TrussRenderer

#endif
