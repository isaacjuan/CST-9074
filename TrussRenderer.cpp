#include "TrussRenderer.h"
#include <QPainter>
#include <QPolygonF>
#include <QFontMetrics>
#include <QtMath>
#include <functional>

namespace {

QColor colorForType(const QString &type)
{
    if (type == "TopChord")
        return QColor(191, 146, 89);
    if (type == "BottomChord")
        return QColor(89, 146, 191);
    return QColor(146, 191, 89);
}

QString formatInches(double v)
{
    if (v < 0) v = -v;
    double whole = qFloor(v);
    double frac = v - whole;
    int sixteenths = qRound(frac * 16.0);
    if (sixteenths == 16) { whole += 1.0; sixteenths = 0; }
    if (sixteenths == 0)
        return QString::number((int)whole) + "\"";

    int num = sixteenths;
    int den = 16;
    while (den % 2 == 0 && num % 2 == 0) { num /= 2; den /= 2; }

    if ((int)whole == 0)
        return QString::number(num) + "/" + QString::number(den) + "\"";
    return QString::number((int)whole) + "-" + QString::number(num) + "/" + QString::number(den) + "\"";
}

void drawDimensions(QPainter &p,
                     std::function<double(double)> tx,
                     std::function<double(double)> ty,
                     double xMin, double yMin, double yMax,
                     const TrussInfo &info)
{
    QFont dimFont = p.font();
    dimFont.setPointSize(7);
    p.setFont(dimFont);

    QColor dimColor(180, 180, 180);
    QColor extColor(140, 140, 140);
    double gap = 3.0;
    double over = 5.0;
    double tickD = 5.0;
    double textGap = 2.0;

    auto drawHorizDim = [&](double x1, double x2, double yAnchor, double offsetY, const QString &label) {
        double sx1 = tx(x1);
        double sx2 = tx(x2);
        double syAnchor = ty(yAnchor);
        double sy = syAnchor + offsetY;

        p.setPen(QPen(extColor, 0.5));
        p.drawLine(QPointF(sx1, syAnchor + gap), QPointF(sx1, sy + over));
        p.drawLine(QPointF(sx2, syAnchor + gap), QPointF(sx2, sy + over));

        p.setPen(QPen(dimColor, 1.0));
        p.drawLine(QPointF(sx1, sy), QPointF(sx2, sy));

        p.drawLine(QPointF(sx1 - tickD, sy - tickD), QPointF(sx1 + tickD, sy + tickD));
        p.drawLine(QPointF(sx2 - tickD, sy + tickD), QPointF(sx2 + tickD, sy - tickD));

        p.setPen(Qt::white);
        QRectF tr(sx1, sy + textGap, sx2 - sx1, 20);
        p.drawText(tr, Qt::AlignHCenter | Qt::AlignTop, label);
    };

    auto drawVertDim = [&](double y1, double y2, double xAnchor, double offsetX, const QString &label, bool leftSide) {
        double sy1 = ty(y1);
        double sy2 = ty(y2);
        double sxAnchor = tx(xAnchor);
        double sx = sxAnchor + offsetX;

        p.setPen(QPen(extColor, 0.5));
        p.drawLine(QPointF(sxAnchor + gap, sy1), QPointF(sx + over, sy1));
        p.drawLine(QPointF(sxAnchor + gap, sy2), QPointF(sx + over, sy2));

        p.setPen(QPen(dimColor, 1.0));
        p.drawLine(QPointF(sx, sy1), QPointF(sx, sy2));

        p.drawLine(QPointF(sx - tickD, sy1 - tickD), QPointF(sx + tickD, sy1 + tickD));
        p.drawLine(QPointF(sx - tickD, sy2 - tickD), QPointF(sx + tickD, sy2 + tickD));

        p.setPen(Qt::white);
        double midY = (sy1 + sy2) / 2.0;
        QFontMetrics fm(p.font());
        double tw = fm.horizontalAdvance(label);
        if (leftSide) {
            p.drawText(QPointF(sx - tw - 6, midY + fm.ascent() / 2.0), label);
        } else {
            p.drawText(QPointF(sx + 6, midY + fm.ascent() / 2.0), label);
        }
    };

    double bottomEdge = yMin;
    double topEdge = yMax;
    double leftEdge = 0.0;
    double rightEdge = info.span;

    drawHorizDim(leftEdge, rightEdge, bottomEdge, 25.0, formatInches(info.span));

    drawVertDim(bottomEdge, topEdge, leftEdge, -25.0, formatInches(info.height), true);

    double leftHeelBottom = topEdge - info.leftHeelHeight;
    drawVertDim(leftHeelBottom, topEdge, leftEdge, -47.0, formatInches(info.leftHeelHeight), true);

    double rightHeelBottom = topEdge - info.rightHeelHeight;
    drawVertDim(rightHeelBottom, topEdge, rightEdge, 25.0, formatInches(info.rightHeelHeight), false);
}

} // anonymous namespace

void TrussRenderer::paint(QPainter &painter, const QSize &size,
                           const QVector<TrussPiece> &pieces,
                           const TrussInfo &info)
{
    painter.setRenderHint(QPainter::Antialiasing);

    if (pieces.isEmpty())
        return;

    double xMin = 1e9, xMax = -1e9, yMin = 1e9, yMax = -1e9;
    for (const TrussPiece &p : pieces) {
        for (const QPointF &pt : p.negative) {
            if (pt.x() < xMin) xMin = pt.x();
            if (pt.x() > xMax) xMax = pt.x();
            if (pt.y() < yMin) yMin = pt.y();
            if (pt.y() > yMax) yMax = pt.y();
        }
        for (const QPointF &pt : p.positive) {
            if (pt.x() < xMin) xMin = pt.x();
            if (pt.x() > xMax) xMax = pt.x();
            if (pt.y() < yMin) yMin = pt.y();
            if (pt.y() > yMax) yMax = pt.y();
        }
    }

    double topM = 35.0, bottomM = 85.0, leftM = 85.0, rightM = 70.0;
    double availW = size.width() - leftM - rightM;
    double availH = size.height() - topM - bottomM;
    double dataW = xMax - xMin;
    double dataH = yMax - yMin;

    if (dataW < 1.0) dataW = 1.0;
    if (dataH < 1.0) dataH = 1.0;

    double scale = qMin(availW / dataW, availH / dataH);

    auto tx = [&](double x) -> double { return leftM + (x - xMin) * scale; };
    auto ty = [&](double y) -> double { return topM + (yMax - y) * scale; };

    painter.fillRect(QRectF(0, 0, size.width(), size.height()), QColor(30, 30, 30));

    for (const TrussPiece &p : pieces) {
        QPolygonF polygon;
        for (const QPointF &pt : p.negative)
            polygon << QPointF(tx(pt.x()), ty(pt.y()));
        for (const QPointF &pt : p.positive)
            polygon << QPointF(tx(pt.x()), ty(pt.y()));

        QColor fillColor = colorForType(p.type);
        fillColor.setAlpha(200);

        painter.setPen(QPen(Qt::white, 1.0));
        painter.setBrush(fillColor);
        painter.drawPolygon(polygon);
    }

    QPen gridPen(QColor(60, 60, 60), 0.5, Qt::DashLine);
    painter.setPen(gridPen);
    painter.setBrush(Qt::NoBrush);
    for (double y = qCeil(yMin); y <= qFloor(yMax); y += 5.0)
        painter.drawLine(QLineF(tx(xMin), ty(y), tx(xMax), ty(y)));
    for (double x = qCeil(xMin); x <= qFloor(xMax); x += 10.0)
        painter.drawLine(QLineF(tx(x), ty(yMin), tx(x), ty(yMax)));

    drawDimensions(painter, tx, ty, xMin, yMin, yMax, info);

    QStringList legend = {"TopChord", "BottomChord", "Web"};
    int legendX = 10, legendY = 10;
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    for (const QString &label : legend) {
        QColor c = colorForType(label);
        c.setAlpha(200);
        painter.setBrush(c);
        painter.setPen(QPen(Qt::white, 1.0));
        painter.drawRect(legendX, legendY, 14, 10);
        painter.setPen(Qt::white);
        painter.drawText(legendX + 18, legendY + 10, label);
        legendY += 16;
    }
}
