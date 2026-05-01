#include <vector>
#include <string>
#include<unordered_map>
#include "node.h"
#include "edge.h"

class Graph{
    private:
        long long N;//no of nodes
        std::vector<Node> Nodes;
        std::unordered_map<long long,Edge> Edges;
        std::vector<std::vector<long long>> Adjacency; 
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
        void forbidden_roadtypes(std::vector<std::string> f_roadtypes);
        bool remove_edge(long long  edgeid);
        bool edge_ispresent(long long edgeid);
        void forbidden_nodes(std::vector<long long> f_nodeids);
        void modify_edge(long long edgeid,std::vector<std::pair<std::string,long double>>modified_values);
        void modify_road_type(long long edgeid,std::string new_road_type);
        void modify_speedprofile(long long edgeid,std::vector<double> new_speedprofile);
        void restore_nodes(std::vector<long long> f_nodeids);
        void restore_roadtypes(std::vector<std::string> f_roadtypes);
        std::pair<std::vector<long long>, long double> Dijkstra(long long u_id,long long v_id,bool length);
        std::pair<std::vector<long long>,long double> Astar(long long u_id,long long v_id,bool length);
        long long get_nearestnode(float x1,float y1);
        std::vector<long long> KNN(float x1,float y1,std::string poi,long long k);
        long double get_Euclidean(std::pair<float,float>position1,std::pair<float,float>position2);
        std::vector<long long>Euclidean(float lat,float log,long long k,std::string destination);
};