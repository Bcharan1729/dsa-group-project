#include "../inc/edge.h"
#include<iostream>

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
long double Edge::get_avgtime(){
    return avg_time;
};
std::string Edge::get_roadtype(){
    return road_type;
};
void Edge::set_valid(bool set_valid){
    valid=set_valid;
    return;
};

long long Edge::get_another_node(long long idx){
    if(idx==u){
        return v; 
    }else if(idx==v){
        return u;
    }else{
        return -1;
    }
};

void Edge::change_length(long double new_length){
    length=new_length;
    return ;
}

void Edge::change_avgtime(long double new_avgtime){
    avg_time=new_avgtime;
    return ;
}

void Edge::change_roadtype(std::string new_roadtype){
    road_type=new_roadtype;
    return;
}

void Edge::change_speedprofile(std::vector<double> new_speedprofile){
    speed_profile=new_speedprofile;
    return;
}
long double Edge::time_taken(long double time_before){
    if(speed_profile.size()==0)return avg_time;
    long long total_blocks=static_cast<long long>(time_before/900);
    int block_number=(total_blocks)%speed_profile.size();
    long double time_block=time_before-900*total_blocks;
    long double distance_left=(900-time_block)*speed_profile[block_number];
    if(distance_left>=length){
        return length/speed_profile[block_number];
    }
    long double t=900-time_block;
    block_number=(block_number+1)%speed_profile.size();
    while(distance_left>0){
        long long dist_seg=speed_profile[block_number]*900;
        if(distance_left>=dist_seg){
            t+=900;
            distance_left-=dist_seg;
        }else{
            t+=(distance_left/speed_profile[block_number]);
            distance_left=0;
        }
        block_number=(block_number+1)%speed_profile.size();
    }
    return t;
};