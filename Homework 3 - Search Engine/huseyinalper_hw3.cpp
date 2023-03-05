//
//
// CS300 HW #03 - Search Engine
// Huseyin Alper Karadeniz
// huseyinalper (00028029)
// Sabanci University 2022
//
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <cctype>
using namespace std;

struct DocumentItem {
    // Document item storing the name of the file, and the number of occurrences of the word in that file
    string documentName;
    int count;
    DocumentItem(const string theDocumentName, const int theCount = 0): documentName(theDocumentName), count(theCount) {}
    DocumentItem(): documentName(""), count(0) {}
};
bool operator==(const DocumentItem & lhs, const DocumentItem & rhs) {return (lhs.documentName == rhs.documentName);}
bool operator!=(const DocumentItem & lhs, const DocumentItem & rhs) {return (lhs.documentName != rhs.documentName);}

struct WordItem {
    // Word item storing the word, and the documents they exist as document items
    string word;
    vector<DocumentItem> docs;
    WordItem(const string theWord, const vector<DocumentItem> theDocs): word(theWord), docs(theDocs) {}
    WordItem(): word("") {}
};
bool operator==(const WordItem & lhs, const WordItem & rhs) {return (lhs.word == rhs.word);}
bool operator!=(const WordItem & lhs, const WordItem & rhs) {return (lhs.word != rhs.word);}
bool operator>(const WordItem & lhs, const WordItem & rhs) {return (lhs.word > rhs.word);}
bool operator<(const WordItem & lhs, const WordItem & rhs) {return (lhs.word < rhs.word);}

// Hash Table Operations Start

bool isPrime(int n) {
    // Checks if a given integer is prime number, or not
    if (n == 2 || n == 3) {return true;}
    if (n == 1 || (n % 2) == 0) {return false;}
    for (int i = 3; i * i <= n; i+= 2) {
        if (n % i == 0) {return false;}
    }
    return true;
}

int nextPrime(int n) {
    // Finds next prime number from an integer
    if (n % 2 == 0) {n++;}
    for (; !isPrime(n); n += 2) {;}
    return n;
}

int hashingFunc(const WordItem & key, int tableSize) {
    // Hashes WordItem objects according to their word
    int hashVal = 0;
    for (unsigned int i = 0; i < key.word.length(); i++) {
        hashVal = 37 * hashVal + key.word[i];
    }
    hashVal = hashVal % tableSize;
    if (hashVal < 0) { // Fixes negative hash value results
        hashVal = hashVal + tableSize;
    }
    return(hashVal);
}

template <class HashedObj>
class HashTable {
public:
    explicit HashTable(const HashedObj & notFound, int size = 101): ITEM_NOT_FOUND(notFound), array(nextPrime(size)), currentSize(0) {makeEmpty();}
    HashTable(const HashTable & rhs): ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array), currentSize(rhs.currentSize) {}
    enum EntryType {ACTIVE, EMPTY, DELETED};
    
    const HashedObj & find(const HashedObj & x) const {
        // Finds an object inside of the hash table, returns its copy
        int currentPos = findPos(x);
        if (isActive(currentPos)) {
            return array[currentPos].element;
        }
        return ITEM_NOT_FOUND;
    }
    
    HashedObj & getOriginal(const HashedObj & x) {
        // Finds an object inside of the hash table, returns its original
        int currentPos = findPos(x);
        if (isActive(currentPos)) {
            return array[currentPos].element;
        }
        return ITEM_NOT_FOUND;
    }
    
    void makeEmpty() {
        // Deletes all elements in the hash table so that makes it empty
        for (unsigned int i = 0; i < array.size(); i++) {
            array[i].info = EMPTY;
        }
        currentSize = 0;
    }
    
    void insert(const HashedObj & x) {
        // Inserts a new element into the hash table
        int currentPos = findPos(x);
        if (isActive(currentPos)) {
            return;
        }
        array[currentPos] = HashEntry(x, ACTIVE);
        if (++currentSize >= array.size() / 1.25) { // Determines the maximum load factor
            rehash();
        }
    }
    
    void remove(const HashedObj & x) {
        // Finds and removes a given object inside of the hash table
        int currentPos = findPos(x);
        if (isActive(currentPos)) {
            array[currentPos].info = DELETED;
        }
    }
    
    const HashTable & operator=(const HashTable & rhs) {
        // Copies the hash table in the right-hand size to the hash table
        if (this != &rhs){
            makeEmpty();
            for (int i = 0; i < rhs.array.size(); i++) {
                array[i] = rhs.array[i];
            }
        }
    }
    
    void finalPrint() {
        cout << endl << "After preprocessing, the unique word count is " << currentSize << ". Current load ratio is " << float(currentSize) / float(array.size()) << endl;
    }
private:
    struct HashEntry {
        HashedObj element;
        EntryType info;
        HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY): element(e), info(i) {}
    };
    vector<HashEntry> array;
    int currentSize;
    HashedObj ITEM_NOT_FOUND;
    
    bool isActive(int currentPos) const { // Checks if the element in the current position is active, or not
        return array[currentPos].info == ACTIVE;
    }
    
    int findPos(const HashedObj & x) const { // Finds the position of a given object inside of the hash table
        int collisionNum = 0;
        int currentPos = hashingFunc(x, int(array.size()));
        while (array[currentPos].info != EMPTY && array[currentPos].element != x) {
            currentPos += ((2 * ++collisionNum) - 1);
            if (currentPos >= int(array.size())) {
                currentPos = currentPos % array.size();
            }
        }
        return currentPos;
    }
    
    void rehash() { // Rehashes the hash table according to target resulting size
        vector<HashEntry> oldArray = array;
        array.resize(nextPrime(2 * int(oldArray.size())));
        for (unsigned int j = 0; j < array.size(); j++) {
            array[j].info = EMPTY;
        }
        currentSize = 0;
        for (unsigned int i = 0; i < oldArray.size(); i++) {
            if (oldArray[i].info == ACTIVE) {
                insert(oldArray[i].element);
            }
        }
        cout << "rehashed..." << endl << "previous table size: " << oldArray.size() << ", " << "new table size: " << array.size() << ", " << "current unique word count " << currentSize << ", " << "current load factor: " << float(currentSize) / float(array.size()) << endl;
    }
};

// Hash Table Operations End

// Binary Search Tree Operations Start

template <class itemType>
class BinarySearchTree;

template <class itemType>
class binaryNode {
public:
    binaryNode(const itemType & theElement): element(theElement), left(NULL), right(NULL) {}
    binaryNode(const itemType & theElement, binaryNode * ln, binaryNode * rn): element(theElement), left(ln), right(rn) {}
    friend class BinarySearchTree<itemType>;
private:
    itemType element;
    binaryNode * left;
    binaryNode * right;
};

template <class itemType>
class BinarySearchTree {
public:
    explicit BinarySearchTree(const itemType & notFoundValue): root(NULL), ITEM_NOT_FOUND(notFoundValue) {}
    BinarySearchTree(const BinarySearchTree & rhs): root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND) {*this = rhs;}
    ~BinarySearchTree() {makeEmpty();}
    const itemType & findMin() const {
        return elementAt(findMin(root));
    }
    const itemType & findMax() const {
        return elementAt(findMax(root));
    }
    const itemType & find(const itemType & x) const {
        return elementAt(find(x, root));
    }
    bool isEmpty() const {
        return (root == NULL);
    }
    void printTree() const {
        if (!isEmpty()) {
            printTree(root);
        }
    }
    void makeEmpty() {
        makeEmpty(root);
    }
    void insert(const itemType & x) {
        insert(x, root);
    }
    void remove(const itemType & x) {
        remove(x, root);
    }
    const BinarySearchTree & operator=(const BinarySearchTree & rhs) {
        if (this != &rhs) {
            makeEmpty();
            root = clone(rhs.root);
        }
        return *this;
    }
    itemType & getOriginal(const itemType & x) {
        // Returns the original element to edit
        return find(x, root)->element;
    }
    bool existenceCheck(const itemType & x) {
        if (find(x, root) == NULL) {
            return false;
        } else {
            return true;
        }
    }
private:
    binaryNode<itemType> * root;
    const itemType ITEM_NOT_FOUND;
    
    const itemType & elementAt(binaryNode<itemType> * t) const {
        if (t == NULL) { // If the item is null, then returns the not found value
            return ITEM_NOT_FOUND;
        } else { // Returns the element inside of the item
            return t->element;
        }
    }
    void insert(const itemType & x, binaryNode<itemType> * & t) const {
        // Inserts a new node inside of the tree
        if (t == NULL) { // Creates a new node
            t = new binaryNode<itemType>(x);
        } else if (x < t->element) {
            insert(x, t->left); // Inserts into left subtree
        } else if (t->element < x) {
            insert(x, t->right); // Inserts into right subtree
        } else {
            ; // Already exists
        }
    }
    void remove(const itemType & x, binaryNode<itemType> * & t) const {
        if (t == NULL) {
            return; // Cannot be found
        }
        if (x < t->element) {
            remove(x, t->left); // Moves to left subtree
        } else if (t->element < x) {
            remove(x, t->right); // Moves to right subtree
        } else if (t->left != NULL && t->right != NULL) { // Has two children
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        } else { // Has one or no children
            binaryNode<itemType> * oldNode = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete oldNode;
        }
    }
    binaryNode<itemType> * find(const itemType & x, binaryNode<itemType> * t) const {
        // Returns the target node
        while (t != NULL) {
            if (x < t->element) {
                t = t->left;
            } else if (t->element < x)
                t = t->right;
            else {
                return t; // Found
            }
        }
        return NULL; // Not found
    }
    binaryNode<itemType> * findMin(binaryNode<itemType> * t) const {
        // Finds the minimum
        if (t != NULL) {
            while (t->left != NULL) {
                t = t->left;
            }
        }
        return t;
    }
    binaryNode<itemType> * findMax(binaryNode<itemType> * t) const {
        // Finds the maximum
        if (t != NULL) {
            while (t->right != NULL) {
                t = t->right;
            }
        }
        return t;
    }
    void makeEmpty(binaryNode<itemType> * & t) const {
        // Deletes all elements in the tree
        if (t != NULL) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = NULL;
    }
    void printTree(binaryNode<itemType> * t) const {
        // Prints elements in tree alphabetically, case-insensitive
        if (t != NULL) {
            printTree(t->left); // Prints left subtree
            cout << t->element << endl; // Prints middle
            printTree(t->right); // Prints right subtree
        }
    }
    binaryNode<itemType> * clone(binaryNode<itemType> * t) const {
        // Creates a clone recursively
        if (t == NULL) {
            return NULL;
        } else {
            return new binaryNode<itemType> (t->element, clone(t->left), clone(t->right));
        }
    }
};

// Binary Search Tree Operations End

vector<string> getInputVector(string completeInput) {
    // Gets an input line and returns the inputs inside of it
    vector<string> resultVector;
    string input = "";
    for (unsigned int i = 0; i < completeInput.size(); i++) { // Creates and then adds inputs to the vector
        char currentChar = completeInput[i];
        if (currentChar >= 'a' && currentChar <= 'z') {
            input += currentChar; // Adds lowercase char directly
        } else if (currentChar >= 'A' && currentChar <= 'Z') {
            input += (currentChar - ('A'-'a')); // Adds uppercase char by converting it to lowercase first
        } else {
            if (input != "") { // Adds the input to the vector
                resultVector.push_back(input);
                input = "";
            }
        }
    }
    if(input != "") { // Adds the last input to the vector
        resultVector.push_back(input);
    }
    return resultVector;
}

void printResults(const vector<string> & fileNames, const vector<string> & wordInputs, const vector<vector<int>> & resultVectors) {
    // Prints results according to target words and resulting vectors of the BST/HT
    vector<char> printableDocs; // Stores the file indices that will be printed
    for (unsigned int i = 0; i < fileNames.size(); i++) {
        printableDocs.push_back('y'); // y for yes, will be printed at the end
        for (unsigned int j = 0; j < wordInputs.size(); j++) {
            if (resultVectors[i][j] == 0) { // There is a target word that doesn't exist in the BST/HT
                printableDocs[i] = 'n'; // n for no, won't be printed at the end
            }
        }
    }
    bool printed = false;
    for (unsigned int i = 0; i < fileNames.size(); i++) {
        if (printableDocs[i] == 'y') { // If all target words exist, its printable, not otherwise
            cout << "in Document " << fileNames[i];
            for (unsigned int j = 0; j < wordInputs.size(); j++) {
                cout << ", " << wordInputs[j] << " found " << resultVectors[i][j] << " times";
            }
            cout << "." << endl;
            printed = true;
        }
    }
    if (!printed) { // No file to print since none of them have all target words
        cout << "No document contains the given query" << endl;
    }
}

int main() {
    
    // Getting Input File Infos
    int fileNumber;
    string fileName;
    vector<string> fileNames;
    cout << "Enter number of input files: ";
    cin >> fileNumber; // Gets the number of files to be used
    
    for (int i = 1; i <= fileNumber; i++){
        cout << "Enter " << i << ". file name: ";
        cin >> fileName; // Gets the names of each file
        fileNames.push_back(fileName); // Adds filenames into a vector
    }
    
    // Reading Input Files
    vector<WordItem> words;
    vector<vector<string>> allInputs;
    
    DocumentItem notFoundDocumentItem("");
    vector<DocumentItem> notFoundVector;
    notFoundVector.push_back(notFoundDocumentItem);
    WordItem notFoundWordItem("", notFoundVector);
    HashTable<WordItem> hashTable(notFoundWordItem);
    BinarySearchTree<WordItem> binarySearchTree(notFoundWordItem);
    
    for (int i = 0; i < fileNumber; i++) {
        ifstream file;
        string fileName = fileNames[i];
        file.open(fileName); // Opens the file to be read
        if (file.is_open()) {
            string input;
            while (!file.eof()) { // Reads till the file ends
                getline(file, input);
                vector<string> inputs = getInputVector(input); // Gets all inputs in the line
                if (inputs.size() != 0) { // Adds if the vector is not empty
                    allInputs.push_back(inputs);
                    for (unsigned int j = 0; j < inputs.size(); j++) {
                        string input = inputs[j];
                        DocumentItem inputDocumentItem(fileName, 1);
                        vector<DocumentItem> inputDocVector;
                        inputDocVector.push_back(inputDocumentItem);
                        WordItem inputWordItem(input, inputDocVector);
                        WordItem * resultingWordItemHT = &hashTable.getOriginal(inputWordItem);
                        WordItem * resultingWordItemBST = &binarySearchTree.getOriginal(inputWordItem);
                        if (*resultingWordItemHT != notFoundWordItem) { // Target word already exists in the table
                            vector<DocumentItem> * docsInHT = &resultingWordItemHT->docs;
                            vector<DocumentItem> * docsInBST = &resultingWordItemBST->docs;
                            bool docFound = false;
                            for (unsigned int k = 0; k < docsInHT->size(); k++) {
                                DocumentItem * docHT = &docsInHT->at(k);
                                DocumentItem * docBST = &docsInBST->at(k);
                                if (docHT->documentName == fileName) {
                                    docFound = true;
                                    docHT->count += 1;
                                    docBST->count += 1;
                                }
                            }
                            if (!docFound) { // Inserts the document if it doesn't exist
                                docsInHT->push_back(inputDocumentItem);
                                docsInBST->push_back(inputDocumentItem);
                            }
                        } else { // Target word doesn't exist in the hash table
                            hashTable.insert(inputWordItem);
                            binarySearchTree.insert(inputWordItem);
                        }
                    }
                }
            }
        } else {
            cout << "There is an error with the filename " << fileNames[i] << endl;
        }
        file.close();
    }
    
    hashTable.finalPrint();
    
    string wordCompleteInput;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, wordCompleteInput);
    vector<string> wordInputs = getInputVector(wordCompleteInput);
    
    // Prints query results using the binary search tree structure
    vector<vector<int>> resultVectorsBST;
    for (unsigned int i = 0; i < fileNames.size(); i++) { // Stores query details for each file
        vector<int> resultVector;
        for (unsigned int j = 0; j < wordInputs.size(); j++) { // Finds details of target words
            WordItem targetWordItem(wordInputs[j], notFoundVector);
            vector<DocumentItem> targetDocs = binarySearchTree.find(targetWordItem).docs;
            bool docFound = false;
            for (unsigned int k = 0; k < targetDocs.size(); k++) {
                if (targetDocs[k].documentName == fileNames[i]) {
                    resultVector.push_back(targetDocs[k].count);
                    docFound = true;
                }
            }
            if (docFound == false) {
                resultVector.push_back(0);
            }
        }
        resultVectorsBST.push_back(resultVector);
    }
    printResults(fileNames, wordInputs, resultVectorsBST); // Prints results according to resulting vector of BST
    
    // Prints query results using the hash table structure
    vector<vector<int>> resultVectorsHT;
    for (unsigned int i = 0; i < fileNames.size(); i++) { // Stores query details for each file
        vector<int> resultVector;
        for (unsigned int j = 0; j < wordInputs.size(); j++) { // Finds details of target words
            WordItem targetWordItem(wordInputs[j], notFoundVector);
            vector<DocumentItem> targetDocs = hashTable.find(targetWordItem).docs;
            bool docFound = false;
            for (unsigned int k = 0; k < targetDocs.size(); k++) {
                if (targetDocs[k].documentName == fileNames[i]) {
                    resultVector.push_back(targetDocs[k].count);
                    docFound = true;
                }
            }
            if (docFound == false) {
                resultVector.push_back(0);
            }
        }
        resultVectorsHT.push_back(resultVector);
    }
    printResults(fileNames, wordInputs, resultVectorsHT); // Prints results according to resulting vector of HT
    
    int k = 100;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        // Queries documents with BST
        for (unsigned int j = 0; j < wordInputs.size(); j++) {
            WordItem wordItem(wordInputs[j], notFoundVector);
            binarySearchTree.find(wordItem);
        }
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
    cout << endl << "Time: " << BSTTime.count() / k << endl;
    
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        // Queries documents with HT
        for (unsigned int j = 0; j < wordInputs.size(); j++) {
            WordItem wordItem(wordInputs[j], notFoundVector);
            hashTable.find(wordItem);
        }
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
    cout << endl << "Time: " << HTTime.count() / k << endl;
    
    cout << "Speed Up: " << float(BSTTime.count()) / float(HTTime.count()) << endl;
    
    return 0;
}
