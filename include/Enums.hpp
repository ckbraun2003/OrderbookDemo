#pragma once

enum class OrderType
{
	Market,
	GoodTillCancel,
	FillAndKill,
	FillOrKill
};

enum class Side
{
	Buy,
	Sell
};