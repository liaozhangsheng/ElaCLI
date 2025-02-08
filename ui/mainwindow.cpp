#include <qobject.h>

#include <QDir>
#include <QFileDialog>
#include <QVBoxLayout>

#include "../src/project_generator.hpp"
#include "ElaPushButton.h"
#include "ElaMessageBar.h"
#include "ElaText.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : ElaWidget(parent),
      input_project_name(new ElaLineEdit(this)),
      input_project_path(new ElaLineEdit(this)),
      checkbox_packetio(new ElaCheckBox("包含 PacketIO", this)),
      checkbox_widgettools(new ElaCheckBox("包含 WidgetTools", this)) {
    this->setup_ui();

    this->init_content();
}

MainWindow::~MainWindow() = default;

void MainWindow::setup_ui() {
    this->setWindowButtonFlags(ElaAppBarType::MinimizeButtonHint | ElaAppBarType::CloseButtonHint);
    this->setFixedSize(550, 350);
    this->setIsFixedSize(true);
    this->setWindowTitle("ElaCLI");
    this->setWindowIcon(QIcon(":/favicon.svg"));
    this->moveToCenter();
    ElaMessageBar::information(ElaMessageBarType::TopRight, "欢迎使用！😊", "请填写信息并点击“创建”。", 2000,
                     this);
}

void MainWindow::init_content() {
    auto layout_without_margin_and_spacing = [](QBoxLayout *layout) {
        layout->setContentsMargins(32, 0, 32, 0);
        layout->setSpacing(16);
        return layout;
    };

    auto *layout_main = layout_without_margin_and_spacing(new QVBoxLayout());

    {
        auto *layout_project_name = layout_without_margin_and_spacing(new QHBoxLayout());
        layout_project_name->setContentsMargins(32, 64, 32, 0);

        {
            auto text_project_name = new ElaText("项目名称：", this);
            text_project_name->setTextStyle(ElaTextType::Body);

            input_project_name->setFixedHeight(32);

            layout_project_name->addWidget(text_project_name);
            layout_project_name->addWidget(input_project_name);
        }

        auto *layout_project_path = layout_without_margin_and_spacing(new QHBoxLayout());

        {
            auto *text_project_path = new ElaText("项目路径：", this);
            text_project_path->setTextStyle(ElaTextType::Body);

            input_project_path->setFixedHeight(32);

            auto *btn_browse = new ElaPushButton("浏览", this);
            btn_browse->setFixedSize(64, 32);

            connect(btn_browse, &ElaPushButton::clicked, [this]() {
                auto path =
                    QFileDialog::getExistingDirectory(nullptr, "选择项目路径", QDir::homePath());
                if (!path.isEmpty()) {
                    this->input_project_path->setText(path);
                }
            });

            layout_project_path->addWidget(text_project_path);
            layout_project_path->addWidget(input_project_path);
            layout_project_path->addWidget(btn_browse);
        }

        auto *layout_component = layout_without_margin_and_spacing(new QHBoxLayout());

        {
            checkbox_packetio->setChecked(true);

            checkbox_widgettools->setChecked(true);

            layout_component->addStretch();
            layout_component->addWidget(checkbox_packetio);
            layout_component->addWidget(checkbox_widgettools);
            layout_component->addStretch();
        }

        layout_main->addLayout(layout_project_name);
        layout_main->addLayout(layout_project_path);
        layout_main->addLayout(layout_component);

        auto *btn_create = new ElaPushButton("创建", this);
        btn_create->setFixedSize(64, 32);

        connect(btn_create, &ElaPushButton::clicked, [this]() {
            QString msg;
            bool success = ProjectGenerator::generate(
                input_project_name->text(), input_project_path->text(),
                checkbox_packetio->isChecked(), checkbox_widgettools->isChecked(), msg);
            if (!success) {
                ElaMessageBar::error(ElaMessageBarType::TopRight, "创建失败！😥", msg, 2000, this);
            } else {
                ElaMessageBar::success(ElaMessageBarType::TopRight, "创建成功！😍", msg, 2000,
                                       this);
            }
        });

        layout_main->addWidget(btn_create, 1, Qt::AlignHCenter);
    }

    this->setLayout(layout_main);
}