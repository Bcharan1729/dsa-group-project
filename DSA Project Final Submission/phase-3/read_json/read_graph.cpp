#include "inc/read.h"

Graph read_graph_from_json(const std::string& filename){
    std::ifstream graphs_file(filename);
    if (!graphs_file.is_open()) {
        throw std::runtime_error("Failed to open " + filename);
    }
    json graphs_json;
    graphs_file >> graphs_json;
    Graph MAP;

    MAP.set_nodes(graphs_json["meta"]["nodes"]);
    for(const auto& node : graphs_json["nodes"]) {
        std::vector<std::string> pois;
        if(node.contains("pois")){
            pois=node["pois"].get<std::vector<std::string>>();
        }
        MAP.add_node(node["id"],node["lat"],node["lon"],pois);
    }

    for(const auto& edge : graphs_json["edges"]) {
        std::vector<double> speed_profile;
        if(edge.contains("speed_profile")){
            speed_profile=edge["speed_profile"].get<std::vector<double>>();
        }
        MAP.add_edge(edge["id"],edge["u"],edge["v"],edge["length"],edge["average_time"],edge["road_type"],speed_profile,edge["oneway"]);
    }
    return MAP;
}