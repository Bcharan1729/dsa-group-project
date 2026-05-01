#include <vector>
#include <string>

class Node{
    private:
        long long id;
        bool valid;
        float lat;
        float log;
        std::vector<std::string> pois;
    public:
        Node();
        Node(long long id1,float lat1,float log1 ,std::vector<std::string>pois1);
        bool is_valid();
        long long get_id();
        std::pair<float,float> get_position();
        void set_valid(bool set_to);
        std::vector<std::string> get_pois();
};