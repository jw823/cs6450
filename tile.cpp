#include <queue>
#include <unordered_map>
#include <iostream>
#include "tile.h"

// TODO: leave a comment at the top expressing our constraints
// TODO: create the wrapper function
// TODO: fix the build errors and define the classes needed
// TODO: add the tuple for the coordinate and calculate the time for routing

// Both: investigate run_wrapper in main, and take notes on how to implement it
// Both: take notes on how to calculate the distance
// Both: brainstorm how to get around contention
// Bahar: define generator class and fix related functions
// Jessie: define CHA class based off of RequestAgent class (RequestAgent.h); all classes go in tile.h and fix related functions

// TODO: run the simulation on extremely constraint example

#define MAX_INSTRUCTION_LIMIT_PER_AGENT 100000

Generator::Generator(uint32_t num_cha_agents)
{
    // TODO: Need to create an agent type for IMC tiles
    // These tiles still need to communicate with the Core tiles
    this->max_total_instructions = std::min((uint32_t)500000, (cha_agents) * (MAX_INSTRUCTION_LIMIT_PER_AGENT));

    // TODO: initialize the LLC with randomly generated dirty lines
    // For now, we assume all LLC's are empty and all requests are fresh

    // TODO: initialize time to the cycles (whatever we figure it out to be)

    // Create and initialize CHAAgents
    // Add them to requests vector in Generator

    // ID for agent will be the tuple which is the (x, y) coordinate of the tile it's in
    for (int i = 0; i < num_cha_agents; i++)
    {
        this->agents.push_back(new CHA(0, i, /* (x, y) */, this));
    }
}

CHA::CHA(uint32_t type, uint32_t id, /* tuple coordinate */ Generator *generator)
{
    this->id = id;
    this->type = type;
    this->max_inflight_instruction_limit = CPU_INFLIGHT_REQUEST_LIMIT;
    this->max_instructions_to_serve = MAX_INSTRUCTION_LIMIT_PER_AGENT;
    this->inflight_instructions = 0;
    this->served_instructions = 0;
    this->total_instructions = 0;
    this->generator = generator;

    // TODO: need to output a request log after simulation is complete

    // TODO: need to create a trace to pass in instructions
    // this->trace = new Trace(tracefilename.c_str());
}

void CHA::begin()
{
    assert(this->== 0);
    assert(this->served_instructions == 0);
    assert(this->total_instructions == 0);
    // assert(this->generator->total_instructions_sent == 0);

    while (this->inflight_instructions < this->max_inflight_instruction_limit && (this->total_instructions < this->max_instructions_to_serve) && (this->generator->total_instructions_sent < this->generator->max_total_instructions))
    {
        this->sendRequest();
    }
}

RequestAgent::sendRequest()
{

    // TODO: currently, we hardcode the addr and ins_type
    // We will get that from the trace later
    // We also only process read requests, and write will be added later
    // We will add a request type to process load instructions because they require a write for eviction, and read for allocation

    long addr = 1;
    int ins_type = 0;
    this->create_request_pair(addr, ins_type);

    this->inflight_instructions++;
    this->total_instructions++;
    this->generator->total_instructions_sent++;
    return true;
}

void CHA::create_request_pair(long addr, int ins_type)
{
    uint32_t req_id = this->generator->total_requests_sent;
    // TODO: figure out how we want to handle current time
    // long cur_time = this->generator->some cycle counter;
    // We also hardcode the request latency for now
    long req_lat = this->read_req_lat;
    std::tuple<long, uint32_t, long, uint32_t> tuple = std::make_tuple(addr, this->id, req_id, /* cur_time */ req_lat);
    this->generator->total_requests_sent++;
    this->generator->requests_to_send.push(tuple);

    // TODO: add to request log once pair has been added to queue
    // assert(this->generator->request_log.find(req_id) == this->generator->request_log.end());
    // this->generator->request_log.insert(std::pair<uint32_t,RequestLog*>(req_id,new RequestLog(req_id,this->id,this->type,cur_time,-1,req_type,ins_type,addr)));
}

bool Generator::get_next_request(long &addr, uint32_t &agent_id, uint32_t &req_id)
{

    if (this->requests_to_send.size() > 0)
    {
        auto pair = this->requests_to_send.top();
        // TODO: set the current time based off of something
        // long cur_time = this->memory_dram_cycles;
        // long req_creation_time = std::get<4>(pair);

        // TODO: We need to calculate time based off Y-X routing
        // If Tile (0,0) needs to check the LLC of (1, 1),
        // then it needs to travel on one link in the Y direction, then one link in the X direction,
        // Time is based off of 2 factors:
        // Distance and contention

        // TODO: fix indexing for all pair gets
        // long req_lat = std::get<5>(pair);

        // TODO: implement the check on time
        // if (cur_time >= req_creation_time + req_lat)
        
        addr = std::get<0>(pair);
        type = std::get<1>(pair);
        agent_id = std::get<2>(pair);
        req_id = std::get<3>(pair);
        agent_type = std::get<7>(pair);
        this->requests_to_send.pop();
        return true;
        
    }
}

// ************************** OLD CLASSES *************************** //

// CHA ID -> Tile ID
// use static hash function for now to convert memory address to CHA ID
std::unordered_map<int, int> chaIdToTile;

// Priority queue where requests will be pushed when they are sent
// initialize here

CHA::CHA(int chaId)
{
    this->chaId = chaId;
}

LLCSlice::LLCSlice(int sliceId)
{
    this->sliceId = sliceId;
    this->llcSliceMap = {};
}

CoreTile::CoreTile(int coreId, int sliceId, int chaId)
{
    this->coreId = coreId;
    this->l2Cache = {};
    this->llcslice = new LLCSlice(sliceId);
    this->cha = new CHA(chaId);
}

// communicating between tiles: use their coordinates
// rn we need to write the sendRequest function - i think it needs to create a request object and add it to the queue
// request object: (request id, tile id)
// basically, it should get an instruction, and then it will compute the cha ID using hash function, then figure out what tile to go to
//
int CoreTile::sendRequest(int memoryAddress)
{
    // use static hash function to turn memory addr into cha id
    // use map from cha id to get corresponding tile id
    // create request object with request id and tile id
    // add to queue
}

int main()
{
    std::cout << "Hello World" << std::endl;
    const int ROWS = 2;
    const int COLS = 2;

    Tile tiles[ROWS][COLS];

    CoreTile coreTile(0, 1, 2);
    Tile *tileOnePtr = new CoreTile(0, 1, 2);
    Tile *tileTwoPtr = new CoreTile(1, 2, 3);

    // Assign a value to an element of the array
    tiles[0][0] = *tileOnePtr;

    // Assign a value to an element of the array
    tiles[0][1] = *tileTwoPtr;

    // Access the value of an element of the array
    Tile objOne = tiles[0][0];
    Tile objTwo = tiles[0][1];

    // Loop through the elements of the array
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            // Do something with myArray[i][j]
        }
    }
    // instantiate: we need to figure out how to input "instructions"
    // CoreTile *coreZero = new CoreTile(0);
    // CoreTile *coreOne = new CoreTile(1);

    // adjusting fields, calling functions of class
    //  coreZero->myVar = 42;
    //  coreOne->myMethod();

    // Remember to delete the dynamically allocated object when done
    //  delete myObjPtr;
}
