#include <queue>
#include <unordered_map>
#include <iostream>

struct RequestPair
{
    long addr;
    uint32_t type;
    uint32_t agent_id;
    uint32_t req_id;
    uint32_t agent_type;
};

class Generator
{
public:
    Generator(uint32_t num_cha_agents);
    // ~Generator();

    bool get_next_request(long &addr, uint32_t &agent_id, uint32_t &req_id);

    // TODO: setup priority queue
    std::queue<RequestPair> requests_to_send;

    std::vector<CHA *> agents;

    bool is_finished();
    bool finished = false;
    uint32_t total_requests_sent = 0;
    uint32_t total_instructions_sent = 0;

    // TODO: deal with cache
    // uint32_t total_l2_hits = 0;
    // uint32_t total_l3_hits = 0;
    uint64_t total_instructions_finished = 0;
    uint64_t max_total_instructions = 0;

    // TODO: set up output request log
    // std::map<uint32_t,RequestLog*> request_log;

    // void print_request_log(const char* logfile);
    // bool logged = false;
};

class CHA
{
public:
    CHA(uint32_t id, Generator *generator);

    uint32_t id;
    bool sendRequest();
    void create_request_pair(long addr, int ins_type);
    // void readArriveRemoteNUMA(Request& req);
    void readComplete(Request &req);
    void writeComplete(Request &req);
    void begin();

    long get_dram_clock_cycles_from_ns(double time_ns);
    long find_eviction_addr(long write_addr);

    uint32_t inflight_instructions;
    uint32_t max_inflight_instruction_limit;
    uint32_t served_instructions;
    uint32_t max_instructions_to_serve;
    uint32_t total_instructions;
    long read_req_lat = 70;  // read request agent to memory controller latency in DRAM clock cycles
    long write_req_lat = 70; // write request agent to memory controller latency in DRAM clock cycles

    long offset_rd_wr_bytes = 128 * 1024;               // 8MB
    long MAX_ADDRESSABLE_BYTE = 1 * 1024 * 1024 * 1024; // 1GB

    Generator *generator;
    // Trace *trace;
};

class LLCSlice
{
public:
    LLCSlice(int sliceId);

private:
    int sliceId;
    // address (string) -> yes/no in slice
    // 0: not inside slice
    // 1: yes inside slice
    std::unordered_map<std::string, int> llcSliceMap;
};

// **** OLD ****

class Tile
{
public:
    virtual ~Tile() {} // make it a pure virtual destructor for polymorphism

    // Gonna need other virtual functions: virtual int getTileType() const = 0;
};

class CoreTile : public Tile
{
public:
    // Constructor
    CoreTile(int coreId, int sliceId, int chaId);
    // Will return:
    // 0: if from memory
    // 1: if from LLC slice
    // 2: if from L2 cache
    int sendRequest(int memoryAddress);

private:
    // For us to identify what tile it is.
    int coreId;

    // The L2 cache represented by a queue.
    std::unordered_map<std::string, int> l2Cache;

    // LLC Slice
    LLCSlice *llcslice;

    // CHA
    CHA *cha;
};

class Request
{
public:
    bool is_first_command;
    long addr;
    // long addr_row;
    std::vector<int> addr_vec;
    // specify which core this request sent from, for virtual address translation
    int coreid;
    uint32_t req_id;

    enum class Type
    {
        READ,
        WRITE,
    } type;

    long arrive = -1;
    long depart = -1;
    std::function<void(Request &)> callback; // call back with more info

    Request(long addr, Type type, int coreid = 0)
        : is_first_command(true), addr(addr), coreid(coreid), type(type),
          callback([](Request &req) {}) {}
};
