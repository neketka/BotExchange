#pragma once

#include <concepts>
#include <vector>
#include <ranges>
#include <enet/enet.h>

template<class T>
concept pod = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

template<class T>
concept serializable = 
	pod<T> || requires(T t, const std::ranges::subrange<std::byte>& u) {
		{ t.GetSerializedSize() } -> std::same_as<size_t>;
		{ t.Serialize(u) } -> std::same_as<void>;
	};

template<serializable T>
class Packet {
public:
	~Packet() {

	}
private:
	Packet(ENetPacket *packet) {

	}
};

