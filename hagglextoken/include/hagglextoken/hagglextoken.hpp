#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

#include <string>

namespace eosiosystem {
   class system_contract;
}

using namespace eosio;

   using std::string;

   CONTRACT hagglextoken : public contract {
      public:
         using contract::contract;
         
          [[eosio::action]] 
         void create( const name&   issuer,
                      const asset&  maximum_supply);
        
          [[eosio::action]] 
         void issue( const name& to, const asset& quantity, const string& memo );

          [[eosio::action]] 
         void retire( const asset& quantity, const string& memo );

   
          [[eosio::action]] 
         void transfer( const name&    from,
                        const name&    to,
                        const asset&   quantity,
                        const string&  memo );
       
          [[eosio::action]] 
         void open( const name& owner, const symbol& symbol, const name& ram_payer );

        
          [[eosio::action]] 
         void close( const name& owner, const symbol& symbol );


         [[eosio::action]]
         void mint(const symbol_code& sym);

         [[eosio::action]]
         void blacklist( name account, string memo );

         [[eosio::action]]
         void unblacklist( name account );


         static asset get_supply( const name& token_contract_account, const symbol_code& sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }


         static asset get_balance( const name& token_contract_account, const name& owner, const symbol_code& sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }




         asset get_reward( asset currentsupply, symbol_code sym ){

            asset reward;

            if (currentsupply.amount/10000 <= 4672000){ //halvening 0
               reward =  asset(3200, symbol(sym, 4));
            }
            else if (currentsupply.amount/10000 <= 2336000) {//halvening 1
               reward =  asset(1600, symbol(sym, 4));
            }
            else if (currentsupply.amount/10000 <= 1168000) {//halvening 2
               
               reward =  asset(800, symbol(sym, 4));
            }
            else if (currentsupply.amount/10000 <= 584000) { //halvening 3
               
               reward =  asset(400, symbol(sym, 4));
            }
            else if (currentsupply.amount/10000 < 240000) { //halvening 4
               
               reward =  asset(200, symbol(sym, 4));
            }
            else {

               reward =  asset(0, symbol(sym, 4));
            }
            return reward;
         } 

   

         using create_action = eosio::action_wrapper<"create"_n, &hagglextoken::create>;
         using issue_action = eosio::action_wrapper<"issue"_n, &hagglextoken::issue>;
         using retire_action = eosio::action_wrapper<"retire"_n, &hagglextoken::retire>;
         using transfer_action = eosio::action_wrapper<"transfer"_n, &hagglextoken::transfer>;
         using open_action = eosio::action_wrapper<"open"_n, &hagglextoken::open>;
         using close_action = eosio::action_wrapper<"close"_n, &hagglextoken::close>;

         
      private:
         TABLE account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         TABLE currency_stats {
            asset          supply;
            asset          max_supply;
            name           issuer;
            uint32_t       starttime;
            uint32_t       minetime;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

          TABLE blacklist_table {
            name      account;

            auto primary_key() const {  return account.value;  }
         };


         typedef eosio::multi_index< "accounts"_n, account > accounts;
         typedef eosio::multi_index< "stat"_n, currency_stats > stats;
         typedef eosio::multi_index< "blacklists"_n, blacklist_table > blacklists;


         void sub_balance( const name& owner, const asset& value );
         void add_balance( const name& owner, const asset& value, const name& ram_payer );
   };

