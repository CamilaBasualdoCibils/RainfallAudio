#include "NodeFlowView.hpp"
#include "NodeFlow/NodeRegistry.hpp"
#include <QAction>
#include <QMenu>
#include <qobject.h>
void NodeFlowView::ShowNodesContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    NodeRegistry::Get().ForEach_NodeType(
        [&](const NodeRegistry::NodeEntry &e)
        {
            QAction *action = menu.addAction(QString::fromStdString(e.Name));
            connect(action, &QAction::triggered,
                    [&]()
                    {
                        std::shared_ptr<Node> node =
                            e.Create(parentFlow, parentFlow->ReserveNodeID());
                        node->setPos(mapToScene(pos));
                        parentFlow->AddNode(node);

                        // auto it = m_callbacks.find(nodeName);
                        // if (it != m_callbacks.end())
                        //     it->second(); // call associated function
                    });
        });

    menu.exec(mapToGlobal(pos));
}
