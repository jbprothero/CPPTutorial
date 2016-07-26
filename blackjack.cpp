// blackjack.cpp : Simulate a game of blackjack. (No doubling down or splitting)
// jbprothero
// July 26, 2016

#include "stdafx.h"
#include "cards.h"
#include <iostream>
#include <array>
#include <vector>
#include <random>

//Defines the data types of the deck and players' hands
using namespace Cards;
typedef std::array<Card, 52> Deck_t;
typedef std::vector<Card> Hand_t;

//Defines the characteristics of a blackjack player
struct Player
{
	int score = 0;
	int aces = 0;
	Hand_t hand;
};

//Switch the values of two Card objects. Used in shuffleDeck()
void swapCard(Card &cardX, Card &cardY)
{
	Card tempCard = cardX;
	cardX = cardY;
	cardY = tempCard;
}

//Execute a series of random swaps to shuffle the deck of cards. Used in gameLoop()
void shuffleDeck(Deck_t &deck, std::uniform_int_distribution<int> &shuffle, std::mt19937 &mersenne)
{
	for (int card = 0; card < 52; card++)
	{
		int newCard = shuffle(mersenne);
		swapCard(deck[card], deck[newCard]);
	}
}

//Print out a two-character code representing a card. Used in printDeck()
void printCard(const Card &card)
{
	using std::cout;

	switch (card.rank)
	{
	case TWO: cout << '2'; break;
	case THREE: cout << '3'; break;
	case FOUR: cout << '4'; break;
	case FIVE: cout << '5'; break;
	case SIX: cout << '6'; break;
	case SEVEN: cout << '7'; break;
	case EIGHT: cout << '8'; break;
	case NINE: cout << '9'; break;
	case TEN: cout << 'T'; break;
	case JACK: cout << 'J'; break;
	case QUEEN: cout << 'Q'; break;
	case KING: cout << 'K'; break;
	case ACE: cout << 'A'; break;
	}

	switch (card.suit)
	{
	case CLUB: cout << 'C'; break;
	case DIAMOND: cout << 'D'; break;
	case HEART: cout << 'H'; break;
	case SPADE: cout << 'S'; break;
	}
}

//Print a set of cards in order. Used in printScore()
void printDeck(const Hand_t &hand)
{
	for (Card card : hand)
	{
		printCard(card);
		std::cout << ' ';
	}
}

//Print the current score of the player & dealer. Used in playBlackjack()
void printScore(const Player &user, const Player &dealer)
{
	std::cout << "Player: " << user.score << '\t';
	printDeck(user.hand);
	std::cout << "\n";
	std::cout << "Dealer: " << dealer.score << '\t';
	printDeck(dealer.hand);
	std::cout << "\n\n";
}

//Return the value of a card in Blackjack. Used in dealCard()
int getCardValue(const Card &card)
{
	switch (card.rank)
	{
	case ACE:
		return 11;
	case JACK:
	case QUEEN:
	case KING:
		return 10;
	default:
		return card.rank + 1;
	}
}

//Deal a card from the deck into the current Player's hand and adjust the Player's score. Used in playBlackjack()
void dealCard(const Card &card, Player &player)
{
	player.score += getCardValue(card);
	player.hand.push_back(card);
	if (card.rank == ACE)
	{
		player.aces++;
	}
	if (player.score > 21 && player.aces > 0)
	{
		player.score -= 10;
		player.aces--;
	}
}

//Simulate a single game of blackjack. Used in gameLoop()
bool playBlackjack(const Deck_t &deck)
{
	using std::cout;
	using std::cin;

	Player user;
	Player dealer;
	user.hand.reserve(5);
	dealer.hand.reserve(5);

	const Card *cardPtr = &(deck[0]);

	//Deal starting hands to the user and dealer
	cout << "Here's the starting deal:\n";
	dealCard(*cardPtr++, user);
	dealCard(*cardPtr++, dealer);
	dealCard(*cardPtr++, user);
	printScore(user, dealer);

	//Player's loop until they stand or bust.
	while (user.score <= 21)
	{
		char hitChoice;
		cout << "Enter 'h' to hit or 's' to stand: ";
		cin >> hitChoice;

		//If hitting, deal a new card and check for busting
		if (hitChoice == 'h')
		{
			dealCard(*cardPtr++, user);
			printScore(user, dealer);
			if (user.score > 21) 
			{
				cout << "You busted!\n";
				return false;
			}
		}
		//If standing, exit the loop
		else if (hitChoice == 's')
		{
			cout << "Now it's the dealer's turn!\n";
			break;
		}
		//If invalid input, clear cin and try again
		else
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(32767, '\n');
			}
			cin.ignore(32767, '\n');
			cout << "Invalid input. Please try again.\n";
		}
	}

	//Dealer's loop (hit until score is 17 or greater)
	while (dealer.score < 17)
	{
		dealCard(*cardPtr++, dealer);
		printScore(user, dealer);

		if (dealer.score > 21)
		{
			cout << "The dealer busts! You win!\n";
			return true;
		}
	}

	//Score Comparison
	if (user.score > dealer.score)
	{
		cout << "You win!\n";
		return true;
	}
	else
	{
		cout << "The dealer wins! Better luck next time.\n";
		return false;
	}
}

//Execute playBlackjack() in a loop that tracks winrate. Each game uses a newly shuffled deck
void gameLoop(Deck_t &deck, std::uniform_int_distribution<int> &shuffle, std::mt19937 &mersenne)
{
	std::cout << "Welcome to Blackjack!\n";

	int wins = 0;
	int games = 0;

	bool playAgain = true;
	while (playAgain)
	{
		games++;
		std::cout << '\n';

		shuffleDeck(deck, shuffle, mersenne);
		wins += playBlackjack(deck);

		std::cout << "You've won " << wins << " out of " << games << " games\n";
		std::cout << "Would you like to play again? (y or n) ";
		char y;
		std::cin >> y;
		if (y != 'y')
		{
			playAgain = false;
		}
	}

	return;
}

int main()
{
	//Set up random number generators
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<int> shuffle(0, 51);
	
	//Set up the deck
	std::array<Card, 52> deck;
	for (int suit = 0; suit < 4; suit++)
	{
		for (int rank = 0; rank < 13; rank++)
		{
			deck[suit * 13 + rank].rank = static_cast<cardRank>(rank);
			deck[suit * 13 + rank].suit = static_cast<cardSuit>(suit);
		}
	}

	//Execute the game
	gameLoop(deck, shuffle, mersenne);

	return 0;
}
