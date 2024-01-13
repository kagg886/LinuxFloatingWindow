#include "MainWindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), dragging(false) {
    connect(this, &MainWindow::updateLabel, this, &MainWindow::handleUpdateLabel);
    setWindowFlags(Qt::ToolTip);

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
    event->accept();
}


void MainWindow::handleUpdateLabel(const std::string& id, const QString& text) {
    QLabel* label = map[id];
    if (label == nullptr) {
        label = new QLabel(this);
        label->show();
        map[id] = label;
    }

    QFontMetrics m(label->font());

    label->resize(this->width(),m.height() * text.split("\n").size());
    label->setText(text);
    label->update();
}
