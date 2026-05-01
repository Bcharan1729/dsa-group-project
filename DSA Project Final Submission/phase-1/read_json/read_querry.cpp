#include "inc/read.h"

json read_queries(const std::string& filename, Graph& MAP) {
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
            if(query["type"]=="remove_edge"){
                bool done=MAP.remove_edge(query["edge_id"]);
                result["done"]=done;
            }
            else if(query["type"]=="modify_edge")
            {
                long long edge_id=query["edge_id"];
                if(!MAP.edge_ispresent(edge_id)){
                    result["done"]=false;
                    auto end_time = std::chrono::high_resolution_clock::now();
                    result["processing_time"] = std::chrono::duration<double, std::milli>(end_time - start_time).count();
                    results.push_back(result);
                    continue;
                }
                std::vector<std::pair<std::string,long double>>modified_values;
                if(query.contains("patch")){
                    for( const auto& s:query["patch"].items()){
                            if(s.key()=="length"){
                                modified_values.push_back({"length",s.value()});
                            }
                            if(s.key()=="average_time"){
                                modified_values.push_back({"average_time",s.value()});
                            }
                            if(s.key()=="road_type"){
                                MAP.modify_road_type(edge_id,s.value());
                            }
                            if(s.key()=="speed_profile"){
                                std::vector<double> new_speedprofile=s.value().get<std::vector<double>>();
                                MAP.modify_speedprofile(edge_id,new_speedprofile);
                            }
                        }
                }
                MAP.modify_edge(edge_id,modified_values);
                result["done"]=true;
            }
            else if(query["type"]=="shortest_path"){
                long long src=query["source"];
                long long tar=query["target"];

                std::vector<long long> f_nodeids;
                std::vector<std::string> f_roadtypes;

                if(query.contains("constraints")){
                    if(query["constraints"].contains("forbidden_nodes")){
                        f_nodeids=query["constraints"]["forbidden_nodes"].get<std::vector<long long>>();
                    }if(query["constraints"].contains("forbidden_road_types")){
                        f_roadtypes=query["constraints"]["forbidden_road_types"].get<std::vector<std::string>>();
                    }
                }
                MAP.forbidden_nodes(f_nodeids);
                MAP.forbidden_roadtypes(f_roadtypes);
                bool weight_is_dis=false;
                if(query["mode"]=="distance"){
                    weight_is_dis=true;
                }
                std::pair<std::vector<long long>,long double> path=MAP.Astar(src,tar,weight_is_dis);
                if(path.first.empty() || path.second<0){
                    result["possible"]=false;
                }else{
                    result["possible"]=true;
                    result["path"]=path.first;
                    if(query["mode"]=="distance"){
                        result["minimum_time/minimum_distance"]=path.second;
                    }else{
                        result["minimum_time/minimum_distance"]=path.second;
                    }
                }
                MAP.restore_nodes(f_nodeids);
                MAP.restore_roadtypes(f_roadtypes);
            }else if(query["type"]=="knn"){
                if(query["metric"]=="shortest_path"){
                    std::vector<long long> path=MAP.KNN(query["query_point"]["lat"],query["query_point"]["lon"],query["poi"],query["k"]);
                    result["nodes"]=path;
                }else if(query["metric"]=="euclidean"){
                    std::vector<long long> path=MAP.Euclidean(query["query_point"]["lat"],query["query_point"]["lon"],query["k"],query["poi"]);
                    result["nodes"]=path;                
                }
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