#ifndef _MANUAL_H_
#define _MANUAL_H_

#include <vector>
#include "tree/tree.hh"

#define DESC_ONLY
#define DESC_WITH_BRO
#define BRO_ONLY
#define LEAF

class Manual{
public:
    Manual()
    int readSGF(const string &filename);
    int readLIB()
private:
    vector<char> moveSequence;
    tree<char> moveTree;

};

#endif // _MANUAL_H_
