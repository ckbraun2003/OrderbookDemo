#include "Orderbook.hpp"

Trades Orderbook::AddOrder(OrderPointer order)
{
	if (orders_.contains(order->GetOrderId()))
		return { };

	if (order->GetOrderType() == OrderType::Market)
	{
		if (order->GetSide() == Side::Buy && !asks_.empty())
		{
			const auto& [worstAsk, _] = *asks_.rbegin();
			order->ToGoodTillCancel(worstAsk);
		}
		else if (order->GetSide() == Side::Sell && !bids_.empty())
		{
			const auto& [worstBid, _] = *bids_.rbegin();
			order->ToGoodTillCancel(worstBid);
		}
		else
		{
			return { };
		}
	}

	OrderPointers::iterator iterator;

	if (order->GetSide() == Side::Buy)
	{
		auto& orders = bids_[order->GetPrice()];
		orders.push_back(order);
		iterator = std::next(orders.begin(), orders.size() - 1);
	}
	else
	{
		auto& orders = asks_[order->GetPrice()];
		orders.push_back(order);
		iterator = std::next(orders.begin(), orders.size() - 1);
	}

	orders_.insert({ order->GetOrderId(), OrderEntry{ order, iterator } });
	return MatchOrders();
};

void Orderbook::CancelOrder(OrderId orderId)
{
	if (!orders_.contains(orderId))
		return;

	const auto& [order, iterator] = orders_.at(orderId);
	orders_.erase(orderId);

	if (order->GetSide() == Side::Sell)
	{
		auto price = order->GetPrice();
		auto& orders = asks_.at(price);
		orders.erase(iterator);
		if (orders.empty())
			asks_.erase(price);
	}
	else
	{
		auto price = order->GetPrice();
		auto& orders = bids_.at(price);
		orders.erase(iterator);
		if (orders.empty())
			bids_.erase(price);
	}
};

Trades Orderbook::ModifyOrder(OrderModify order)
{
	if (!orders_.contains(order.GetOrderId()))
		return { };

	const auto& [existingOrder, _] = orders_.at(order.GetOrderId());
	CancelOrder(order.GetOrderId());
	return AddOrder(order.ToOrderPointer(existingOrder->GetOrderType()));
};

bool Orderbook::CanMatch(Side side, Price price) const
{
	if (side == Side::Buy)
	{
		if (asks_.empty())
			return false;

		const auto& [bestAsk, _] = *asks_.begin();
		return price >= bestAsk;
	}
	else
	{
		if (bids_.empty())
			return false;

		const auto& [bestBid, _] = *bids_.begin();
		return price <= bestBid;
	}
};

Trades Orderbook::MatchOrders()
{

	Trades trades;
	trades.reserve(orders_.size());

	while (true)
	{
		if (bids_.empty() || asks_.empty())
			break;

		auto& [bidPrice, bids] = *bids_.begin();
		auto& [askPrice, asks] = *asks_.begin();

		if (bidPrice < askPrice)
			break;

		while (!bids.empty() && !asks.empty())
		{
			auto start = std::chrono::high_resolution_clock::now();
			auto bid = bids.front();
			auto ask = asks.front();

			Price tradePrice;

			if (bid->GetOrderId() > ask->GetOrderId())
			{
				tradePrice = ask->GetPrice();
			}
			else
			{
				tradePrice = bid->GetPrice();
			}

			Quantity tradeQuantity = std::min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity());

			bid->Fill(tradeQuantity);
			ask->Fill(tradeQuantity);

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

			trades.push_back(Trade{
			TradeInfo{ bid->GetOrderId(), tradePrice, tradeQuantity, bid->GetInitialQuantity(), duration.count()},
			TradeInfo{ ask->GetOrderId(), tradePrice, tradeQuantity, ask->GetInitialQuantity(), duration.count()}
				});

			std::cout << trades.empty() << "\n";

			if (bid->IsFilled())
			{
				bids.pop_front();
				orders_.erase(bid->GetOrderId());
			}
			if (ask->IsFilled())
			{
				asks.pop_front();
				orders_.erase(ask->GetOrderId());
			}
		}
		if (bids.empty())
			bids_.erase(bidPrice);

		if (asks.empty())
			asks_.erase(askPrice);
	}

	return trades;
};

OrderbookLevelInfos Orderbook::GetLevelInfos() const
{
	LevelInfos bidInfos, askInfos;
	bidInfos.reserve(orders_.size());
	askInfos.reserve(orders_.size());

	auto CreateLevelInfos = [](Price price, const OrderPointers& orders)
		{
			return LevelInfo{ price, std::accumulate(orders.begin(), orders.end(), (Quantity)0,
				[](Quantity runningSum, const OrderPointer& order)
				{ return runningSum + order->GetRemainingQuantity(); }) };
		};

	for (const auto& [price, orders] : bids_)
		bidInfos.push_back(CreateLevelInfos(price, orders));

	for (const auto& [price, orders] : asks_)
		askInfos.push_back(CreateLevelInfos(price, orders));

	return OrderbookLevelInfos{ bidInfos, askInfos };
};