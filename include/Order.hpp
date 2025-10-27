#pragma once

#include <list>
#include <memory>
#include <format>

#include "Types.hpp"
#include "Enums.hpp"

class Order
{
public:
	Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
		: orderType_{ orderType }
		, orderId_{ orderId }
		, side_{ side }
		, price_{ price }
		, initialQuantity_{ quantity }
		, remainingQuantity_{ quantity }
	{ }
	
	Order(OrderId orderId, Side side, Quantity quantity)
		: Order(OrderType::Market, orderId, side, std::numeric_limits<Price>::quiet_NaN(), quantity)
	{ }

	OrderType GetOrderType() const { return orderType_; }
	OrderId GetOrderId() const { return orderId_; }
	Side GetSide() const { return side_; }
	Price GetPrice() const { return price_; }
	Quantity GetInitialQuantity() const { return initialQuantity_; }
	Quantity GetRemainingQuantity() const { return remainingQuantity_; }
	Quantity GetFilledQuantity() const { return GetInitialQuantity() - GetRemainingQuantity(); }
	bool IsFilled() const { return GetRemainingQuantity() == 0; }
	void Fill(Quantity quantity);
	void ToGoodTillCancel(Price price);

private:
	OrderType orderType_;
	OrderId orderId_;
	Side side_;
	Price price_;
	Quantity initialQuantity_;
	Quantity remainingQuantity_;
};

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;
using PersonalOrderEntries = std::unordered_map<OrderId, OrderPointer>;

class OrderModify
{
public:
	OrderModify(OrderId orderId, Side side, Price price, Quantity quantity)
		: orderId_{ orderId }
		, side_{ side }
		, price_{ price }
		, quantity_{ quantity }
	{
	}

	OrderId GetOrderId() const { return orderId_; }
	Side GetSide() const { return side_; }
	Price GetPrice() const { return price_; }
	Quantity GetQuantity() const { return quantity_; }

	OrderPointer ToOrderPointer(OrderType orderType) {
		return std::make_shared<Order>(orderType, GetOrderId(), GetSide(), GetPrice(), GetQuantity());
	}

private:
	OrderId orderId_;
	Side side_;
	Price price_;
	Quantity quantity_;

};