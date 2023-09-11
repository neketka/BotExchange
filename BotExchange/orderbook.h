#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <limits>

class Orderbook {
public:
	using OrderId = size_t;
	struct Order {
		double AvgPrice;
		double AskingPrice;
		double Quantity;
		double QuantityFilled;
		double QuantityLeft;
		bool Buy;
	};

	double GetBestBid();
	double GetBestOffer();
	double GetSpread();
	double GetMarketPrice();

	OrderId CreateLimitOrder(bool buy, double price, double quantity);
	OrderId CreateMarketOrder(bool buy, double quantity);
	Order GetOrder(OrderId orderId);

	void CancelOrder(OrderId orderId);
private:
	OrderId _createOrder(bool buy, double askingPrice, double quantity, bool limit);

	double m_lastMarketPrice;
	std::map<double, std::list<OrderId>> m_offers;
	std::map<double, std::list<OrderId>> m_bids;
	std::vector<Order> m_orders;
};