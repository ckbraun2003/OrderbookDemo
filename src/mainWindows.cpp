#include <iostream>
#include <unordered_map>

#include "Orderbook.hpp"
#include "Terminal.hpp"

int main()
{
  OrderId orderId = 1;

	Orderbook orderbook;
  Terminal terminal;
  Trades trades;
  PersonalOrderEntries personalOrders;

  bool printOrders = false;
  bool printTrades = false;

  terminal.PopulateOrderbook(orderbook, orderId);

  terminal.ClearScreen();

    
  while (true)
  {
    terminal.PrintTitle();
    terminal.PrintOrderbook(orderbook);

    if (printTrades)
      terminal.PrintTrades(trades);

    if (printOrders)
      terminal.PrintPersonalOrders(personalOrders);


    int choice = terminal.MenuSelection();
    switch (choice)
    {
    case 1:
      terminal.ClearScreen();
      trades = terminal.PlaceOrder(orderbook, orderId, personalOrders);
      continue;
    case 2:
      terminal.ClearScreen();
      terminal.CancelOrder(orderbook, personalOrders);
      continue;

    case 3:
      terminal.ClearScreen();
      trades = terminal.ModifyOrder(orderbook, personalOrders);
      continue;

    case 4:
      terminal.PopulateOrderbook(orderbook, orderId);
      terminal.ClearScreen();
      continue;

    case 5:
      printTrades = !printTrades;
      terminal.ClearScreen();
      continue;

    case 6:
      printOrders = !printOrders;
      terminal.ClearScreen();
      continue;

    case 7:
      return 0;

    default:
      terminal.ClearScreen();
      std::cout << "Selection not found\n";
      continue;
    }
  };
  return 0;
}
