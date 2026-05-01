#include "../inc/graph.h"
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
                            // weight=Edges[edgeid].time_taken(sp[cur_node]);
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

std::vector<long double>Graph::Dijkstra_oneway(long long u_id){
    std::priority_queue<std::pair<long double,long long>, std::vector<std::pair<long double,long long>>, std::greater<std::pair<long double,long long>>> unkown;
    std::vector<long double> sp(N,max_double);
    std::vector<bool>visited(N,false);
    unkown.push({0.0,u_id});
    sp[u_id]=0.0;
    while(!unkown.empty()){
        std::pair<long double,long long> p_node;
        p_node=unkown.top();
        unkown.pop();
        long long cur_node=p_node.second;
        if(p_node.first>sp[cur_node]){
            continue;
        }
        if(!visited[cur_node]){
            visited[cur_node]=true;
            for(auto edgeid:Adjacency[cur_node]){
                long long next_node=Edges[edgeid].get_another_node(cur_node);
                if(next_node!=-1 && Edges[edgeid].is_valid()&& Nodes[next_node].is_valid()){
                    if(!visited[next_node]){
                        long double weight=Edges[edgeid].get_length();
                        if(sp[next_node]>sp[cur_node]+weight){
                            sp[next_node]=sp[cur_node]+weight;
                            unkown.push({sp[next_node],next_node});
                        }
                    }
                }
            }
        }
    }
    return sp;
}