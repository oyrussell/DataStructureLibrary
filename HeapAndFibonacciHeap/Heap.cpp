/*
Heap
Owen Yeats Russell
*/

#include <iostream>
#include <math.h>
using namespace std;

template <class keytype>
class CDA{
private:
    keytype* arr = NULL; //Data Array
    int front; //Front Index
    int size; //Amount of Values Stored
    int capacity; //Amount of Slots Total
    keytype* copy = NULL; //Copy Ptr
    int nextAvailable; //Next Open Array Slot Index

public:

    CDA(){ //Default CDA Constructor
        arr = new keytype[1];
        size = 0; //Size is init 0
        capacity = 1; //Capacity is Default 1
        copy = NULL; //Copy ptr
        front = -1; //Init front for empty detection
        nextAvailable = -1; //Empty
    }

    CDA(int s){ //CDA Constructor with Specified Capacity and Size
        size = 0;
        capacity = s;
        keytype* newArray = new keytype[s];
        delete[] arr; //Prevent Mem Leak
        arr = newArray;
        front = -1; //Init front to allow for empty detection
    }

    CDA(const CDA& A){ //Copy Assignment
        CDA(); //Init a new CDA
        this->size = A.size; //Copy Size
        this->capacity = A.capacity; //Copy Capacity
        delete[] arr; //Prevent Mem Leak
        arr = new keytype[capacity]; //Init the new Array of size capacity
        for (int i = 0; i < size; i++){ //Loop through enire array
            arr[i] = A.arr[i]; //copy over at every index
        }
        front = 0; //Front is init at 0
    }

    CDA& operator =(CDA& C){//New Copy Constructor
        if (this != &C){
            delete[] arr; //To prevent Mem Leak
            size = C.size; //Copy Size
            capacity = C.capacity; //Copy Capacity
            front = 0; //Update the front index
            arr = new keytype[capacity]; //Init the New Structure
            for (int i = 0; i < size; i++){
                arr[i] = C.arr[i]; //Loop Through and Copy over
            }
        }
        return(*this); //Return for Copy
    }

    ~CDA(){//Destructor
        delete[] arr;
    }

    //Operator to Allow Default Array Like Access to the CDA
    keytype& operator[](int i){
        if (arr == NULL){ //If array is empty
            cout << "Out of Bounds" << endl;
            return *(copy);
        }

        if (i < 0 || i >= size){ //If index requested is out of bounds
            cout << "Out of Bounds" << endl;
            return *(copy);
        }
        return arr[(front + i) % capacity];
    }


    void doubleSize(){ //When the Array is Full this Doubles the capacity of the array
        int index; //Int For the Index Calculation
        keytype* temp = new keytype[capacity * 2]; //New Array that is Double the capacity
        for (int i = 0; i < size; i++){
            index = ((front + i) % capacity);
            temp[i] = arr[index];
        }
        capacity *= 2; //Capacity is now double after copying the array over
        delete[] arr; //Delete the old array

        arr = new keytype[capacity]; //Init new array
        arr = temp; //Set new array to the temp that was copied into
        front = 0; //Front is now 0 after the copy
    }

    // checks room, increases size if there is none left
    void checkRoom(){
        if (size + 1 > capacity){
            doubleSize();
        }
        size++;
        return;
    }

    void AddEnd(keytype v){ //Add to the end of the array
        int index;
        checkRoom();
        // if the array is empty
        if (front == -1){
            front = 0;
            arr[front] = v;
            return;
        }
        
        else if (front == 0){
            arr[size - 1] = v;
            return;
        }

        index = (front + (size - 1)) % capacity;
        arr[index] = v;
        return;
    }

    void AddFront(keytype v){//Add to the front of the array
        checkRoom();
        // if arr is empty
        if (front == -1){
            front = 0;
            arr[front] = v;
            return;
        }
        else if (front == 0){
            front = capacity - 1;
            arr[front] = v;
            return;
        }
        else if (capacity / size == 2){
            front = capacity - 1;
            arr[front] = v;
            return;
        }

        else if (front > 0){
            front = front - 1;
            arr[front] = v;
            return;
        }
    }

    int Length(){//Return size of Array
        return size;
    }

    int Capacity(){//Return the cap of the array
        return capacity;
    }

    void Clear(){//Delete Array and Make a new one of Capacity 4
        delete[] arr;
        arr = new keytype[4];
        size = 0;
        capacity = 4;
        front = 0;
        return;
    }

    void swap(keytype i, keytype j){//Swap Function
        keytype *temp = &copy[i];
        copy[i] = copy[j];
        copy[j] = *temp;
    }

    void clearCopy(){//Delete the Copy Ptr and NULL it
        delete[] copy;
        copy = NULL;
    }
    
    keytype Select(int k){//Returns the keytype at index k
        keytype result;
        clearCopy();
        copy = new keytype[capacity];
        for (int i = 0; i < size; i++){
            copy[i] = operator[](i);
        }
        quickSort(copy, 0, size - 1);
        result = copy[k - 1];
        return result;
    }

    keytype getter(int i){//Helper to Access an array slot
        keytype ans = arr[i];
        return ans;
    }

    void tSwap(int i, int j){//New Swap Function
        keytype tempNum = arr[i];
        arr[i] = arr[j];
        arr[j] = tempNum;
    }

    void DelCheck(){//If Less 25% or less of the array is used it will shrink
        int index;
        keytype* temp;
        if (((size / capacity) >= .25) && (capacity > 4)){
            index = capacity - (.25 * capacity);
            temp = new keytype[index];
            for (int y = 0; y < size; y++){
                temp[y] = arr[(front + y) % capacity];
            }
            delete[] arr;
            arr = temp;
            capacity = index;
        }
    }

    void DelEnd(){//Delete from the End of the Array
        if (size == 0 || capacity == 0){ //If the Array is Empty
            return;
        }
        if (front == (size - 1)){ //If the Front is At the End of the Array
            front = -1;
            size--;
        }
        else if (((front + size) % capacity) == 0){
            size -= 1;
        }
        else{
            size -= 1;
        }
        DelCheck();
    }
    
    void DelFront(){//Delete from the Front of the Array
        if (size == 0 || capacity == 0){
            return;
        }
        if ((front == ((front + size) % capacity)) && !(size == capacity)){
            front = -1;
            size--;
        }
        else{
            if (front == (capacity - 1)){
                front = 0;
                size -= 1;
            }
            else{
                front += 1;
                size -= 1;
            }
        }
        DelCheck();
    }
};

/*
CS 201 PHASE 3 Heap Class [Part 1]
Owen Russell
*/

template <typename keytype>
class Heap{
private:
    int capacity;
    int heapSize;

    void heapify(int i){//Heapify For Insert
        int parent = (i - 1) / 2;
        if (i > 0){
            if ((HeapPTR->getter(i) < HeapPTR->getter(parent))){
                HeapPTR->tSwap(i, parent);
                heapify(parent);
            }
        }
     
    }

    //Bottom Up Heap Build
    void minHeapify(int i){
        int total;
        if (capacity != 0)
            total = capacity;
        else
            total = heapSize;

        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;
        // reorganize heap in accordance with smallest key
        if (l < total && HeapPTR->getter(l) < HeapPTR->getter(smallest)){
            smallest = l;
        }
        if (r < total && HeapPTR->getter(r) < HeapPTR->getter(smallest)){
            smallest = r;
        }
        if (smallest != i){
            HeapPTR->tSwap(i, smallest);
            minHeapify(smallest);
        }
    }

    // return the height of the heap
    int height(int N){
        return (int)ceil(log(N + 1) / log(2)) - 1;
    }

    // print the heap
    void printTree(){
        for (int i = 0; i < heapSize+1; i++) {
            for (int j = 0; j < pow(2, i) && j + pow(2, i) < heapSize+1; j++) {
                cout << HeapPTR->getter(j + (int)pow(2, i) - 1) << " ";
            }
        }
    }
    
    // build the heap (Bottom Up)
    void buildHeap(int n){
        int index = (n / 2) - 1;
        for (int i = index; i >= 0; i--){
            minHeapify(i); 
        }
    }

public:
    CDA<keytype> *HeapPTR; //Heap (CDA)
    
    //Default Heap Constructor
    Heap(){
        heapSize = 0;
        capacity = 0;
        HeapPTR = new CDA<keytype>;
    }

    //Construct heap from a keytype array of size s
    Heap(keytype k[], int s){
        capacity = s;
        HeapPTR = new CDA<keytype>;
        heapSize = 0;
        for (int i = 0; i < s; i++){
            HeapPTR->AddEnd(k[i]);
        }
        buildHeap(capacity);
        heapSize = capacity;
    }

    //Destructor
    ~Heap(){
        delete HeapPTR;
    }

    // insert key k into heap
    void insert(keytype k){
        HeapPTR->AddEnd(k);
        heapSize++;
        heapify(heapSize - 1);
    }

    //Required Peek Key Function
    keytype peekKey(){
        keytype ans = HeapPTR->getter(0); //Get the Min Key in the heap
        return ans;
    }

    //Does a Peek Key but then Deletes the Min Node as well
    keytype extractMin(){
        keytype min;
        if (heapSize == 1){
            heapSize--;
            min = HeapPTR->getter(0);
            HeapPTR->DelFront();
            return min;
        }

        min = HeapPTR->getter(0);
        HeapPTR->tSwap(0, heapSize-1);
       
        heapSize--;
        minHeapify(0);
        return min;
    }

    // print heap
    void printKey(){
        if (capacity != 0){
            printTree();
            cout << endl;
            return;
        }

        for (int i = 0; i < heapSize; i++){
            cout << HeapPTR->getter(i) << " ";
        }
        cout << endl;
    }
};


