#include<string>
#include<vector>
#include<unordered_map>
#include<queue>
#include<limits>
#include<cmath>
#include<algorithm>
#include<iostream>
#include "node.h"
#include "edge.h"

#define max_double std::numeric_limits<long double>::max()

struct pair_hash {
    size_t operator()(const std::pair<long long,long long>& p) const {
        return std::hash<long long>()(p.first) ^ (std::hash<long long>()(p.second) << 1);
    }
};

class Graph{
    private:
        long long N;//no of nodes
        std::vector<Node> Nodes;
        std::unordered_map<long long,Edge> Edges;
        std::unordered_map<std::pair<long long,long long>,long long,pair_hash> Edge_nodes;
        std::vector<std::vector<long long>> Adjacency; 
        std::vector<std::vector<long long>> reverse_Adjacency;
        //once check this to get more efficient one 
        std::vector<long long > primary;
        std::vector<long long > secondary;
        std::vector<long long >tertiary;
        std::vector<long long >local;
        std::vector<long long >expressway;
        std::unordered_map<std::string,std::vector<long long>> POIS;
    public:
        Graph();
        void add_node(long long id1,float lat1,float log1 ,std::vector<std::string>pois1);
        void add_edge(long long id1,long long u1,long long v1,long double length1,long double avg_time1,std::string road_type1,std::vector<double> speed_profile1,bool oneway1);
        void set_nodes(long long n);
        std::pair<std::vector<long long>, long double> Dijkstra(long long u_id,long long v_id,bool length);
        std::pair<std::vector<long long>,long double> Astar(long long u_id,long long v_id,bool length);
        std::vector<long double>Dijkstra_oneway(long long u_id); 
        std::vector<std::pair<std::vector<long long>,long double>> K_shortest(long long u_id,long long v_id,int K);
        std::vector<long long>centers(int C);
        std::vector<std::vector<long double>>distancesfrom_centers(std::vector<long long>centers);
        long double heuristic(long long u_id , long long v_id,std::vector<std::vector<long double>>&dist_from_cent);
        long double approx_shortest_path(long long u_id,long long v_id,std::vector<std::vector<long double>> dist_from_cent,double eps); 
        std::vector<std::pair<std::vector<long long>,long double>>heuristic_kshortest(long long u_id,long long v_id,int K,long double alpha);//change1
        long double Total_penalty(long double short_dist,long double alpha,std::vector<std::vector<long long>> b,std::unordered_map<long long,Edge> &original);
        long double length_of_path(std::vector<long long>b,std::unordered_map<long long,Edge> &original);    
    };