import requests
import json
import os
import random
import sys

# Overpass query (bounding box: lat1, lon1, lat2, lon2)
# This example: area around IIT Bombay, Powai
# query = """
# [out:json];
# way["highway"](17.44417,78.29905,17.55485,78.48616);
# (._;>;);
# out;
# """
query = """
[out:json];
(
  way["highway"="expressway"](17.44417,78.29905,17.55485,78.48616);
  way["highway"="primary"](17.44417,78.29905,17.55485,78.48616);
  way["highway"="secondary"](17.44417,78.29905,17.55485,78.48616);
  way["highway"="tertiary"](17.44417,78.29905,17.55485,78.48616);
  way["highway"="local"](17.44417,78.29905,17.55485,78.48616);
);
(._;>;);
out;
"""
#check this
Lata=17.44417
Latb=17.55485
Lona=78.29905
Lonb=78.48616

# Send query to Overpass API



if len(sys.argv)>1:
    argument=sys.argv[1]
else:
    argument="0"

# print(argument)

if argument=="0":
# Parse and save JSON
    url = "https://overpass-api.de/api/interpreter"
    response = requests.get(url, params={"data": query})

    if response.status_code != 200 or not response.text.strip():
        print("Error: Empty or failed response from Overpass API")
        sys.exit(1)

    data = response.json()
else:   
#from a json file
    with open('graphs/graph.json','r') as f:
        data=json.load(f)





pois=["restaurant","petrol station","hospital","pharmacy","hotel","atm"]
ALLOWED_POIS = {
    "restaurant": ["amenity", "restaurant"],
    "petrol station": ["amenity", "fuel"],
    "hospital": ["amenity", "hospital"],
    "pharmacy": ["amenity", "pharmacy"],
    "hotel": ["tourism", "hotel"],
    "atm": ["amenity", "atm"]
}
speed_estimate={
    "expressway":28.0,
    "local":5.0,
    "primary":18.0,
    "secondary":14.0,
    "tertiary":10.0
}
given_roadtypes=["expressway","primary","secondary","tertiary","local"]

def distance(nodea,nodeb):
    lata=nodes[nodea]["lat"]
    lona=nodes[nodea]["lon"]
    latb=nodes[nodeb]["lat"]
    lonb=nodes[nodeb]["lon"]
    ans=((lata-latb)**2+(lona-lonb)**2)**0.5
    ans=ans*40000000
    ans=ans/360
    return ans

def extract_pois(tags):
    pois = []
    for name, (key, value) in ALLOWED_POIS.items():
        if tags.get(key) == value:
            pois.append(name)
    return pois
if argument=="0":
    nodes=[e for e in data["elements"] if e["type"]=="node"]
    mapnodes={}
    for i in range(len(nodes)):
        mapnodes[nodes[i]["id"]]=i
        nodes[i]["id"]=i
        if "tags" in nodes[i]:
            needed_tag=["amenity","tourism"]
            is_there=False
            for tag_name in needed_tag:
                if tag_name in nodes[i]["tags"]:
                    given_pois=nodes[i]["tags"][tag_name].split(',')
                    needed_poi=[]
                    for poi in pois:
                        if poi in given_pois:
                            needed_poi.append(poi)
                    if len(needed_poi)==0:
                        continue
                    tag_needed=",".join(needed_poi)
                    nodes[i]["tags"]=tag_needed
                    is_there=True
            if is_there==False:
                del nodes[i]["tags"]
    ways=[e for e in data["elements"] if e["type"]=="way"]
    edgeid_sequence=0
    edges=[]

    for way in ways:
        points=way["nodes"]
        roadtype=way["tags"]["highway"]
        if(roadtype in given_roadtypes):
            for i in range(len(way["nodes"])):
                if(i==len(way["nodes"])-1):
                    continue
                edge={}
                edge["u"]=mapnodes[points[i]]
                edge["v"]=mapnodes[points[i+1]]
                edge["id"]=edgeid_sequence
                edgeid_sequence=edgeid_sequence+1
                edge["oneway"]=False
                edge["length"]=round(distance(mapnodes[points[i]],mapnodes[points[i+1]]),2)
                edge["average_time"]=round(edge["length"]/speed_estimate[roadtype],2)
                y=random.random()
                average_speed=speed_estimate[roadtype]
                if(y>0.9):
                    edge["speed_profile"]=[]
                    for i in range(96):
                        x=random.uniform(-5,6)
                        edge["speed_profile"].append(round(average_speed+x,1))
                edge["one_way"]=False
                edge["road_type"]=roadtype
                edges.append(edge)
    meta={
        "id":"mumbai_sample",
        "nodes":len(nodes),
        "description":"IITB premises"
    }
    final_map={
        "meta":meta,
        "nodes":nodes,
        "edges":edges
    }
    with open("generated_graph.json", "w", encoding="utf-8") as f:
        json.dump(final_map, f, indent=2)
else:
    nodes=data["nodes"]
    edges=data["edges"]
    meta=data["meta"]
    final_map={
        "meta":meta,
        "nodes":nodes,
        "edges":edges
    }





queries={}
queries["meta"]="phase1_queries"
queries["events"]=[]
n=1000  #no of queries wanted
nodes_size=len(nodes)
edges_size=len(edges)
if edges_size>0:
    for i in range(n):
        p=random.random()
        if p<=0.1:
            dynamic_query={}
            prob_type=random.random()
            dynamic_query["id"]=i
            if prob_type>0.5:
                dynamic_query["type"]="remove_edge"
                dynamic_query["edge_id"]=random.randrange(0,edges_size)
            else:
                dynamic_query["type"]="modify_edge"
                dynamic_query["edge_id"]=random.randrange(0,edges_size)
                dynamic_query["patch"]={}
                for i in range(len(given_roadtypes)):
                        if(edges[dynamic_query["edge_id"]]["road_type"]==given_roadtypes[i]):
                            roadtype_before=i
                            break
                length_before=edges[dynamic_query["edge_id"]]["length"]
                if prob_type>0.475:
                    random_road=random.randrange(1,len(given_roadtypes))
                    dynamic_query["patch"]["road_type"]=given_roadtypes[(roadtype_before+random_road)%len(given_roadtypes)]
                    change_length_ratio=random_road-roadtype_before
                    dynamic_query["patch"]["length"]=round(length_before+random.uniform(0,change_length_ratio*length_before),2)
                else:
                    change_length_ratio=0.2
                    dynamic_query["patch"]["length"]=round(length_before+random.uniform(-0.2*length_before,0.2*length_before),2)
                if prob_type>0.1:
                    avg_time_before=edges[dynamic_query["edge_id"]]["length"]
                    dynamic_query["patch"]["average_time"]=round(avg_time_before+random.uniform(-change_length_ratio*avg_time_before,0),2)
                if prob_type>0.45:
                    dynamic_query["patch"]["speed_profile"]=[]
                    for i in range(96):
                        x=random.uniform(0,6)
                        dynamic_query["patch"]["speed_profile"].append(round(max(0,dynamic_query["patch"]["length"]/dynamic_query["patch"]["average_time"])+x,1))
            queries["events"].append(dynamic_query) 
        elif p<=0.55:
            shortest_path_query={}
            shortest_path_query["type"]="shortest_path"
            shortest_path_query["id"]=i
            node_select_S=random.randrange(0,nodes_size)
            node_select_T=random.randrange(1,nodes_size)
            shortest_path_query["source"]=node_select_S
            shortest_path_query["target"]=(node_select_S+node_select_T)%nodes_size
            using_nodes=[node_select_S,shortest_path_query["target"]]
            if p<=0.325:
                shortest_path_query["mode"]="time"
            else:
                shortest_path_query["mode"]="distance"
            if p<=0.2:
                shortest_path_query["constraints"]={}
                if p<=0.17:
                    if nodes_size>2:
                        number_of_forbidden=random.gauss(0,7)
                        if number_of_forbidden<0:
                            number_of_forbidden=-number_of_forbidden
                        number_of_forbidden=int(number_of_forbidden)
                        shortest_path_query["constraints"]["forbidden_nodes"]=[]
                        for i in range(number_of_forbidden):
                            fnode_id=random.randrange(0,nodes_size)
                            if fnode_id in using_nodes:
                                continue
                            shortest_path_query["constraints"]["forbidden_nodes"].append(fnode_id)
                        if len(shortest_path_query["constraints"]["forbidden_nodes"])==0:
                            del shortest_path_query["constraints"]["forbidden_nodes"]
                if p>=0.15:
                    shortest_path_query["constraints"]["forbidden_roadtypes"]=[]
                    number_of_forbidden_roadtype=random.gauss(0,1)
                    if number_of_forbidden_roadtype<0:
                        number_of_forbidden_roadtype=-number_of_forbidden_roadtype
                    number_of_forbidden_roadtype=min(int(number_of_forbidden_roadtype),len(given_roadtypes)-2)#*****
                    forbidden_roadtypes=[]
                    for i in range(number_of_forbidden_roadtype):
                        froadtype=random.randrange(0,len(given_roadtypes))
                        if froadtype in forbidden_roadtypes:
                            continue
                        shortest_path_query["constraints"]["forbidden_roadtypes"].append(given_roadtypes[froadtype])
                    if len(shortest_path_query["constraints"]["forbidden_roadtypes"])==0:
                        del shortest_path_query["constraints"]["forbidden_roadtypes"]
            queries["events"].append(shortest_path_query)
        else:
            knn_query={}
            knn_query["type"]="knn"
            knn_query["id"]=i
            lat=round(Lata+random.random()*(Latb-Lata),6)
            lon=round(Lona+random.random()*(Lonb-Lona),6)
            knn_query["query_point"]={
                "lat":lat,
                "lon":lon
            }
            knn_query["k"]=max(int(random.gauss(5,2)),0)
            if p>=0.7:
                knn_query["metric"]="shortest_path"
            else:
                knn_query["metric"]="euclidean"

            knn_query["poi"]=pois[random.randrange(0,len(pois))]
            queries["events"].append(knn_query)
    directory_path="queries"
    os.makedirs(directory_path, exist_ok=True) 
    filepath=f"{directory_path}/phase1_queries.json"
    with open(filepath,"w",encoding="utf-8") as f:
        json.dump(queries,f,indent=2) 
    queries={}
    queries["meta"]="phase2_queries"
    queries["events"]=[]
    for i in range(n):
        p=random.random()
        if p<0.33:
            k_shortest_paths_query={}
            k_shortest_paths_query["type"]="k_shortest_paths_query"
            k_shortest_paths_query["id"]=n+i
            k_shortest_paths_query["mode"]="distance"
            k_shortest_paths_query["source"]=random.randint(0,nodes_size-1)
            k_shortest_paths_query["target"]=random.randint(0,nodes_size-1)
            k_shortest_paths_query["k"]=random.randint(2,20)
            queries["events"].append(k_shortest_paths_query)
        elif p<0.67:
            k_shortest_H_query={}
            k_shortest_H_query["type"]="k_shortest_paths_heuristic"
            k_shortest_H_query["id"]=n+i
            k_shortest_H_query["source"]=random.randint(0,nodes_size-1)
            k_shortest_H_query["target"]=random.randint(0,nodes_size-1)
            k_shortest_H_query["k"]=random.randint(2,20)
            k_shortest_H_query["overlap_threshold"]=round(random.random()*60.0+20.0,1)
            queries["events"].append(k_shortest_H_query)
        else:
            approx_shortest_query={}
            approx_shortest_query["type"]="approx_shortest_path"
            approx_shortest_query["id"]=n+i
            no_of_tests=random.randint(1,10)
            approx_shortest_query["time_budget_ms"]=no_of_tests*5.0
            approx_shortest_query["acceptable_error_pct"]=round(random.random()*10.0+5.0,1)
            approx_queries=[]
            for j in range(no_of_tests):
                setj={}
                setj["source"]=random.randint(0,nodes_size-1)
                setj["target"]=random.randint(0,nodes_size-1)
                approx_queries.append(setj)
            approx_shortest_query["queries"]=approx_queries
            queries["events"].append(approx_shortest_query)
    directory_path="queries"
    os.makedirs(directory_path, exist_ok=True) 
    filepath=f"{directory_path}/phase2_queries.json"
    with open(filepath,"w",encoding="utf-8") as f:
        json.dump(queries,f,indent=2) 
    queries={}
    queries["meta"]="pahse3_queries"
    queries["events"]={}
    no_of_orders=random.randint(1,15)
    no_of_delivery_guys=random.randint(2,5)
    orders=[]
    for j in range(no_of_orders):
        order={}
        order["order_id"]=j
        order["pickup"]=random.randint(0,nodes_size-1)
        order["dropoff"]=random.randint(0,nodes_size-1)
        orders.append(order)
    fleet={}
    fleet["num_delivery_guys"]=no_of_delivery_guys
    fleet["depot_node"]=random.randint(0,nodes_size-1)
    # queries["events"].append(orders)
    # queries["events"].append(fleet)
    queries["events"]["orders"]=orders
    queries["events"]["fleet"]=fleet
    directory_path="queries"
    os.makedirs(directory_path, exist_ok=True) 
    filepath=f"{directory_path}/phase3_queries.json"
    with open(filepath,"w",encoding="utf-8") as f:
        json.dump(queries,f,indent=2) 


    
else:
    print("error: edges of given tag are not found")     

print("✅ Downloaded", len(nodes),len(edges), "OSM elements")