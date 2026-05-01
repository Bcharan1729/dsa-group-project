#include "../inc/graph.h"

Graph::Graph(){}
void Graph::set_nodes(long long n){
    N=n;
    Nodes.resize(N);
    Adjacency.resize(N);
    reverse_Adjacency.resize(N);
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
    reverse_Adjacency[v1].push_back(id1);
    if(!oneway1){
        Adjacency[v1].push_back(id1);
        reverse_Adjacency[u1].push_back(id1);
    }
    Edge_nodes[{u1,v1}]=id1;
    Edge_nodes[{v1,u1}]=id1;    
    if(road_type1=="primary")primary.push_back(id1);
    if(road_type1=="secondary")secondary.push_back(id1);
    if(road_type1=="tertiary")tertiary.push_back(id1);
    if(road_type1=="local")local.push_back(id1);
    if(road_type1=="expressway")expressway.push_back(id1);            
}


std::vector<std::pair<std::vector<long long>, long double>> Graph::delivery(long long depot,std::vector<Order>& orders,long long num_drivers) {
    struct Driver {
        long long d_id;
        std::vector<long long> route;
        long long curr_loc;
        std::set<long long> picked;
        long double total_time;
        Driver(long long id, long long depot_loc): d_id(id), curr_loc(depot_loc), total_time(0.0) {route.push_back(depot_loc);}
    };
    std::vector<Driver> drivers;
    for (long long i=0;i<num_drivers;i++) {
        drivers.push_back(Driver(i,depot));
    }
    long long tot_orders=orders.size();
    long long done_cnt=0;
    while (done_cnt<tot_orders) {
        bool ass=false;
        for (long long d = 0; d < num_drivers; d++) {
            long long best_order=-1;
            long double best_dist=max_double;
            bool is_pickup=false;
            for (long long picked_idx : drivers[d].picked) {
                if (!orders[picked_idx].recieved) {
                    auto result=Dijkstra(drivers[d].curr_loc,orders[picked_idx].drop_off,true);
                    long double distance=result.second;
                    if (distance!=-1.0 && distance<best_dist) {
                        best_dist=distance;
                        best_order=picked_idx;
                        is_pickup=false;
                    }
                }
            }
            if (best_order==-1) {
                for (long long o=0; o<orders.size();o++) {
                    if (!orders[o].picked && !orders[o].assigned) {
                        auto result=Dijkstra(drivers[d].curr_loc,orders[o].pick_up, true);
                        long double distance=result.second;
                        if (distance!=-1.0 && distance<best_dist) {
                            best_dist=distance;
                            best_order=o;
                            is_pickup=true;
                        }
                    }
                }
            }
            if (best_order!=-1 && best_dist!=max_double) {
                ass=true;
                if (is_pickup) {
                    drivers[d].route.push_back(orders[best_order].pick_up);
                    drivers[d].curr_loc = orders[best_order].pick_up;
                    drivers[d].total_time += best_dist;
                    orders[best_order].picked = true;
                    orders[best_order].assigned = true;
                    drivers[d].picked.insert(best_order);
                } else {
                    drivers[d].route.push_back(orders[best_order].drop_off);
                    drivers[d].curr_loc = orders[best_order].drop_off;
                    drivers[d].total_time += best_dist;
                    orders[best_order].recieved = true;
                    drivers[d].picked.erase(best_order);
                    done_cnt++;
                }
            }
        }
        
        if (!ass) break;
    }
    
    std::vector<std::pair<std::vector<long long>, long double>> result;
    for (auto& driver : drivers) {
        result.push_back({driver.route, driver.total_time});
    }
    return result;
}
