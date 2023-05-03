#include <vector>
#include <iostream>


namespace Ivan{

	template<typename Key, typename Value>
	using par = std::pair<Key, Value>;

	template<typename Key, typename Value>
	class cache_table final
	{
		
		private:
			struct Node
			{
			    Key k;
			    Value v;
			    int16_t distance_from_desired_pos = -1;
		            
			};
		std::vector<Node> table;
		size_t num_elements = 0;
	        float MAX_LOAD_FACTOR = 0.8;
		

		public:
		    cache_table(): table(5), num_elements(0) {}
		    cache_table(uint16_t sz): table(sz) {}
		    
		using It = typename decltype(table)::iterator;
		using const_It = typename decltype(table)::const_iterator;
		using rev_It = typename decltype(table)::reverse_iterator;
		using crev_It = typename decltype(table)::const_reverse_iterator;
		
		static size_t hash_key(const Key& key)
		{
			size_t h =  std::hash<Key>{}(key);
			return h;
		}
		
		size_t bucket_size() const
		{
	            return table.size();
		}
		
		It entry(size_t pos)
		{
		    return begin() + pos;
		}
		void swap(It x, It y)
		{
		    std::swap(x->k, y->k);
		    std::swap(x->v, y->v);
		}
		size_t num_items() const
		{
		    return num_elements;
		}

		void resize(size_t sz)
		{
		    table.resize(sz);
		    for(auto it = begin(), it1 = end(); it != it1; ++it)
		    {
			if(it->distance_from_desired_pos == -1)
				continue;
			
			size_t pos = hash_key(it->k) % sz;
			It new_pos = entry(pos);
			if(it == new_pos)
				continue;
			for(int16_t index = 0;;++index, ++new_pos)
			{
			    if(new_pos->distance_from_desired_pos == -1)
			    {
			        create(new_pos, it->k, it->v, index);
				destroy(it);
				break;
			    }
			    else if(new_pos->distance_from_desired_pos < index)
			    {
				    swap(it, new_pos);
				    break;
			    }
			}
		    }
		}

		template<typename K, typename V>
		void insert(K&& key, V&& value)
		{
			size_t sz = bucket_size();
			float load_factor = num_elements / static_cast<float>(sz);
			if(load_factor >= MAX_LOAD_FACTOR)
			{
			    resize( sz * 2);
			}
			
			size_t pos = hash_key(key) % sz;
			It it = entry(pos);
			for(int16_t index = 0;;++index, ++it)
			{
			    if(it->distance_from_desired_pos == -1)
			    {
			        create(it, key, value, index);
				break;
			    }
			    else if(it->k == key)
				    return;
			    else if(it->distance_from_desired_pos < index)
			    {
			        std::swap(it->distance_from_desired_pos, index);
				std::swap(it->k, key);
				std::swap(it->v, value);
			    }

			   if(index == std::max<int>(4, std::log2(sz)))
				    resize(sz * 2);
			}
			++num_elements;

		}

		It find(const Key& key)
		{
		    size_t pos = hash_key(key) % bucket_size();
		    auto it = begin() + pos;
		    for(int16_t index = 0;; ++index, ++it)
		    {
			if(it->distance_from_desired_pos < index)
				return end();
		        if(it->k == key)
		            return it;
		    }
		}
		
		const_It find(const Key& key) const
		{
		    size_t pos = hash_key(key) % bucket_size();
		    auto it = cbegin() + pos;
		    for(int16_t index = 0;; ++index, ++it)
		    {
			if(it->distance_from_desired_pos < index)
				return cend();
		        if(it->k == key)
		            return it;
		    }
		}
		
		void erase(const Key& key)
		{
		    auto it = find(key);
		    if(it != end())
			    destroy(it);
		    --num_elements;
		}

		It erase(const Key& key)
		{
		    auto it = find(key);
		    if( it != end())
		    {
			destroy(it);
			--num_items;
			return it;
		    }
		    return end();
		}


		It begin()
		{
		    return table.begin();	    
		}

		const_It cbegin() const
		{
		   return table.cbegin();
		}

		It end()
		{
		    return table.end();
		}

		const_It cend() const
		{
		    return table.cend();
		}

		rev_It rbegin()
		{
		    return table.rbegin();
		}

		rev_It rend()
		{
		    return table.rend();
		}

		crev_It crbegin() const
		{
		    return table.crbegin();
		}

		crev_It crend() const
		{
		    return table.crend();
		}
		
		void create(It it, const Key& key, const Value& val, int8_t index)
		{
		    it->k = key;
		    it->v = val;
		    it->distance_from_desired_pos = index;
		}

		void destroy(It it)
		{
		    it->k.~Key();
		    it->v.~Value();
		    it->distance_from_desired_pos = -1;
		}

	};
}


