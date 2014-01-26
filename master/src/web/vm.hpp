#ifndef TVIRT_VM_H_
#define TVIRT_VM_H_

#include <Wt/Widget>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

#include <Wt/WIconPair>
#include <Wt/WText>
#include <Wt/WTree>
#include <Wt/WTreeNode>


namespace tvirt {

namespace webgui {

class VM : public Wt::WContainerWidget {
private:
    Wt::WContainerWidget *parent;

public:

    VM (Wt::WContainerWidget *parent);


};

} // webgui
} // tvirt

#endif /* TVIRT */
