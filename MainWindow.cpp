#include "MainWindow.h"
#include <QPushButton>
#include "Ini.h"

#define wWidth 150
#define wHeight 150

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), dragging(false) {
    connect(this, &MainWindow::updateLabel, this, &MainWindow::handleUpdateLabel);
    setWindowFlags(Qt::ToolTip);
//    setAttribute(Qt::WA_TranslucentBackground);

    label = new QLabel("", this);
    label->resize(wWidth, wHeight);

//    this->setStyleSheet("QMainWindow {background-image:url(:/image/resources/bg.png)}");

    QPalette qp = this->palette();
    QImage ai(":/image/resources/bg.png");
    ai = ai.scaled(wWidth, wHeight, Qt::IgnoreAspectRatio);
    qp.setBrush(QPalette::Window, QBrush(ai));
    this->setPalette(qp);

    Ini i("conf.ini");
    int x = i.readInt("pos", "x", 0);
    int y = i.readInt("pos", "y", 0);

    resize(wWidth, wHeight);
    move(x, y);

    label->show();
}

MainWindow::~MainWindow() {
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton && dragging) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    dragging = false;
    std::cout << event->pos().x() << "," << event->pos().y() << std::endl;
    Ini i("conf.ini");
    i.writeInt("pos", "x", static_cast<int>(event->globalPosition().x()) - this->width());
    i.writeInt("pos", "y", static_cast<int>(event->globalPosition().y()) - this->height());
    event->accept();

}


void MainWindow::handleUpdateLabel(const std::string &id, const QString &text) {
    map[id] = text;

    QString buf;

    for (const auto &item: map) {
        buf.append(item.second);
    }
    label->setText(buf);
    label->update();

}
