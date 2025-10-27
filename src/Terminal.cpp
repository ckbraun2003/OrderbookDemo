#include <iostream>
#include <format>
#include <ranges>

#include "Terminal.hpp"

void Terminal::PrintTitle()
{
  std::cout << "Orderbook Demo\nBy Cameron Braun\n\n";
};

void Terminal::PrintTrades(const Trades& trades)
{
  std::cout << "\nTrades\n";
  std::cout << std::string(80, '=') << "\n";
  for (const auto& tradeInfo : trades)
  {
    TradeInfo bidInfo = tradeInfo.GetBidTrade();
    TradeInfo askInfo = tradeInfo.GetAskTrade();

    std::cout << std::format("Order[{}] -- [{}/{} filled @ ${}] -- [{}ns :matchEngineTime]\n", 
      bidInfo.orderId_, bidInfo.quantity_, bidInfo.initialQuantity_, bidInfo.price_, bidInfo.matchEngineTime_);
    std::cout << std::format("Order[{}] -- [{}/{} filled @ ${}] -- [{}ns :matchEngineTime]\n", 
      askInfo.orderId_, askInfo.quantity_, askInfo.initialQuantity_, askInfo.price_,  askInfo.matchEngineTime_);
  }
  std::cout << std::string(80, '=') << "\n";
}

int Terminal::MenuSelection()
{
  int choice = 0;
  while (true)
  {
    std::cout << "\n=== MAIN MENU ===\n";
    std::cout << "1. Add Order\n";
    std::cout << "2. Cancel Order\n";
    std::cout << "3. Modify Order\n";
    std::cout << "4. Populate Orderbook\n";
    std::cout << "5. Print Trades (Toggle)\n";
    std::cout << "6. Print Orders (Toggle)\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice (1-7): ";

    std::cin >> choice;

    // Handle invalid input (non-numeric)
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(1000, '\n');
      std::cout << "Invalid input. Please enter a number.\n";
      continue;
    }
    break;
  }
  return choice;
}

void Terminal::PrintOrderbook(const Orderbook& orderbook)
{

  OrderbookLevelInfos levelInfos = orderbook.GetLevelInfos();

  LevelInfos bids = levelInfos.GetBids();
  LevelInfos asks = levelInfos.GetAsks();

  Price bidAskSpread = 0;;

  if (!bids.empty() && !asks.empty())
  {
    auto greatestBid = bids.front();
    auto lowestAsk = asks.front();
    bidAskSpread = lowestAsk.price_ - greatestBid.price_;
  }

  std::cout << "\Orderbook\n";
  std::cout << std::string(40, '=') << "\n";

  std::cout << std::format("\n{:>10} |$ {:>8} | {:>10}\n",
    "Amount", "Price", "Quantity");
  std::cout << std::string(40, '_') << "\n";

  std::cout << "\nAsks\n";

  for (const auto& askInfo : std::views::reverse(asks))
  {
    int length = (askInfo.quantity_ * 10) / 100;
    std::string bar(length, '#');

    std::cout << std::format("{:>10} |$ {:>8.2f} | {}{}{}\n",
      askInfo.quantity_, askInfo.price_, red, bar, reset);
  }

  std::cout << "\n";
  std::cout << std::format("[Spread: ${:.2f}]\n", bidAskSpread);
  std::cout << "\n";

  for (const auto& bidInfo : bids)
  {
    int length = (bidInfo.quantity_ * 10) / 100;
    std::string bar(length, '#');

    std::cout << std::format("{:>10} |$ {:>8.2f} | {}{}{}\n",
      bidInfo.quantity_, bidInfo.price_, green, bar, reset);
  }

  std::cout << "Bids\n";
  std::cout << std::string(40, '=') << "\n";

};

void Terminal::PrintPersonalOrders(const PersonalOrderEntries& personalOrders)
{
  std::cout << "\nOrders\n";
  std::cout << std::string(85, '=') << "\n";

  std::cout << std::format("\n{:>8} | {:>6} |$ {:>6} | {:>8} | {:>20} | {:>20}\n",
    "Order ID", "Side", "Price", "IsFilled", "Initial Quantity", "Remaining Quantity");
  std::cout << std::string(85, '-') << "\n";

  for (const auto& [orderId, order] : personalOrders)
  {

    Side side = order->GetSide();
    Price price = order->GetPrice();
    Quantity initialQuantity = order->GetInitialQuantity();
    Quantity remainingQuantity = order->GetRemainingQuantity();
    bool isFilled = order->IsFilled();

    std::string sideColor = green;
    std::string sideString = "BUY";

    if (side == Side::Sell)
    {
      sideColor = red;
      sideString = "SELL";
    }

    std::cout << std::format("{:>8} | {}{:>6}{} |$ {:>6} | {:>8} | {:>20} | {:>20}\n",
      orderId, sideColor, sideString, reset, price, isFilled, initialQuantity, remainingQuantity);
  }

  std::cout << std::string(85, '=') << "\n";
}

void Terminal::PopulateOrderbook(Orderbook& orderbook, OrderId& orderId)
{
  Price startingPrice = 100;

  for (int i = 0; i < 10; ++i)
  {
    orderbook.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel,
      orderId, Side::Buy, (startingPrice - (i * 1)), GetRandomInt(10, 10 + (i * 5))));
    ++orderId;

    orderbook.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel,
      orderId, Side::Sell, (startingPrice + (i * 1)), GetRandomInt(10, 10 + (i * 5))));
    ++orderId;
  };
};


Trades Terminal::PlaceOrder(Orderbook& orderbook, OrderId& orderId, PersonalOrderEntries& personalOrders)
{
  std::cout << "\n=== Place Order ===\n";

  OrderType orderType = GetOrderType();
  Trades trades_;

  OrderPointer order;
  if (orderType == OrderType::Market)
  {
    order = std::make_shared<Order>(orderId, GetPosition(), GetQuantity());
  }
  else
  {
    order = std::make_shared<Order>(orderType, orderId, GetPosition(), GetPrice(), GetQuantity());
  }

  trades_ = orderbook.AddOrder(order);
  personalOrders.insert({ orderId, order });
  
  ++orderId;

  ClearScreen();

  return trades_;
  
};

Trades Terminal::ModifyOrder(Orderbook& orderbook, PersonalOrderEntries& personalOrders)
{
  PrintPersonalOrders(personalOrders);
  std::cout << "\n=== Modify Order ===\n";

  OrderId orderId = GetOrderId();
  if (!personalOrders.contains(orderId))
    ClearScreen();
    return { };

  OrderModify order = OrderModify(orderId, GetPosition(), GetPrice(), GetQuantity());

  Trades trades = orderbook.ModifyOrder(order);

  const auto& existingOrder = personalOrders.at(orderId);
  personalOrders[orderId] = order.ToOrderPointer(existingOrder->GetOrderType());

  ClearScreen();

  return trades;
}

void Terminal::CancelOrder(Orderbook& orderbook, PersonalOrderEntries& personalOrders)
{
  PrintPersonalOrders(personalOrders);
  std::cout << "\n=== Modify Order ===\n";

  OrderId orderId = GetOrderId();

  if (!personalOrders.contains(orderId))
    ClearScreen();
    return;

  orderbook.CancelOrder(orderId);
  personalOrders.erase(orderId);

  ClearScreen();
}

OrderType Terminal::GetOrderType()
{
  ClearScreen();
  PrintTitle();
  while (true)
  {

    int choice = 0;

    std::cout << "\n=== ORDERTYPE ===\n";
    std::cout << "1. Market\n";
    std::cout << "2. GoodTillCancel\n";
    std::cout << "Enter your choice (1-2): ";

    std::cin >> choice;

    // Handle invalid input (non-numeric)
    if (std::cin.fail()) 
    {
      std::cin.clear();
      std::cin.ignore(1000, '\n');
      std::cout << "Invalid input. Please enter a number.\n";
      continue;
    }

    switch (choice)
    {
    case 1:
      return OrderType::Market;
    case 2:
      return OrderType::GoodTillCancel;
    default:
      continue;
    }
  }
};

Side Terminal::GetPosition()
{
  while (true)
  {

    int choice = 0;

    std::cout << "\n=== SIDE ===\n";
    std::cout << "1. Buy\n";
    std::cout << "2. Sell\n";
    std::cout << "Enter your choice (1-2): ";

    std::cin >> choice;

    // Handle invalid input (non-numeric)
    if (std::cin.fail()) 
    {
      std::cin.clear();
      std::cin.ignore(1000, '\n');
      std::cout << "Invalid input. Please enter a number.\n";
      continue;
    }

    switch (choice)
    {
    case 1:
      return Side::Buy;
    case 2:
      return Side::Sell;
    default:
      continue;
    }
  }
};

Price Terminal::GetPrice()
{
  while (true)
  {

    Price choice = 0;

    std::cout << "\n=== PRICE ===\n";
    std::cout << "Enter your price: $";

    std::cin >> choice;

    // Handle invalid input (non-numeric)
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(1000, '\n');
      std::cout << "Invalid input. Please enter a number.\n";
      continue;
    }
    else
    {
      return choice;
    }
  }
};

Quantity Terminal::GetQuantity()
{
  while (true)
  {

    Quantity choice = 0;

    std::cout << "\n=== Quantity ===\n";
    std::cout << "Enter your quantity: ";

    std::cin >> choice;

    // Handle invalid input (non-numeric)
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(1000, '\n');
      std::cout << "Invalid input. Please enter a number.\n";
      continue;
    }
    else
    {
      return choice;
    }
  }
};

OrderId Terminal::GetOrderId()
{
  while (true)
  {

    OrderId choice = 0;

    std::cout << "\n=== Order ID ===\n";
    std::cout << "Enter the Order ID: ";

    std::cin >> choice;

    // Handle invalid input (non-numeric)
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(1000, '\n');
      std::cout << "Invalid input. Please enter a number.\n";
      continue;
    }
    else
    {
      return choice;
    }
  }
}


void Terminal::ClearScreen()
{
  std::system("cls"); // Windows
};

int Terminal::GetRandomInt(int start, int end)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(start, end);
  return dist(gen);
}