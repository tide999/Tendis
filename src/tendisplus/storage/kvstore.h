#ifndef SRC_TENDISPLUS_STORAGE_KVSTORE_H_
#define SRC_TENDISPLUS_STORAGE_KVSTORE_H_

#include <string>
#include <utility>
#include <memory>
#include <iostream>
#include "tendisplus/utils/status.h"
#include "tendisplus/storage/record.h"

namespace tendisplus {

class KVStore;
using PStore = std::shared_ptr<KVStore>;

class Transaction {
 public:
    Transaction() = default;
    Transaction(const Transaction&) = delete;
    Transaction(Transaction&&) = delete;
    virtual ~Transaction() = default;
    virtual Status commit() = 0;
    virtual Status rollback() = 0;
    virtual Expected<std::string> getKV(const std::string& key) = 0;
    virtual Status setKV(const std::string& key, const std::string& val) = 0;
    virtual Status delKV(const std::string& key) = 0;
};

class KVStore {
 public:
    explicit KVStore(const std::string& id);
    virtual ~KVStore() = default;
    virtual Expected<std::unique_ptr<Transaction>> createTransaction() = 0;
    virtual Expected<RecordValue> getKV(const RecordKey& key,
        Transaction* txn) = 0;
    virtual Status setKV(const Record& kv, Transaction* txn) = 0;
    virtual Status delKV(const RecordKey& key, Transaction* txn) = 0;
    // NOTE(deyukong): INSTANCE_NUM can not be dynamicly changed.
    static constexpr size_t INSTANCE_NUM = size_t(100);
 private:
    std::string _id;
};

}  // namespace tendisplus

#endif   // SRC_TENDISPLUS_STORAGE_KVSTORE_H_