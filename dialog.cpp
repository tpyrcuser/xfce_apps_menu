#include <iostream>

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(const std::vector<std::string> &categories, QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);

    for (const std::string &category : categories) {
        ui->listWidget->addItem(category.c_str());
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &Dialog::accepted);
}

void Dialog::accepted() {
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();

    for (const QListWidgetItem *item : items) {
        selectedItems.push_back(item->text().toStdString());
    }
}

Dialog::~Dialog()
{
    delete ui;
}