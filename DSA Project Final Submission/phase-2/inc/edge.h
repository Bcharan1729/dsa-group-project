#include <vector>
#include <string>

class Edge{
    private:
        long long id;
        long long u;
        long long v;
        long double length;
        long double avg_time;
        std::string road_type;
        std::vector<double> speed_profile;
        bool valid;
        bool oneway;  
    public:
        Edge();
        Edge(long long id1,long long u1,long long v1,long double length1,long double avg_time1,std::string road_type1,std::vector<double> speed_profile1,bool oneway1);
        bool is_valid();
        std::pair<long long,long long> get_nodes();
        long long get_id();
        long double get_length();
        void set_valid(bool set_valid);
        long long get_another_node(long long idx);
        void change_length(long double new_length);
};