#include <iostream>
#include <cassert> 

enum Suit {Club, Diamond, Heart, Spade, JokerA, JokerB};

class Card
{
private:
  Suit suit;
  int rank;
public:
  Card(Suit suit, int rank)
    {
      this->suit = suit;
      this->rank = rank;
    }
  Card(const Card& in)
  {
    this->suit = in.suit;
    this->rank = in.rank;
  }
  int getCardNum()
  {
    // get numeric value of card, 1-13
    if (this->suit == JokerA || this->suit == JokerB)
      return 53;
    if (this->suit == Club)
      return this->rank + 0;
    if (this->suit == Diamond)
      return this->rank + 13;
    if (this->suit == Heart)
      return this->rank + 26;
    if (this->suit == Spade)
      return this->rank + 39;
  }
  void print()
  {
    std::cout << "Suit: " << this->suit << " rank: " << this->rank << " ";
  }
};

class Node
{
public:
  Node* prev;
  Node* next;
  Card card;
  Node(Card inCard, Node* inPrev, Node* inNext):
    card(inCard), prev(inPrev), next(inNext)
  {
  }
};

// doubly linked list to represent deck
class DLL
{
private:
  Node* first;
  Node* last;
  int size;
public:
  DLL()
  {
    first = NULL;
    last = NULL;
    size = 0;
  }
  ~DLL()
  {
    Node* del = first;
    if (del != NULL)
      {
	Node* next = first->next;
	while (next != NULL)
	  {
	    // delete from first to last
	    delete del;
	    del = next;
	    next = del->next;
	  }
	delete del;
      }
  }
  
  int getSize()
  {
    return this->size;
  }

  Card value(Node* n)
  {
    assert(n != NULL);
    return n->card;
  }

  void print()
  {
    Node* out = first;
    int cardNum = 1;
    while (out != NULL)
      {
	std::cout << "Card#" << cardNum++ << " ";
	out->card.print();
	out = out->next;
	std::cout << "\n";
      }
  }

  void prepend(Card in)
  {
    Node* newNode = new Node(in, NULL, first);
    first = newNode;
    if (size == 0)
      {
	last = newNode;
      } else
      {
	newNode->next->prev = newNode;
      }
      size++;
    }
  
  void append(Card in)
  {
    Node* newNode = new Node(in, last, NULL);
    last = newNode;
    if (size == 0)
      {
	first = newNode;
      } else
      {
	newNode->prev->next = newNode;
      }
    size++;
  }
  // get node num into list
  Node* getNode(int num)
  {
    assert(num >= 0 && num < size);
    Node* out = NULL;
    if (num <= size/2)
      {
	out = first;
	while (num > 0)
	  {
	    out = out->next;
	    num--;
	  }
      } else
      {
	out = last;
	while (num < size - 1)
	  {
	    out = out->prev;
	    num++;
	  }
      }
    return out;
  }
  
  void swapNodes(int posA, int posB)
  {
    // assume b > a, i.e. later in deck
    //assert(posB > posA && posB >= 0 && posB < size && posA >= 0 && posA < size);
    Node* a = this->getNode(posA);
    Node* b = this->getNode(posB);
    Card tempCard = b->card;
    b->card = a->card;
    a->card = tempCard;
  }

  // works for single cut too, set a=b !
  void tripleCut(int posA, int posB)
  {
    //swap the cards above a with those below b
    assert(posA <= posB);
    Node* a = this->getNode(posA);
    Node* b = this->getNode(posB);

    Node* temp = b->next;
    b->next = first;
    last->next = a;
    first = temp;
    last = a->prev;
    last->next = NULL;
  }

  void countCut()
  {
    Node* temp = last;
    int count = last->card.getCardNum();
    last = last->prev; //cut out add again at bottom
    last->next = NULL;
    Node* grab = this->getNode(count-1);
    this->tripleCut(count-1, count-1);
    // must leave bottom card on bottom:
    last->next = temp;
    last = temp;
    // losing last card!!
  }
  
  void genDeck()
  {
    // generate all 52+2 cards then shuffle
    for (int s = 0; s < 4; s++)
      {
	for (int r = 1; r <= 13; r++)
	  {
	    this->append(Card(static_cast<Suit>(s), r));
	  }
      }
    this->append(Card(static_cast<Suit>(4), 0));
    this->append(Card(static_cast<Suit>(5), 0));
  }
  // now shuffle
};

int main()
{
  DLL d;
  // d.append(Card(Spade, 1));
  // d.append(Card(Club, 2));
  // d.append(Card(Diamond, 5));
  // d.append(Card(Diamond, 3));
  // d.append(Card(Diamond, 8));
  d.genDeck();
  d.print();
  std::cout << "---";
  //d.swapNodes(1, 3);
  //d.tripleCut(1, 1);
  d.countCut();
  d.print();
  return 0;
}
