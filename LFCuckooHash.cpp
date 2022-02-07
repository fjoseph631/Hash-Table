#include "CuckooHash.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <random>
#include <cstdlib>
#include <pthread.h>
#include <algorithm>


//Initializing Table to NULL
void CuckooHashTable::init()
{

    for(int i=0;i<CuckooHashTable::getSizeOne();i++)
    {
        table1[i]=makePointer(nullptr,INIT);
    }
    for(int i=0;i<CuckooHashTable::getSizeTwo();i++)
    {
        table2[i]=makePointer(nullptr,INIT);
    }
    operations=nullptr;

    fails=0;
}
//Get Size One
const int CuckooHashTable::getSizeOne()
{
    return sOne;
}
//Get Size
int CuckooHashTable::getSize(int table)
{
    if(table==TABLEONE)
        return CuckooHashTable::getSizeOne();
    else
        return CuckooHashTable::getSizeTwo();
}
// Get Size of Table Two
const int CuckooHashTable::getSizeTwo()
{
    return sTwo;
}
//Hash  Function 1
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

//Marking Least Significant Bit
Entry* marking(Entry const* ptr)
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
    if(ptr!=nullptr)
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
//Rehasing
void CuckooHashTable::rehash() {
    fails++;
    //std::cout<<"Hash functions failed table"<<std::endl;
    return;
}
//Checking If entry has been marked
bool marked(void *ptr)
{
	if((uint64_t)ptr & 1<<0)
		return true;
	else
		return false;
}

//Print Table for testing purposes
void CuckooHashTable::printTable()
{
	printf("******************hash_table 1*****************\n");
	int i;
	CountPointer *e;
	Entry* tmp=nullptr;
	for(i=0;i<getSizeOne();i++){
		if(tables[TABLEONE][i] != nullptr)
            {
                e = atomic_load_explicit(&tables[TABLEONE][i],memory_order_relaxed);
                tmp = getAddress(e);

                if(tmp != nullptr)
                {
                    printf("%d\t%d %d\n",i,tmp->key,tmp->value);
                    //cout<<getCount(e)<<endl;
                }
                else
                {
                    printf("%d\tnullptr\n",i);
                }

            }
		else
			printf("%d\tnullptr\n",i);

	}
	printf("****************hash_table 2*******************\n");
	for(i=0;i<getSizeTwo();i++)
	{
		if(table2[i] != nullptr)
            {
			e = atomic_load_explicit(&tables[TABLETWO][i],memory_order_relaxed);
			tmp = getAddress(e);
            if(tmp != nullptr)
                printf("%d\t%d %d\n",i,tmp->key,tmp->value);
            else
                printf("%d\t%p\n",i,e);
            }
		else
			printf("%d\tnullptr\n",i);
	}
	printf("\n");
}
//Search For key
int CuckooHashTable::search(int key)
{
    int h1;
    int h2;
    int cOne,cTwo,cOneB,cTwoB;
    Entry* e;
    CountPointer* c;
    while(true)
    {
	return -1;
        //hash functions

        h1 = hashOne(key,getSizeOne());
        h2 = hashTwo(key,getSizeTwo());
        //get address
        c = atomic_load(&tables[TABLEONE][h1]);
        //extracting address and count
        cOne=getCount(c);
        e= getAddress(c);
        if (e!=nullptr && e->key == key)
        {
            //delete c;
            return e->value;
        }


        c = atomic_load(&tables[TABLETWO][h2]);

		cTwo = getCount(c);
		e= getAddress(c);
        if (e!=nullptr && e->key == key)
            return e->value;
        //second round of query
        c = atomic_load(&tables[TABLEONE][h1]);

        cOneB=getCount(c);
        e= getAddress(c);
        if (e!=nullptr && e->key == key)
        {
             return e->value;
        }


        c = atomic_load(&tables[TABLETWO][h2]);

		cTwoB = getCount(c);
		e= getAddress(c);
        if (e!=nullptr && e->key == key)
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
//Find
int CuckooHashTable::find(int key,CountPointer** e1,CountPointer**e2)
{
    int h1;
    int h2;
    int result;
    int cOne,cTwo,cOneB,cTwoB;
    //hashing
    h1 = hashOne(key,getSizeOne());
    h2 = hashTwo(key,getSizeTwo());
    //Other declarations
    CountPointer *c1,*c2;
    Entry* e;

    while(true)
    {
        //Thread local entry*
        c1 = atomic_load_explicit(&tables[TABLEONE][h1],memory_order_relaxed);
        *e1=c1;
        cOne=getCount(c1);
        e= getAddress(c1);
        if(e!=nullptr)
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
        c2 = atomic_load_explicit(&tables[TABLETWO][h2],memory_order_relaxed);
        *e2=c2;

        cTwo=getCount(c2);
        e= getAddress(c2);
        if(e!=nullptr)
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
        c1= atomic_load_explicit(&tables[TABLEONE][h1],memory_order_relaxed);
        *e1=c1;
        cOneB=getCount(c1);

        e= getAddress(c1);

        if(e!=nullptr)
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


        c2 = atomic_load_explicit(&tables[TABLETWO][h2],memory_order_relaxed);
        *e2=c2;
        cTwoB=getCount(c2);
        e= getAddress(c2);

        if(e!=nullptr)
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
}
//Remove Key
void CuckooHashTable::remove(int key)
{
    int h1 = hashOne(key,getSizeOne());
    int h2 = hashTwo(key,getSizeTwo());

    CountPointer*c1,*c2;
    c1=nullptr; c2=nullptr;
    int countOne=0,countTwo=0;
    while(true)
    {

        int ret=find(key,&c1,&c2);
        if(ret==NOTFOUND)
            return;
        if(ret==FIRST)
        {
            countOne=getCount(c1);
            if((&tables[TABLEONE][h1])->compare_exchange_strong(c1,makePointer(nullptr,countOne)))
            {
                //delete c1;
				aSizes[TABLEONE]--;
				//Entry*temp =getAddress(c1);
				//hazards->push_back(temp);
                return;
            }

        }
        if(ret==SECOND)
        {
            if(tables[TABLEONE][h1]!=c1)
                continue;
            countTwo=getCount(c2);
            if((&tables[TABLETWO][h2])->compare_exchange_strong(c2,makePointer(nullptr,countTwo)))
            {
                //delete c2;
				aSizes[TABLETWO]--;
				//Entry*temp =getAddress(c2);
				//hazards->push_back(temp);
                return;
            }
        }
    }
}
//Insert
void CuckooHashTable::insert (int key,int value)
{
    //Hashing

    int h1=hashOne(key,getSizeOne());
    int h2=hashTwo(key,getSizeTwo());
    int countOne,countTwo;
    //New Pointer
    int trials=0;

    Entry* newNode=new Entry(key,value);
    int f;
	
	/*while(std::find(hazards->begin(),hazards->end(),newNode)!=hazards->end())
	{
			hazards->push_back(newNode);
			newNode= new Entry(key,value);
	}*/
		
    //    newNode->compare_exchange_strong(nullptr,new Entry(key,value));
    CountPointer *cnt1,*cnt2,*temp;

    startInsert:
        int result=find(key,&cnt1,&cnt2);
        if(result==FIRST)
        {
            countOne=getCount(cnt1);
            if (!(&tables[TABLEONE][h1])->compare_exchange_strong(cnt1,makePointer(newNode,countOne)))
            {
                //cout<<"Problem"<<endl;
                ++trials;
                goto startInsert;
            }
            else
            {

                return;
            }
        }
        if(result==SECOND)
        {
            countTwo=getCount(cnt2);
            if(!(&tables[TABLETWO][h2])->compare_exchange_strong(cnt2,makePointer(newNode,countTwo)))
            {
                 //delete newNode,cnt2,cnt1;
                               //  cout<<"Problem"<<endl;
                   ++trials;

                 goto startInsert;
            }

            else
            {
                 //delete newNode,cnt2,cnt1;
				  //free(newNode);
                return;
            }
        }

        if(getAddress(cnt1)==nullptr)
        {
            countOne=getCount(cnt1);
            if(!(&tables[TABLEONE][h1])->compare_exchange_strong(cnt1,makePointer(newNode,countOne)))
            {
                //delete newNode,cnt2,cnt1;
                                //cout<<"Problem"<<endl;
                ++trials;
				 //free(newNode);
                goto startInsert;
            }
            else
            {
                //delete newNode,cnt2,cnt1;
				//free(newNode);
				aSizes[TABLEONE]++;
                return;
            }
        }
        if(getAddress(cnt2)==nullptr)
        {
            countTwo=getCount(cnt2);
            if(!(&tables[TABLETWO][h2])->compare_exchange_strong(cnt2,makePointer(newNode,countTwo)))
            {
                //delete newNode,cnt2,cnt1;
                                //cout<<"Problem"<<endl;
                ++trials;
                goto startInsert;
            }
            else
            {
                //delete newNode,cnt2,cnt1;
				// free(newNode);
				aSizes[TABLETWO]++;
                return;
            }
        }
        if(++trials>log2(getSizeOne()+getSizeTwo()))
        {
              //cout<<"Too many attempts used"<<endl;

              return;
        }

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

            rehash();
        }

    //delete newNode,cnt2,cnt1;
    return;
}
//Relocate
bool CuckooHashTable::relocate(int which, int index)
{
    //initializations
    int THRESHOLD=getSizeOne()+getSizeTwo();
    THRESHOLD=log2(THRESHOLD);
    int route[THRESHOLD]; //storing cuckoo path
    int startLevel,tbl,idx,preIDX;
    startLevel=0;
    tbl=which;
    idx=index;preIDX=0;

    CountPointer*pre=nullptr,*c1,*c2;
    Entry* e1=nullptr,*preADDR=nullptr,*e2;
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
            c1=atomic_load(&tables[tbl][idx]);
//            c1=tables[tbl][idx];
            e1=getAddress(c1);

            while(marked(e1))
            {
                helpRelocate(tbl,idx,false);
                c1=atomic_load(&tables[tbl][idx]);
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
                if(preADDR!=nullptr&&e1!=nullptr)
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

            if(e1!=nullptr)
            {
                route[depth]=idx;
                key=e1->key;
                pre=c1;
                preIDX=idx;
                tbl=1-tbl;
                idx= (tbl==0)? hashOne(key,getSizeOne()):hashTwo(key,getSizeTwo());
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
            c1=atomic_load(&tables[tbl][idx]);
            e1=getAddress(c1);
            if(marked(e1))
            {
                helpRelocate(tbl,idx,false);
                c1=atomic_load(&tables[tbl][idx]);
            }
            if(e1==nullptr)
            {
                continue;
            }
            int destIDX=(tbl==0)?hashTwo(key,getSizeTwo()):hashOne(key,getSizeOne());
            c2=atomic_load(&tables[1-tbl][destIDX]);
            e2=getAddress(c2);
            if(e2!=nullptr)
            {
                startLevel=i+1;
                idx=destIDX;
                tbl=1-tbl;
                trials++;

                goto pathDiscovery;
            }
            helpRelocate(tbl,idx,true);
        }
        }

        //++trials;
        return found;
    }

    //++trials;
    return found;
}

void CuckooHashTable::helpRelocate(int which,int index,bool initiator)
{

    CountPointer* src,*dest;
    Entry* temp,*dst;
    int countOne,countTwo,nCount;
    while(true)
    {
        src=atomic_load(&tables[which][index]);
        countOne=getCount(src);
        temp=getAddress(src);

        if(temp==nullptr)
        {
            return;
        }
        //Marking pointer to be relocated
        while(!marked(temp)&&initiator)
        {
            CountPointer*c1=makePointer(temp,countOne);

            temp=marking(temp);

            (&tables[which][index])->compare_exchange_strong(c1,makePointer(temp,countOne));
            src=atomic_load(&tables[which][index]);
        }
        if(!marked(temp))
        {
            return;
        }
        int hd= (1-which==0)?hashOne(temp->key,getSizeOne()):hashTwo(temp->key,getSizeTwo());
        dest=atomic_load(&tables[1-which][hd]);
        dst=getAddress(dest);
        countTwo=getCount(dest);
        nCount=countOne>countTwo?countOne+1:countTwo+1;
        if(src!=atomic_load(&tables[which][index]))
        {
            continue;
        }
        if((&tables[1-which][hd])->compare_exchange_strong(dest,makePointer(deMarking(temp),nCount)))
        {
            (&tables[which][index])->compare_exchange_strong(src,makePointer(nullptr,countOne+1));
			aSizes[which]--;
            return;
        }
        if(dst==temp)
        {
            (&tables[which][index])->compare_exchange_strong(src,makePointer(nullptr,countOne+1));
			aSizes[which]--;
            return;
        }
        (&tables[which][index])->compare_exchange_strong(src,makePointer(deMarking(getAddress(src)),countOne+1));
    }
}
//Delete Duplicate in Secondary Table
void CuckooHashTable::delDup(int iOne,CountPointer *e1,int iTwo,CountPointer *e2)
{
    CountPointer*temp;
    if(e1==nullptr||e2==nullptr)
        return;
    if(e1!=tables[TABLEONE][iOne]&&e2!=tables[TABLETWO][iTwo])
        return;

    if(getAddress(e1)->key!=getAddress(e2)->key)
        return;

     temp=makePointer(nullptr,getCount(e2));
     (&tables[TABLETWO][iOne])->compare_exchange_strong(e2,temp);
	 aSizes[TABLETWO]--;
     //delete temp;

}
void testInsertOne (int key,int value,CuckooHashTable*t)
{
    //Hashing


	 int hd= hashOne(key,t->getSizeOne());
    int countOne,countTwo;
    //New Pointer
    int trials=0;
    //int trials=0;

    Entry* newNode=new Entry(key,value);
    if(newNode==nullptr)
    {
        t->fails++;
        return;
    }
    //    newNode->compare_exchange_strong(nullptr,new Entry(key,value));
    CountPointer *cnt1,*cnt2,*temp;
	int result=t->find(key,&cnt1,&cnt2);
            countOne=getCount(cnt1);
            if(!(&t->tables[TABLEONE][hd])->compare_exchange_strong(cnt1,makePointer(newNode,countOne)))
            {
                //delete newNode,cnt2,cnt1;
                                //cout<<"Problem"<<endl;
               // t->trials++;
				 //free(newNode);
                return;
            }
            else
            {
                //delete newNode,cnt2,cnt1;
				//free(newNode);
				t->aSizes[TABLEONE]++;
                return;
            }


}
void testInsertTwo (int key,int value,CuckooHashTable*t)
{
    //Hashing


	 int hd= hashTwo(key,t->getSizeTwo());
    int countOne,countTwo;
    //New Pointer
    int trials=0;

    Entry* newNode=new Entry(key,value);
    if(newNode==nullptr)
    {
        t->fails++;
        return;
    }
    //    newNode->compare_exchange_strong(nullptr,new Entry(key,value));
    CountPointer *cnt1,*cnt2,*temp;
	//	int result=find(key,&cnt1,&cnt2);
	int result=t->find(key,&cnt1,&cnt2);
            countOne=getCount(cnt1);
            if(!(&t->tables[TABLETWO][hd])->compare_exchange_strong(cnt2,makePointer(newNode,countOne)))
            {
                //delete newNode,cnt2,cnt1;
                                //cout<<"Problem"<<endl;
               // t->trials++;
				 //free(newNode);
                return;
            }
            else
            {
                //delete newNode,cnt2,cnt1;
				//free(newNode);
				t->aSizes[TABLETWO]++;
                return;
            }


}
void filling(CuckooHashTable *z,std::mt19937 gen)
{
	int numElements=z->aSizes[TABLEONE]+z->aSizes[TABLETWO];
	double load=numElements/(double)(z->getSizeOne()+z->getSizeTwo()*2);
	load=0.0;
	double threshold=0.4;
	while(load<threshold)
	{
		int k=(int)gen();

		int v=(int)gen();
		int b =(int)gen();
		testInsertOne(abs(k),abs(v),z);

		numElements=z->aSizes[TABLEONE];
		z->loads[TABLEONE]=(double)z->aSizes[TABLEONE]/(double)TESTSIZE;
		load=z->loads[TABLEONE];

	}

	load=0.0;
	while (load<threshold)
	{
		int a=(int)gen();
		int b =(int)gen();
		z->aSizes[TABLETWO];
		testInsertTwo(abs(a),abs(b),z);
		z->loads[TABLETWO]=(double)z->aSizes[TABLETWO]/(double)z->getSizeTwo();
		load=z->loads[TABLETWO];
	//std::cout<<load<<std::endl;
	}

//	std::cout<<z->loads[TABLEONE]<<std::endl;
//	std::cout<<z->loads[TABLETWO]<<std::endl;

}

void test(CuckooHashTable *z,std::mt19937 gen)
{
		//std::cout<<"i"<<endl;
    int i=0;
     std::random_device rd{};

        // Use Mersenne twister engine to generate pseudo-random numbers.
        std::mt19937 engine{rd()};

        // "Filter" MT engine's output to generate pseudo-random double values,
        // **uniformly distributed** on the closed interval [0, 1].
        // (Note that the range is [inclusive, inclusive].)
        std::uniform_real_distribution<float> dist{0.0, 100.0};
	
	//cout<<std::atomic_load(operations)<<endl;
	//i=std::atomic_load(operations);
    while(std::atomic_load(operations)<NUMOPS)
    {
        int n=0;


        int a=dist(engine);
        if(a<90)
        {
            int b=(int)gen();
            b=abs(b);
            z->search(b);
        }


        else if(a<95)
        {
             int k=(int)gen();
             int v=(int)gen();
             k=abs(k);
			 v=abs(v);
             z->insert(k,v);
           
        }
        else
        {
            int r=(int)gen();
            z->remove(abs(r));
        }

         i =atomic_load(operations);
        (operations)->compare_exchange_strong(i,atomic_load(operations)+1);

    }
}
int main (int argc,char**argv)
{

		
		//return 0;
	
   
 //   return 9;
    numThreads=32;
    int i=0;
	float time=0.0;

	    std::thread myThreads[numThreads];
	  //  thread_local std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
	 thread_local std::mt19937 generator(0);
	    //std::mersenne_twister_engine<int> generator()=new std::mersenne_twister_engine();
		std::cout<<argc<<std::endl;
	if(argc>3)
	{
		TESTSIZE= atoi(argv[1])>10?atoi(argv[1]):100000;
		NUMOPS =  atoi(argv[2])>10?atoi(argv[2]):100000;
		numThreads=atoi(argv[3])>0?atoi(argv[3]):8;
				std::cout<<TESTSIZE<<std::endl;
				std::cout<<NUMOPS<<std::endl;

	}		
	for(int i=0; i<1;i++)
	{
		
	CuckooHashTable* z= new CuckooHashTable(TESTSIZE,TESTSIZE);
		    //CuckooHashTable *z = new CuckooHashTable(5,5);
		  operations=new std::atomic<int>;
		  operations->store(0);
		 // std::atomic_load(operations);
		//cout<<std::atomic_load(operations)<<endl;
		filling(z,generator);


	    //cout<<z->loadFactorA<<endl;
	    //cout<<z->loadFactorB<<endl;
	    //cout<<"Min "<<generator.min()<<endl;;

	    auto  startPoint= std::chrono::_V2::high_resolution_clock::now();

	    for (int i=0; i<numThreads; i++)
	    {
			myThreads[i] = std::thread(test,z,generator);
	    }

	    for (int i=0; i<numThreads; i++)
	    {
			myThreads[i].join();
	    }


	    auto endPoint= std::chrono::_V2::high_resolution_clock::now();
	//    cout<<endPoint<<endl;
	    auto period=endPoint-startPoint;
	    float duration=std::chrono::duration_cast<std::chrono::milliseconds>(endPoint-startPoint).count();
		time+=duration;
	    cout<<"Time "<<std::chrono::duration<double, std::milli>(period).count()<<endl ;
		cout<<"Throughput "<<NUMOPS/duration<<endl;
	    //cout<<"Fails "<<z->fails<<endl;
	    //cout<<"Relocates "<<z->relocates<<endl;
	    //cout<<atomic_load(z->operations)<<endl;
		//cout<<z->hazards->size()<<endl;
		//double s= (double)z->aSizes[0]/(double)TESTSIZE/200;
		//double b= (double)z->aSizes[1]/(double)TESTSIZE/200;
	//	cout<<s<<endl;
	//	cout<<b<<endl;
	//    z->~CuckooHashTable();
		 //z->~CuckooHashTable();
	//	z= new CuckooHashTable(TESTSIZE,TESTSIZE);
	}
	   
//	pthread_create();
	//cout<<"Average"<<(double)NUMOPS/(double)(time/3)<<endl;
	//delete z;
    return 0;

}
