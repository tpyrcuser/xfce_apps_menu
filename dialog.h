#ifndef XFCE_APPS_MENU_DIALOG_H
#define XFCE_APPS_MENU_DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
public:
    explicit Dialog(const std::vector<std::string> &categories, QWidget *parent = nullptr);

    ~Dialog() override;

    std::vector<std::string> selectedItems;

private:
    Ui::Dialog *ui;

    void accepted();
};

#endif
