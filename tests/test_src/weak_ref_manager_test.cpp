#include <catch2/catch.hpp>
#include "utils.h"

TEST_CASE("Add/Get Consistency", "[weak_ref_manager]") 
{
    Utils::WeakRefManager<size_t> manager(100, 10);

    const size_t num_ids = GENERATE(as<size_t>{}, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048);
    GIVEN("A ref manager with some amount of ids")
    {
        WHEN("Some values are added")
        {
            std::vector<Utils::WeakRef> refs;
            for (size_t i = 0; i < num_ids; i++)
            {
                auto ref = manager.add(i);
                refs.push_back(ref);
            }
            
            for (size_t i = 0; i < num_ids; i++)
            {
                size_t* val = manager.get(refs[i]);
                REQUIRE(val);
                REQUIRE(*val == i);
            }
        }
    }
}

TEST_CASE("Removal Invalidates WeakRefs", "[weak_ref_manager]")
{
    Utils::WeakRefManager<int> manager(100, 10);

    const size_t num_ids = GENERATE(as<int>{}, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048);
    std::vector<Utils::WeakRef> refs;
    for (int i = 0; i < num_ids; i++)
    {
        auto ref = manager.add(i);
        refs.push_back(ref);
    }

    for (int i = 0; i < num_ids; i++)
    {
        manager.remove(refs[i]);
    }
    
    for (int i = 0; i < num_ids; i++)
    {
        int* val = manager.get(refs[i]);
        REQUIRE(!val);
    }
}

TEST_CASE("Removal Frees WeakRefs", "[weak_ref_manager]")
{
    Utils::WeakRefManager<int> manager(1, 1);

    auto old_ref = manager.add(888);
    manager.remove(old_ref);

    auto new_ref = manager.add(999);
    REQUIRE(new_ref.index == old_ref.index);
    REQUIRE(new_ref.generation == old_ref.generation + 1);
    REQUIRE(*manager.get(new_ref) == 999);
    REQUIRE(manager.get(old_ref) == nullptr);
}

TEST_CASE("No Free Indices Grows Manager", "[weak_ref_manager]")
{
    Utils::WeakRefManager<int> manager(1, 1);

    auto old_ref = manager.add(888);
    auto new_ref = manager.add(999);
    REQUIRE(new_ref.index == old_ref.index + 1);
    REQUIRE(new_ref.generation == 0);
    REQUIRE(*manager.get(new_ref) == 999);
    REQUIRE(*manager.get(old_ref) == 888);
}