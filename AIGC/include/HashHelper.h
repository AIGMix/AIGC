#pragma once

#include <string>
#include <vector>
#include <map>

namespace aigc
{
template<typename TVALUE>
class HashHelper
{
public:
    HashHelper(int capacity);
    ~HashHelper();
    
    bool Add(std::string key, TVALUE value);
    bool Remove(std::string key);
    TVALUE& Get(std::string key);

private:
    typedef struct NodeStruct
    {
        int hash;
        std::string key;
        TVALUE value;
        struct NodeStruct *next;
    }Node;

    int CalcCapacity(int capacity, int& cmpCapacity);
    int CalcHash(std::string key);
    int CalcSeatIndex(int hash);

    void ExpandIfNecessary();

    Node* CreatNode(int hash, std::string key, TVALUE value);
    void RemoveNode(Node* node);


    int m_capacity;
    int m_cmpCapacity;
    int m_count;
    Node** m_array;
};

} // namespace aigc
