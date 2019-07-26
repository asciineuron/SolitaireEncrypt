// will contain functions to generate a deck, split and so forth
#include "Card.h"
#include <iostream>
#include <vector>

std::vector<Card>& genDeck()
{
  std::vector<Card> deck;
  for (int i = 0; i < 4; i++)
    {
      for (int j = 1; j <= 13; i++)
	{
	  deck.push_back(Card(i, j));
	}
    }
  // added all 52 standard cards, now jokers:
  deck.push_back(Card(4, 0));
  deck.push_back(Card(5, 0));
  return &deck;
}


