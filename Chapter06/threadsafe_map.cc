/**
 * @file threadsafe_map.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 线程安全的查找表
 * @version 0.1
 * @date 2020-08-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <boost/thread/shared_mutex.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table {
public:
  typedef Key   key_type;
  typedef Value mapped_type;
  typedef Hash  hash_type;

  threadsafe_lookup_table(unsigned    num_buckets = 19,
                          Hash const &hasher_     = Hash())
      : buckets(num_buckets),
        hasher(hasher_) {
    for (unsigned i = 0; i < num_buckets; i++) {
      buckets[i].reset(new bucket_type);
    }
  }

  threadsafe_lookup_table(const threadsafe_lookup_table &) = delete;
  threadsafe_lookup_table &operator=(const threadsafe_lookup_table &) = delete;

  Value value_for(Key const &key, Value const &default_value = Value()) const {
    return get_bucket(key).value_for(key.default_value);
  }

  void add_or_update_mapping(Key const &key, Value const &value) {
    get_bucket(key).add_or_update_mapping(key, value);
  }

  void remove_mapping(Key const &key) {
    get_bucket(key).remove_mapping(key);
  }

  std::map<Key, Value> get_map() const {
    std::vector<std::unique_lock<boost::shared_mutex>> locks;
    for (unsigned i = 0; i < buckets.size(); i++) {
      locks.push_back(std::unique_lock<boost::shared_mutex>(buckets[i].muetx));
    }

    std::map<Key, Value> res;
    for (unsigned i = 0; i < buckets.size(); i++) {
      for (auto it = buckets[i].data.begin(); it != buckets[i].data.end();
           ++it) {
        res.insert(*it);
      }
    }
    return res;
  }

private:
  class bucket_type {
  public:
    Value value_for(Key const &key, Value const &default_value) const {
      boost::shared_lock<boost::shared_mutex> lk{mutex};
      bucket_iterator const                   find_entry = find_entry_for(key);
      return find_entry == data.end() ? default_value : find_entry->second;
    }

    void add_or_update_mapping(Key const &key, Value const &value) {
      std::unique_lock<boost::shared_mutex> lk{mutex};
      bucket_iterator const                 found_entry = find_entry_for(key);
      if (found_entry == data.end()) {
        data.push_back(std::make_pair(key, value));
      } else {
        found_entry->second = value;
      }
    }

    void remove_mapping(Key const &key) {
      std::unique_lock<boost::shared_mutex> uk{mutex};
      bucket_iterator                       found_entry = find_entry_for(key);
      if (found_entry != data.end()) {
        data.erase(found_entry);
      }
    }

  private:
    typedef std::pair<Key, Value>          bucket_value;
    typedef std::list<bucket_value>        bucket_data;
    typedef typename bucket_data::iterator bucket_iterator;
    bucket_data                            data;
    mutable boost::shared_mutex            mutex;

    bucket_iterator find_entry_for(Key const &key) const {
      return std::find_if(data.begin(), data.end(),
                          [&](bucket_value const &item) {
                            return item.first == key;
                          });
    }
  };

  std::vector<std::unique_ptr<bucket_type>> buckets;
  Hash                                      hasher;
  bucket_type &                             get_bucket(Key const &key) const {
    std::size_t const bucket_index = hasher(key) % buckets.size();
    return *buckets[bucket_index];
  }
};

int main(int argc, char **argv) {
}