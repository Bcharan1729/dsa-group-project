#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "read_json/inc/read.h"
/*
    Add other includes that you require, only write code wherever indicated
*/

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <graph.json> <queries.json> <output.json>" << std::endl;
        return 1;
    }   

    // Read graph from first file
    /*
        Add your graph reading and processing code here
        Initialize any classes and data structures needed for query processing
    */
    try{
        Graph MAP=read_graph_from_json(argv[1]);
        //precompute
        std::vector<std::vector<long double>> dist_from_centers=MAP.distancesfrom_centers(MAP.centers(16));
        json output =read_queries(argv[2], MAP,dist_from_centers);


        std::ofstream output_file(argv[3]);
        if (!output_file.is_open()) {
            std::cerr << "Failed to open output.json for writing" << std::endl;
            return 1;
        }
        output_file << output.dump(4) << std::endl;
        output_file.close();
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}