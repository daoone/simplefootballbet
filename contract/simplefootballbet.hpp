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

	simplefootballbet(account_name self) : contract(self) {}

	void offer();

	void canceloffer();

	void record();

	void reveal();

	void claim();

	void recycle();

	void deposit();

	void withdraw();

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

	struct str_pair {
		string str1;
		string str2;
	};
	
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

