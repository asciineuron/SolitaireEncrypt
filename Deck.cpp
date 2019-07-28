#include <iostream>
#include <cassert> 
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

enum Suit {Club, Diamond, Heart, Spade, JokerA, JokerB};

class Card
{
private:
public:
  Suit suit;
  int rank;
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
  int getKeystreamNum()
  {
    if (this->suit == Club || this->suit == Heart)
      {
	return this->rank;
      } else if (this->suit == Diamond || this->suit == Spade)
      {
	return this->suit+13;
      }
    return -1; // shouldnt call
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
public:
  
  Node* first;
  Node* last;
  int size;
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

  void revPrint()
  {
    Node* out = last;
    int cardNum = 1;
    while (out != NULL)
      {
	std::cout << "Card#" << cardNum++ << " ";
	out->card.print();
	out = out->prev;
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
    assert(num >= 0);
    assert(num < this->size);
    Node* out = NULL;

    out = first;
    while (num > 0)
      {
	out = out->next;
	num--;
      }
    return out;
    /*
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
	  }*/
    //return out;
  }
  
  void swapNodes(int posA, int posB)
  {
    //assert(posB > posA && posB >= 0 && posB < size && posA >= 0 && posA < size);
    Node* a = this->getNode(posA);
    Node* b = this->getNode(posB);

    /*
    Card tempCard = b->card;
    b->card = a->card;
    a->card = tempCard;
    */
    Suit s = b->card.suit;
    int r = b->card.rank;

    b->card.suit = a->card.suit;
    b->card.rank = a->card.rank;
    a->card.suit = s;
    a->card.rank = r;
      
  }

  // works for single cut too, set a=b !
  void tripleCut(int posA, int posB)
  {
    //swap the cards above a with those below b
    assert(posA <= posB);
    Node* a = this->getNode(posA);
    Node* b = this->getNode(posB);

    if (first == a)
      {
	this->cut(posB);
	return;
      }
    if (last == b)
      {
	first->prev = b;
	a->prev->next = NULL;
	b->next = first;
	first = a;
	return;
      }
    Node* temp = b->next;
    b->next = first; 
    last->next = a;
    first = temp;
    Node* tempLast = last;
    last = a->prev;
    a->prev = tempLast;
    
    last->next = NULL;
  }

  void cut(int pos)
  {
    // take cards below pos and place at beginning of deck
    Node* n = this->getNode(pos);
    if ( n != last){
    last->next = first;
    Node* tempLast = last;
    last = n;
    first->prev =tempLast;
    first = n->next;
    first->prev = NULL;
    n->next = NULL;}
    // else do nothing
  }
  
  void countCut()
  {
    Node* temp = last;
    int count = last->card.getCardNum();
    last = last->prev; //cut out add again at bottom
    last->next = NULL;
    //Node* grab = this->getNode(count);
    if (count != 53) this->cut(count-1);
    // must leave bottom card on bottom:
    last->next = temp;
    last = temp;
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
  
  // now shuffle
  for (int posa = 0; posa < 54; posa++)
    {
      int swapPos = rand() % 53;
      this->swapNodes(posa, swapPos);
    }
  }

  void setDeck(std::string file)
  {
    // will read file describing key, set deck accordingly
  }
  void saveDeck()
  {
    // will save current deck config as key for decrypt
  }
};

std::string encryptMessage(std::string plain, std::vector<int> keystream)
{
  assert(plain.length() == keystream.size());
  char* out = new char[plain.length() + 1];
  out[plain.length()] = '\0';
  for (int pos = 0; pos < plain.length(); pos++)
    {
      char outChar = (char)(((plain.at(pos) - 'a' + 1 + keystream[pos]) % 26) + 'a' - 1); // add modulo 26
      out[pos] = outChar;
    }
  std::string outstr(out);
  delete[] out;
  return outstr;
}

std::string decryptMessage(std::string cipher, std::vector<int> keystream)
{
  assert(cipher.length() == keystream.size());
  char* out = new char[cipher.length() + 1];
  out[cipher.length()] = '\0';
  for (int pos = 0; pos < cipher.length(); pos++)
    {
      char outChar = (char)(((cipher.at(pos) - 'a' + 1 + 26 - keystream[pos]) % 26) + 'a' - 1); // add modulo 26
      out[pos] = outChar;
    }
  std::string outstr(out);
  delete[] out;
  return outstr;
}

std::vector<int> genKeyStream(int length)
{
  std::vector<int> keyStream;
  // generates a keystream of length characters (ignoring char representation going straight to int % 26
  DLL* d = new DLL();
  d->genDeck(); // shuffled
  assert(d->size == 54);
  //write key somewhere!
  d->print();
  while (length > 0)
    {
      length--;
      int jokAPos = 0;
      Node* jo = d->getNode(jokAPos);
      Card c = jo->card;
      while (c.suit != JokerA)
	{ jokAPos++;
	  jo = d->getNode(jokAPos);
	  c = jo->card;
	}
       Node* jokA = d->getNode(jokAPos);
       
       if (d->last != jokA)
	{
	  d->swapNodes(jokAPos, jokAPos+1);
	  // jokAPos++; // pos +1
	} else
	{
	  // bot put after 1st
	  d->last = jokA->prev;
	  d->last->next = NULL;
	  d->first->next->prev = jokA;
	  jokA->next = d->first->next;
	  d->first->next = jokA;
	  jokA->prev = d->first;
	  // jokAPos = 1; // pos 1
	  }
       
      // now move jokB 2 cards
      int jokBPos = 0;
      while (d->getNode(jokBPos)->card.suit != JokerB)
	{ jokBPos++;}
      Node* jokB = d->getNode(jokBPos);
      
      if (jokB == d->last) // put after 2nd card
	{
	  d->last = jokB->prev;
	  d->last->next = NULL;
	  d->first->next->next->prev = jokB;
	  jokB->next = d->first->next->next;
	  d->first->next->next = jokB;
	  jokB->prev = d->first->next;
	  // jokBPos = 2;
	} else if (jokB->next == d->last) // put after 1st card
	{
	  jokB->next->prev = jokB->prev;
	  jokB->prev->next = jokB->next;
	  d->first->next->prev = jokB;
	  jokB->next = d->first->next;
	  d->first->next = jokB;
	  jokB->prev = d->first;
	  // jokBPos = 1;
	} else // put 2 down
	{ // !!!!!
	  d->swapNodes(jokBPos, jokBPos+1);
	  d->swapNodes(jokBPos+1, jokBPos+2);
	  // jokBPos = jokBPos + 2;

	  
	  }
      
jokAPos = 0;
      while (d->getNode(jokAPos)->card.suit != JokerA)
	{ jokAPos++;}
      jokBPos = 0;
      while (d->getNode(jokBPos)->card.suit != JokerB)
	{ jokBPos++;}
      
      std::cout << jokAPos << " " << jokBPos << " ";
      // triple cut
      if (jokAPos < jokBPos)
	{
	  
	  d->tripleCut(jokAPos, jokBPos);
	} else
	{
	  std::cout << "<-";
	  d->tripleCut(jokBPos, jokAPos);
	}
      /*
      // count cut
      // d->countCut();
      // outout card, count top card # down, pick next, repeat if joker
      int downCount = -1;
      downCount = d->first->card.getCardNum();
      std::cout << downCount << " !";
      Node* outPick = d->getNode(downCount);
      if (outPick->card.suit == JokerA || outPick->card.suit == JokerB)
	{
	  length++;
	  continue; // skip if joker
	} else
	{
	  keyStream.push_back(outPick->card.getKeystreamNum());
	  }
      */
	}
  delete d;
  return keyStream;
}

int main()
{
  // seed random
  srand(time(NULL));
  /*DLL d;
  d.genDeck();
  d.print();
  std::cout << "---";
  d.countCut();
  d.print();*/

  DLL* d = new DLL();
  d->genDeck();

  d->print();
  //d->tripleCut(0, 12);
  //d->print();
  //d->revPrint();
  d->swapNodes(0, 2);
  d->print();
  delete d;
  std::vector<int> test = genKeyStream(100);
  //for (int i : test)
  //{
      //std::cout << " " << i;
  //}
  //  DLL d;
  //  d.genDeck();
  //  d.print();
  //d.tripleCut(4, 54);
  //std::cout << "||||||\n";
  //d.revPrint();
  //d.tripleCut(2, 51);
  //d.countCut();
  //d.print();
  return 0;
}
