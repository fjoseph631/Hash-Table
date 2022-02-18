#ifndef CUCKOOHASH_H_INCLUDED
#define CUCKOOHASH_H_INCLUDED
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <atomic>
#include <thread>
#include <vector>
//#include "mingw.thread.h"
//#include  "include/thread.h"
//#include "include/conditionvariable.h"
//#include "include/future.h"
//#include "include/mutex.h"
//#include "include/shared_mutex.h"

#define POINTERBITS 0x0000fffffffffffc
#define FULLPOINTER 0xffff000000000000

#define COUNTSHIFT 281474976710656
#define null NULL
// Int Declarations
const int NOTFOUND = -1;
const int INIT = 0;
const int TABLEONE = 0;
const int FIRST = 1;
const int TABLETWO = 1;
const int SECOND = 2;
const int NUMTABLES = 2;
const int SHIFTAMOUNT = 48;
int TESTSIZE = 8388617;
// const int TESTSIZE=8388608;
int NUMOPS = 10000000;
// int NUMOPS=10;

const double loadFactor = .4;
int numThreads = 32;
int ops = 0;
std::atomic<int> *operations;
// operations->init(0);

// std::atomic_init(operations,0);
// Support Functions
int hashOne(int key, int size);
int hashTwo(int key, int size);
bool checkCounter(int ts1, int ts2, int ts1x, int ts2x);

// Hash entry
class Entry
{
public:
    int key;
    int value;
    Entry(int k, int v)
    {
        key = k;
        value = v;
    }
    ~Entry()
    {
    }
};
// vector::

// Declaring CountPointer
class CountPointer
{
public:
    CountPointer makePointer(Entry *e, uint16_t cnt);
    ~CountPointer()
    {
    }
};
CountPointer *makePointer(Entry *e, uint16_t cnt)
{
    return (CountPointer *)((((uint64_t)cnt) << SHIFTAMOUNT) | ((uint64_t)e & 0xFFFFFFFFFFFF));
}
// Extracting Address from Slot Pointer
Entry *getAddress(CountPointer *ptr)
{
    // unsigned long a=(unsigned long)ptr&0xFFFFFFFFFFFE;
    Entry *e = (Entry *)((uint64_t)ptr & (POINTERBITS));
    return e;
}
// Primary Class
class CuckooHashTable
{
public:
    // Variable Declarations

    int fails;
    int relocates;

    vector<Entry *> *hazards;
    //    atomic<int>* operations;
    atomic<CountPointer *> *table1;
    atomic<CountPointer *> *table2;
    atomic<CountPointer *> **tables;
    int *aSizes;
    double *loads;
    // Core functions
    void insert(int key, int value);
    void remove(int key);
    int search(int key);

    void printTable();
    int find(int key, CountPointer **p1, CountPointer **p2);
    void testInit();
    // Ancillary Functions
    const int getSizeOne();
    const int getSizeTwo();
    int getSize(int table);

    // Constructor
    CuckooHashTable(int sizeA, int sizeB)
    {
        // printf("%d",sizeA);
        sOne = sizeA;
        sTwo = sizeB;

        // operations=new std::atomic<int>;

        tables = new std::atomic<CountPointer *> *[NUMTABLES];
        table1 = new std::atomic<CountPointer *>[sOne];
        table2 = new std::atomic<CountPointer *>[sTwo];

        tables[TABLEONE] = table1;
        tables[TABLETWO] = table2;
        hazards = new vector<Entry *>();
        fails = INIT;
        relocates = INIT;
        aSizes = new int[NUMTABLES];
        aSizes[TABLEONE] = INIT;
        aSizes[TABLETWO] = INIT;
        loads = new double[NUMTABLES];
        loads[TABLEONE] = (double)INIT;
        loads[TABLETWO] = (double)INIT;

        init();
        // testInit();
    }
    ~CuckooHashTable()
    {
        for (int i = 0; i < getSizeOne(); i++)
        {
            Entry *temp = getAddress(table1[i]);
            if (temp != null)
            {
                table1[i] = NULL;
                free(temp);
            }
            //    free(table1[i]);
        }
        free(table1);
        for (int i = 0; i < getSizeTwo(); i++)
        {
            Entry *temp = getAddress(table2[i]);
            if (temp != null)
            {
                table2[i] = NULL;
                free(temp);
            }
        }
        delete[] aSizes;
        delete[] loads;
        free(table2);
        free(tables);
        // delete this;
    }
    void clear(CuckooHashTable *z)
    {
        for (int j = 0; j < z->getSizeOne(); j++)
        {
            Entry *temp = getAddress(table1[j]);
            if (temp != null)
            {
                table1[j] = makePointer(NULL, INIT);
                free(temp);
            }
        }
        for (int i = 0; i < getSizeTwo(); i++)
        {
            Entry *temp = getAddress(table2[i]);
            if (temp != null)
            {
                table2[i] = makePointer(NULL, INIT);
                free(temp);
            }
        }
    }

private:
    int sOne;
    int sTwo;
    bool relocate(int which, int index);
    // Helper Functions

    void init();
    void helpRelocate(int which, int index, bool initiator);
    void delDup(int iOne, CountPointer *e1, int iTwo, CountPointer *e2);
    void rehash();
};

#endif // CUCKOOHASH_H_INCLUDED
