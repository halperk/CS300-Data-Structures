//
//
// CS300 HW #04 - Sorting and Searching
// Huseyin Alper Karadeniz
// huseyinalper (00028029)
// Sabanci University 2022
//
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cctype>
using namespace std;

// A struct to store the information about a person.
struct person {
    string name;
    string fullName;
    string phoneNo;
    string city;
    person(const string theName, const string theSurname, const string thePhoneNo, const string theCity): name(theName), fullName(theName + " " + theSurname), phoneNo(thePhoneNo), city(theCity) {}
    person(): name(""), fullName(""), phoneNo(""), city("") {}
    
    bool operator==(const person & rhs) {return (fullName == rhs.fullName);}
    bool operator!=(const person & rhs) {return (fullName != rhs.fullName);}
    bool operator>(const person & rhs) {return (fullName > rhs.fullName);}
    bool operator>=(const person & rhs) {return (fullName >= rhs.fullName);}
    bool operator<(const person & rhs) {return (fullName < rhs.fullName);}
    bool operator<=(const person & rhs) {return (fullName <= rhs.fullName);}
    bool operator==(const string & rhs) {return (fullName == rhs);}
    bool operator!=(const string & rhs) {return (fullName != rhs);}
    bool operator>(const string & rhs) {return (fullName > rhs);}
    bool operator>=(const string & rhs) {return (fullName >= rhs);}
    bool operator<(const string & rhs) {return (fullName < rhs);}
    bool operator<=(const string & rhs) {return (fullName <= rhs);}
};

ostream& operator<<(ostream & os, const person & thePerson) {
    os << thePerson.fullName << " " << thePerson.phoneNo << " " << thePerson.city;
    return os;
}

// SORTING ALGORITHMS:
// 1) Insertion Sort
// 2) Quick Sort
// 3) Merge Sort
// 4) Heap Sort

// SEARCHING ALGORITHMS:
// 1) Sequential Search
// 2) Binary Search

// SORTING-1: Insertion Sort Algorithm

template <class Comparable>
void insertionSort(vector<Comparable> & dataVector) {
    int i, j;
    Comparable key;
    for (i = 1; i < dataVector.size(); i++) {
        key = dataVector[i];
        j = i - 1;
        while (j >= 0 && dataVector[j] > key) {
            dataVector[j + 1] = dataVector[j];
            j = j - 1;
        }
        dataVector[j + 1] = key;
    }
}

// SORTING-2: Quick Sort Algorithm
template <class Comparable>
void swapItems(Comparable & item1, Comparable & item2) {
    Comparable temp = item1;
    item1 = item2;
    item2 = temp;}
template <class Comparable>
const Comparable & median3(vector<Comparable> & dataVector, int left, int right) {
    int center = (left + right) / 2;
    if (dataVector[center] < dataVector[left])
        swapItems(dataVector[left], dataVector[center]);
    if (dataVector[right] < dataVector[left])
        swapItems(dataVector[left], dataVector[right]);
    if (dataVector[right] < dataVector[center])
        swapItems(dataVector[center], dataVector[right]);
    swapItems(dataVector[center], dataVector[right-1]);
    return dataVector[right-1];}
template <class Comparable>
void insertionSortQuick(vector<Comparable> & dataVector, int left, int right) {
    for (int p = left+1; p <= right; p++) {
        Comparable temp = dataVector[p];
        int j;
        for (j = p; j > left && temp < dataVector[j-1]; j--) {
            dataVector[j] = dataVector[j-1];
        }
        dataVector[j] = temp;}}
template <class Comparable>
void quicksort(vector<Comparable> & dataVector, int left, int right){
    if (left + 10 <= right) {
        Comparable pivot = median3(dataVector, left, right);
        int i = left, j = right-1;
        for ( ; ; ) {
            while (dataVector[++i] < pivot) {}
            while (pivot < dataVector[--j]) {}
            if (i < j) {
                swapItems(dataVector[i], dataVector[j]);
            } else {
                break;
            }
        }
        swapItems(dataVector[i], dataVector[right-1]);
        quicksort(dataVector, left, i-1);
        quicksort(dataVector, i+1, right);
    } else {
        insertionSortQuick(dataVector, left, right);}}
template <class Comparable>
void quickSort(vector<Comparable> & dataVector) {
    quicksort(dataVector, 0, int(dataVector.size()-1));}

// SORTING-3: Merge Sort Algorithm

template <class Comparable>
void merge(vector<Comparable> & dataVector, int start, int mid, int end) {
    int start2 = mid + 1;
 
    if (dataVector[mid] <= dataVector[start2]) {return;}
    while (start <= mid && start2 <= end) {
        if (dataVector[start] <= dataVector[start2]) {
            start++;
        } else {
            Comparable value = dataVector[start2];
            int index = start2;
 
            while (index != start) {
                dataVector[index] = dataVector[index - 1];
                index--;
            }
            dataVector[start] = value;
            start++;
            mid++;
            start2++;
        }
    }
}

template <class Comparable>
void mergeSort(vector<Comparable> & dataVector, int l, int r) {
    if (l < r) {
        int m = l + (r-l) / 2;
        
        mergeSort(dataVector, l, m);
        mergeSort(dataVector, m + 1, r);
 
        merge(dataVector, l, m, r);
    }
}

// SORTING-4: Heap Sort Algorithm

template <class Comparable>
void heapify(vector<Comparable> & dataVector, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && dataVector[l] > dataVector[largest]) {
        largest = l;
    }
    if (r < n && dataVector[r] > dataVector[largest]) {
        largest = r;
    }
    if (largest != i) {
        swapItems(dataVector[i], dataVector[largest]);
        heapify(dataVector, n, largest);
    }
}
 
template <class Comparable>
void heapSort(vector<Comparable> & dataVector)
{
    int n = int(dataVector.size());
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(dataVector, n, i);
    }
    
    for (int i = n - 1; i > 0; i--) {
        swapItems(dataVector[0], dataVector[i]);
        heapify(dataVector, i, 0);
    }
}

// SEARCHING-1: Sequential Search for Full Name Search

template <class Comparable>
vector<int> sequentialSearch(vector<Comparable> & dataVector, string x) {
    vector<int> resultIndices;
    int i, n = int(dataVector.size());
    for (i = 0; i < n; i++) {
        if (dataVector[i] == x) {
            resultIndices.push_back(i);
        }
    }
    return resultIndices;
}

// SEARCHING-1: Sequential Search for Partial Name Search

bool partialCheck(const string & theFullName, const string & x) {
    if (theFullName.size() >= x.size()) {
        for (int i = 0; i < int(x.size()); i++) {
            if (theFullName[i] != x[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

vector<int> sequentialSearchPartial(vector<person> & dataVector, string x) {
    vector<int> resultIndices;
    int i, n = int(dataVector.size());
    for (i = 0; i < n; i++) {
        if (partialCheck(dataVector[i].fullName, x)) {
            resultIndices.push_back(i);
        }
    }
    return resultIndices;
}

// SEARCHING-2: Binary Search for Full Name Search

template <class Comparable>
int binarySearchFirstFind(vector<Comparable> & dataVector, int l, int r, string x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        
        if (dataVector[mid] == x) {
            return mid;
        }
 
        if (dataVector[mid] > x) {
            return binarySearchFirstFind(dataVector, l, mid - 1, x);
        }

        return binarySearchFirstFind(dataVector, mid + 1, r, x);
    }
    return -1;
}

template <class Comparable>
vector<int> binarySearch(vector<Comparable> & dataVector, int l, int r, string x) {
    int firstIdx = binarySearchFirstFind(dataVector, l, r, x);
    vector<int> resultIndices;
    if (firstIdx != -1) { // If exists
        resultIndices.push_back(firstIdx); // Inserts the index which is found first
        if (firstIdx-1 >= 0) { // Searchs before the index
            int leftIdx = firstIdx-1;
            while (leftIdx != -1) {
                if (dataVector[leftIdx] == x) { // Inserts the indices before the first one
                    resultIndices.push_back(leftIdx);
                    leftIdx--;
                } else {
                    break;
                }
            }
        }
        if (firstIdx+1 < dataVector.size()) { // Searchs after the index
            int rightIdx = firstIdx+1;
            while (rightIdx != int(dataVector.size()-1)) {
                if (dataVector[rightIdx] == x) { // Inserts the indices after the first one
                    resultIndices.push_back(rightIdx);
                    rightIdx++;
                } else {
                    break;
                }
            }
        }
    }
    return resultIndices;
}

// SEARCHING-2: Binary Search for Partial Name Search

int binarySearchFirstFindPartial(vector<person> & dataVector, int l, int r, string x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        
        if (partialCheck(dataVector[mid].fullName, x)) {
            return mid;
        }
 
        if (dataVector[mid] > x) {
            return binarySearchFirstFindPartial(dataVector, l, mid - 1, x);
        }

        return binarySearchFirstFindPartial(dataVector, mid + 1, r, x);
    }
    return -1;
}

vector<int> binarySearchPartial(vector<person> & dataVector, int l, int r, string x) {
    int firstIdx = binarySearchFirstFindPartial(dataVector, l, r, x);
    vector<int> resultIndices;
    if (firstIdx != -1) { // If exists
        resultIndices.push_back(firstIdx); // Inserts the index which is found first
        if (firstIdx-1 >= 0) { // Searchs before the index
            int leftIdx = firstIdx-1;
            while (leftIdx != -1) {
                if (partialCheck(dataVector[leftIdx].fullName, x)) { // Inserts the indices before the first one
                    resultIndices.push_back(leftIdx);
                    leftIdx--;
                } else {
                    break;
                }
            }
        }
        if (firstIdx+1 < dataVector.size()) { // Searchs after the index
            int rightIdx = firstIdx+1;
            while (rightIdx != int(dataVector.size()-1)) {
                if (partialCheck(dataVector[rightIdx].fullName, x)) { // Inserts the indices after the first one
                    resultIndices.push_back(rightIdx);
                    rightIdx++;
                } else {
                    break;
                }
            }
        }
    }
    return resultIndices;
}

// Printing Section

template <class Comparable>
void printVector(vector<Comparable> dataVector, vector<int> printIndices) {
    int i;
    for (i = 0; i < int(printIndices.size()); i++) {
        cout << dataVector[printIndices[i]] << endl;
    }
}

template <class Comparable>
void printVectorBasic(vector<Comparable> dataVector) {
    int i;
    for (i = 0; i < dataVector.size(); i++)
        cout << dataVector[i] << endl;
    cout << endl << endl;
}

// Printing SpeedUps

void printSearchSpeedUp (int binarySearchTime, int sequentialSearchTime) {
    cout << "SpeedUp between Search Algorithms" << endl << "======================================" << endl;
    if (sequentialSearchTime >= binarySearchTime) {
        cout << "(Sequential Search / Binary Search) SpeedUp = " << float(sequentialSearchTime) / float(binarySearchTime) << endl;
    } else {
        cout << "(Binary Search / Sequential Search) SpeedUp = " << float(binarySearchTime) / float(sequentialSearchTime) << endl;
    }
}

void printSortSpeedUps(int quickSortTime, int insertionSortTime, int mergeSortTime, int heapSortTime) {
    cout << "SpeedUps between Sorting Algorithms" << endl << "======================================" << endl;
    int smallestTime = quickSortTime;
    string smallestTimeAlgo = "Quick Sort";
    if (smallestTime > insertionSortTime) {smallestTime = insertionSortTime; smallestTimeAlgo = "Insertion Sort";}
    if (smallestTime > mergeSortTime) {smallestTime = mergeSortTime; smallestTimeAlgo = "Merge Sort";}
    if (smallestTime > heapSortTime) {smallestTime = heapSortTime; smallestTimeAlgo = "Heap Sort";}
    if (smallestTimeAlgo == "Quick Sort") {
        cout << "(Insertion Sort / Quick Sort) SpeedUp = " << float(insertionSortTime) / float(quickSortTime) << endl;
        cout << "(Merge Sort / Quick Sort) SpeedUp = " << float(mergeSortTime) / float(quickSortTime) << endl;
        cout << "(Heap Sort / Quick Sort) SpeedUp = " << float(heapSortTime) / float(quickSortTime) << endl;
    } else if (smallestTimeAlgo == "Insertion Sort") {
        cout << "(Quick Sort / Insertion Sort) SpeedUp = " << float(quickSortTime) / float(insertionSortTime) << endl;
        cout << "(Merge Sort / Insertion Sort) SpeedUp = " << float(mergeSortTime) / float(insertionSortTime) << endl;
        cout << "(Heap Sort / Insertion Sort) SpeedUp = " << float(heapSortTime) / float(insertionSortTime) << endl;
    } else if (smallestTimeAlgo == "Merge Sort") {
        cout << "(Quick Sort / Merge Sort) SpeedUp = " << float(quickSortTime) / float(mergeSortTime) << endl;
        cout << "(Insertion Sort / Merge Sort) SpeedUp = " << float(insertionSortTime) / float(mergeSortTime) << endl;
        cout << "(Heap Sort / Merge Sort) SpeedUp = " << float(heapSortTime) / float(mergeSortTime) << endl;
    } else {
        cout << "(Quick Sort / Heap Sort) SpeedUp = " << float(quickSortTime) / float(heapSortTime) << endl;
        cout << "(Insertion Sort / Heap Sort) SpeedUp = " << float(insertionSortTime) / float(heapSortTime) << endl;
        cout << "(Merge Sort / Heap Sort) SpeedUp = " << float(mergeSortTime) / float(heapSortTime) << endl;
    }
}

void printSpeedUps(int binarySearchTime, int sequentialSearchTime, int quickSortTime, int insertionSortTime, int mergeSortTime, int heapSortTime) {
    printSearchSpeedUp (binarySearchTime, sequentialSearchTime);
    cout << endl;
    printSortSpeedUps(quickSortTime, insertionSortTime, mergeSortTime, heapSortTime);
}

int main() {
    
    vector<person> phoneBook;
    string fileName;
    cout << "Please enter the contact file name:" << endl;
    cin >> fileName;
    
    ifstream file;
    file.open(fileName);
    if(file.is_open()) {
        string targetPerson;
        cout << "Please enter the word to be queried:" << endl;
        cin.ignore();
        getline(cin, targetPerson);
        // Stores the search name and surname
        istringstream searchFullName(targetPerson);
        string searchName, searchSurname;
        searchFullName >> searchName >> searchSurname;
        
        string input;
        while (!file.eof()) { // Reads till the file ends
            getline(file, input);
            
            istringstream ss(input);
            string name, surname, phoneNo, city;
            ss >> name >> surname >> phoneNo >> city;
            transform(name.begin(), name.end(), name.begin(), ::toupper);
            transform(surname.begin(), surname.end(), surname.begin(), ::toupper);
            
            person targetPerson = person(name, surname, phoneNo, city);
            
            phoneBook.push_back(targetPerson);
            
            // cout << "Full Name: " << targetPerson.fullName << " Phone No: " << targetPerson.phoneNo << " City: " << targetPerson.city << endl;
            
        }
        
        // Creates copies to measure the time taken by sorting algorithms
        vector<person> phoneBook1 = phoneBook, phoneBook2 = phoneBook, phoneBook3 = phoneBook;
        
        cout << endl << "Sorting the vector copies" << endl << "======================================" << endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        quickSort(phoneBook);
        auto end = std::chrono::high_resolution_clock::now();
        int quickSortTime = (end - start).count() / 1.0;
        cout << "Quick Sort Time: " << quickSortTime << " Nanoseconds" << endl;
        
        start = std::chrono::high_resolution_clock::now();
        insertionSort(phoneBook1);
        end = std::chrono::high_resolution_clock::now();
        int insertionSortTime = (end - start).count() / 1.0;
        cout << "Insertion Sort Time: " << insertionSortTime << " Nanoseconds" << endl;
        
        start = std::chrono::high_resolution_clock::now();
        mergeSort(phoneBook2, 0, int(phoneBook2.size())-1);
        end = std::chrono::high_resolution_clock::now();
        int mergeSortTime = (end - start).count() / 1.0;
        cout << "Merge Sort Time: " << mergeSortTime << " Nanoseconds" << endl;
        
        start = std::chrono::high_resolution_clock::now();
        heapSort(phoneBook3);
        end = std::chrono::high_resolution_clock::now();
        int heapSortTime = (end - start).count() / 1.0;
        cout << "Heap Sort Time: " << heapSortTime << " Nanoseconds" << endl;
        
        cout << endl << "Searching for " << targetPerson << endl << "======================================" << endl;
        transform(targetPerson.begin(), targetPerson.end(), targetPerson.begin(), ::toupper);
        
        // Measures the time taken by an empty for loop
        start = std::chrono::high_resolution_clock::now();
        int N = 100;
        for (int i = 0; i < N; i++) {;}
        end = std::chrono::high_resolution_clock::now();
        long long emptyLoopTime = (end - start).count() / N;
        
        if (searchSurname != "") { // Full Contact Name Search
            
            vector<int> resultIndices;
            
            // Binary Search for the target
            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < N; i++) {
                resultIndices = binarySearch(phoneBook1, 0, int(phoneBook1.size()-1), targetPerson);
            }
            end = std::chrono::high_resolution_clock::now();
            int binarySearchTime = int(((end - start).count() / N) - emptyLoopTime);
            
            insertionSort(resultIndices); // Sorts the indices
            if (resultIndices.size() == 0) { // No result
                cout << targetPerson << " does NOT exist in the dataset" << endl;
            } else { // Some results
                printVector(phoneBook1, resultIndices);
            }
            cout << "Binary Search Time: " << binarySearchTime << " Nanoseconds" << endl << endl;
            
            // Sequential Search for the target
            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < N; i++) {
                resultIndices = sequentialSearch(phoneBook1, targetPerson);
            }
            end = std::chrono::high_resolution_clock::now();
            int sequentialSearchTime = int(((end - start).count() / N) - emptyLoopTime);
            
            insertionSort(resultIndices); // Sorts the indices
            cout << "Search results for Sequential Search:" << endl;
            if (resultIndices.size() == 0) { // No result
                cout << targetPerson << " does NOT exist in the dataset" << endl;
            } else { // Some results
                printVector(phoneBook1, resultIndices);
            }
            cout << "Sequential Search Time: " << sequentialSearchTime << " Nanoseconds" << endl << endl;
            
            printSpeedUps(binarySearchTime, sequentialSearchTime, quickSortTime, insertionSortTime, mergeSortTime, heapSortTime);
            
        } else { // Partial Contact Name Search
            
            vector<int> resultIndices;
            
            // Binary Search for the target
            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < N; i++) {
                resultIndices = binarySearchPartial(phoneBook1, 0, int(phoneBook1.size()-1), targetPerson);
            }
            end = std::chrono::high_resolution_clock::now();
            int binarySearchTime = int(((end - start).count() / N) - emptyLoopTime);
            
            insertionSort(resultIndices); // Sorts the indices
            if (resultIndices.size() == 0) { // No result
                cout << targetPerson << " does NOT exist in the dataset" << endl;
            } else { // Some results
                printVector(phoneBook1, resultIndices);
            }
            cout << "Binary Search Time: " << binarySearchTime << " Nanoseconds" << endl << endl;
            
            // Sequential Search for the target
            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < N; i++) {
                resultIndices = sequentialSearchPartial(phoneBook1, targetPerson);
            }
            end = std::chrono::high_resolution_clock::now();
            int sequentialSearchTime = int(((end - start).count() / N) - emptyLoopTime);
            
            insertionSort(resultIndices); // Sorts the indices
            cout << "Search results for Sequential Search:" << endl;
            if (resultIndices.size() == 0) { // No result
                cout << targetPerson << " does NOT exist in the dataset" << endl;
            } else { // Some results
                printVector(phoneBook1, resultIndices);
            }
            cout << "Sequential Search Time: " << sequentialSearchTime << " Nanoseconds" << endl << endl;
            
            printSpeedUps(binarySearchTime, sequentialSearchTime, quickSortTime, insertionSortTime, mergeSortTime, heapSortTime);
        }
        
    } else {
        cout << "There is an error with the filename " << fileName << endl;
    }
    
    file.close();
    
    return 0;
}
