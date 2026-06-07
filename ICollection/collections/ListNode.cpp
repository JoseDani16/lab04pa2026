#include "ListNode.h"
#include <stdexcept>

using namespace std;

ListNode::ListNode(ICollectible* elem, ListNode* next):
        next(next) {
    if(elem == NULL)
        invalid_argument("elem is NULL");
    this->elem = elem;
}

ICollectible *ListNode::getElem() const
{
    return elem;
}

void ListNode::setElem(ICollectible* elem)
{
    if(elem == NULL)
        invalid_argument("elem is NULL");
    this->elem = elem;
}

ListNode *ListNode::getNext() const
{
    return next;
}

void ListNode::setNext(ListNode* next)
{
    this->next = next;
}