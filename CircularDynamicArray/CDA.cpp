/*
Circular Dynamic Array
Owen Yeats Russell
*/
#include <random>
#include <iostream>

template <class T>
class CDA {
    private: 
        int capacity;   //the amount of data slots allocated
        int size;       //the size of the array as if it was a regular 0-n array
        int head;       //head index
        T* data;        //Data Array
        T bin;          //Junk data
        int next;       //For Binary Search

       void doubletheCapacity(){                                //Double The Capacity when trying to add one value to a full CDA
           int doubledCapacity = this->capacity * 2;            //Doubled the capacity
           T *doubledData = new T[doubledCapacity];             //Creates the new CDA with the doubled capacity
           for(int i = 0; i < size; i++){
               doubledData[i] = this->data[(this->head + i) % this->capacity];  //Assigns the data into the correct indexes
           }
           delete [] this->data;                                //Deletes the old data list 
           this->data = doubledData;                            //Assigns the new doubled data
           this->capacity = doubledCapacity;                    //Assigns the new doubled capacity
           this->head = 0;                                      //Resets the Head to 0
       }

		void reduceCapacity50(){        //Cut capacity in half when less than 25% of the array is being used
			if (size==0){               //If the array is empty it will always be less than 25%, this accounts for that edge case
                return;
            }
			int reducedCapacity = this->capacity/2;         //Calculates the new capacity (divide by two because of 50% reduction)
			T *reducedData = new T[reducedCapacity];        //initialize the new array of size reducedCapacity
			for (int i=0;i<size;i++) {
				reducedData[i] = this->data[(this->head + i) % this->capacity];
			}
			delete [] this->data;
			this->data=reducedData;
			this->capacity=reducedCapacity;
			this->head=0;
		}
    
        int indexCalculator(int i){ //Passing in a regualr 0-n index and converting it to the corresponding CDA index
            return (this->head + i) % this->capacity;
        }

        void merge(int left, int mid, int right){
            // int left, int mid, int right
            auto const subArrayOne = mid - left + 1;
            auto const subArrayTwo = right - mid;
            auto *leftTemp = new int[subArrayOne];
            auto *rightTemp = new int[subArrayTwo];
            for(auto i = 0; i < subArrayOne; i++){
                leftTemp[i] = this->data[indexCalculator(i+left)];
            }
            for(auto j = 0; j < subArrayTwo; j++){
                rightTemp[j] = this->data[indexCalculator(j+mid+1)];
            }
            auto indexOfSubArrayOne = 0;
            auto indexOfSubArrayTwo = 0;
            int indexOfMergedArray = left;

            while(indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo){
                if(leftTemp[indexOfSubArrayOne] <= rightTemp[indexOfSubArrayTwo]){
                    this->data[indexCalculator(indexOfMergedArray)] = leftTemp[indexOfSubArrayOne];
                    indexOfSubArrayOne++;
                }
                else{
                    this->data[indexCalculator(indexOfMergedArray)] = rightTemp[indexOfSubArrayTwo];
                    indexOfSubArrayTwo++;
                }
                indexOfMergedArray++;
            }
            while(indexOfSubArrayOne < subArrayOne){
                this->data[indexCalculator(indexOfMergedArray)] = leftTemp[indexOfSubArrayOne];
                indexOfSubArrayOne++;
                indexOfMergedArray++;
            }
            while(indexOfSubArrayTwo < subArrayTwo){
                this->data[indexCalculator(indexOfMergedArray)] = rightTemp[indexOfSubArrayTwo];
                indexOfSubArrayTwo++;
                indexOfMergedArray++;
            }
        }

        void mergeSort(int begin, int end){
            if(begin >= end){
                return; //Recursive Return statement
            }
            auto mid = begin + (end - begin) / 2;
            mergeSort(begin, mid);
            mergeSort(mid+1, end);
            merge(begin, mid, end);
        }

    public:
        CDA(){//Finished
            this->capacity = 1;                 //set capacity to 1
            this->size = 0;                     //set the size to 0
            this->head = 0;                     //set the original head to 0
            this->data = new T[this->capacity];       //create the array
            this->bin = 0;                      //start the bin data [only changed on deletion]
        }

        CDA(int s){//Finished
            this->size = s;                     //set the size to s
            this->capacity = s;                 //set the capcaity to s
            this->head = 0;                     //set the original head to 0
            this->data = new T[this->capacity];       //create the array
            this->bin = 0;                      //start the bin data [only changed on deletion]
        }

        CDA(int s, CDA init){ //FIXME - REBUILD
            this->size = s;                     //set the size to s
            this->capacity = s;                 //set the capacity to s
            this->head = 0;                     //set the original head to 0
            this->data = new T[this->capacity]; //Initialize the new data array
            this->bin = 0;                      //Set junk to 0

        }

        ~CDA(){
            delete [] this->data;                  //free the alocated memory for the array
        }

        T& operator[](int i){            
            //this function allows the user to access the CDA as if it was a normal 0..N array
            if( i < 0 || i >= this->size){      //check if the index requested is in range 0-size-1;
                std::cout << "Reference is out of bounds for " << i << std::endl; //
                return this->bin;               //return junk
            }
            else{
                return this->data[(this->head + i) % this->capacity];       //return the data in the relative index requested for
            }
        };

        void AddEnd(T v){
            if(this->size == this->capacity){
                this->doubletheCapacity();                                          //This private function doubles the capacity of the array and copies the data into the new array
                this->data[(this->size + this->capacity) % this->capacity] = v;     //This assigns the data into the correct index of the list
                this->size++;                                                       //Increment the size by 1 after adding the data
                return;
            }
            else{
                this->data[(this->size+this->head)%this->capacity] = v;             //add the pointer to the end location
                this->size++;                                                       //Increment the size by 1 after adding the data
                return;
            }
        }

        void AddFront(T v){//Finished
            if(this->size == this->capacity){                                     //Check if the array is trying to access the first out of range slot
                this->doubletheCapacity();                                          //If so double the capacity
                this->head = (this->capacity - 1 + this->head) % this->capacity;    //Then assign the new head indeces
                this->data[this->head] = v;                                         //Assign the data
                this->size++;                                                       //Increment the size to account for the new data
            }
            else{
                this->head = (this->capacity - 1 + this->head) % this->capacity;    //Assign the new head indeces
                this->data[this->head] = v;                                         //
                this->size++;
            }
        }

        void DelEnd(){//Finished
            //Delete from the last index of the CDA but keeps the memeory allocated incase of a new write
            //Memory will only be freed on downsizing the array
            this->size--;                                                                       //Recduce the size by 1 while leaving capacity as is

            if (((double) this->size)/((double) this->capacity) <= .25) {       //Check if the array is using less than 25% of its allocated memory after the delete
				this->reduceCapacity50();                                       //If so reduce the size of the array by 50%
                //Memory will only be freed upon size reduction
			}
            //Amortized runtime O(1);
        }

        void DelFront(){//Finished
            //Delete from the first index of the CDA
            //T data = this->data[this->head];                                    //maybe uncomment this if it breaks
			this->head=(this->head+1)%this->capacity;                           //set the new head as one after the current head
			this->size--;                                                       //reduce the size by 1

			if (((double) this->size)/((double) this->capacity) <= .25) {       //Check if less than 25% of the array is used
				this->reduceCapacity50();                                       //If so then cut the array in half
			}
        }

        int Length(){
            return size;    //Return the size of the array
        }

        int Capacity(){
            return capacity;  //Retrun the capacity of the array
        }

        int partition(int min, int max, int pivot){//Partition function for Quick Select
            if(max <= min){
                return min;
            } 

            //Swapping the pivot index and the minimum index
            T swap = this->data[indexCalculator(min)];
            this->data[indexCalculator(min)] = this->data[indexCalculator(pivot)];
            this->data[indexCalculator(pivot)] = swap;

            T pivotList = this->data[indexCalculator(min)];
            min++;
            int i = min;
            while(true){
                if(this->data[indexCalculator(i)] <= pivotList){
                    this->data[indexCalculator(i-1)] = this->data[indexCalculator(i)];
                    i++;
                    if(i > max){
                        this->data[indexCalculator(i-1)] = pivotList;
                        return i-1;
                    }
                }
                else if (this->data[indexCalculator(i)] > pivotList){
                    swap = this->data[indexCalculator(max)];
                    this->data[indexCalculator(max)] = this->data[indexCalculator(i)];
                    this->data[indexCalculator(i)] = swap;
                    max--;

                    if(i > max){
                        data[indexCalculator(i-1)] = pivotList;
                        return i-1;
                    }
                }
            }
        }

        int randomPivot(int min, int max){//Pivot function for Quick Select
            srand(time(NULL));
            int randomPiv = min + rand() % (max - min);                             //Generate a random pivot
            return randomPiv;                                                       //return the random pivot
        }

        T Select(int k){//Quick Select Function
            k--;
            if(k < 0 || k > this->size-1){
                return this->bin; //returning junk if out of bounds reference
            }
            int pivotIndex = 0;
            int max = size-1;
            int min = 0;
            pivotIndex = randomPivot(min, max);                                     //Collect a random pivot point
            while(pivotIndex != k){                                                 //enter the selection loop
                if(pivotIndex<k){
                    min = pivotIndex + 1;
                }
                else{
                    max = pivotIndex - 1;
                }
                pivotIndex = partition(min, max, min);
            }
            return this->data[indexCalculator(k)];
        }

        void Sort(){//Sort function using Merge Sort
            mergeSort(0, size-1); 
        }

        int Search(T e){//Search for an element within the array in O(n) time
            for (int i=0;i<this->size;i++) {//Loop through the array
				if (e == this->data[(this->head + i) % this->capacity]) return i; //When element is found return the index of the element
			}
			return -1; //if it is not found return -1
        }

        int recursiveBinSearch(int lower, int upper, T e){//Recurisive helper function for Binary Search
            int middle;
            if(upper >= lower){
                middle = (lower + upper) / 2;
                if(operator[](middle) == e){
                    return middle;
                }
                if(operator[](middle) > e){
                    next = middle;
                    return recursiveBinSearch(lower, middle - 1, e);
                }
                return recursiveBinSearch(middle+1, upper, e);
            }
            return -1;
        }

        int BinSearch(T e){//Main Binary Search Function
            int answer = recursiveBinSearch(0, size-1, e);
            if(answer == -1){
                answer = ~(next);
            }
            return answer;
        }
};
