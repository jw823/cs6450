#include <queue>
#include <unordered_map>
#include <iostream>
#include "tile.h"

// CHA ID -> Tile ID
// use static hash function for now to convert memory address to CHA ID
std::unordered_map<int, int> chaIdToTile;

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

    // instantiate: we need to figure out how to input "instructions"
    // CoreTile *coreZero = new CoreTile(0);
    // CoreTile *coreOne = new CoreTile(1);

    // adjusting fields, calling functions of class
    //  coreZero->myVar = 42;
    //  coreOne->myMethod();

    // Remember to delete the dynamically allocated object when done
    //  delete myObjPtr;
}
