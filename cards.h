#pragma once

//Namespace containing the data to generate the deck of cards
namespace Cards
{
	enum cardRank
	{
		ACE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		TEN,
		JACK,
		QUEEN,
		KING
	};

	enum cardSuit
	{
		CLUB,
		DIAMOND,
		HEART,
		SPADE
	};

	struct Card
	{
		cardRank rank;
		cardSuit suit;
	};
}