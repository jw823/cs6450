#include <queue>
#include <unordered_map>
#include <iostream>

int main()
{
    std::cout << "Hello World" << std::endl;

    // instiatite
    CoreTile *coreZero = new CoreTile(0);
    CoreTile *coreOne = new CoreTile(1);

    // adjusting fields, calling functions of class
    //  coreZero->myVar = 42;
    //  coreOne->myMethod();

    // Remember to delete the dynamically allocated object when done
    //  delete myObjPtr;
}

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
    CoreTile(int coreId)
    {
        this->coreId = coreId;
    }

private:
    // For us to identify what tile it is.
    int coreId;

    // The L2 cache represented by a queue.
    std::unordered_map<std::string, int> l2Cache;

    // LLC Slice
    LLCSlice LLCSlice;
};

class LLCSlice
{
public:
    // Constructor
    LLCSlice(int sliceId)
    {
        this->sliceId = sliceId;
    }

private:
    int sliceId;
    // address (string) -> yes/no in slice
    // 0: not inside slice
    // 1: yes inside slice
    std::unordered_map<std::string, int> llcSliceMap;
    CHA cha;
};

class CHA
{
public:
    // Constructor
    CHA()
    {
    }

private:
    int chaId;
    std::unordered_map<int, int> chaIdToTile;
};