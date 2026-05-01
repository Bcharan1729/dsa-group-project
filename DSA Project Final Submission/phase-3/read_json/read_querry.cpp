#include "inc/read.h"

json read_queries(const std::string& query_file, Graph& MAP) {
    std::ifstream file(query_file);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open query file: " + query_file);
    }
    
    json query_json;
    file >> query_json;
    file.close();
    auto start_time = std::chrono::high_resolution_clock::now();
    json result;
    if(!query_json.contains("events")||query_json["events"].empty()) {
        throw std::runtime_error("No events found in query file");
    }

    json event = query_json["events"];//modified
    std::vector<Order> orders;

    for (const auto& order_ :event["orders"]) {
        long long id=order_["order_id"];
        long long pick_up=order_["pickup"];
        long long drop_off=order_["dropoff"];
        orders.push_back(Order(id,pick_up,drop_off));
    }

    long long num_drivers = event["fleet"]["num_delivery_guys"];
    long long depot = event["fleet"]["depot_node"];

    try {
        auto temp_res=MAP.delivery(depot,orders,num_drivers);
        json temp=json::array();
        long double tot_time=0.0;
        for (long long i=0; i<(long long)temp_res.size();i++) {
            json temp2;
            temp2["driver_id"]=i;
            temp2["route"]=temp_res[i].first;
            std::set<long long> d_set;
            for (const auto& order:orders) {
                if (order.assigned) {
                    bool has_pick_up=false;
                    bool has_drop_off=false;
                    for (long long node
                        :temp_res[i].first) {
                        if (node==order.pick_up)has_pick_up=true;
                        if (node==order.drop_off)has_drop_off=true;
                    }      
                    if (has_pick_up && has_drop_off) {
                        d_set.insert(order.id);
                    }
                }
            }      
            std::vector<long long> driver_orders(d_set.begin(),d_set.end());
            temp2["order_ids"] = driver_orders;       
            temp.push_back(temp2);
            tot_time+=temp_res[i].second;
        }   
        result["assignments"] = temp;
        result["metrics"]["total_delivery_time_s"] = tot_time;   
    } catch (const std::exception& e) {
        result["error"] = e.what();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    result["processing_time"] = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    return result;
}