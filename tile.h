#include <queue>
#include <unordered_map>
#include <iostream>

class CHA
{
public:
    CHA(int chaId);

private:
    int chaId;
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

class Tile
{
public:
    virtual ~Tile() {} // make it a pure virtual destructor for polymorphism

    // Gonna need other virtual functions: virtual int getTileType() const = 0;
};

class CoreTile
{
public:
    // Constructor
    CoreTile(int coreId, int sliceId, int chaId);
    // Will return:
    // 0: if from memory
    // 1: if from LLC slice
    // 2: if from L2 cache
    int sendRequest();

private:
    // For us to identify what tile it is.
    int coreId;

    // The L2 cache represented by a queue.
    std::unordered_map<std::string, int> l2Cache;

    // LLC Slice
    LLCSlice* llcslice;

    // CHA
    CHA* cha;
};
