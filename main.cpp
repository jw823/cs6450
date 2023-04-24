#include <queue>
#include <unordered_map>
#include <iostream>
#include "main.h"



int main()
{
    std::cout << "Hello World" << std::endl;

    // instantiate: we need to figure out how to input "instructions"
    CoreTile *coreZero = new CoreTile(0);
    CoreTile *coreOne = new CoreTile(1);

    // adjusting fields, calling functions of class
    //  coreZero->myVar = 42;
    //  coreOne->myMethod();

    // Remember to delete the dynamically allocated object when done
    //  delete myObjPtr;
}
