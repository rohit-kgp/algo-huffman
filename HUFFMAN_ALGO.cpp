#include "huffman.hpp"

void huffman ::createArr()
{
    for (int i = 0; i < 128; i++)
    {
        arr.push_back(new Node(i, 0));
    }
}

void huffman ::createMiniHeap()
{
    input.open("file.txt", ios::in);
    char ch;
    input.get(ch);

    while (!input.eof())
    {
        arr[ch]->freq++;
        input.get(ch);
    }

    input.close();

    for (int i = 0; i < 128; i++)
    {
        if (arr[i]->freq > 0)
        {
            q.push(arr[i]);
            initial_wt += 8 * arr[i]->freq;
        }
    }
}

void huffman ::createTree()
{
    Node *left, *right;
    priority_queue<Node *, vector<Node *>, compare> temp(q);
    while (temp.size() != 1)
    {
        left = temp.top();
        temp.pop();

        right = temp.top();
        temp.pop();

        root = new Node('#', left->freq + right->freq);
        root->left = left;
        root->right = right;
        temp.push(root);
    }

    root = temp.top();
}

void huffman::createCodes(Node *root, string s)
{
    if (root->left == NULL && root->right == NULL)
    {
        if (s.empty())
            s += '0';
        root->code = s;
        return;
    }

    createCodes(root->left, s + '0');
    createCodes(root->right, s + '1');
}

void huffman::getCodes()
{
    cout << "Char" << setw(11) << "Code" << setw(12) << "Freq\n\n";
    for (int i = 0; i < 128; i++)
    {
        if (arr[i]->freq != 0)
        {
            if (i == 10)
            {
                cout << "\\n " << setw(12) << arr[i]->code;
            }
            else if (i == 32)
            {
                cout << "Space " << setw(9) << arr[i]->code;
            }
            else if (i == 0)
            {
                cout << "Null " << setw(10) << arr[i]->code;
            }
            else
            {
                cout << arr[i]->data << setw(14) << arr[i]->code;
            }
            cout << setw(9) << arr[i]->freq << "\n";

            final_wt += arr[i]->freq * arr[i]->code.length();
        }
    }
}

void huffman::encodeTree(string &s, Node *r)
{
    if (isLeaf(r))
    {
        s += '1';
        s += r->data;
    }
    else
    {
        s += '0';
        encodeTree(s, r->left);
        encodeTree(s, r->right);
    }
}

void huffman::encode()
{
    output.open("tree.txt", ios::out);
    input.open("file.txt", ios::in);

    string temp;
    encodeTree(temp, root);

    output << temp;
    output.close();
    output.open("encode.txt", ios::out);
    char c;
    input.get(c);
    while (!input.eof())
    {
        output << arr[c]->code;
        input.get(c);
    }
    output.close();
    input.close();
}

int idx = -1;
Node *huffman::decodeTree(string s)
{
    ++idx;
    if (idx == s.length())
    {
        idx--;
        return NULL;
    }

    if (s[idx] == '1')
    {
        idx++;
        return new Node(s[idx], 0);
    }
    else
    {
        Node *leftChild = decodeTree(s);
        Node *rightChild = decodeTree(s);
        Node *node = new Node('#', 0);
        node->left = leftChild;
        node->right = rightChild;
        return node;
    }
}

void huffman::decode()
{
    input.open("tree.txt", ios::in);
    output.open("decode.txt", ios::out);
    string s, tree;
    while (getline(input, s))
    {
        tree.append(s);
        tree.push_back('\n');
    }

    for (auto i : s)
    {
        if (i == '0' || i == '1')
            final_wt += 1;
        else
            final_wt += 8;
    }

    input.close();
    Node *node = decodeTree(tree);
    Node *head = node;

    input.open("encode.txt", ios::in);
    bool single = (node->data != '#');

    char ch;
    input.get(ch);
    while (!input.eof())
    {
        if (node->data != '#')
        {
            output << node->data;
            node = head;
            if (single)
                input.get(ch);
        }
        else if (ch == '0')
        {
            node = node->left;
            input.get(ch);
        }
        else if (ch == '1')
        {
            node = node->right;
            input.get(ch);
        }
    }
    if (!single)
        output << node->data;
    input.close();
    output.close();
}

int main()
{
    cout << "\n";
    cout << "*******************************\n";
    cout << "*******************************\n";
    cout << "\n";
    huffman h;
    h.createArr();
    h.createMiniHeap();
    h.createTree();
    h.createCodes(h.root, "");
    h.getCodes();
    h.encode();
    h.decode();
    cout << "\n";
    cout << "*******************************\n";
    cout << "*******************************\n";
    cout << "\n";
    cout << "Given file size: " << setw(15) << h.initial_wt << " bits\n";
    cout << "Compressed file size: " << setw(10) << h.final_wt << " bits\n";
    cout << "Percentage compression: " << setw(10) << setprecision(7) << (1 - (h.final_wt / (double)h.initial_wt)) * 100 << "\n";
    cout << "\n";
    return 0;
}