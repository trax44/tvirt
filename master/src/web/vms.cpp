#include "vms.hpp"

#include <boost/property_tree/ptree.hpp>      
#include <boost/property_tree/json_parser.hpp>


namespace tvirt {

namespace webgui {


VMSTree::VMSTree (const std::string & address, 
                  const std::string & login,
                  const std::string & password,
                  Wt::WContainerWidget *parent) : 
    parent(parent),
    address(address),
    salt(address, login, password){
    
    init();
}


void VMSTree::init() {
    Wt::WTree *tree = new Wt::WTree(parent);
    tree->setSelectionMode(Wt::ExtendedSelection);

    Wt::WIconPair *worldIcon  = new Wt::WIconPair("proxy-open.png", "proxy.png", false);
    Wt::WIconPair *serverIcon = new Wt::WIconPair("server.png", "server.png", false);

    Wt::WTreeNode *rootNode = new Wt::WTreeNode("world", worldIcon);

    tree->setTreeRoot(rootNode);
    rootNode->label()->setTextFormat(Wt::PlainText);
    rootNode->setLoadPolicy(Wt::WTreeNode::NextLevelLoading);
    
    const boost::property_tree::ptree &ret = salt.getListVms("*");
    const boost::property_tree::ptree &tab0 = ret.front().second;
    const boost::property_tree::ptree &servers = tab0.front().second;
    
    for (boost::property_tree::ptree::const_iterator 
             server = servers.begin() , server_end    = servers.end() ; 
         server != server_end ; ++server) {

        Wt::WTreeNode *serverNode = new Wt::WTreeNode(server->first, serverIcon);
        serverNode->expand();

        for (boost::property_tree::ptree::const_iterator 
                 vms = server->second.begin()  , vms_end = server->second.end() ; 
             vms != vms_end ; ++vms) {

            serverNode->addChildNode(new Wt::WTreeNode(vms->second.data(), serverIcon));
            serverNode->expand();
            
        }
        rootNode->addChildNode(serverNode);

    }
}



} // webgui

} // tvirt

