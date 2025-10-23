#pragma once

#include <random>

#include "Orderbook.hpp"

class Terminal
{
public:
  void PrintTitle();
  int MenuSelection();
  Trades PlaceOrder(Orderbook& orderbook, OrderId& orderId);
  void PrintOrderbook(const Orderbook& orderbook);
  void PopulateOrderbook(Orderbook& orderbook, OrderId&);
  void ClearScreen();
  void PrintTrades(const Trades& trades);
private:
  OrderType GetOrderType();
  Side GetPosition();
  Price GetPrice();
  Quantity GetQuantity();
  int GetRandomInt(int start, int end);
};