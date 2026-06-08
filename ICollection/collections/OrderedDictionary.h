#ifndef ORDEREDDICTIONARY_H
#define	ORDEREDDICTIONARY_H

#include "OrderedDictionaryEntry.h"
#include "ListNode.h"
#include "../interfaces/IDictionary.h"

using namespace std;






class OrderedDictionary: public IDictionary {
public:
    OrderedDictionary();
    
    





    
    void add(IKey *k, ICollectible *val);
    
    



    bool member(IKey *k)  const;
    
    



    void remove(IKey *k);
    
    




    ICollectible *find(IKey *k) const;
    
    



    bool isEmpty() const;
    
    



    int getSize() const;
    
    



    IIterator *getIterator();
    
    



    IIterator *getInverseIterator();
    
    




    ICollectible *getMax();
    
    




    ICollectible *getMin();
    
    virtual ~OrderedDictionary();

private:
    
    static void makeListOrder(OrderedDictionaryEntry *entry, ListNode *&head,
            ListNode *&last);
    static void makeListReverseOrder(OrderedDictionaryEntry *entry,
            ListNode *&head);
    
    int size;
    OrderedDictionaryEntry *root;
};

#endif	

