#include "Card.h"

class Node
{
private:
public:
  Node(Node *prev, Node *next, Card *card)
  {
    this->prevNode = prev;
    this->nextNode = next;
    this->cardPnt = card;
  }
  Node* prev()
  {
    return this->prevNode;
  }
  Node* next()
  {
    return this->nextNode;
  }
  Card* value()
  {
    return this->cardPnt;
  }
  
  Node *prevNode;
  Node *nextNode;
  Card *cardPnt;
};


// deck implemented as doubly linked list for easier swapping etc
class Deck
{
 private:
  Node* firstCard;
  Node* lastCard;
  int size;
  
 public:
  Deck()
    {

    }
  Card* first()
  {
      if (size!= 0)
	{
	  return (*firstCard).value();
	}
  }
  Card* last()
  {
    if (size!= 0)
      {
	return (*lastCard).value();
      }
  }
  Node* firstNode()
  {
    return firstCard;
  }
  Node* lastNode()
  {
    return lastCard;
  }
  Card* value(Node* n)
  {
    if (n != NULL)
      {
	return (*n).value();
      }
  }
  void print()
  {
    Node* n = firstCard;
    while (n != NULL)
      {
	(*n).value()->print();
	n = n->nextNode;
	//	*((*n).value())->print();
      }
  }
  void prepend(Card* in)
  {
    Node inNode(NULL, firstCard, in);
    firstCard = &inNode;
    
    if (size == 0)
      {
	lastCard = &inNode;
      } else
      {
	inNode.next()->prevNode = &inNode;
      }
    
  }
};

