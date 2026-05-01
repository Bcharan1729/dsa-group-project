#include "../inc/graph.h"
#include<algorithm>
#include<queue>
#include<cmath>

long long Graph::get_nearestnode(float x1,float y1){//cv changed
    std::vector<long double> distances;
    std::pair<float,float> position={x1,y1};
    long double min=std::numeric_limits<long double>::max();
    for(auto node:Nodes){
        distances.push_back(get_Euclidean(position,node.get_position()));
        if(min>distances[node.get_id()]){
            min=distances[node.get_id()];
        }
    }
    for(auto node:Nodes){
        if(min==distances[node.get_id()]){
            return node.get_id();
        }
    }
    return 0;
}

std::vector<long long> Graph::KNN(float x1,float y1,std::string poi,long long k){//cv changed
    long long u_id=get_nearestnode(x1,y1);
    std::priority_queue<std::pair<long double,long long>, std::vector<std::pair<long double,long long>>, std::greater<std::pair<long double,long long>>> unkown;
    std::vector<long double> sp(N,std::numeric_limits<long double>::max());
    std::vector<bool>visited(N,false);
    std::vector<long long> parent(N,-1);
    std::vector<long long> Nearest;
    unkown.push({0.0,u_id});
    sp[u_id]=0.0;
    while(!unkown.empty()){
        std::pair<long double,long long> p_node=unkown.top();
        unkown.pop();
        long long cur_node=p_node.second;
        if(p_node.first > sp[cur_node]) continue;
        bool is_poi=false;
        for(auto p_poi:Nodes[p_node.second].get_pois()){
            if(p_poi==poi){
                is_poi=true;
                break;
            }
        }
        if(is_poi){
            Nearest.push_back(p_node.second);
        }
        if(Nearest.size()==k)break;
        if(!visited[cur_node]){
            visited[cur_node]=true;
            for(auto edgeid:Adjacency[cur_node]){
                long long next_node=Edges[edgeid].get_another_node(cur_node);
                if(next_node!=-1 &&  Edges[edgeid].is_valid()&& Nodes[next_node].is_valid()){
                    if(!visited[next_node]){
                        long double weight;
                        weight=Edges[edgeid].get_length();
                        if(sp[next_node]>sp[cur_node]+weight){
                            parent[next_node]=cur_node;
                            sp[next_node]=sp[cur_node]+weight;
                            unkown.push({weight+sp[cur_node],next_node});
                        }
                    }
                }
            }
        }
    }
    return Nearest;
}

long double Graph::get_Euclidean(std::pair<float,float>position1,std::pair<float,float>position2){
    long double ans;
    ans=(position1.first-position2.first)*(position1.first-position2.first);
    ans+=(position1.second-position2.second)*(position1.second-position2.second);
    ans=sqrt(ans);
    return ans;
}
std::vector<long long> Graph::Euclidean(float lat ,float log,long long k,std::string destination){
    std::priority_queue<std::pair<long double ,long long> >shortest;
    for(auto node_id:POIS[destination]){
        double Euclidean_dist=get_Euclidean(Nodes[node_id].get_position(),{lat,log});
        shortest.push({Euclidean_dist,node_id});
        if(shortest.size()>k){
            shortest.pop();
        }
    }
    std::pair<long double,long long> tmp;
    std::vector<long long >ans;
    while(!shortest.empty()){
        tmp=shortest.top();
        ans.push_back(tmp.second);
        shortest.pop();
    }
    std::reverse(ans.begin(),ans.end());
    return ans;
}