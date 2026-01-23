
#include "RainfallEditor.hpp"

#include "GUI/GUI.hpp"
#include "NodeFlow/Node.hpp"
#include "NodeFlow/NodeFlowView.hpp"
#include "imgui.h"
#include <iostream>
#include <qgraphicsscene.h>
#include <qwindowdefs.h>
#include <stdexcept>

#include <NodeFlow/NodeFlow.hpp>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QWidget>
#include <QWindow>
RainfallEditor::RainfallEditor() {}
int RainfallEditor::Run(int &argc, char **argv)
{
    QApplication app(argc, argv);

    // Create main window
    QMainWindow window;
    window.setWindowTitle("Qt6 Menu Example");
    window.resize(1270, 800);

    // Create a menu bar
    QMenuBar *menuBar = window.menuBar();

    // Add a "File" menu
    QMenu *fileMenu = menuBar->addMenu("File");

    // Add actions to the File menu
    QAction *newAction = new QAction("New", &window);
    QAction *exitAction = new QAction("Exit", &window);

    fileMenu->addAction(newAction);
    fileMenu->addSeparator(); // optional separator
    fileMenu->addAction(exitAction);

    // Connect actions
    QObject::connect(newAction, &QAction::triggered,
                     [&]() { QMessageBox::information(&window, "New", "New action triggered!"); });
    QObject::connect(exitAction, &QAction::triggered, &app, &QApplication::quit);

    NodeFlow *nodeflow = new NodeFlow(&window);
    nodeflow->setSceneRect(-500, -500, 1000, 1000);

    NodeFlowView *view = new NodeFlowView(nodeflow, &window);
    window.setCentralWidget(view);
    view->setWindowTitle("Qt Node Editor Example");
    view->resize(800, 600);
    view->show();
    window.show();

    //QMainWindow *window2 = new QMainWindow();
    //NodeFlowView *view2 = new NodeFlowView(nodeflow, window2);
    //view2->setRenderHint(QPainter::Antialiasing);
    //window2->setCentralWidget(view2);
    //window2->setWindowTitle("Node Editor Window 2");
    //window2->resize(800, 600);
    //window2->move(850, 50); // offset so it doesn't cover the first window
    //window2->show();

    return app.exec();
}
