#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include "read_json/inc/read.h"

using json = nlohmann::json;


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <graph.json> <queries.json> <output.json>" << std::endl;
        return 1;
    }
    try {
        Graph MAP = read_graph_from_json(argv[1]);
        json output = read_queries(argv[2], MAP);

        std::ofstream output_file(argv[3]);
        if (!output_file.is_open()) {
            std::cerr << "Failed to open output.json for writing" << std::endl;
            return 1;
        }
        output_file << output.dump(4);
        output_file.close();        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}