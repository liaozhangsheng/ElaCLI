#pragma once

#include "ElaCheckBox.h"
#include "ElaLineEdit.h"
#include "ElaWidget.h"

class MainWindow : public ElaWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    
    ~MainWindow();

    void setup_ui();

    void init_content();

private:
    ElaLineEdit *input_project_name;
    ElaLineEdit *input_project_path;
    ElaCheckBox *checkbox_packetio;
    ElaCheckBox *checkbox_widgettools;
};
