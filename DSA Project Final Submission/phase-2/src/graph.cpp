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
std::vector<std::pair<std::vector<long long>,long double>>Graph::K_shortest(long long u_id,long long v_id,int K){//part1
    std::vector<std::vector<long long>> shortest_k(K);
    std::vector<std::pair<std::vector<long long>,long double>>ANS;
    std::priority_queue<std::pair<long double,std::vector<long long>>> potential_shortest;
    std::vector<long long>forbidden_edges;
    std::vector<long long>root_nodes;
    auto Astar_output=Astar(u_id,v_id,true);
    shortest_k[0]=Astar_output.first;
    ANS.push_back({shortest_k[0],Astar_output.second});
    for(int i=1;i<K;i++){
        std::vector<long long> root_path;
        long double length_root=0.0;
        for (int j=0;j<shortest_k[i-1].size()-1;j++){
            auto present_node=shortest_k[i-1][j];
            root_path.push_back(shortest_k[i-1][j]);//root path start and propagation step 
            if(j>=1){//remove nodes
                Nodes[shortest_k[i-1][j-1]].set_valid(false);
                root_nodes.push_back(shortest_k[i-1][j-1]);
                length_root+=Edges[Edge_nodes[{root_path[j-1],root_path[j]}]].get_length();
            }
            for(int x=0;x<i;x++){
                auto path=shortest_k[x];
                bool is_part=true;
                for(int z=0;z<root_path.size()&& z<path.size();z++){
                    if(root_path[z]!=path[z]){
                        is_part=false;
                        break;
                    }
                }
                if(is_part){
                    auto rm_edgeid=Edge_nodes[{present_node,path[root_path.size()]}];
                    Edges[rm_edgeid].set_valid(false);
                    forbidden_edges.push_back(rm_edgeid);//remove edges
                }
            }
            auto total_path=root_path;
            Astar_output=Astar(present_node,v_id,true);
            if(Astar_output.second==-1.0){
                continue;
            }
            auto root_path_complement=Astar_output.first;
            auto total_length=length_root+Astar_output.second;
            for(int alpha=1;alpha<root_path_complement.size();alpha++){
                total_path.push_back(root_path_complement[alpha]);
            }
            potential_shortest.push({-total_length,total_path});
        }
         for(auto edge:forbidden_edges){
            Edges[edge].set_valid(true);
        }
        for(auto node:root_nodes){
            Nodes[node].set_valid(true);
        }
        root_nodes.clear();
        forbidden_edges.clear();
        if(potential_shortest.empty()){
            break;
        }
        auto req_pair=potential_shortest.top();
        shortest_k[i]=req_pair.second;
        ANS.push_back({shortest_k[i],-req_pair.first});
        potential_shortest.pop();

        while(potential_shortest.top()==req_pair){
            potential_shortest.pop();
        }
    }
    return ANS;
}

std::vector<long long>Graph::centers(int C){
    std::vector<long long>centers;
    long long first_center=0;
    centers.push_back(first_center);

    std::vector<long double >closest_center_distance(N,max_double);

    closest_center_distance=Dijkstra_oneway(first_center);
    for(int i=1;i<C;i++){
        long long next_center=0;
        long double potential_center_dist=-1;
        for(int j=0;j<N;j++){
            if(closest_center_distance[j]!=max_double && closest_center_distance[j]>potential_center_dist){
                potential_center_dist=closest_center_distance[j];
                next_center=j;
            }
        }
        centers.push_back(next_center);
        std::vector<long double>dist_newcenter=Dijkstra_oneway(next_center);
        for(int j=0;j<N;j++){
            closest_center_distance[j]=std::min(closest_center_distance[j],dist_newcenter[j]);
        }
    }
    return centers;

}

std::vector<std::vector<long double>>Graph::distancesfrom_centers(std::vector<long long>centers){
    int C=centers.size();
    std::vector<std::vector<long double>>dist_from_cent(C,std::vector<long double>(N));
    for(int i=0;i<C;i++){
        dist_from_cent[i]=Dijkstra_oneway(centers[i]);

        
    }
    return dist_from_cent;
}

long double Graph::heuristic(long long u_id , long long v_id,std::vector<std::vector<long double>>&dist_from_cent){
    long double h=0;
    int C=dist_from_cent.size();
    for(int i=0;i<C;i++){
        long double dist_C_to_u=dist_from_cent[i][u_id],dist_C_to_v=dist_from_cent[i][v_id];
        if(dist_C_to_u==max_double || dist_C_to_v==max_double){
            continue;
        }
        h=std::max(h,std::abs(dist_C_to_u-dist_C_to_v));
        
    }
    return h;
}

long double Graph::approx_shortest_path(long long u_id,long long v_id,std::vector<std::vector<long double>> dist_from_cent,double eps){
    if(u_id==v_id)return 0;
    std::vector<long double> dist_from_u(N,max_double);
    std::vector<long double> dist_from_v(N,max_double);
    long double best_path_len=max_double;
    struct searchNode{
        long long id;
        long double act_dist;
        long double h_dist;
        bool operator>(const searchNode& other) const{
            return h_dist>other.h_dist;
        }
    };

    std::priority_queue<searchNode,std::vector<searchNode>,std::greater<searchNode>>fwd_queue,bwd_queue;
    dist_from_u[u_id]=0;
    fwd_queue.push({u_id,0,heuristic(u_id,v_id,dist_from_cent)});
    dist_from_v[v_id]=0;
    bwd_queue.push({v_id,0,heuristic(v_id,u_id,dist_from_cent)});

    while(!(fwd_queue.empty() && bwd_queue.empty())){
        long double fwd_top_h=max_double,bwd_top_h=max_double;
        if(!fwd_queue.empty())fwd_top_h=fwd_queue.top().h_dist;
        if(!bwd_queue.empty())bwd_top_h=bwd_queue.top().h_dist;
        long double lower_bound=std::max(fwd_top_h,bwd_top_h);

        if(best_path_len < max_double && best_path_len<=(1.0+eps)*lower_bound){
            return (best_path_len+lower_bound)/2;
        }

        if(!fwd_queue.empty() && fwd_top_h<=bwd_top_h){
            searchNode curr_node=fwd_queue.top();
            fwd_queue.pop();

            if(curr_node.act_dist!=dist_from_u[curr_node.id]){
                continue;
            }

            if(dist_from_v[curr_node.id]<max_double){
                best_path_len=std::min(best_path_len,dist_from_u[curr_node.id]+dist_from_v[curr_node.id]);
            }
            for(auto & edge:Adjacency[curr_node.id]){
                long long next_node_id=Edges[edge].get_another_node(curr_node.id);
                long double new_dist=dist_from_u[curr_node.id]+Edges[edge].get_length();
                if(new_dist<dist_from_u[next_node_id]){
                    dist_from_u[next_node_id]=new_dist;
                    fwd_queue.push({next_node_id,new_dist,new_dist+heuristic(next_node_id,v_id,dist_from_cent)});
                }
            }
        }else{
            searchNode curr_node=bwd_queue.top();
            bwd_queue.pop();

            if(curr_node.act_dist!=dist_from_v[curr_node.id]){
                continue;
            }

            if(dist_from_u[curr_node.id]<max_double){
                best_path_len=std::min(best_path_len,dist_from_v[curr_node.id]+dist_from_u[curr_node.id]);
            }
            for(auto & edge:reverse_Adjacency[curr_node.id]){
                long long next_node_id=Edges[edge].get_another_node(curr_node.id);
                long double new_dist=dist_from_v[curr_node.id]+Edges[edge].get_length();
                if(new_dist<dist_from_v[next_node_id]){
                    dist_from_v[next_node_id]=new_dist;
                    bwd_queue.push({next_node_id,new_dist,new_dist+heuristic(next_node_id,u_id,dist_from_cent)});
                }
            }
        }
    }
    return best_path_len;
}

std::vector<std::pair<std::vector<long long>,long double>>Graph::heuristic_kshortest(long long u_id,long long v_id,int K,long double alpha){
    std::vector<std::pair<std::vector<long long>,long double>>potential_shortest;
    std::vector<std::pair<std::vector<long long>,long double>>Ans;
    std::vector<std::vector<long long>>A;
    std::unordered_map<long long,Edge>original=Edges;
    std::vector<std::vector<long long>>pathsas_edge;
    std::vector<long long>track;
    long double short_dist=-1;
    for(int i=0;i<50;i++){
        std::pair<std::vector<long long>,long double>Astar_output=Astar(u_id,v_id,true);
        if(i==0){
            short_dist=Astar_output.second;
        }
        if(Astar_output.second==-1){
            return Ans;
        }
        bool is_continue=false;
        for(auto x:potential_shortest){
            if(x.first==Astar_output.first){
                is_continue=true;
                break;
            }
        }
        std::vector<long long>path=Astar_output.first;
        long double modify_fraction=1+(100/(alpha*(path.size()-1)));
        if(is_continue){
            std::vector<long long>path_inedge;
            for(long long i=0;i<path.size()-1;i++){
            long long edge_id=Edge_nodes[{path[i],path[i+1]}];
            path_inedge.push_back(edge_id);
            Edges[edge_id].change_length(Edges[edge_id].get_length()*modify_fraction);
            }
            continue;
        }
        potential_shortest.push_back(Astar_output);
        std::vector<long long>path_inedge;
        for(long long i=0;i<path.size()-1;i++){
            long long edge_id=Edge_nodes[{path[i],path[i+1]}];
            path_inedge.push_back(edge_id);
            Edges[edge_id].change_length(Edges[edge_id].get_length()*modify_fraction);
        }
        pathsas_edge.push_back(path_inedge);
    }
    Ans.push_back(potential_shortest[0]);
    A.push_back(potential_shortest[0].first);
    std::vector<bool>visited(potential_shortest.size(),false);
    visited[0]=true;
    track.push_back(0);
    for(int j=1;j<K;j++){
        int a=0;
        long double min_penalty=max_double;
        for(int i=1;i<potential_shortest.size();i++){
            if(visited[i]){
                continue;
            }
            A.push_back(potential_shortest[i].first);
            long double total_penalty=Total_penalty(short_dist,alpha,A,original);
            if(total_penalty<min_penalty){
                a=i;
                min_penalty=total_penalty;
            }
            A.pop_back();
        }
        A.push_back(potential_shortest[a].first);
        track.push_back(a);
        visited[a]=true;
        if(A.size()>2){
            long double min_penaltydash=max_double;
            long long b=0;
            for(int i=1;i<potential_shortest.size();i++){
                if(visited[i]){
                    continue;
                }
                A[j-1]=(potential_shortest[i].first);
                long double total_penalty=Total_penalty(short_dist,alpha,A,original);
                if(total_penalty<min_penaltydash){
                    b=i;
                    min_penaltydash=total_penalty;
                }
            }
            if(min_penaltydash<min_penalty){
                A[j-1]=potential_shortest[b].first;
                visited[track[j-1]]=false;
                track[j-1]=b;
                visited[b]=true;
            }else{
                A[j-1]=potential_shortest[track[j-1]].first;
            }
        }

    }
    for(int x=1;x<track.size();x++){
        std::pair<std::vector<long long>,long double> tmp;
        tmp.first=potential_shortest[track[x]].first;
        tmp.second=length_of_path(pathsas_edge[track[x]],original);
        Ans.push_back(tmp);
    }
    Edges=original; 
    return Ans;
}//change1
long double Graph::Total_penalty(long double short_dist,long double alpha, std::vector<std::vector<long long>>b,std::unordered_map<long long,Edge>&original){
    std::vector<std::vector<long long>>pathsas_edge;
    std::vector<long long>path_inedge;
    long double total_penalty=0.0;
    for(auto path:b){
        for(long long i=0;i<path.size()-1;i++){
            long long edge_id=Edge_nodes[{path[i],path[i+1]}];
            path_inedge.push_back(edge_id);
        }
        pathsas_edge.push_back(path_inedge);
    }
    for(auto x:pathsas_edge){
        long double distance_penalty=(length_of_path(x,original)/short_dist)-0.9;
        long long overlap_penalty=0;
        for(auto y:pathsas_edge){
            if(y==x){
                overlap_penalty++;
                continue;
            }
            long long count=0;
            std::sort(x.begin(),x.end());
            std::sort(y.begin(),y.end());
            bool is_break=false;
            long long a=0,z=0;  
            while(a<y.size() && z<x.size()){
                if(x[z]<y[a]){
                    z++;
                }
                if(x[z]==y[a]){
                    count++;
                    z++,a++;
                    if(count>=alpha*(1/100)*x.size()){
                        overlap_penalty++;
                        is_break=true;
                        break;
                    }
                }
                if(x[z]>y[a]){
                    a++;
                }
            }

        }
        total_penalty+=distance_penalty*overlap_penalty;
    }
    return total_penalty;
};
long double Graph::length_of_path(std::vector<long long>b,std::unordered_map<long long,Edge>& original){
    long double ans=0;
    for(auto x:b){
        ans+=original[x].get_length();
    }
    return ans;
}