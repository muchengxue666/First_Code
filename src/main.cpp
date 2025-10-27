#include <QApplication>
#include "MainWindow.h"
#include "DataManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Load data
    DataManager::getInstance().loadAllData();
    
    MainWindow window;
    window.show();
    
    return app.exec();
}