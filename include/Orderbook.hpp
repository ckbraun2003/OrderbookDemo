#pragma once

#include <numeric>
#include <map>
#include <unordered_map>
#include <chrono>

#include "Order.hpp"
#include "Trade.hpp"
#include "Enums.hpp"

struct LevelInfo
{
	Price price_;
	Quantity quantity_;
};

using LevelInfos = std::vector<LevelInfo>;


class OrderbookLevelInfos
{
public:
	OrderbookLevelInfos(const LevelInfos& bids, const LevelInfos& asks)
		: bids_{ bids }
		, asks_{ asks }
	{ };
	const LevelInfos& GetBids() const { return bids_; };
	const LevelInfos& GetAsks() const { return asks_; };

private:
	LevelInfos bids_;
	LevelInfos asks_;
};


class Orderbook
{
public:
	Trades AddOrder(OrderPointer order);
	void CancelOrder(OrderId orderId);
	Trades ModifyOrder(OrderModify order);
	OrderbookLevelInfos GetLevelInfos() const;
	std::size_t Size() const { return orders_.size(); };

private:
	struct OrderEntry
	{
		OrderPointer order_{ nullptr };
		OrderPointers::iterator location_;
	};
	std::map<Price, OrderPointers, std::greater<Price>> bids_;
	std::map<Price, OrderPointers, std::less<Price>> asks_;
	std::unordered_map<OrderId, OrderEntry> orders_;

	bool CanMatch(Side side, Price price) const;
	Trades MatchOrders();
};
