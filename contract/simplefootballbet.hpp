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

	struct int_pair {
		int64_t i1;
		int64_t i2;
	};

	simplefootballbet(account_name self) : contract(self) {}

	void offer(account_name offer, asset bouns, str_pair game_name, time_point_sec begin);	// return offerId

	void canceloffer(int64_t id);

	void record(int64_t id, time_point_sec end, int_pair score);

	void reveal(int64_t id, account_name player);

	void claim(int64_t id, account_name player);

	void recycle(int64_t id);

	void deposit(int64_t id, account_name from, int_pair score, asset bet_asset);

	void withdraw(int64_t id, account_name to, int_pair score, asset bet_asset);

private:
	//@abi table bet i64 
	struct bet {
		uint64_t             id;
		account_name         offer;
		asset                bouns_pool;
		uint64_t             game_id;
		
		auto primary_key()const { return id; }

		EOSLIB_SERIALIZE(bet, (id)(offer)(bouns_pool)(game_id))
	};
	
	typedef multi_index<N(bet), bet> bets;

	
	//@abi table game i64 
	struct game {
		uint64_t               id;
		str_pair              name;
		time_point_sec        begin;
		time_point_sec        end;
		int_pair              score;
		
		auto primary_key()const { return id; }

		EOSLIB_SERIALIZE(game, (id)(name)(begin)(end)(score))
	};
	
	typedef multi_index<N(game), game> games;
	
	typedef vector<int_pair> vec_int_pair;
	typedef vector<asset> vec_asset;
	//@abi table player i64 
	struct player {
		account_name          account;
		vec_asset             bet_asset;
		vec_int_pair          guess_score;
		
		auto primary_key()const { return account; }

		EOSLIB_SERIALIZE(player, (account)(bet_asset)(guess_score))
	};
	
	typedef multi_index<N(player), player> players;

	static int find(vec_int_pair & scores, int_pair score);
	static int find(const vec_int_pair & scores, int_pair score);
};

