#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
    public:
        using contract::contract;

        ACTION dummy() {}

        [[eosio::on_notify("eosio.token::transfer")]]
    void ontransfer(name from, name to, asset quantity, std::string memo) {
        if(from == get_self()) {
            senders myTable(get_self(), get_self().value);
        if(myTable.begin() == myTable.end()) {
            myTable.emplace(from, [&](auto& row) {
            row.balance = quantity;
            row.user = to;
            
        });
    } else {
        auto itr = myTable.begin();
            myTable.modify(itr, from, [&](auto& row) {
            row.balance += quantity;
        });
    }
    } else if(to == get_self()){
        recievers myTable(get_self(), get_self().value);
        if(myTable.begin() == myTable.end()){
            myTable.emplace(to, [&](auto& row){
                row.balance = quantity;
                row.user = from;
            });
        } else{
            auto itr = myTable.begin();
            myTable.modify(itr, to, [&](auto& row){
                row.balance += quantity;
            });
        }
        

    }
}
    private:
        TABLE outstruct {
            asset balance;
            name user;

    uint64_t primary_key() const { return user.value; }
};

typedef multi_index<"sender"_n, outstruct> senders;
typedef multi_index<"reciever"_n, outstruct> recievers;

};