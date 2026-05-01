#include<string>
#include<vector>
#include<unordered_map>
#include<queue>
#include<limits>
#include<cmath>
#include<algorithm>
#include<iostream>
#include<set>
#include "node.h"
#include "edge.h"

#define max_double std::numeric_limits<long double>::max()

struct pair_hash {
    size_t operator()(const std::pair<long long,long long>& p) const {
        return std::hash<long long>()(p.first) ^ (std::hash<long long>()(p.second) << 1);
    }
};

struct Order{
    long long id;
    long long pick_up;
    long long drop_off;
    bool assigned;
    bool picked;
    bool recieved;
    Order(long long id1,long long p1,long long d1):id(id1),pick_up(p1),drop_off(d1),assigned(false),picked(false),recieved(false){}
};

class Graph{
    private:
        long long N;
        std::vector<Node> Nodes;
        std::unordered_map<long long,Edge> Edges;
        std::unordered_map<std::pair<long long,long long>,long long,pair_hash> Edge_nodes;
        std::vector<std::vector<long long>> Adjacency; 
        std::vector<std::vector<long long>> reverse_Adjacency;
        std::vector<long long> primary;
        std::vector<long long> secondary;
        std::vector<long long> tertiary;
        std::vector<long long> local;
        std::vector<long long> expressway;
        std::unordered_map<std::string,std::vector<long long>> POIS;
    public:
        Graph();
        void add_node(long long id1,float lat1,float log1,std::vector<std::string>pois1);
        void add_edge(long long id1,long long u1,long long v1,long double length1,long double avg_time1,std::string road_type1,std::vector<double> speed_profile1,bool oneway1);
        void set_nodes(long long n);
        std::pair<std::vector<long long>, long double> Dijkstra(long long u_id,long long v_id,bool length);
        std::vector<long double> Dijkstra_oneway(long long u_id); 
        std::vector<std::pair<std::vector<long long>, long double>> delivery(long long depot,std::vector<Order>& orders,long long num_drivers);
};