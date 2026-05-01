#include "inc/read.h"

json read_queries(const std::string& filename, Graph& MAP,std::vector<std::vector<long double>> dist_from_centers) {
    std::ifstream queries_file(filename);
    if (!queries_file.is_open()) {
        throw std::runtime_error("Failed to open " + filename);
    }
    
    json queries_json;
    queries_file >> queries_json;
    
    std::vector<json> results;
    
    for (const auto& query : queries_json["events"]) {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        json result;
        result["id"] = query["id"];
        
         try{
            if(query["type"]=="k_shortest_paths"){
                long long src=query["source"];
                long long tar=query["target"];
                long long k_val=query["k"];
                std::vector<std::pair<std::vector<long long> , long double >> K_short=MAP.K_shortest(src,tar,k_val);
                json paths = json::array();
                for (auto path_and_length:K_short){
                    json temp;
                    temp["path"]=path_and_length.first;
                    temp["length"]=path_and_length.second;
                    paths.push_back(temp);
                }
                result["paths"]=paths;
            }else if(query["type"]=="k_shortest_paths_heuristic"){
                int k=query["k"];
                long double overlap_threshould=query["overlap_threshold"];
                long long src=query["source"];
                long long targ=query["target"];
                std::vector<std::pair<std::vector<long long>,long double>> res=MAP.heuristic_kshortest(src,targ,k,overlap_threshould);
                json paths=json::array();
                for(auto path_and_length:res){
                    json temp;
                    temp["path"]=path_and_length.first;
                    temp["length"]=path_and_length.second;
                    paths.push_back(temp);
                }
                result["paths"]=paths;
            }else if(query["type"]=="approx_shortest_path"){
                long double eps =query["acceptable_error_pct"];
                eps=eps/100;
                json distances=json::array();
                for(auto q:query["queries"]){
                    long long src=q["source"];
                    long long targ=q["target"];
                    json tmp;
                    tmp["source"]=src;
                    tmp["target"]=targ;
                    tmp["approx_shortest_distance"]=MAP.approx_shortest_path(src,targ,dist_from_centers,eps);
                    distances.push_back(tmp);
                }
                result["distances"]=distances;
            }
        }catch(const std::exception& e){
            std::cerr << "Error processing query " << query["id"] << ": " << e.what() << std::endl;
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        result["processing_time"] = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        results.push_back(result);
    }
    
    json output;
    output["meta"] = queries_json["meta"];
    output["results"] = results;
    
    return output;
}