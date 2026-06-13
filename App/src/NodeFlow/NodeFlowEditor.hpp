#include "NodeFlow/IDs.hpp"
#include "NodeFlow/NodeFlow.hpp"
#include "NodeFlow/NodeFlowView.hpp"
#include "NodeFlow/Port.hpp"
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <qdebug.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <qwidget.h>
class NodeFlowEditor : public QWidget
{

  public:
    NodeFlowEditor(NodeFlow *flow, QWidget *parent = nullptr) : QWidget(parent)
    {
        // Top bar
        topBar = new QWidget(this);
        topBar->setFixedHeight(36);
        topBar->setStyleSheet("background:#2b2b2b;");

        auto *barLayout = new QHBoxLayout(topBar);
        barLayout->setContentsMargins(8, 4, 8, 4);
        barLayout->addWidget(new QLabel("NodeFlow"));
        barLayout->addStretch();
        barLayout->addWidget(new QPushButton("Compile"));

        // Scene view
        view = new NodeFlowView(flow, this);

        // Layout
        auto *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(topBar);
        layout->addWidget(view);
    }
    ~NodeFlowEditor() override = default;
    NodeFlowView *getView() const { return view; }

  private:
    bool autoCompile = true;
    QWidget *topBar;
    NodeFlowView *view;
};
