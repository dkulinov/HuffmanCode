#include <iostream>
using namespace std;

struct heapNode
{
    string theCode = "";
    char letter;
    int frequency;
    heapNode* left=nullptr, *right=nullptr;
    heapNode(){};
    heapNode(char l, int f){letter=l; frequency=f;};
};
// size of heap should be 2*numCharacters-1 + 1 for the first blank element
// contains the original array of characters and frequencies
// create an array of strings of size 256, the ascii value element will have the code and the others will be empty 
class minHeap
{
    public:
        int capacity=0;
        int size=0;
        heapNode** heap = nullptr; // heap[0] is a placeholder
        void createArray(string given);
        void printHeap();
        void heapify();
        void buildHeap(); //inserting all nodes
        void insert(heapNode* temp);
        void heapSort();
        heapNode* remove();
};


class huffman
{
    public:
        heapNode* root = nullptr;
        minHeap theHeap;
        string original;
        huffman(string given);
        void assignCodes(heapNode* cu);
        void printTree(heapNode* cu);
        void encode();
        void decode();
        string encoded;
        string codesByASCII[256] = {""};
        void deleteAll(heapNode* cu);
        ~huffman();
};

int main(int argc, char* argv[])
{
    string given = "";
    cout << "Please provide a string:" << endl;
    cin >> given;
    huffman tree(given);
    tree.assignCodes(tree.root);
    tree.encode();
    tree.decode();
    return 0;
}

// O(n) complexity
void minHeap::createArray(string given)
{
    int counts[256] = {0};
    for(int i=0; i<given.length(); i++)
    {
        if(counts[(int)given[i]]==0)
        {
            capacity++;
            size++;
        }
        counts[(int)given[i]]++;
    }
    capacity *= 2;
    heap = new heapNode*[capacity];
    for(int i=0; i<capacity; i++)
        heap[i] = nullptr;
    int j=1; // used for the heap 
    for(int i=0; i<256; i++)
    {
        if(counts[i]>0)
        {
            heapNode* temp = new heapNode;
            temp->letter = (char) i;
            temp->frequency = counts[i];
            heap[j] = temp;
            j++;
        }
    }
}

void minHeap::printHeap()
{
    for(int i=1; i<=size; i++)
        cout << "Char " << heap[i]->letter << " has a frequency of " << heap[i]->frequency << endl;
}
// O(n) complexity
void minHeap::heapify()
{
    for(int i=size/2; i>0; i--)
    {
        int j = i;
        int child = 2*j;
        while(child <= size)
        {
            if(child+1<=size && heap[child+1]->frequency < heap[child]->frequency)
                child++;
            if(heap[child]->frequency < heap[j]->frequency)
            {
                heapNode* helper = heap[j];
                heap[j] = heap[child];
                heap[child] = helper;
                j = child;
                child = j*2;
            }
            else 
                break;
        }
    }
}

// O(log(n))
void minHeap::insert(heapNode* temp)
{
    if(size == capacity)
        cout << "Heap is full" << endl;
    else
    {
        int index = size + 1;
        while(index>1 && heap[index/2]->frequency > temp->frequency)
        {
            heapNode* helper = heap[index];
            heap[index] = heap[index/2];
            heap[index/2] = helper;
            index /= 2;
        }
        heap[index] = temp;
        size++;
    }
}

// O(log(n)) 
heapNode* minHeap::remove()
{
    if(size == 0)
    {
        cout << "Heap is empty" << endl;
        return nullptr;
    }
    else 
    {
        heapNode* temp = heap[1]; // value to be deleted
        heap[1] = heap[size]; 
        heap[size] = temp;
        size--;
        int index = 1;
        int child = 2*index;
        while(child <= size)
        {
            if(child+1<=size && heap[child+1]->frequency < heap[child]->frequency)
                child++;
            if(heap[child]->frequency < heap[index]->frequency)
            {
                heapNode* helper = heap[index];
                heap[index] = heap[child];
                heap[child] = helper;
                index = child;
                child = index*2;
            }
            else 
                break;
        }
        return temp;
    }
}

// O(nlog(n))
huffman::huffman(string given)
{
    original = given;
    theHeap.createArray(given);
    theHeap.heapify();
    // theHeap.buildHeap();
    heapNode *min1 = nullptr, *min2 = nullptr, *parent = nullptr; 
    while(theHeap.size!=1)
    {
        parent = new heapNode;
        heapNode* temp;
        temp = theHeap.remove();
        min1 = temp;
        temp = theHeap.remove();
        min2 = temp;
        parent->frequency = min1->frequency + min2->frequency;
        parent->letter = '^'; // placeholder
        parent->left = min1;
        parent->right = min2;
        theHeap.insert(parent);
    }
    root = theHeap.remove();
}

// pass root
// also assigns the codes to the vars
// O(n)
void huffman::assignCodes(heapNode* cu)
{
    if(cu)
    {
        static string code = "";
        if(!cu->left && !cu->right)
        {
            cu->theCode = code;
            cout << "Char " << cu->letter << ", freq: " << cu->frequency << ", huffman code: " << cu->theCode << endl;
            codesByASCII[(int)cu->letter] = cu->theCode;
        }
        else
        {
            code.append("0");
            assignCodes(cu->left);
            code.pop_back();
            code.append("1");
            assignCodes(cu->right);
            code.pop_back();
        }
    }
}

// O(nlog(n)) but isnt called
void minHeap::buildHeap()
{
    int ogSize = size;
    size = 1;
    for(size; size<ogSize; size++)
        insert(heap[size+1]);
}

// O(n)
void huffman::encode()
{
    for(int i=0; i<original.length(); i++)
        encoded.append(codesByASCII[(int)original[i]]);
    cout << "Encoded message: " << encoded << endl;
}

// O(nlog(n))
void huffman::decode()
{
    cout << "Decoded message: " ;
    heapNode* temp = root;
    for(int i=0; i<encoded.length(); i++)
    {
        if(encoded[i]=='0')
            temp = temp->left;
        else if(encoded[i]=='1')
            temp = temp->right;
        if(!temp->left && !temp->right)
        {
            cout << temp->letter;
            temp = root;
        }
    }
    cout << endl;
}

void huffman::deleteAll(heapNode* cu)
{
    if(cu)
    {
        deleteAll(cu->left);
        deleteAll(cu->right);
        delete cu;
    }
}

huffman::~huffman()
{
    deleteAll(root);
    delete theHeap.heap;
}

void huffman::printTree(heapNode* cu)
{
    if(cu)
    {
        printTree(cu->left);
        cout << "Char " << cu->letter << " , freq= " << cu->frequency << endl;
        printTree(cu->right);
    }
}
