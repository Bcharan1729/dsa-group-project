#include "../inc/graph.h"
#include<queue>
#include<limits>
#include<algorithm>
#include <cmath>


std::pair<std::vector<long long>,long double> Graph::Dijkstra(long long u_id,long long v_id,bool length){
    std::priority_queue<std::pair<long double,long long>, std::vector<std::pair<long double,long long>>, std::greater<std::pair<long double,long long>>> unkown;
    std::vector<long double> sp(N,std::numeric_limits<long double>::max());
    std::vector<bool>visited(N,false);
    std::vector<long long> parent(N,-1);
    unkown.push({0.0,u_id});
    sp[u_id]=0.0;
    while(!unkown.empty()){
        std::pair<long double,long long> p_node;
        p_node=unkown.top();
        unkown.pop();
        long long cur_node=p_node.second;
        if(!visited[cur_node]){
            visited[cur_node]=true;
            if(cur_node==v_id){
                break;
            }
            for(auto edgeid:Adjacency[cur_node]){
                long long next_node=Edges[edgeid].get_another_node(cur_node);
                if(next_node!=-1 && Edges[edgeid].is_valid()&& Nodes[next_node].is_valid()){
                    if(!visited[next_node]){
                        long double weight;
                        if(length){
                            weight=Edges[edgeid].get_length();
                        }else{
                            weight=Edges[edgeid].time_taken(sp[cur_node]);
                        }
                        if(sp[next_node]>sp[cur_node]+weight){
                            parent[next_node]=cur_node;
                            sp[next_node]=sp[cur_node]+weight;
                            unkown.push({sp[next_node],next_node});
                        }
                    }
                }
            }
        }
    }
    std::vector<long long > shortest_path;
    if(sp[v_id]==std::numeric_limits<long double>::max()){
        return {shortest_path,-1.0};
    }
    long long start=v_id;
    shortest_path.push_back(start);
    while(start!=u_id){
        shortest_path.push_back(parent[start]);
        start=parent[start];
    }
    std::reverse(shortest_path.begin(),shortest_path.end());
    return {shortest_path,sp[v_id]};
}

long double haversine(double lat1, double lon1, double lat2, double lon2) {
    lat1 *= M_PI / 180.0;
    lon1 *= M_PI / 180.0;
    lat2 *= M_PI / 180.0;
    lon2 *= M_PI / 180.0;
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) + std::cos(lat1) * std::cos(lat2) * std::sin(dlon / 2) * std::sin(dlon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return 6371.0 * c;
}

std::pair<std::vector<long long>,long double> Graph::Astar(long long u_id,long long v_id,bool length){
    std::priority_queue<std::pair<long double,long long>, std::vector<std::pair<long double,long long>>, std::greater<std::pair<long double,long long>>> unkown;
    std::vector<long double> sp(N,std::numeric_limits<long double>::max());
    std::vector<bool>visited(N,false);
    std::vector<long long> parent(N,-1);
    long double h = haversine(Nodes[u_id].get_position().first,Nodes[u_id].get_position().second,Nodes[v_id].get_position().first,Nodes[v_id].get_position().second);
    unkown.push({h,u_id});
    sp[u_id]=0.0;
    while(!unkown.empty()){
        std::pair<long double,long long> p_node;
        p_node=unkown.top();
        unkown.pop();
        long long cur_node=p_node.second;
        if(!visited[cur_node]){
            visited[cur_node]=true;
            if(cur_node==v_id)break;
            for(auto edgeid:Adjacency[cur_node]){
                long long next_node=Edges[edgeid].get_another_node(cur_node);
                if(Edges[edgeid].is_valid()&& Nodes[next_node].is_valid()){
                    if(!visited[next_node]){
                        long double weight;
                        if(length){
                            weight=Edges[edgeid].get_length();
                        }else{
                            weight=Edges[edgeid].time_taken(p_node.first);
                        }
                        if(sp[next_node]>sp[cur_node]+weight){
                            parent[next_node]=cur_node;
                            sp[next_node]=sp[cur_node]+weight;
                            long double h=haversine(Nodes[next_node].get_position().first,Nodes[next_node].get_position().second,Nodes[v_id].get_position().first,Nodes[v_id].get_position().second);
                            if(length){
                                unkown.push({weight+sp[cur_node]+h,next_node});
                            }else {
                                unkown.push({weight+sp[cur_node],next_node});
                            }
                        }
                    }
                }
            }
        }
    }
    std::vector<long long > shortest_path;
    if(sp[v_id]==std::numeric_limits<long double>::max()){
        return {shortest_path,-1.0};
    }
    long long start=v_id;
    shortest_path.push_back(start);
    while(start!=u_id){
        shortest_path.push_back(parent[start]);
        start=parent[start];
    }
    std::reverse(shortest_path.begin(),shortest_path.end());
    return {shortest_path,sp[v_id]};
}