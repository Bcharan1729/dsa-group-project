#include "../inc/node.h"

Node::Node(){};

Node::Node(long long id1,float lat1,float log1 ,std::vector<std::string>pois1){
    id=id1;
    lat=lat1;
    log=log1;
    pois=pois1;
    valid=true;
};
bool Node::is_valid(){
    return valid;
};
long long Node::get_id(){
    return id;
};
std::pair<float,float> Node::get_position(){
    std::pair<float,float> position;
    position={lat,log};
    return position;
};
void Node::set_valid(bool set_to){
    valid=set_to;
    return ;
};
std::vector<std::string> Node::get_pois(){
    return pois;
};