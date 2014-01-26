#ifndef VIRT_VMS_H_
#define VIRT_VMS_H_

#include <Wt/WWidget>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

#include <Wt/WIconPair>
#include <Wt/WText>
#include <Wt/WTree>
#include <Wt/WTreeNode>

#include <string>
#include <list>


namespace tvirt {
namespace webgui {

class VMSTree : public Wt::WContainerWidget {
private:
    Wt::WContainerWidget *parent;
    std::string address;
    salt::Salt salt;
    void init();
    
public:
    VMSTree (const std::string  & address, 
             const std::string  & login,
             const std::string  & password,
             Wt::WContainerWidget *parent);
    VMSTree (const std::list<std::string> &address, Wt::WContainerWidget *parent);
    

};

} // webgui
} // tvirt


#endif /* VIRT */
