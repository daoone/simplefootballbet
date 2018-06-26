#include "simplefootballbet.hpp"
#include <eosiolib/eosio.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/permission.hpp>

using namespace eosio;
using namespace std;

#define PRETTY_ID_FMT(n, v) 	result_fmt += string(n) + ": "; \
								result_fmt += to_string(v); \
								result_fmt += "\n";

#define PRETTY_STR_FMT(n, v) 	result_fmt += string(n) + ": "; \
								result_fmt += v; \
								result_fmt += "\n";

#define PRETTY_ASSET_FMT(n, v) 	result_fmt += string(n) + ": "; \
								result_fmt += v; \
								result_fmt += "\n";


#define PRETTY_STR_PAIR_FMT(n, v) 	result_fmt += string(n) + ": "; \
									result_fmt += v.str1 + ":" + v.str2; \
									result_fmt += "\n";


void simplefootballbet::offer(account_name offer, asset bouns, str_pair game_name, time_point_sec begin) {
	require_auth(offer);

	eosio_assert(bouns.is_valid(), "invalid asset");
	eosio_assert(bouns.amount >= 0, "must offer positive asset");
	eosio_assert(!game_name.str1.empty() && !game_name.str2.empty(), "invaild game name");
	eosio_assert(begin >= time_point_sec(now() + 7200/* 2 hour */) , "invaild time");

	bets bettable(_self, _self);
	games gametable(_self, _self);
	
    // new bet
	auto bet_itr = bettable.emplace(_self, [&](auto& bet) {
		bet.id         = bettable.available_primary_key();
		bet.offer      = offer;
		bet.bouns_pool = bouns;
		bet.game_id    = gametable.available_primary_key();
	});

	// new game
	auto game_itr = gametable.emplace(_self, [&](auto& game) {
		game.id        = bet_itr->game_id;
		game.name      = game_name;
		game.begin     = begin;
	});

	// transfer bouns to contract
	action(
		permission_level{_self, N(active)},
		N(eosio.token), N(transfer),
		std::make_tuple(offer, _self, bouns, std::string(""))
	).send();

	// print the id of bet, "offer_id: xxxxxxxxx"
	string result_fmt = "offer_id: ";
	result_fmt += to_string(bet_itr->id);
	print(result_fmt);
}

void simplefootballbet::canceloffer(int64_t id) {
	print("todo ......");
}

void simplefootballbet::record(int64_t id, time_point_sec end, int_pair score) {
	bets bettable(_self, _self);
	games gametable(_self, _self);

	auto bet_itr = bettable.find(id);
    eosio_assert(bet_itr != bettable.end(), "unknown id");
	require_auth(bet_itr->offer);
	auto game_itr = gametable.find(bet_itr->game_id);
    eosio_assert(game_itr != gametable.end(), "unknown game id");
	eosio_assert(end >= game_itr->begin + seconds(90 * 60), "invaild time");
	eosio_assert(end <= time_point_sec(now()), "invaild time");
	eosio_assert(score.i1 >= 0  && score.i2 >= 0, "invaild score");

	// modify game info
	gametable.modify(game_itr, 0, [&]( auto& game ) {
		game.end = end;
		game.score = score;
	});
}

void simplefootballbet::reveal(int64_t id, account_name player) {
	bets bettable(_self, _self);
	games gametable(_self, _self);
	players playertable(_self, id);

	auto bet_itr = bettable.find(id);
    eosio_assert(bet_itr != bettable.end(), "unknown id");
	auto game_itr = gametable.find(bet_itr->game_id);
    eosio_assert(game_itr != gametable.end(), "unknown game id");

	/*string result_fmt;
	PRETTY_ID_FMT("id", bet_itr->id);
	PRETTY_STR_FMT("offer", bet_itr->offer);
	PRETTY_ASSET_FMT("bouns_pool", bet_itr->bouns_pool);
	PRETTY_STR_PAIR_FMT("score", game_itr->name);*/
	
	auto player_itr = playertable.find(player);
	if (player_itr != playertable.end()) {
		player_itr->bet_asset;
		player_itr->guess_score;
	}

	// TODO: print the info of bet, json format
	print("todo ......");
}

void simplefootballbet::claim(int64_t id, account_name player) {
	print("todo ......");
}

void simplefootballbet::recycle(int64_t id) {
	print("todo ......");
}

void simplefootballbet::deposit(int64_t id, account_name from, int_pair score, asset bet_asset) {
	bets bettable(_self, _self);
	games gametable(_self, _self);
	players playertable(_self, id);

	
	auto bet_itr = bettable.find(id);
	eosio_assert(bet_itr != bettable.end(), "unknown id");
	auto game_itr = gametable.find(bet_itr->game_id);
	eosio_assert(game_itr != gametable.end(), "unknown game id");

	eosio_assert(game_itr->begin >= time_point_sec(now() + 3600), "can't deposit in current time");
	eosio_assert(score.i1 >= 0  && score.i2 >= 0, "invaild score");
	eosio_assert(bet_asset.is_valid(), "invalid asset");
    eosio_assert(bet_asset.amount > 0, "must deposit positive asset");
	eosio_assert(bet_asset.symbol == bet_itr->bouns_pool.symbol, "invalid asset symbol");

	auto player_itr = playertable.find(from);
	if (player_itr == playertable.end()) {
		player_itr = playertable.emplace(_self, [&](auto& player) {
			player.account = from;
			player.bet_asset.push_back(bet_asset);
			player.guess_score.push_back(score);
		});
	} else {
		playertable.modify(player_itr, 0, [&]( auto& player ) {
			auto idx = find(player.guess_score, score);
			if (idx >= player.guess_score.size()) {
				player.bet_asset.push_back(bet_asset);
				player.guess_score.push_back(score);
			} else {
				player.bet_asset[idx] += bet_asset;
			}
		});
	}

	action(
		permission_level{from, N(active)},
		N(eosio.token), N(transfer),
		std::make_tuple(from, _self, bet_asset, std::string(""))
	).send();
}

void simplefootballbet::withdraw(int64_t id, account_name to, int_pair score, asset bet_asset) {
	require_auth(to);

	bets bettable(_self, _self);
	games gametable(_self, _self);
	players playertable(_self, id);

	auto bet_itr = bettable.find(id);
	eosio_assert(bet_itr != bettable.end(), "unknown id");
	auto game_itr = gametable.find(bet_itr->game_id);
	eosio_assert(game_itr != gametable.end(), "unknown game id");

	eosio_assert(game_itr->begin >= time_point_sec(now() + 3600), "can't deposit in current time");
	eosio_assert(score.i1 >= 0  && score.i2 >= 0, "invaild score");
	eosio_assert(bet_asset.is_valid(), "invalid asset");
    eosio_assert(bet_asset.amount > 0, "must deposit positive asset");
	eosio_assert(bet_asset.symbol == bet_itr->bouns_pool.symbol, "invalid asset symbol");

	auto player_itr = playertable.find(to);
	if (player_itr == playertable.end()) {
		eosio_assert(false, "unknown account");
	} else {
		playertable.modify(player_itr, 0, [&]( auto& player ) {
			auto idx = find(player.guess_score, score);
			if (idx >= player.guess_score.size()) {
				eosio_assert(false, "unknown guess_score");
			} else {
				eosio_assert(player.bet_asset[idx] > bet_asset, "insufficient asset");
				player.bet_asset[idx] -= bet_asset;
			}
		});
	}

	action(
		permission_level{_self, N(active)},
		N(eosio.token), N(transfer),
		std::make_tuple(_self, to, bet_asset, std::string(""))
	).send();
}

int simplefootballbet::find(vec_int_pair& scores, int_pair score)  {
	int i = 0;
	for (auto& a : scores) {
		if (a.i1 == score.i1 && a.i2 == score.i2)
			break;
		i ++;
	}
	return i;
}

EOSIO_ABI( simplefootballbet, (offer)(canceloffer)(record)(reveal)(claim)(recycle)(deposit)(withdraw) )
