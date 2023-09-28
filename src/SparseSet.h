#pragma once
#include <Types.h>
#include <Components.h>

namespace Ligmengine
{
    class SparseSetHolder
    {
        public:
            virtual ~SparseSetHolder() = default;
            virtual bool Has(EntityID) const = 0;
            virtual void Drop(EntityID) = 0;
    };

    template< typename T > class SparseSet : public SparseSetHolder
    {
        public:
            std::unordered_map< EntityID, T > data;
            bool Has(EntityID e) const override { return data.count(e) > 0; };
            void Drop(EntityID e) override { data.erase(e); };
    };
}
