#ifndef LISTNODE_H
#define	LISTNODE_H

#include "../interfaces/ICollection.h"
#include <cstddef>

class ListNode {
public:
    




    ListNode(ICollectible *elem, ListNode *next = NULL);
    
    
    ICollectible *getElem() const;
    void setElem(ICollectible *elem);
    
    
    ListNode *getNext() const;
    void setNext(ListNode *next);
    
private:
    ICollectible *elem;
    ListNode *next;
};

#endif	

