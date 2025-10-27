#pragma once

#include <random>

#include "Orderbook.hpp"

class Terminal
{
public:
  void PrintTitle();
  int MenuSelection();
  Trades PlaceOrder(Orderbook& orderbook, OrderId& orderId, PersonalOrderEntries& personalOrders);
  Trades ModifyOrder(Orderbook& orderbook, PersonalOrderEntries& personalOrders);
  void CancelOrder(Orderbook& orderbook, PersonalOrderEntries& personalOrders);
  void PrintOrderbook(const Orderbook& orderbook);
  void PrintPersonalOrders(const PersonalOrderEntries& personalOrders);
  void PopulateOrderbook(Orderbook& orderbook, OrderId&);
  void ClearScreen();
  void PrintTrades(const Trades& trades);
private:
  OrderId GetOrderId();
  OrderType GetOrderType();
  Side GetPosition();
  Price GetPrice();
  Quantity GetQuantity();
  int GetRandomInt(int start, int end);

  const std::string red = "\033[1;31m";
  const std::string green = "\033[1;32m";
  const std::string reset = "\033[0m";
};
