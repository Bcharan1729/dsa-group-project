#include "../inc/graph.h"


Graph::Graph(){}
void Graph::set_nodes(long long n){
    N=n;
    Nodes.resize(N);
    Adjacency.resize(N);
}

void Graph::add_node(long long id1,float lat1,float log1 ,std::vector<std::string>pois1){
    Nodes[id1]=Node(id1,lat1,log1,pois1);
    for(auto poi:Nodes[id1].get_pois()){
        POIS[poi].push_back(id1);
    }
}
void Graph::add_edge(long long id1,long long u1,long long v1,long double length1,long double avg_time1,std::string road_type1,std::vector<double> speed_profile1,bool oneway1){
    Edge e(id1,u1,v1,length1,avg_time1,road_type1,speed_profile1,oneway1);
    Edges[id1]=e;
    Adjacency[u1].push_back(id1);
    if(!oneway1)Adjacency[v1].push_back(id1);
    if(road_type1=="primary")primary.push_back(id1);
    if(road_type1=="secondary")secondary.push_back(id1);
    if(road_type1=="tertiary")tertiary.push_back(id1);
    if(road_type1=="local")local.push_back(id1);
    if(road_type1=="expressway")expressway.push_back(id1);            
}
void Graph::forbidden_roadtypes(std::vector<std::string> f_roadtypes){  
    for(auto f_roadtype:f_roadtypes){
        if(f_roadtype=="primary"){
            for(auto edgeid:primary){
                Edges[edgeid].set_valid(false);
            }
        }else if (f_roadtype=="secondary"){
            for(auto edgeid:secondary){
                Edges[edgeid].set_valid(false);
            }
        }else if (f_roadtype=="tertiary"){
            for(auto edgeid:tertiary){
                Edges[edgeid].set_valid(false);
            }
        }else if (f_roadtype=="local"){
            for(auto edgeid:local){
                Edges[edgeid].set_valid(false);
            }
        }else if (f_roadtype=="expressway"){
            for(auto edgeid:expressway){
                Edges[edgeid].set_valid(false);
            }
        }
    }
    return ;
};
bool Graph::remove_edge(long long  edgeid){
    if(Edges.find(edgeid) == Edges.end()){
        return false;
    }
    if(Edges[edgeid].is_valid())  {
        Edges[edgeid].set_valid(false);
        return true;
    }return false;
};

bool Graph::edge_ispresent(long long edgeid){
    if(Edges.find(edgeid)==Edges.end())return false;
    return true;
}
void Graph::forbidden_nodes(std::vector<long long> f_nodeids){
    for(auto f_nodeid:f_nodeids){
        Nodes[f_nodeid].set_valid(false);
    }
    return ;
};
void Graph::restore_nodes(std::vector<long long> f_nodeids){
    for(auto f_nodeid:f_nodeids){
        Nodes[f_nodeid].set_valid(true);
    }
    return ;
};
void Graph::restore_roadtypes(std::vector<std::string> f_roadtypes){
    for(auto f_roadtype:f_roadtypes){
        if(f_roadtype=="primary"){
            for(auto edgeid:primary){
                Edges[edgeid].set_valid(true);
            }
        }else if (f_roadtype=="secondary"){
            for(auto edgeid:secondary){
                Edges[edgeid].set_valid(true);
            }
        }else if (f_roadtype=="tertiary"){
            for(auto edgeid:tertiary){
                Edges[edgeid].set_valid(true);
            }
        }else if (f_roadtype=="local"){
            for(auto edgeid:local){
                Edges[edgeid].set_valid(true);
            }
        }else if (f_roadtype=="expressway"){
            for(auto edgeid:expressway){
                Edges[edgeid].set_valid(true);
            }
        }
    }
    return;
}
void Graph::modify_edge(long long edgeid,std::vector<std::pair<std::string,long double>>modified_values){
    if(Edges.find(edgeid) == Edges.end()){
        return;
    }    
    for(auto m_value:modified_values){
        if(m_value.first=="length"){
            Edges[edgeid].change_length(m_value.second);
        }else if(m_value.first=="average_time"){
            Edges[edgeid].change_avgtime(m_value.second);
        }
    }
    Edges[edgeid].set_valid(true);
    return ;
};

void Graph::modify_road_type(long long edgeid,std::string new_road_type){
    Edges[edgeid].change_roadtype(new_road_type);
}
void Graph::modify_speedprofile(long long edgeid,std::vector<double> new_speedprofile){
    Edges[edgeid].change_speedprofile(new_speedprofile);
};
