#include "../../../nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "../../inc/graph.h"
/*
    Add other includes that you require, only write code wherever indicated
*/

using json = nlohmann::json;

Graph read_graph_from_json(const std::string& filename);
json read_queries(const std::string& filename, Graph& MAP,std::vector<std::vector<long double>> dist_from_centers);