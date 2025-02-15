#include "mainwindow.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

#include "ElaText.h"

MainWindow::MainWindow(QWidget *parent) : ElaWidget(parent) {
    this->_setup_ui();

    this->_init_content();
}

MainWindow::~MainWindow() = default;

void MainWindow::_setup_ui() {
    this->setFixedSize(400, 600);
    this->setIsFixedSize(true);
    this->setWindowTitle("MainWindow");
    this->setWindowIcon(QIcon(":/favicon.svg"));
    this->moveToCenter();
}

void MainWindow::_init_content() {
    auto *layout_main = new QVBoxLayout(this);
    layout_main->setContentsMargins(0, 64, 0, 64);

    auto *label_qrcode = new QLabel(this);
    label_qrcode->setPixmap(QPixmap(":res/QRCode.png"));

    auto *text_title = new ElaText("欢迎加入QQ交流群~", this);
    text_title->setTextStyle(ElaTextType::Title);

    layout_main->addWidget(label_qrcode, 0, Qt::AlignCenter);
    layout_main->addWidget(text_title, 0, Qt::AlignCenter);

    this->setLayout(layout_main);
}