# A Growing Data Structure Library
This is my library to practice implementing more complex data structures.

## What do these data structures do?
All of these data structures are implemented as C++ classes and can work with templated data allowing for easy integration with any types of data projects. The data structures implemented in this library include:
* Circular Dynamic Array
* Treap
* Fibonacci Heap
* Heap

## The Circular Dynamic Array (C++)
This Circular Dynamic Array has the following attributes:
  * Capacity Doubles when adding data to a full array
  * Capacity is reduced by half when less than 25% of the capacity contains data<br>
Here is an image illustrating the format of the data structure:

## Treap | (Binary Search Tree + Heap)
The requirements for this treap include:
  * Maintain binary search tree order:
      * smaller data points are always left children of the current node
      * larger data points are always right children of the current node
  * Maintain a heap heirarchy with values of higher priority at the top of the treap and lower priority towards the leaves of the treap<br>
Here is an image illustrating the structure of the treap:
