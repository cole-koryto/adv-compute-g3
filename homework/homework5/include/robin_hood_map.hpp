// robin hood hash map implementation

#ifndef ROBIN_HOOD_MAP_HPP
#define ROBIN_HOOD_MAP_HPP

#include <array>
#include <cstdint> // for uint64_t, int32_t
#include <cstring> // for memcpy
#include <string>
#include <vector>
#include <stdexcept> // for std::invalid_argument
#include <utility>   // for std::as_const

typedef std::array<char, 8> Key;

template <typename Value>
struct Slot
{
    Key key;
    Value value;
    int32_t psl = -1;
};

template <typename Value>
class RobinHoodMap
{
    std::vector<Slot<Value>> table;
    size_t capacity;
    size_t size;

public:
    RobinHoodMap(size_t initial_capacity = 16)
        : capacity(initial_capacity), size(0)
    {
        // ensure power of 2
        if ((capacity & (capacity - 1)) != 0)
        {
            throw std::invalid_argument("Capacity must be a power of 2");
        }

        table.resize(capacity);
    }

    const std::vector<Slot<Value>> &get_table() const
    {
        return table;
    }

    static uint64_t hash(const std::array<char, 8> &key)
    {
        uint64_t h{0};
        std::memcpy(&h, key.data(), 8); // copy 8 bytes of the key into the hash value

        h ^= (h >> 30);
        h *= 0xbf58476d1ce4e5b9;
        h ^= (h >> 27);
        h *= 0x94d049bb133111eb;
        h ^= (h >> 31);

        return h;
    }

    void insert(std::string_view key, const Value &value)
    {
        if (size * 100 >= capacity * 85) // load factor of 0.85, resize when exceeded
            resize();

        auto normalized_key = normalize_key(key);
        // Implementation for inserting key-value pair
        insert_normalized(normalized_key, value);
    }

    void erase(std::string_view key)
    {
        auto normalized_key = normalize_key(key);
        auto h = hash(normalized_key);
        size_t index = h & (capacity - 1);
        int32_t probe_count = 0;

        // need to find the index of the key to be erased first:
        while (true)
        {
            auto &slot = table[index];

            if (slot.psl == -1)
            {
                return; // Key not found, nothing to erase
            }

            if (slot.key == normalized_key)
            {
                break; // Key found, proceed to erase
            }

            if (slot.psl < probe_count)
            {
                return; // Key not found, as we have probed past the expected position
            }

            probe_count++;
            index = (index + 1) & (capacity - 1); // Move to the next slot
        }
        // Now we have the index of the key to be erased
        // Mark the slot as deleted by setting PSL to -1 and clearing the key
        table[index].psl = -1;
        size_t hole = index;
        size_t next = (index + 1) & (capacity - 1);

        while (table[next].psl > 0)
        {
            table[hole] = table[next];
            table[hole].psl--;
            table[next].psl = -1;
            hole = next;
            next = (next + 1) & (capacity - 1);
        }
        size--;
    }

    Value *find(std::string_view key)
    {
        // cast away const-ness since find does not modify the map,
        // and we want to return a non-const pointer for convenience
        return const_cast<Value *>(std::as_const(*this).find(key));
    }

    const Value *find(std::string_view key) const
    {
        auto normalized_key = normalize_key(key);
        // Implementation for finding value by key
        auto h = hash(normalized_key);
        size_t index = h & (capacity - 1);
        int32_t probe_count = 0;

        while (true)
        {
            const auto &slot = table[index];

            if (slot.psl == -1)
            {
                return nullptr; // Key not found
            }

            if (slot.key == normalized_key)
            {
                return &slot.value; // Key found
            }

            if (slot.psl < probe_count)
            {
                // Robin Hood gaurantees that if we have probed more than the current slots PSL
                // then the key we are looking for cannot be in the table, as it would have been swapped out
                return nullptr; // Key not found, as we have probed past the expected position
            }

            probe_count++;
            index = (index + 1) & (capacity - 1); // Move to the next slot
        }
    }

    void resize()
    {
        auto old_table = std::move(table);
        capacity *= 2;
        table = std::vector<Slot<Value>>(capacity);
        size = 0;
        for (const auto &slot : old_table)
        {
            if (slot.psl != -1)
            {
                insert_normalized(slot.key, slot.value);
            }
        }
    }

private:
    // check for size of key --> convert to 8 byte array
    static std::array<char, 8> normalize_key(std::string_view key)
    {
        if (key.size() > 8)
        {
            throw std::invalid_argument("Key must be at most 8 characters long");
        }

        std::array<char, 8> normalized_key{};

        std::memcpy(normalized_key.data(), key.data(), key.size()); // copy key data into normalized_key
        return normalized_key;
    }

    void insert_normalized(const std::array<char, 8> &normalized_key, const Value &value)
    {
        // Implementation for inserting a normalized key-value pair
        auto h = hash(normalized_key);
        size_t index = h & (capacity - 1); // since capacity power of 2, % capacity = & (capacity - 1)
        // Insert logic with Robin Hood hashing

        Slot<Value> candidate{normalized_key, value, 0};
        while (true)
        {
            auto &slot = table[index];

            if (slot.psl == -1)
            {
                slot = candidate;
                size++;
                break;
            }

            if (slot.key == normalized_key)
            {
                slot.value = value;
                break;
            }

            if (slot.psl < candidate.psl)
            {
                std::swap(slot, candidate);
                index = (index + 1) & (capacity - 1);
                candidate.psl++;
            }

            else
            {
                candidate.psl++;
                index = (index + 1) & (capacity - 1);
            }
        }
    }
};

#endif // ROBIN_HOOD_MAP_HPP