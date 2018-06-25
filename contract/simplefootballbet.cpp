#include "simplefootballbet.hpp"
#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/permission.hpp>

using namespace eosio;
using namespace std;

void simplefootballbet::offer(asset bouns, str_pair game_name, time_point_sec begin) {
	print("todo ......");
}

void simplefootballbet::canceloffer(int64_t id) {
	print("todo ......");
}

void simplefootballbet::record(int64_t id, time_point_sec end, str_pair score) {
	print("todo ......");
}

void simplefootballbet::reveal(int64_t id, account_name player) {
	print("todo ......");
}

void simplefootballbet::claim(int64_t id, account_name player) {
	print("todo ......");
}

void simplefootballbet::recycle(int64_t id) {
	print("todo ......");
}

void simplefootballbet::deposit(int64_t id, str_pair score, asset bet_asset) {
	print("todo ......");
}

void simplefootballbet::withdraw(int64_t id, str_pair score, asset bet_asset) {
	print("todo ......");
}

EOSIO_ABI( simplefootballbet, (offer)(canceloffer)(record)(reveal)(claim)(recycle)(deposit)(withdraw) )
