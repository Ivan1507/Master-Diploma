#include <catch2/catch_all.hpp>
#include "hashtable.cpp"
#include <unordered_map>




TEMPLATE_TEST_CASE_SIG("insert", "[ins]", ((size_t N), N), 10, 100, 1000, 10000, 100000)
{
    Ivan::cache_table<int, int> map;
    BENCHMARK("Insert into cache_table")
    {
        for(int i = 0 ; i < N; ++i)
        {
	    int key = std::rand() % 1000 + 1;
	    map.insert(key, i);
        }
    };
}

TEMPLATE_TEST_CASE_SIG("insert", "[ins1]", ((size_t N), N), 10, 100, 1000, 10000, 100000, 1000000)
{
    std::unordered_map<int, int> map;
    BENCHMARK("insert into std::unordered_map")
    {
        for(int i = 0 ; i < N; ++i)
        {
	    int key = std::rand() % N + 1;
	    map.insert({key, i});
        }
    };	
}


TEMPLATE_TEST_CASE_SIG("find", "[find], [cache]", ((size_t N), N), 10, 100, 1000, 10000, 100000, 1000000)
{
    Ivan::cache_table<int, int> map;
    for(int i = 0 ; i < N; ++i)
    {
	    int key = std::rand() % 1000 + 1;
	    map.insert(key, i);
    }
    BENCHMARK("find in cache_table")
    {
	for(int i =0 ;i < N; ++i)
	{
	    int key = std::rand() % 1000 + 1;
	    map.find(key);
	}
    };
}

TEMPLATE_TEST_CASE_SIG("find", "[find], [umap]", ((size_t N), N), 10, 100, 1000, 10000, 100000, 1000000)
{
    std::unordered_map<int, int> map;
    for(int i = 0 ; i < N; ++i)
    {
	    int key = std::rand() % N + 1;
	    map.insert({key, i});
    }
    BENCHMARK("find in std::unordered_map")
    {
	for(int i =0; i < N; ++i)
	{
	    int key = std::rand() % N + 1;
	    map.find(key);
	}
    };
}

TEMPLATE_TEST_CASE_SIG("erase", "[erase], [cachemap]", ((size_t N), N), 10, 100, 1000, 10000, 100000)
{
    Ivan::cache_table<int, int> map;
    for(int i = 0 ; i < N; ++i)
    {
	    int key = std::rand() % N + 1;
	    map.insert(key, i);
    }
    BENCHMARK("erase in Ivan::cache_table")
    {
	for(int i =0; i < N; ++i)
	{
	    int key = std::rand() % N + 1;
	    map.erase(key);
	}
    };
}

TEST_CASE("erase", "[er]")
{
    int N = 1000000;
    std::unordered_map<int, int> map;
    for(int i = 0 ; i < N; ++i)
    {
	    int key = std::rand() % N + 1;
	    map.insert({i, i});
    }
    BENCHMARK("erase in Ivan::cache_table")
    {
	for(int i =0; i < N; ++i)
	{
	    int key = std::rand() % N + 1;
	    map.erase(i);
	}
    };

}

TEMPLATE_TEST_CASE_SIG("erase", "erase", ((size_t N), N), 10, 100, 1000, 10000, 100000, 1000000)
{
    std::unordered_map<int, int> map;
    for(int i = 0 ; i < N; ++i)
    {
	    int key = std::rand() % N + 1;
	    map.insert({key, i});
    }
    BENCHMARK("erase in std::unordered_map")
    {
	for(int i = 0; i < N; ++i)
	{
	    int key = std::rand() % N + 1;
	    map.erase(key);
	}
    };
}
