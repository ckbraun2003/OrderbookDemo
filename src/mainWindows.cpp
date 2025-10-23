#include <iostream>

#include "Orderbook.hpp"
#include "Terminal.hpp"

int main()
{
  OrderId orderId = 1;

	Orderbook orderbook;
  Terminal terminal;

  Trades trades;
    
  while (true)
  {
    terminal.PrintTitle();
    terminal.PrintOrderbook(orderbook);
    terminal.PrintTrades(trades);

    int choice = terminal.MenuSelection();
    switch (choice)
    {
    case 1:
      terminal.ClearScreen();
      trades = terminal.PlaceOrder(orderbook, orderId);
      continue;
    case 2:
      terminal.PopulateOrderbook(orderbook, orderId);
      terminal.ClearScreen();
      continue;

    case 3:
      return 0;

    default:
      terminal.ClearScreen();
      std::cout << "Selection not found\n";
      continue;
    }
  };
  return 0;
}
