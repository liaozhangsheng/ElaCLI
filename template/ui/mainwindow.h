#pragma once

#include "ElaWidget.h"

class MainWindow : public ElaWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void setup_ui();

    void init_content();

private:
};
