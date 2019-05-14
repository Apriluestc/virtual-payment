#ifndef INCLUDE_REDISCACHE_H
#define INCLUDE_REDISCACHE_H

#include <iostream>
#include <string>

#include <mysql/mysql.h>
#include <hiredis/hiredis.h>
#include <boost/noncopyable.hpp>

class RedisCache : boost::noncopyable {
public:
    static void connectRedis();
    static void initRedis();
    static void closeRedis();
    
    static void setString(const std::string& key, const std::string& value);
    static void(setString(const std::string& key, const int& value));
    static void setString(const std::string& data);

    static void getString(const std::string& key, std::string& value);
    static void getString(const std::string& key, int& value);
    static void getString(const std::string& key);

    static void releaseRedis();
private:
    RedisCache() = delete;
    virtual ~RedisCache() = delete;
    RedisCache(const RedisCache& rhx) = delete;
    RedisCache& operator=(const RedisCache& rhx) = delete;
};

#endif
