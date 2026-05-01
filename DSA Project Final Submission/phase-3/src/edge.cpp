#include "../inc/edge.h"

Edge::Edge(){};
Edge::Edge(long long id1,long long u1,long long v1,long double length1,long double avg_time1,std::string road_type1,std::vector<double> speed_profile1,bool oneway1){
    id=id1;
    u=u1;
    v=v1;
    length=length1;
    avg_time=avg_time1;
    road_type=road_type1;
    speed_profile=speed_profile1;
    valid=true;
    oneway=oneway1;
};
bool Edge::is_valid(){
    return valid;
};
std::pair<long long,long long> Edge::get_nodes(){
    std::pair<long long,long long> nodes={u,v};
    return nodes;
};
long long Edge::get_id(){
    return id;
};
long double Edge::get_length(){
    return length;
};
void Edge::set_valid(bool set_valid){
    valid=set_valid;
    return;
};
void Edge::change_length(long double new_length){
    length=new_length;
    return ;
}
long long Edge::get_another_node(long long idx){
    if(idx==u){
        return v; 
    }else if(idx==v){
        return u;
    }else{
        return -1;
    }
};