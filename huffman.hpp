#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <climits>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <iomanip>
#include <fstream>
using namespace std;

struct Node
{
    char data;
    int freq;
    string code;
    Node *left, *right;

    Node(char ch, int fr)
    {
        data = ch;
        freq = fr;
        left = NULL;
        right = NULL;
    }
};

class huffman
{
public:
    vector<Node *> arr;
    fstream input, output, out_ascii;
    Node *root;
    Node *top;

    class compare
    {
    public:
        bool operator()(Node *a, Node *b)
        {
            return a->freq > b->freq;
        }
    };

    bool isLeaf(Node *r)
    {
        if (!r->left && !r->right)
            return true;

        return false;
    }
    int initial_wt = 0;
    int final_wt = 0;
    priority_queue<Node *, vector<Node *>, compare> q;
    void createArr();
    void createMiniHeap();
    void createTree();
    void createCodes(Node *root, string s);
    void getCodes();
    void encode();
    void encodeTree(string &s, Node *r);
    void decode();
    Node *decodeTree(string s);
};

#endif