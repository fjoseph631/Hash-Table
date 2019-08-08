#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <api/api.hpp>
#include <common/platform.hpp>
#include <common/locks.hpp>
#include "bmconfig.hpp"
#include "../include/api/api.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<atomic>
#include<thread>
#include <math.h>
#include <vector>
#include <random>
//#include<stm>
//unsigned long b = 281474976710656;
#define POINTERBITS 0x0000fffffffffffc
#define FULLPOINTER 0xffff000000000000
#define COUNTSHIFT 281474976710656
//Int Declaration
int fails=0;
//int numThreads=1;
const int NOTFOUND=-1;
const int INIT=0;
const int TABLEONE=0;
const int FIRST=1;
const int TABLETWO=1;
const int SECOND =2;
const int NUMTABLES =2;
const int SHIFTAMOUNT=48;
const int TESTSIZE=8388608;
const int NUMOPS=10000000;
//int NUMOPS=10;
const double loadFactor=.4;
int numThreads=32;

std::atomic<int>* operations;
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
};
class CountPointer
{
    public:
        CountPointer makePointer(Entry* e, uint16_t cnt) ;
        ~CountPointer()
        {

        }
};
CountPointer* makePointer(Entry* e, uint16_t cnt) {
  return (CountPointer*)((((uint64_t)cnt) << SHIFTAMOUNT) | ((uint64_t)e & 0xFFFFFFFFFFFF));
}
int hashOne(int key,int size);

int hashTwo(int key,int size);
bool checkCounter(int ts1, int ts2, int ts1x, int ts2x);
//Primary Class
class CuckooHashTable
{
public:
    int sOne;
    int sTwo;
    int activeSizeA;
    int activeSizeB;
    int fails;
    int relocates;
    double loadFactorA;
    double loadFactorB;
    std::vector<Entry*> *hazards;
   // atomic<int>* operations;

    //std::vector<Entry*> *hazards;
    CountPointer** table1;
    CountPointer** table2;
    CountPointer***tables;

    int find(int key, CountPointer** e1,CountPointer**e2);
    void init();
    void printTable();
    void insert(int key, int value/*,int id*/);
    void remove(int key/*,int id*/);
    int search(int key/*,int id*/);
    bool relocate(int which, int index/*,int id*/);
    void helpRelocate(int which, int index, bool initiator/*,int id*/);
    void delDup(int iOne, CountPointer* e1,int iTwo ,CountPointer* e2/*,int id*/);

    //Constructor
    CuckooHashTable(int sizeA,int sizeB)
    {
        sOne=sizeA;
        sTwo=sizeB;
        //;

        tables= (CountPointer***)TM_ALLOC(NUMTABLES*sizeof(CountPointer**));
        table1 = (CountPointer**)TM_ALLOC(sizeof(CountPointer*) *sOne);
        table2 = (CountPointer**)TM_ALLOC(sizeof(CountPointer*) *sTwo);


        tables[TABLEONE] = table1;
		tables[TABLETWO] = table2;
        //hazards =new vector<Entry *>();
        init();
    }
};
CuckooHashTable* z;
//Initializing Table to NULL
void CuckooHashTable::init()
{

    for(int i=0;i<sOne;i++)
    {
        table1[i]=makePointer(NULL,INIT);
    }
    for(int i=0;i<sTwo;i++)
    {
        table2[i]=makePointer(NULL,INIT);
    }
    operations=0;
    activeSizeA=0;
    fails=0;
}
int hashOne(int key, int size){
     //return key%size;
      int c2=0x27d4eb2d; // a prime or an odd constant
      key = (key ^ 61) ^ (key >> 16);
      key = key + (key << 3);
      key = key ^ (key >> 4);
      key = key * c2;
      key = key ^ (key >> 15);
       key=abs(key);
	return (key)&(size-1)%size;
}

//Hash Function 2
int hashTwo(int key, int size){
    //return key/size % size;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
     key=abs(key);
	return (key)&(size-1)%size;
}

//Check Counter of Slots
bool checkCounter(int c1,int c2,int c1s,int c2s)
{
    if(c1s-c1>=2||c2s-c2>=2)
        return true;
    return false;
}
//Get Number of Relocations For Slot
uint16_t getCount(CountPointer* ptr)
{
    return (uint16_t)(((uint64_t)ptr >> 48) & 0xFFFF);
}
//Extracting Address from Slot Pointer
Entry* getAddress(CountPointer* ptr)
{
    //unsigned long a=(unsigned long)ptr&0xFFFFFFFFFFFE;
    Entry* e = (Entry *)((uint64_t)ptr & (POINTERBITS));
	return e;

}
//Marking Least Significant Bit
Entry* marking(Entry* ptr)
{
    int k,v;
    k=ptr->key;
    v=ptr->value;
    Entry* e = (Entry *)((uint64_t)ptr | (1<<0));
    e->key=k;
    e->value=v;
	return e;
}
//UnMarking Least Significant Bit
Entry* deMarking(Entry* ptr)
{
    int k,v;
    Entry *e;
    if(ptr!=NULL)
    {
        k=ptr->key;
        v=ptr->value;
        e = (Entry *)((uint64_t)ptr & (~1));
        e->key=k;
        e->value=v;
    }
    else
    {
        e = (Entry *)((uint64_t)ptr & (~1));
    }
	return e;
}

//Checking If entry has been marked
bool marked(void *ptr)
{
	if((uint64_t)ptr & 1<<0)
		return true;
	else
		return false;
}
using namespace std;

const int THREAD_COUNT = 8;
const int NUM_TRANSACTIONS = 100000;

// shared variable that will be incremented by transactions
int x = 0;

Config::Config() :
    bmname(""),
    duration(1),
    execute(0),
    threads(THREAD_COUNT),
    nops_after_tx(0),
    elements(256),
    lookpct(34),
    inspct(66),
    sets(1),
    ops(1),
    time(0),
    running(true),
    txcount(0)
{
}

Config CFG TM_ALIGN(64);

void* run_thread(void* i) {
    // each thread must be initialized before running transactions
    TM_THREAD_INIT();
	std::random_device rd{};

        // Use Mersenne twister engine to generate pseudo-random numbers.
        std::mt19937 engine{rd()};

        // "Filter" MT engine's output to generate pseudo-random double values,
        // **uniformly distributed** on the closed interval [0, 1].
        // (Note that the range is [inclusive, ins].)
        std::uniform_real_distribution<float> dist{0.0, 100.0};
    for(int i=0; i<10000000/THREAD_COUNT; i++) {
        // mark the beginning of a transaction
        TM_BEGIN(atomic)
        {
        thread_local std::mt19937 gen(time(NULL));
        int a=dist(engine);
    
        if(a<94)
        {
            int b=(int)gen();
            b=abs(b);
            z->search(b/*, pthread_self()*/);
        }


        else if(a<97)
        {
             int k=(int)gen();
             int v=(int)gen();
             k=abs(k);
             z->insert(k,v/*, pthread_self()*/);
          //   z->insert(abs((int)gen()/TESTSIZE),(int)gen()/TESTSIZE);
        }
        else
        {
            int r=(int)gen();
            z->remove(abs(r)/*, pthread_self()*/);
        }
        //z->search(abs(s));
        //z->search(abs(s));
        //z->insert(abs(k),abs(v));
	// add this memory location to the read set
	int y = TM_READ(x);
	// add this memory location to the write set
	TM_WRITE(x, y+1);
	}

        TM_END; // mark the end of the transaction
    }

    TM_THREAD_SHUTDOWN();
}
void filling()
{
	int numElements=0;
	double load=0.0;
	double loadthreshold =0.4;
	thread_local std::mt19937 gen(time(NULL));
	while(load<loadthreshold)
	{
		int k=(int)gen();
		int v=(int)gen();
		z->insert(abs(k),abs(v));
		numElements++;
		load=(double)numElements/double(z->sOne+z->sTwo);
	}
}

void CuckooHashTable::insert(int key,int value/*,int id*/)
{
//Hashing

    int h1=hashOne(key,sOne);
    int h2=hashTwo(key,sTwo);
    int countOne,countTwo;
    //New Pointer
    int trials=0;

    Entry* newNode=(Entry*)TM_ALLOC( sizeof(Entry*));
    if(newNode==NULL)
    {
        fails++;
        return;
    }
    //    newNode->compare_exchange_strong(NULL,new Entry(key,value));
    CountPointer *cnt1,*cnt2,*temp;

    startInsert:
        int result=find(key,&cnt1,&cnt2);
        if(result==FIRST)
        {
            countOne=getCount(cnt1);
			CountPointer*s =tables[TABLEONE][h1];
			tables[TABLEONE][h1]=makePointer(newNode,countOne);
            if (!(s==(cnt1)))
            {
                //cout<<"Problem"<<endl;
                ++trials;
                goto startInsert;
            }
            else
            {
                //tables[TABLEONE][h1]=makePointer(newNode,countOne+1);

                return;
            }
        }
        if(result==SECOND)
        {
            countTwo=getCount(cnt2);
			CountPointer*s =tables[TABLETWO][h2];
			tables[TABLETWO][h2]=makePointer(newNode,countTwo);
			//TM_WRITE(tables[TABLETWO][h2],makePointer(newNode,countTwo));
            if(!(s==cnt2))
            {
                 //delete newNode,cnt2,cnt1;
                               //  cout<<"Problem"<<endl;
                   ++trials;
                 goto startInsert;
            }

            else
            {
                 //delete newNode,cnt2,cnt1;
                 //tables[TABLETWO][h2]=makePointer(newNode,countTwo+1);

                return;
            }
        }

        if(getAddress(cnt1)==NULL)
        {
            countOne=getCount(cnt1);
			CountPointer*s =tables[TABLEONE][h1];
			tables[TABLEONE][h1]=makePointer(newNode,countOne);
            if (!(s==(cnt1)))
            {
                 //delete newNode,cnt2,cnt1;
                               //  cout<<"Problem"<<endl;
                   ++trials;
                 goto startInsert;
            }

            else
            {
                 //delete newNode,cnt2,cnt1;
                 //tables[TABLEONE][h1]=makePointer(newNode,countOne+1);

                return;
            }
        }
        if(getAddress(cnt2)==NULL)
        {
            countTwo=getCount(cnt2);
			CountPointer*s =tables[TABLETWO][h2];
			tables[TABLETWO][h2]=makePointer(newNode,countTwo);
			//TM_WRITE(tables[TABLETWO][h2],makePointer(newNode,countTwo));
            if(!(s==cnt2))
			{
                 //delete newNode,cnt2,cnt1;
                               //  cout<<"Problem"<<endl;
                   ++trials;
                 goto startInsert;
            }

            else
            {
                 //delete newNode,cnt2,cnt1;
                 //tables[TABLETWO][h2]=makePointer(newNode,countTwo+1);

                return;
            }
        }
		if(++trials>log2(sOne+sTwo))
			return;
    
            bool r=false;
           // cout<<"Relocate Needed "<<trials<<endl;
           ++relocates;
         r=relocate(TABLEONE,h1);
        if(r)
        {
            ++trials;
            goto startInsert;
        }
        else
        {
            //cout<<"Rehash really Needed "<<newNode->key<<endl;
             //delete[] newNode,cnt2,cnt1;
           // rehash();
        }
    //delete newNode,cnt2,cnt1;
    return;
}
void CuckooHashTable::remove(int key/*,int id*/)
{
     int h1 = hashOne(key,sOne);
    int h2 = hashTwo(key,sTwo);

    CountPointer*c1,*c2;
    c1=NULL; c2=NULL;
    int countOne=0,countTwo=0;
    while(true)
    {

        int ret=find(key,&c1,&c2);
        if(ret==NOTFOUND)
            return;
        if(ret==FIRST)
        {
            countOne=getCount(c1);
			
			CountPointer*s =tables[TABLEONE][h1];
			tables[TABLEONE][h1]=makePointer(NULL,countOne);
            if ((s==(c1)))
            {
                //tables[TABLEONE][h1]=makePointer(NULL,countOne);
                //delete c1;
                return;
            }

        }
        if(ret==SECOND)
        {
            if((tables[TABLEONE][h1])!=c1)
                continue;
            countTwo=getCount(c2);
			CountPointer*s =tables[TABLETWO][h2];
			tables[TABLETWO][h2]=makePointer(NULL,countTwo);
            if ((s==(c2)))
            {
                //tables[TABLETWO][h2]=makePointer(NULL,countOne);
                //delete c1;
                return;
            }
        }
    }

}
int CuckooHashTable::search(int key/*,int id*/)
{
    int h1;
    int h2;
    int cOne,cTwo,cOneB,cTwoB;
    Entry* e;
    CountPointer* c;
    while(true)
    {
        //hash functions
        h1 = hashOne(key,sOne);
        h2 = hashTwo(key,sTwo);
        //get address
        c =(tables[TABLEONE][h1])/*,memory_order_relaxed*/;
        //extracting address and count
        cOne=getCount(c);
        e= getAddress(c);
        if (e!=NULL && e->key == key)
        {
            //delete c;
            return e->value;
        }


        c =(tables[TABLETWO][h2])/*memory_order_relaxed*/;

		cTwo = getCount(c);
		e= getAddress(c);
        if (e!=NULL && e->key == key)
            return e->value;
        //second round of query
        c =(tables[TABLEONE][h1])/*memory_order_relaxed*/;
        cOneB=getCount(c);
        e= getAddress(c);
        if (e!=NULL && e->key == key)
        {
             return e->value;
        }


        c =(tables[TABLETWO][h2])/*memory_order_relaxed*/;

		cTwoB = getCount(c);
		e= getAddress(c);
        if (e!=NULL && e->key == key)
        {
              return e->value;
              //delete c;
        }

        //Check counter
        if(checkCounter(cOne,cTwo,cOneB,cTwoB))
            continue;
        else
        {
//            delete c;
            return NOTFOUND;
        }

    }
    return NOTFOUND;
}
int CuckooHashTable::find(int key,CountPointer **e1,CountPointer**e2/*,int id*/)
{
    int h1;
    int h2;
    int result;
    int cOne,cTwo,cOneB,cTwoB;
    //hashing
    h1 = hashOne(key,sOne);
    h2 = hashTwo(key,sTwo);
    //Other declarations
    CountPointer *c1,*c2;
    Entry* e;

    while(true)
    {
        //Thread local entry*
        c1=(tables[TABLEONE][h1]);
        *e1=c1;
        cOne=getCount(c1);
        e= getAddress(c1);
        if(e!=NULL)
        {
            if(marked(e))
            {
                helpRelocate(TABLEONE,h1,false);
                continue;
            }
            if(e->key==key)
            {
                result=FIRST;
            }
        }
        //Check Second Table
        //c2=TM_READ(tables[TABLETWO][h2],pthread_self(void));
        c2=(tables[TABLETWO][h2]);

        *e2=c2;

        cTwo=getCount(c2);
        e= getAddress(c2);
        if(e!=NULL)
        {
            //If Marked Help with relocation
            if(marked(e))
            {
                helpRelocate(TABLETWO,h2,false);
                continue;
            }
            if(e->key==key)
            {
                if(result==FIRST)
                {

                    delDup(h1,*e1,h2,*e2);
                }
                else
                {
                    result=SECOND;
                }

            }
        }
        if(result==FIRST||result==SECOND)
        {
            //delete[] c1,c2,e;
            return result;
        }


        //Round Two
        c1=(tables[TABLEONE][h1]);
        *e1=c1;
        cOneB=getCount(c1);

        e= getAddress(c1);

        if(e!=NULL)
        {
            if(marked(e))
            {
                helpRelocate(TABLEONE,h1,false);
                continue;
            }
            if(e->key==key)
            {
                result=FIRST;
            }
        }


        c2=(tables[TABLETWO][h2]);
        *e2=c2;
        cTwoB=getCount(c2);
        e= getAddress(c2);

        if(e!=NULL)
        {
            if(marked(e))
            {
                helpRelocate(TABLETWO,h2,false);
                continue;
            }
            if(e->key==key)
            {
                if(result==FIRST)
                {
                    delDup(h1,*e1,h2,*e2);
                }

                else
                    result=SECOND;
            }
        }
        if(result==FIRST||result==SECOND)
        {

            return result;
        }

        if(checkCounter(cOne,cTwo,cOneB,cTwoB))
            continue;
        else
        {

            return NOTFOUND;
        }

    }
    return NOTFOUND;
    return NOTFOUND;
}
bool CuckooHashTable::relocate(int which, int index/*,int id*/)
{
    //initializations
    int THRESHOLD=sOne+sTwo;
    THRESHOLD=log2(THRESHOLD);
    int route[THRESHOLD]; //storing cuckoo path
    int startLevel,tbl,idx,preIDX;
    startLevel=0;
    tbl=which;
    idx=index;preIDX=0;

    CountPointer*pre=NULL,*c1,*c2;
    Entry* e1=NULL,*preADDR=NULL,*e2;
    int trials=0;
    int key=0;
    bool found=false;
    pathDiscovery:
        {
        if(trials>10)
            return found;

        int depth=startLevel;
        for(int i=0;i<THRESHOLD;i++)
        {
            route[i]=0;
        }
        do
        {
            //cout<<"Address "<<&tables[tbl][idx]<<endl;
            c1=(tables[tbl][idx]);
//            c1=tables[tbl][idx];
            e1=getAddress(c1);

            while(marked(e1))
            {
                helpRelocate(tbl,idx,false);
                c1=(tables[tbl][idx]);
            }
                if(pre==c1)
                {
                    if(tbl==0)
                    {
                        delDup(idx,c1,preIDX,pre);
                    }
                    else
                    {
                        delDup(preIDX,pre,idx,c1);
                    }
                }
                if(preADDR!=NULL&&e1!=NULL)
                {
                    if(preADDR->key==e1->key)
                    {

                        if(tbl==0)
                        {
                            delDup(idx,c1,preIDX,pre);
                        }
                        else
                        {
                            delDup(preIDX,pre,idx,c1);
                        }
                    }
                }

            if(e1!=NULL)
            {
                route[depth]=idx;
                key=e1->key;
                pre=c1;
                preIDX=idx;
                tbl=1-tbl;
                idx= (tbl==0)? hashOne(key,sOne):hashTwo(key,sTwo);
            }
            else
            {
                //trials++;
                found=true;
            }

        }while(!found&&++depth<THRESHOLD);
    if(found)
    {
        tbl=1-tbl;
        for(int i=depth-1;i>=0;--i,tbl=1-tbl)
        {
            idx=route[i];
            c1=(tables[tbl][idx]);
            e1=getAddress(c1);
            if(marked(e1))
            {
                helpRelocate(tbl,idx,false);
                c1=(tables[tbl][idx]);
            }
            if(e1==NULL)
            {
                continue;
            }
            int destIDX=(tbl==0)?hashTwo(key,sTwo):hashOne(key,sOne);
            c2=(tables[1-which][destIDX]);
            e2=getAddress(c2);
            if(e2!=NULL)
            {
                startLevel=i+1;
                idx=destIDX;
                tbl=1-tbl;
                trials++;

                goto pathDiscovery;
            }
            helpRelocate(tbl,idx,true/*,id*/);
        }
        }

        //++trials;
        return found;
    }

    //++trials;
    return found;
}
void CuckooHashTable::delDup(int iOne, CountPointer* e1,int iTwo ,CountPointer* e2/*,int id*/)
{
     CountPointer*temp;
    if(e1==NULL||e2==NULL)
        return;
    if(e1!=(tables[TABLEONE][iOne])&&e2!=(tables[TABLETWO][iTwo]))
        return;

    if(getAddress(e1)->key!=getAddress(e2)->key)
        return;
	(tables[TABLETWO][iTwo]=makePointer(NULL,getCount(e2)));
     
     //(&tables[TABLETWO][iOne])->compare_exchange_strong(e2,temp);
     //delete temp;
}
void CuckooHashTable::helpRelocate(int which, int index, bool initiator/*,int id*/)
{
    CountPointer* src,*dest;
    Entry* temp,*dst;
    int countOne,countTwo,nCount;
    while(true)
    {
        //src=tables[which][index];
		src=(tables[which][index]);
        countOne=getCount(src);
        temp=getAddress(src);

        if(temp==NULL)
        {
            return;
        }
        //Marking pointer to be relocated
        while(!marked(temp)&&initiator)
        {
            CountPointer*c1=makePointer(temp,countOne);

            temp=marking(temp);

	    (tables[which][index]=makePointer(temp,countOne));
		src=(tables[which][index]);
        }
        if(!marked(temp))
        {
            return;
        }
        int hd= (1-which==0)?hashOne(temp->key,sOne):hashTwo(temp->key,sTwo);
        //dest=(tables[1-which][hd]);
		dest=(tables[1-which][hd]);
        dst=getAddress(dest);
        countTwo=getCount(dest);
        nCount=countOne>countTwo?countOne+1:countTwo+1;
        if(src!=(tables[which][index]))
        {
            continue;
        }
		CountPointer* e=(tables[1-which][hd]);
		if(e==tables[1-which][hd])
		{
			(tables[1-which][hd]=makePointer(temp,nCount));
			if((tables[which][index]))
			{
				(tables[which][index]=makePointer(temp,countOne+1));
			}
		}
		if(dst==temp)
		{
			(tables[which][index]=makePointer(NULL,countOne+1));
			return;
		}
	
	(tables[which][index]=makePointer(deMarking(temp),countOne+1));
	return;
       
	}
}
//Print Table for testing purposes
void CuckooHashTable::printTable()
{
	printf("******************hash_table 1*****************\n");
	int i;
	CountPointer *e;
	Entry* tmp=NULL;
	for(i=0;i<sOne;i++){
		if(tables[TABLEONE][i] != NULL)
            {
                e =(tables[TABLEONE][i]);
                tmp = getAddress(e);

                if(tmp != NULL)
                {
                    printf("%d\t%d %d\n",i,tmp->key,tmp->value);
                    //cout<<getCount(e)<<endl;
                }
                else
                {
                    printf("%d\tNULL\n",i);
                }

            }
		else
			printf("%d\tNULL\n",i);

	}
	printf("****************hash_table 2*******************\n");
	for(i=0;i<sTwo;i++)
	{
		if(table2[i] != NULL)
            {
			e = (tables[TABLETWO][i]);
			tmp = getAddress(e);
            if(tmp != NULL)
                printf("%d\t%d %d\n",i,tmp->key,tmp->value);
            else
                printf("%d\t%p\n",i,e);
            }
		else
			printf("%d\tNULL\n",i);
	}
	printf("\n");
}
int main(int argc, char** argv) {
    TM_SYS_INIT();

    // original thread must be initalized also
    TM_THREAD_INIT();

    void* args[256];
    pthread_t tid[256];
    z =new CuckooHashTable(8388617,8388617);
	filling();
    // set up configuration structs for the threads we'll create
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    for (uint32_t i = 0; i < CFG.threads; i++)
        args[i] = (void*)i;

 auto  startPoint= std::chrono::_V2::high_resolution_clock::now();

    // actually create the threads
    for (uint32_t j = 1; j < CFG.threads; j++)
        pthread_create(&tid[j], &attr, &run_thread, args[j]);

    // all of the other threads should be queued up, waiting to run the
    // benchmark, but they can't until this thread starts the benchmark
    // too...
    run_thread(args[0]);

    // everyone should be done.  Join all threads so we don't leave anything
    // hanging around
    for (uint32_t k = 1; k < CFG.threads; k++)
        pthread_join(tid[k], NULL);

    // And call sys shutdown stuff
    TM_SYS_SHUTDOWN();
 	auto endPoint= std::chrono::_V2::high_resolution_clock::now();
//    cout<<endPoint<<endl;
    auto period=endPoint-startPoint;
    float duration=std::chrono::duration_cast<std::chrono::milliseconds>(endPoint-startPoint).count();
	float throughput=(float)NUMOPS/duration;
    cout<<"Through Put "<<throughput<<endl ;
	cout<<"duration "<<duration<<endl;
    printf("x = %d\n", x); // x should equal (THREAD_COUNT * NUM_TRANSACTIONS)

    //z->printTable();
    free(z->table1);
    free(z->table2);
    free(z);
    return 0;
}