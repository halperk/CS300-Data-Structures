//
//
// CS300 HW #02 - Notebook
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

// Lower function below is needed and sufficient to use
// if case-insensitivity is wanted for title comparisons,
// also in the overloading operators of the nodes
// there are six command lines to be opened for that purposes
//string lower(string word) {
//    // Used to compare titles in all lowercase
//    string result = "";
//    for(int i = 0; i < word.length(); i++) {
//        result += tolower(word[i]);
//    }
//    return result;
//}

struct itemNode {
    string title;
    string info;
    itemNode(const string & theTitle, const string & theInfo): title(theTitle), info(theInfo) {}
};
// These two lines of code below is the alternative usage of comparisons for case-insensitivity case
// bool operator>(const itemNode & lhs, const itemNode & rhs) {return (lower(lhs.title) > lower(rhs.title));}
// bool operator<(const itemNode & lhs, const itemNode & rhs) {return (lower(lhs.title) < lower(rhs.title));}
bool operator>(const itemNode & lhs, const itemNode & rhs) {return (lhs.title > rhs.title);}
bool operator<(const itemNode & lhs, const itemNode & rhs) {return (lhs.title < rhs.title);}
bool operator==(const itemNode & lhs, const itemNode & rhs) {return (lhs.title == rhs.title);}
bool operator!=(const itemNode & lhs, const itemNode & rhs) {return (lhs.title != rhs.title);}
ostream& operator<<(ostream& os, const itemNode & rhs) {return os << rhs.title;}

// Binary Search Tree class codes start here!

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

struct binarySectionNode;

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
        if(!isEmpty()) {
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
        if(t == NULL) { // Creates a new node
            t = new binaryNode<itemType>(x);
        } else if(x < t->element) {
            insert(x, t->left); // Inserts into left subtree
        } else if(t->element < x) {
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
        } else if(t->element < x) {
            remove(x, t->right); // Moves to right subtree
        } else if(t->left != NULL && t->right != NULL) { // Has two children
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

struct binarySectionNode {
    string title;
    BinarySearchTree<itemNode> itemTree;
    binarySectionNode(const string & theTitle, BinarySearchTree<itemNode> & theItemTree): title(theTitle), itemTree(theItemTree) {}
};
// These two lines of code below is the alternative usage of comparisons for case-insensitivity case
//bool operator>(const binarySectionNode & lhs, const binarySectionNode & rhs) {return (lower(lhs.title) > lower(rhs.title));}
//bool operator<(const binarySectionNode & lhs, const binarySectionNode & rhs) {return (lower(lhs.title) < lower(rhs.title));}
bool operator>(const binarySectionNode & lhs, const binarySectionNode & rhs) {return (lhs.title > rhs.title);}
bool operator<(const binarySectionNode & lhs, const binarySectionNode & rhs) {return (lhs.title < rhs.title);}
bool operator==(const binarySectionNode & lhs, const binarySectionNode & rhs) {return (lhs.title == rhs.title);}
bool operator!=(const binarySectionNode & lhs, const binarySectionNode & rhs) {return (lhs.title != rhs.title);}
ostream& operator<<(ostream& os, const binarySectionNode & rhs) {return os << rhs.title;}

// Binary Search Tree class codes end here!

// AVL Tree class codes start here!

template <class itemType>
class AVLTree;

template <class itemType>
class avlNode {
public:
    avlNode(const itemType & theElement, int h = 0): element(theElement), left(NULL), right(NULL), height(h) {}
    avlNode(const itemType & theElement, avlNode * ln, avlNode * rn, int h = 0): element(theElement), left(ln), right(rn), height(h) {}
    friend class AVLTree<itemType>;
private:
    itemType element;
    int height;
    avlNode * left;
    avlNode * right;
};

struct avlSectionNode;

template <class itemType>
class AVLTree {
public:
    explicit AVLTree(const itemType & notFoundValue): root(NULL), ITEM_NOT_FOUND(notFoundValue) {}
    AVLTree(const AVLTree & rhs): root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND) {*this = rhs;}
    ~AVLTree() {makeEmpty();}
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
        if(!isEmpty()) {
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
    const AVLTree & operator=(const AVLTree & rhs)  {
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
private:
    avlNode<itemType> * root;
    const itemType ITEM_NOT_FOUND;
    
    int max(int lhs, int rhs) const {
        return (lhs > rhs)? lhs : rhs;
    }
    int height(avlNode<itemType> * t) const {
        if (t == NULL) {
            return -1;
        }
        return t->height;
    }
    void rotateWithLeftChild(avlNode<itemType> * & k2) const {
        // Rotates right
        avlNode<itemType> * k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2 = k1;
    }
    void rotateWithRightChild(avlNode<itemType> * & k1) const {
        // Rotates left
        avlNode<itemType> * k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1 = k2;
    }
    void doubleWithLeftChild(avlNode<itemType> * & k3) const {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }
    void doubleWithRightChild(avlNode<itemType> * & k4) const {
        rotateWithLeftChild(k4->right);
        rotateWithRightChild(k4);
    }
    int getBalance(avlNode<itemType> * t) const {
        if (t == NULL) {
            return 0;
        }
        return height(t->left) - height(t->right);
    }
    const itemType & elementAt(avlNode<itemType> * t) const {
        if (t == NULL) { // If the item is null, then returns the not found value
            return ITEM_NOT_FOUND;
        } else { // Returns the element inside of the item
            return t->element;
        }
    }
    void insert(const itemType & x, avlNode<itemType> * & t) const {
        // Inserts a new node to the tree and rotates accordingly
        if (t == NULL) {
            t = new avlNode<itemType>(x); // Creates there a new node
            return;
        }
        
        if (x < t->element) { // Inserts left if smaller
            insert(x, t->left);
        } else if (x > t->element) { // Inserts right if larger
            insert(x, t->right);
        } else {
            return;
        }
        
        t->height = max(height(t->left), height(t->right)) + 1; // Updates the height
        int balanceFactor = getBalance(t); // Finds the balance factor
        
        if (balanceFactor > 1) { // Right rotations needed
            if (x < t->left->element) {
                rotateWithLeftChild(t);
                return;
            } else if (x > t->left->element) {
                doubleWithLeftChild(t);
                return;
            }
        }
        
        if (balanceFactor < -1) { // Left rotations needed
            if (x > t->right->element) {
                rotateWithRightChild(t);
                return;
            } else if (x < t->right->element) {
                doubleWithRightChild(t);
                return;
            }
        }
    }
    void remove(const itemType & x, avlNode<itemType> * & t) const {
        // Deletes a target node inside of the tree and rotates the tree accordingly
        if (t == NULL) {
            return;
        }
        if (x < t->element) { // Deletes left if smaller
            remove(x, t->left);
        } else if (x > t->element) { // Deletes right if larger
            remove(x, t->right);
        } else { // Founds the element
            if ((t->left == NULL) || (t->right == NULL)) { // Has one or no children
                avlNode<itemType> * temp = t->left ? t->left : t->right;
                if (temp == NULL) { // No child
                    temp = t;
                    t = NULL;
                } else { // One child
                    *t = *temp;
                }
            } else { // Has two children
                avlNode<itemType> * temp = findMin(t->right);
                t->element = temp->element;
                remove(temp->element, t->right);
            }
        }
        
        if (t == NULL) {
            return;
        }
        
        t->height = max(height(t->left), height(t->right)) + 1; // Updates the height
        int balanceFactor = getBalance(t); // Finds the balance factor
        
        if (balanceFactor > 1) { // Right rotations
            if (getBalance(t->left) >= 0) {
                rotateWithLeftChild(t);
                return;
            } else {
                doubleWithLeftChild(t);
                return;
            }
        }
        
        if (balanceFactor < -1) { // Left rotations
            if (getBalance(t->right) <= 0) {
                rotateWithRightChild(t);
                return;
            } else {
                doubleWithRightChild(t);
                return;
            }
        }
    }
    avlNode<itemType> * find(const itemType & x, avlNode<itemType> * t) const {
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
    avlNode<itemType> * findMin(avlNode<itemType> * t) const {
        // Finds the minimum
        if (t != NULL) {
            while (t->left != NULL) {
                t = t->left;
            }
        }
        return t;
    }
    avlNode<itemType> * findMax(avlNode<itemType> * t) const {
        // Finds the maximum
        if (t != NULL) {
            while (t->right != NULL) {
                t = t->right;
            }
        }
        return t;
    }
    void makeEmpty(avlNode<itemType> * & t) const {
        // Deletes all elements in the tree
        if (t != NULL) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = NULL;
    }
    void printTree(avlNode<itemType> * t) const {
        // Prints the elements of the tree, case-insensitive
        if (t != NULL) {
            printTree(t->left);
            cout << t->element << endl;
            printTree(t->right);
        }
    }
    avlNode<itemType> * clone(avlNode<itemType> *t) const {
        // Creates a clone recursively
        if (t == NULL) {
            return NULL;
        } else {
            return new avlNode<itemType> (t->element, clone(t->left), clone(t->right), t->height);
        }
    }
};

struct avlSectionNode {
    string title;
    AVLTree<itemNode> itemTree;
    avlSectionNode(const string & theTitle, AVLTree<itemNode> & theItemTree): title(theTitle), itemTree(theItemTree) {}
};
// These two lines of code below is the alternative usage of comparisons for case-insensitivity case
//bool operator>(const avlSectionNode & lhs, const avlSectionNode & rhs) {return (lower(lhs.title) > lower(rhs.title));}
//bool operator<(const avlSectionNode & lhs, const avlSectionNode & rhs) {return (lower(lhs.title) < lower(rhs.title));}
bool operator>(const avlSectionNode & lhs, const avlSectionNode & rhs) {return (lhs.title > rhs.title);}
bool operator<(const avlSectionNode & lhs, const avlSectionNode & rhs) {return (lhs.title < rhs.title);}
bool operator==(const avlSectionNode & lhs, const avlSectionNode & rhs) {return (lhs.title == rhs.title);}
bool operator!=(const avlSectionNode & lhs, const avlSectionNode & rhs) {return (lhs.title != rhs.title);}
ostream& operator<<(ostream& os, const avlSectionNode & rhs) {return os << rhs.title;}

// AVL Tree class codes end here!

int main() {
    
    cout << "Welcome to the Notebook!" << endl;
    
    string line, sectionTitleOld, sectionTitleNew, itemTitle, itemInfo;
    itemNode notFoundItem("", "");
    
    //BST tree initalization for not found trees
    BinarySearchTree<itemNode> notFoundBinaryItemTree(notFoundItem);
    BinarySearchTree<binarySectionNode> bstNotebook(binarySectionNode("", notFoundBinaryItemTree));
    
    //AVL tree initalization for not found trees
    AVLTree<itemNode> notFoundAvlItemTree(notFoundItem);
    AVLTree<avlSectionNode> avlNotebook(avlSectionNode("", notFoundAvlItemTree));
    
    ifstream file;
    long long totalTimerAvl = 0, totalTimerBst = 0;
    file.open("data.txt");
    
    // Creates all notebooks by reading the file, placements are case-insensitive
    if (file.is_open()){
        getline(file, line);
        sectionTitleNew = line; // Gets the first section title
        while (!file.eof()) { // Goes until the whole notebook sections finishes
            cout << endl;
            BinarySearchTree<itemNode> bstItemTree(notFoundItem); // Creates the BST of the section
            AVLTree<itemNode> avlItemTree(notFoundItem); // Creates the AVL tree of the section
            bool sectionFinished = false;
            sectionTitleOld = sectionTitleNew;
            while (!sectionFinished) { // Goes until the whole section items finishes
                getline(file, line); // Gets the details in the new line
                if (line[0] != '-') { // Finishes the section if a new section comes
                    sectionTitleNew = line;
                    sectionFinished = true;
                } else { // Saves the title and information in the line to the section tree
                    line = line.substr(1, line.length()-1);
                    itemTitle = line.substr(0, line.find("-"));
                    itemInfo = line.substr(line.find("-")+1, line.length()-1);
                    auto startAvl = std::chrono::high_resolution_clock::now();
                    avlItemTree.insert(itemNode(itemTitle, itemInfo)); // Inserts the item into the AVL tree
                    auto endAvl = std::chrono::high_resolution_clock::now();
                    long long timerAvl = (endAvl-startAvl).count() / 1000.0;
                    totalTimerAvl = totalTimerAvl + timerAvl;
                    auto startBst = std::chrono::high_resolution_clock::now();
                    bstItemTree.insert(itemNode(itemTitle, itemInfo)); // Inserts the item into the BST tree
                    auto endBst = std::chrono::high_resolution_clock::now();
                    long long timerBst = (endBst-startBst).count() / 1000.0;
                    totalTimerBst = totalTimerBst + timerBst;
                }
            }
            binarySectionNode bstSection(sectionTitleOld, bstItemTree);
            avlSectionNode avlSection(sectionTitleOld, avlItemTree);
            auto startAvl = std::chrono::high_resolution_clock::now();
            avlNotebook.insert(avlSection); // Inserts the section tree into the AVL notebook
            auto endAvl = std::chrono::high_resolution_clock::now();
            long long timerAvl = (endAvl-startAvl).count() / 1000.0;
            totalTimerAvl = totalTimerAvl + timerAvl;
            cout << "Section \"" << sectionTitleOld << "\" has been inserted into the AVL notebook." << endl;
            cout << "[AVL] Elapsed time: " << timerAvl << " microseconds" << endl;
            totalTimerAvl = 0;
            auto startBst = std::chrono::high_resolution_clock::now();
            bstNotebook.insert(bstSection); // Inserts the section tree into the BST notebook
            auto endBst = std::chrono::high_resolution_clock::now();
            long long timerBst = (endBst-startBst).count() / 1000.0;
            totalTimerBst = totalTimerBst + timerBst;
            cout << "Section \"" << sectionTitleOld << "\" has been inserted into the BST notebook." << endl;
            cout << "[BST] Elapsed time: " << timerBst << " microseconds" << endl;
            totalTimerBst = 0;
        }
        file.close();
    }
    
    // Starts the menu and gets inputs from the user
    cout << endl << "MENU" << endl;
    cout << "Please enter an input between [1 -6]:" << endl;
    cout << "1- Display the sections [AVL]" << endl;
    cout << "2- Display the sections [BST]" << endl;
    cout << "3- Select a section" << endl;
    cout << "4- Add new section" << endl;
    cout << "5- Delete a section" << endl;
    cout << "6- Exit" << endl;
    string input;
    bool exitTime = false;
    while (!exitTime) {
        cout << "Input: ";
        cin >> input;
        cin.ignore();
        if (input == "1") {
            // Prints the tree in the AVL notebook
            cout << endl << "*****" << endl;
            avlNotebook.printTree();
            cout << "*****" << endl << endl;
        } else if (input == "2") {
            // Prints the tree in the BST notebook
            cout << endl << "*****" << endl;
            bstNotebook.printTree();
            cout << "*****" << endl << endl;
        } else if (input == "3") {
            // Opens a target section tree, consisting of items
            string sectionTitle;
            cout << "Enter the title of the section: ";
            getline(cin, sectionTitle);
            if (bstNotebook.existenceCheck(binarySectionNode(sectionTitle, notFoundBinaryItemTree))) {
                // If the section exists, then gets its tree from both AVL and BST notebooks
                binarySectionNode * bstSectionNode = &bstNotebook.getOriginal(binarySectionNode(sectionTitle, notFoundBinaryItemTree));
                BinarySearchTree<itemNode> * bstSection = &bstSectionNode->itemTree;
                avlSectionNode * avltSectionNode = &avlNotebook.getOriginal(avlSectionNode(sectionTitle, notFoundAvlItemTree));
                AVLTree<itemNode> * avlSection = &avltSectionNode->itemTree;
                cout << endl << "Selected section -> " << sectionTitle << endl;
                cout << "Please enter an input between [1 -7]:" << endl;
                cout << "1- Display the items [AVL]" << endl;
                cout << "2- Display the items [BST]" << endl;
                cout << "3- Display the information of an item" << endl;
                cout << "4- Add new item" << endl;
                cout << "5- Update the information of an item" << endl;
                cout << "6- Delete an item" << endl;
                cout << "7- Return to main menu" << endl;
                string innerInput;
                bool exitTimeInner = false;
                while (!exitTimeInner) {
                    cout << "Input: ";
                    cin >> innerInput;
                    cin.ignore();
                    if (innerInput == "1") {
                        // Prints the items of the selected section in the AVL notebook
                        cout << endl << "*****" << endl;
                        avlSection->printTree();
                        cout << "*****" << endl << endl;
                    } else if (innerInput == "2") {
                        // Prints the items of the selected section in the BST notebook
                        cout << endl << "*****" << endl;
                        bstSection->printTree();
                        cout << "*****" << endl << endl;
                    } else if (innerInput == "3") {
                        // Prints the information of a specific node from its title
                        string searchTitle;
                        cout << "Enter the title of the item: ";
                        getline(cin, searchTitle);
                        if (bstSection->existenceCheck(itemNode(searchTitle, ""))) { // Checks if the target item exists in the section
                            auto startAvl = std::chrono::high_resolution_clock::now();
                            itemNode searchItem = avlSection->find(itemNode(searchTitle, "")); // Finds the target node in AVL tree
                            auto endAvl = std::chrono::high_resolution_clock::now();
                            long long timerAvl = (endAvl-startAvl).count() / 1000.0;
                            cout << "[AVL] Elapsed time: " << timerAvl << " microseconds" << endl;
                            auto startBst = std::chrono::high_resolution_clock::now();
                            searchItem = bstSection->find(itemNode(searchTitle, "")); // Finds the target node in BST tree
                            auto endBst = std::chrono::high_resolution_clock::now();
                            long long timerBst = (endBst-startBst).count() / 1000.0;
                            cout << "[BST] Elapsed time: " << timerBst << " microseconds" << endl;
                            cout << searchItem.info << endl << endl;
                        } else {
                            auto startAvl = std::chrono::high_resolution_clock::now();
                            avlSection->find(itemNode(searchTitle, "")); // Finds the target node in AVL tree
                            auto endAvl = std::chrono::high_resolution_clock::now();
                            long long timerAvl = (endAvl-startAvl).count() / 1000.0;
                            cout << "[AVL] Elapsed time: " << timerAvl << " microseconds" << endl;
                            auto startBst = std::chrono::high_resolution_clock::now();
                            bstSection->find(itemNode(searchTitle, "")); // Finds the target node in BST tree
                            auto endBst = std::chrono::high_resolution_clock::now();
                            long long timerBst = (endBst-startBst).count() / 1000.0;
                            cout << "[BST] Elapsed time: " << timerBst << " microseconds" << endl;
                            cout << "Invalid title." << endl << endl;
                        }
                    } else if (innerInput == "4") {
                        // Inserts a new item inside of a section of both notebooks
                        string newTitle, newInfo;
                        cout << "Enter a title for the item: ";
                        getline(cin, newTitle);
                        if (!bstSection->existenceCheck(itemNode(newTitle, ""))) { // Checks if the target item exists in the section
                            cout << "Enter a description for the item: ";
                            getline(cin, newInfo);
                            auto startAvl = std::chrono::high_resolution_clock::now();
                            avlSection->insert(itemNode(newTitle, newInfo)); // Inserts the item into the AVL tree
                            auto endAvl = std::chrono::high_resolution_clock::now();
                            long long timerAvl = (endAvl-startAvl).count() / 1000.0;
                            cout << "[AVL] Elapsed time: " << timerAvl << " microseconds" << endl;
                            auto startBst = std::chrono::high_resolution_clock::now();
                            bstSection->insert(itemNode(newTitle, newInfo)); // Inserts the item into the BST tree
                            auto endBst = std::chrono::high_resolution_clock::now();
                            long long timerBst = (endBst-startBst).count() / 1000.0;
                            cout << "[BST] Elapsed time: " << timerBst << " microseconds" << endl;
                            cout << "The new item \"" << newTitle << "\" has been inserted." << endl << endl;
                        } else {
                            cout << "Item \"" << newTitle << "\" already exists in the \"" << sectionTitle << "\"." << endl << endl;
                        }
                    } else if (innerInput == "5") {
                        // Updates the information of an item from the section of both notebooks
                        string searchTitle, updateInfo;
                        cout << "Enter the title of the item: ";
                        getline(cin, searchTitle);
                        if (bstSection->existenceCheck(itemNode(searchTitle, ""))) { // Checks if the target item exists in the section
                            auto startAvl = std::chrono::high_resolution_clock::now();
                            itemNode * updateAvlItem = &avlSection->getOriginal(itemNode(searchTitle, "")); // Gets original AVL item
                            auto endAvl = std::chrono::high_resolution_clock::now();
                            long long timerAvl = (endAvl-startAvl).count() / 1000.0;
                            cout << "[AVL] Elapsed time: " << timerAvl << " microseconds" << endl;
                            auto startBst = std::chrono::high_resolution_clock::now();
                            itemNode * updateBstItem = &bstSection->getOriginal(itemNode(searchTitle, "")); // Gets original BST item
                            auto endBst = std::chrono::high_resolution_clock::now();
                            long long timerBst = (endBst-startBst).count() / 1000.0;
                            cout << "[BST] Elapsed time: " << timerBst << " microseconds" << endl;
                            cout <<"Enter the new information: ";
                            getline(cin, updateInfo);
                            updateAvlItem->info = updateInfo; // Updates the information of the original AVL item
                            updateBstItem->info = updateInfo; // Updates the information of the original BST item
                            cout << "The content " << searchTitle << " has been updated." << endl << endl;
                        } else {
                            auto startAvl = std::chrono::high_resolution_clock::now();
                            avlSection->find(itemNode(searchTitle, "")); // Finds the target node in AVL tree
                            auto endAvl = std::chrono::high_resolution_clock::now();
                            long long timerAvl = (endAvl-startAvl).count() / 1000.0;
                            cout << "[AVL] Elapsed time: " << timerAvl << " microseconds" << endl;
                            auto startBst = std::chrono::high_resolution_clock::now();
                            bstSection->find(itemNode(searchTitle, "")); // Finds the target node in BST tree
                            auto endBst = std::chrono::high_resolution_clock::now();
                            long long timerBst = (endBst-startBst).count() / 1000.0;
                            cout << "[BST] Elapsed time: " << timerBst << " microseconds" << endl;
                            cout << "Item \"" << searchTitle << "\" does not exist in the \"" << sectionTitle << "\"." << endl << endl;
                        }
                    } else if (innerInput == "6") {
                        // Deletes an item from the section of both notebooks
                        string searchTitle;
                        cout << "Enter the title of the item: ";
                        getline(cin, searchTitle);
                        if (bstSection->existenceCheck(itemNode(searchTitle, ""))) { // Checks if the target item exists in the section
                            auto startAvl = std::chrono::high_resolution_clock::now();
                            avlSection->remove(itemNode(searchTitle, "")); // Deletes the node from the AVL tree
                            auto endAvl = std::chrono::high_resolution_clock::now();
                            long long timerAvl = (endAvl-startAvl).count() / 1000.0;
                            cout << "[AVL] Elapsed time: " << timerAvl << " microseconds" << endl;
                            auto startBst = std::chrono::high_resolution_clock::now();
                            bstSection->remove(itemNode(searchTitle, "")); // Deletes the node from the BST tree
                            auto endBst = std::chrono::high_resolution_clock::now();
                            long long timerBst = (endBst-startBst).count() / 1000.0;
                            cout << "[BST] Elapsed time: " << timerBst << " microseconds" << endl;
                            cout << "The content \"" << searchTitle << "\" has been deleted." << endl << endl;
                        } else {
                            cout << "Item \"" << searchTitle << "\" does not exist in the \"" << sectionTitle << "\"." << endl << endl;
                        }
                    } else if (innerInput == "7") {
                        // Returns to the main menu
                        exitTimeInner = true;
                        cout << endl << "MENU" << endl;
                        cout << "Please enter an input between [1 -6]:" << endl;
                        cout << "1- Display the sections [AVL]" << endl;
                        cout << "2- Display the sections [BST]" << endl;
                        cout << "3- Select a section" << endl;
                        cout << "4- Add new section" << endl;
                        cout << "5- Delete a section" << endl;
                        cout << "6- Exit" << endl;
                    } else {
                        cout << "Invalid Input!" << endl << endl;
                    }
                }
            } else {
                cout << "Invalid title!" << endl << endl;
            }
        } else if (input == "4") {
            // Inserts a new section to both of AVL and BST notebooks
            string newTitle;
            cout << "Enter a title for the section: ";
            getline(cin, newTitle);
            if (!bstNotebook.existenceCheck(binarySectionNode(newTitle, notFoundBinaryItemTree))) {
                avlNotebook.insert(avlSectionNode(newTitle, notFoundAvlItemTree)); // Inserts the new section into AVL notebook
                bstNotebook.insert(binarySectionNode(newTitle, notFoundBinaryItemTree)); // Inserts the new section into BST notebook
                cout << "The new section \"" << newTitle << "\" has been inserted." << endl << endl;
            } else {
                cout << "Section \"" << newTitle << "\" already exists." << endl << endl;
            }
        } else if (input == "5") {
            // Deletes a section from both of AVL and BST notebooks
            string delTitle;
            cout << "Enter a title for the section: ";
            getline(cin, delTitle);
            if (bstNotebook.existenceCheck(binarySectionNode(delTitle, notFoundBinaryItemTree))) {
                avlNotebook.remove(avlSectionNode(delTitle, notFoundAvlItemTree)); // Deletes the section from the AVL notebook
                bstNotebook.remove(binarySectionNode(delTitle, notFoundBinaryItemTree)); // Deletes the section from the BST notebook
                cout << "The section has been deleted." << endl << endl;
            } else {
                cout << "Section \"" << delTitle << "\" does not exist." << endl << endl;
            }
        } else if (input == "6") {
            exitTime = true;
        } else {
            cout << "Invalid Input!" << endl << endl;
        }
    }
    
    return 0;
}
