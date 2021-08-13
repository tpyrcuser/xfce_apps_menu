#include <iostream>
#include <vector>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "File.h"
#include "dialog.h"

const std::string MainWindow::menuDirectory = "/usr/share/applications";
const std::string MainWindow::fileFormat = "[Desktop Entry]\n"
                                           "Name=%1\n"
                                           "Exec=%2\n"
                                           "Icon=%3\n"
                                           "Categories=%4\n"
                                           "Type=Application\n";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    std::map<std::string, QLineEdit *> widgetParam = {
            {"selectFileCommand", ui->commandEdit},
            {"selectFileIcon",    ui->iconEdit},
    };

    std::vector<MainWindow::xfceMenuApp> apps = getFiles();
    std::vector<std::string> categories;

    for (const MainWindow::xfceMenuApp &app : apps) {
        ui->listWidget->addItem(app.name.c_str());
        categories.insert(categories.end(), app.categoriesVector.begin(), app.categoriesVector.end());
    }

    connect(ui->selectFileCommand, &QPushButton::clicked, this, [=]() { this->selectFile(widgetParam); });
    connect(ui->selectFileIcon, &QPushButton::clicked, this, [=]() { this->selectFile(widgetParam); });

    connect(ui->selectFileCategories, &QPushButton::clicked, this, [=]() { this->selectCategory(categories); });
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, [=]() { this->itemSelected(apps); });

    connect(ui->addButton, &QPushButton::clicked, this, [=]() { this->addApp(); });
}

QStringList MainWindow::fileDialog() {
    QStringList result;
    QFileDialog file_dialog;

    file_dialog.setFileMode(QFileDialog::ExistingFile);

    if (file_dialog.exec()) {
        result = file_dialog.selectedFiles();
    }

    return result;
}

std::vector<MainWindow::xfceMenuApp> MainWindow::getFiles() {
    std::vector<std::string> files = File::getFilesInDirectory(menuDirectory);
    std::vector<MainWindow::xfceMenuApp> result;

    for (const std::string &file : files) {
        if (!QString(file.c_str()).endsWith(".desktop")) continue;

        QString content = QString(File::getFileContent(menuDirectory + "/" + file).c_str());
        QStringList contentList = content.split('\n');
        xfceMenuApp app;

        for (const QString &string : contentList) {
            QStringList split = string.split('=');
            const auto param = split[0];

            if (param == "Name") app.name = split[1].toStdString();
            if (param == "Exec") app.command = split[1].toStdString();
            if (param == "Icon") app.icon = split[1].toStdString();

            if (param == "Categories") {
                const auto categoriesString = split[1];

                QStringList categoriesQStringList = categoriesString.split(';');
                std::vector<std::string> categoriesVector;

                for (const QString &element : categoriesQStringList) {
                    if (element.isEmpty()) continue;

                    categoriesVector.push_back(element.toStdString());
                }

                app.categories = categoriesString.toStdString();
                app.categoriesVector = categoriesVector;
            }
        }

        app.fileName = file;
        result.push_back(app);
    }

    return result;
}

void MainWindow::selectCategory(const std::vector<std::string> &categories) {
    Dialog dialog(categories);
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        std::string text;

        for (const std::string &item : dialog.selectedItems) {
            text += item + ";";
        }

        ui->categoriesEdit->setText(text.c_str());
    }
}

void MainWindow::itemSelected(const std::vector<MainWindow::xfceMenuApp> &xfceApps) {
    std::string selectedItem = ui->listWidget->currentItem()->text().toStdString();
    xfceMenuApp app;

    for (const xfceMenuApp &i : xfceApps) {
        if (selectedItem != i.name) continue;

        app = i;
    }

    ui->nameEdit->setText(app.name.c_str());
    ui->commandEdit->setText(app.command.c_str());
    ui->iconEdit->setText(app.icon.c_str());
    ui->categoriesEdit->setText(app.categories.c_str());
    ui->fileNameEdit->setText(app.fileName.c_str());
}

void MainWindow::selectFile(const std::map<std::string, QLineEdit *> &widgetParam) {
    QObject *senderObj = sender();
    QString name = senderObj->objectName();

    auto a = widgetParam.find(name.toStdString());
    a->second->setText(fileDialog().first());
}

void MainWindow::addApp() {
    const QString name = ui->nameEdit->text();
    const QString command = ui->commandEdit->text();
    const QString icon = ui->iconEdit->text();
    const QString categories = ui->categoriesEdit->text();
    const QString fileName = ui->fileNameEdit->text();

    const QString content = QString(fileFormat.c_str()).arg(name, command, icon, categories);

    File::writeFile(menuDirectory + "/" + fileName.toStdString(), content.toStdString());
}

MainWindow::~MainWindow() {
    delete ui;
}