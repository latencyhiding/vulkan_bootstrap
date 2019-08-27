#pragma once
#include <cstdio>
#include <vector>
#include <queue>

namespace Utils
{
#define LOG(logtype, format, ...) \
    do \
    { \
        fprintf(stderr, logtype " %s::%s, line %d: ", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, format "\n", ##__VA_ARGS__);                              \
    } while (false);

#define LOG_ERROR(format, ...) LOG("ERROR", format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) LOG("WARNING", format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) LOG("INFO", format, ##__VA_ARGS__)
    
#define RUNTIME_ERROR(format, ...)                             \
    do                                                         \
    {                                                          \
        LOG_ERROR(format, ##__VA_ARGS__)\
        exit(2);\
    } while (false);


#define ASSERT_MSG(cond, msg, ...) if (!(cond)) { RUNTIME_ERROR("Assertion failed: " #cond ". " msg, ##__VA_ARGS__) };
#define ASSERT(cond) ASSERT_MSG(cond, "")

    // Shit ass implementation of a weak ref manager
    // uses std for convenience, profile this if you think it's actually a problem
    struct WeakRef
    {
        uint32_t index;
        uint32_t generation;
    };

    template <typename T>
    class WeakRefManager
    {
    public:
        // Reserve size is basically a soft cap. If you add more refs past it,
        // the manager expands to accomodate
        WeakRefManager(uint32_t in_reserve_size, uint32_t in_min_free_indices = 1024)
            : reserve_size(in_reserve_size)
            , min_free_indices(in_min_free_indices)
        {
            grow(reserve_size);
        }

        WeakRef add(T val)
        {
            if (free_indices.size() < min_free_indices)
            {
                grow(reserve_size * 2);
            }

            uint32_t index = free_indices.front();
            free_indices.pop();
            uint8_t generation = generations[index];
            data[index] = val;
            return (WeakRef) {
                index,
                generation
            };
        }

        bool ref_is_valid(WeakRef& ref)
        {
            uint8_t current_generation = generations[ref.index];
            return ref.index < data.size() && current_generation == ref.generation;
        }

        void remove(WeakRef ref)
        {
            if (!ref_is_valid(ref))
            {
                log_invalid_ref_warning(ref);
                return;
            }

            free_indices.push(ref.index);
            generations[ref.index]++;
        }

        T* get(WeakRef ref)
        {
            if (!ref_is_valid(ref))
            {
                log_invalid_ref_warning(ref);
                return nullptr;
            }

            return &data[ref.index];
        }
    private:
        std::vector<T> data;
        std::vector<uint8_t> generations;
        std::queue<size_t> free_indices;

        uint32_t reserve_size;
        uint32_t min_free_indices;

        void log_invalid_ref_warning(const WeakRef& ref)
        {
            uint8_t current_generation = generations[ref.index];
            LOG_WARNING("Invalid ref. Index should be less than %llu, is %u. Generation should be %d, is %d", data.size(), ref.index, current_generation, ref.generation);
        }

        void grow(size_t new_reserve_size)
        {
            ASSERT(new_reserve_size >= reserve_size);
            reserve_size = new_reserve_size;
            data.reserve(reserve_size);
            generations.reserve(reserve_size);

            for (size_t i = data.size(); i < reserve_size; i++)
            {
                free_indices.push(i);

                T default_value;
                data.push_back(default_value);

                generations.push_back(0);
            }
        }
    };
}