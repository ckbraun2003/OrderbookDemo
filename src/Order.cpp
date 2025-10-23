#include "Order.hpp"

void Order::Fill(Quantity quantity)
{
	if (quantity > GetRemainingQuantity())
		throw std::logic_error(std::format("Order ({}) cannot be filled for more than remaining quantity.", GetOrderId()));

	remainingQuantity_ -= quantity;
};

void Order::ToGoodTillCancel(Price price)
{
	if (GetOrderType() != OrderType::Market)
		throw std::logic_error(std::format("Order ({}) cannot have price adjusted.", GetOrderId()));

	if (!std::isfinite(price))
		throw std::logic_error(std::format("Order ({}) price must be tradable.", GetOrderId()));

	price_ = price;
	orderType_ = OrderType::GoodTillCancel;
}