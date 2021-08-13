#ifndef XFCE_APPS_MENU_MAINWINDOW_H
#define XFCE_APPS_MENU_MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
public:
    struct xfceMenuApp {
        std::string name;
        std::string command;
        std::string icon;
        std::string categories;
        std::vector<std::string> categoriesVector;
        std::string fileName;

        xfceMenuApp() = default;;

        xfceMenuApp(const std::string &name,
                    const std::string &command,
                    const std::string &icon,
                    const std::string &categories,
                    const std::vector<std::string> &categoriesVector,
                    const std::string &fileName) {
            this->name = name;
            this->command = command;
            this->icon = icon;
            this->categories = categories;
            this->categoriesVector = categoriesVector;
            this->fileName = fileName;
        }
    };

private:
    Ui::MainWindow *ui;

    static QStringList fileDialog();

    static std::vector<MainWindow::xfceMenuApp> getFiles();

    void selectFile(const std::map<std::string, QLineEdit *> &widgetParam);

    void selectCategory(const std::vector<std::string> &categories);

    void itemSelected(const std::vector<MainWindow::xfceMenuApp> &xfceApps);

    void addApp();

public:
    static const std::string menuDirectory;
    static const std::string fileFormat;

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;
};

#endif
