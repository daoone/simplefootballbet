/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/time.hpp>

#include <vector>

using namespace eosio;
using namespace std;



class simplefootballbet : public contract {
public:
	struct str_pair {
		string str1;
		string str2;
	};

	simplefootballbet(account_name self) : contract(self) {}

	void offer(asset bouns, str_pair game_name, time_point_sec begin);	// return offerId

	void canceloffer(int64_t id);

	void record(int64_t id, time_point_sec end, str_pair score);

	void reveal(int64_t id, account_name player);

	void claim(int64_t id, account_name player);

	void recycle(int64_t id);

	void deposit(int64_t id, str_pair score, asset bet_asset);

	void withdraw(int64_t id, str_pair score, asset bet_asset);

private:
	//@abi table bet i64 
	struct bet {
		int64_t             id;
		account_name        owner;
		asset               bouns_pool;
		int64_t             game_id;
		
		auto primary_key()const { return id; }

		EOSLIB_SERIALIZE(bet, (id)(owner)(bouns_pool)(game_id))
	};
	
	typedef multi_index<N(bet), bet> bets;

	
	//@abi table game i64 
	struct game {
		int64_t               id;
		str_pair              name;
		time_point_sec        begin;
		time_point_sec        end;
		str_pair              score;
		
		auto primary_key()const { return id; }

		EOSLIB_SERIALIZE(game, (id)(name)(begin)(end)(score))
	};
	
	typedef multi_index<N(game), game> games;
	
	typedef vector<str_pair> vec_str_pair;
	typedef vector<asset> vec_asset;
	//@abi table player i64 
	struct player {
		account_name          account;
		vec_asset             bet_asset;
		vec_str_pair          guess_score;
		
		auto primary_key()const { return account; }

		EOSLIB_SERIALIZE(player, (account)(bet_asset)(guess_score))
	};
	
	typedef multi_index<N(player), player> players;
};

