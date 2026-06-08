#ifndef LIST_H
#define	LIST_H

#include "ListNode.h"




class List: public ICollection {
private:
    ListNode *head;
    int size;
public:
    List();
    
    



    void add(ICollectible *);
    
    
    



    void remove(ICollectible *);
    
    
    


 
    bool member(ICollectible *) const;
    
    


    bool isEmpty() const;
    
    


    int getSize() const;
    
    



    IIterator *getIterator();

    


    virtual ~List();
};

#endif	

