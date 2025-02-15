#pragma once

#include "ElaWidget.h"

class MainWindow : public ElaWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    void _setup_ui();

    void _init_content();
};
