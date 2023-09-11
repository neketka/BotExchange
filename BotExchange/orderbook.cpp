#include "orderbook.h"

double Orderbook::GetBestBid() {
	if (m_bids.empty()) {
		return 0;
	}

	return m_bids.rbegin()->first;
}

double Orderbook::GetBestOffer() {
	if (m_offers.empty()) {
		return std::numeric_limits<double>::max();
	}

	return m_offers.begin()->first;
}

double Orderbook::GetSpread() {
	return GetBestOffer() - GetBestBid();
}

double Orderbook::GetMarketPrice() {
	return m_lastMarketPrice;
}

Orderbook::OrderId Orderbook::CreateLimitOrder(bool buy, double price, double quantity) {
	return _createOrder(buy, price, quantity, true);
}


Orderbook::OrderId Orderbook::CreateMarketOrder(bool buy, double quantity) {
	return _createOrder(buy, -1.0, quantity, false);
}

Orderbook::Order Orderbook::GetOrder(OrderId orderId) {
	return m_orders[orderId];
}

void Orderbook::CancelOrder(OrderId orderId) {
	Order& order = m_orders[orderId];
	order.QuantityLeft = 0;

	if (order.AskingPrice == -1.0) return;

	auto& orderPair = *(order.Buy ? m_bids : m_offers).find(order.AskingPrice);
	orderPair.second.remove(orderId);
	if (orderPair.second.empty()) {
		(order.Buy ? m_bids : m_offers).erase(order.AskingPrice);
	}
}

Orderbook::OrderId Orderbook::_createOrder(bool buy, double askingPrice, double quantity, bool limit) {
	OrderId orderId = m_orders.size();
	m_orders.push_back(Order());

	Order& order = m_orders.back();
	order.Buy = buy;
	order.Quantity = quantity;
	order.QuantityLeft = quantity;
	order.AskingPrice = askingPrice;

	auto& opp_orders = buy ? m_offers : m_bids;
	auto& orders = buy ? m_bids : m_offers;

	while (!opp_orders.empty() && order.QuantityLeft > 0) {
		auto& [price, bestOppOrders] = buy ? *m_offers.begin() : *m_bids.rbegin();

		if (limit && buy && price > askingPrice || limit && !buy && price < askingPrice) {
			break;
		}

		while (!bestOppOrders.empty() && order.QuantityLeft > 0) {
			Order best_opp_order = m_orders[bestOppOrders.front()];

			double to_take = std::min(order.QuantityLeft, best_opp_order.QuantityLeft);

			order.QuantityLeft -= to_take;
			order.QuantityFilled += to_take;
			order.AvgPrice += price * to_take;

			best_opp_order.QuantityLeft -= to_take;
			best_opp_order.QuantityFilled += to_take;

			m_lastMarketPrice = price;

			if (best_opp_order.QuantityLeft == 0) {
				bestOppOrders.pop_front();
			}
		}

		if (bestOppOrders.empty()) {
			opp_orders.erase(price);
		}
	}

	if (order.QuantityFilled > 0) {
		order.AvgPrice /= order.QuantityFilled;
	}

	if (limit) {
		orders[askingPrice].push_back(orderId);
	}

	return orderId;
}