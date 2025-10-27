#include <QApplication>  // ✅ 使用 QApplication 而不是 QCoreApplication
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  // ✅ 创建图形界面应用
    
    // 创建主窗口
    QMainWindow window;
    window.setWindowTitle("电影院售票系统 - 测试");
    window.resize(500, 400);
    
    // 创建中央部件
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    // 添加标签
    QLabel *label = new QLabel("🎬 欢迎使用电影院售票系统");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 18px; font-weight: bold; margin: 20px;");
    layout->addWidget(label);
    
    // 添加按钮
    QPushButton *testButton = new QPushButton("测试按钮");
    testButton->setStyleSheet("padding: 10px; font-size: 14px;");
    layout->addWidget(testButton);
    
    // 连接按钮信号
    QObject::connect(testButton, &QPushButton::clicked, [&]() {
        QMessageBox::information(&window, "测试", "Qt 图形界面工作正常！");
    });
    
    // 添加状态标签
    QLabel *statusLabel = new QLabel(" 图形界面加载成功");
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);
    
    window.setCentralWidget(centralWidget);
    window.show();  // 显示窗口
    
    return app.exec();  // ✅ 进入事件循环，等待用户交互
}