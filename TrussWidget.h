#ifndef TRUSSWIDGET_H
#define TRUSSWIDGET_H

#include <QWidget>
#include "TrussRenderer.h"

class TrussWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TrussWidget(QWidget *parent = nullptr);

    void setPieces(const QVector<TrussPiece> &pieces);
    void setInfo(const TrussInfo &info);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<TrussPiece> m_pieces;
    TrussInfo m_info;
};

#endif
