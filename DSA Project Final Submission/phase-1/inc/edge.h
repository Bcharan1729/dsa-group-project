#include<string>
#include<vector>

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
        //long double speed_limit;    
    public:
        Edge();
        Edge(long long id1,long long u1,long long v1,long double length1,long double avg_time1,std::string road_type1,std::vector<double> speed_profile1,bool oneway1);
        bool is_valid();
        std::pair<long long,long long> get_nodes();
        long long get_id();
        long double get_length();
        long double get_avgtime();
        std::string get_roadtype();
        void set_valid(bool set_valid);        
        long long get_another_node(long long idx);
        void change_length(long double new_length);
        void change_avgtime(long double new_avgtime);
        void change_roadtype(std::string new_roadtype);
        void change_speedprofile(std::vector<double> new_speedprofile);
        long double time_taken(long double time_before);

};