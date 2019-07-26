#include "Card.h"
#include <iostream>
#include "Deck.h"

int main()
{
  Deck d;
  d.prepend(new Card(Spade, 1));
  //d.print();
}
