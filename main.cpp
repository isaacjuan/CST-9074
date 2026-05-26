#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "TrussWidget.h"

static QVector<TrussPiece> loadPieces(const QString &path, TrussInfo &info)
{
    QVector<TrussPiece> pieces;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open" << path;
        return pieces;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    info.span = root["Span"].toDouble(120.0);
    info.height = root["OverallTrussHeight"].toDouble(24.0);
    info.leftHeelHeight = root["LeftHeelHeight"].toDouble(16.75);
    info.rightHeelHeight = root["RightHeelHeight"].toDouble(12.75);

    QJsonArray pieceArray = root["PieceData"].toArray();

    for (const QJsonValue &val : pieceArray) {
        QJsonObject obj = val.toObject();
        TrussPiece piece;
        piece.type = obj["Type"].toString();
        piece.label = obj["EngineeringLabel"].toString();

        QJsonObject endCap = obj["EndCap"].toObject();
        QJsonArray negArr = endCap["Negative"].toArray();
        QJsonArray posArr = endCap["Positive"].toArray();

        for (const QJsonValue &v : negArr) {
            QJsonObject pt = v.toObject();
            piece.negative.append(QPointF(pt["x"].toDouble(), pt["y"].toDouble()));
        }
        for (const QJsonValue &v : posArr) {
            QJsonObject pt = v.toObject();
            piece.positive.append(QPointF(pt["x"].toDouble(), pt["y"].toDouble()));
        }

        pieces.append(piece);
    }

    return pieces;
}

static void openJson(QMainWindow *window, TrussWidget *widget, const QString &path)
{
    TrussInfo info;
    QVector<TrussPiece> pieces = loadPieces(path, info);

    if (pieces.isEmpty()) {
        QMessageBox::warning(window, "Error",
                             "No piece data found in:\n" + path);
        return;
    }

    widget->setPieces(pieces);
    widget->setInfo(info);

    QFileInfo fi(path);
    window->setWindowTitle("Floor Truss Viewer - " + fi.fileName());
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    TrussWidget *widget = new TrussWidget;
    window.setCentralWidget(widget);

    QMenu *fileMenu = window.menuBar()->addMenu("&File");
    QAction *openAction = fileMenu->addAction("&Open...");
    openAction->setShortcut(QKeySequence::Open);
    QObject::connect(openAction, &QAction::triggered, [&]() {
        QString path = QFileDialog::getOpenFileName(
            &window, "Open Truss JSON File", QString(),
            "JSON Files (*.json);;All Files (*)");
        if (!path.isEmpty())
            openJson(&window, widget, path);
    });

    QAction *exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    QObject::connect(exitAction, &QAction::triggered, &app, &QApplication::quit);

    QString defaultPath = QApplication::applicationDirPath() + "/FT1.json";
    if (QFile::exists(defaultPath))
        openJson(&window, widget, defaultPath);
    else
        window.setWindowTitle("Floor Truss Viewer");

    window.resize(900, 400);
    window.show();

    return app.exec();
}
