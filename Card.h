#ifndef CARD_H
#define CARD_H
#include <iostream>
enum Suit {Diamond, Heart, Spade, Club, JokerA, JokerB};

class Card
{
 public:
  Card(Suit suit, int rank)
    {
      this->suit = suit;
      this->rank = rank;
    }
  void print()
  {
    std::cout << "Suit: " << this->suit << " rank: " << this->rank << " ";
  }
 private:
  Suit suit;
  int rank;
  
};


#endif
