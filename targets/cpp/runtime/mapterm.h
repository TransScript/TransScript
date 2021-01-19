// Copyright (c) 2016 IBM Corporation.
#ifndef _MAPTERM
#define _MAPTERM

// DO NOT INCLUDE DIRECTLY. INSTEAD INCLUDE term.h

#include <unordered_map>
#include <compat.h>
#include <iowrapper.h>
#include <set>
#include <vector>
#include "talloc.h"

// Forward declarations
template<typename V>
class Option;

template<typename V>
class List;

template<typename a> ::Option<a>& newNONE(tosca::Context& ctx);
template<typename a> ::Option<a>& newSOME(tosca::Context& ctx, a& param);

namespace tosca {

    // Forward declarations
    template<typename K, typename V> class MapTerm;
    template<typename K, typename V> MapTerm<K, V>& newMapTerm(Context&);
    template<typename T> T& NewRef(T&);


    // MapTerm type definition
    template<typename K, typename V>
    class MapTerm: public Term
    {
    public:
        virtual ~MapTerm()
        {
        }

        /**
         * Creates a new map reference which inherits all properties in this
         * instance.
         *
         * This reference is consumed.
         *
         * @return A new non-shared map.
         */
        virtual MapTerm<K, V>& extend(Context& ctx)
        {
            throw new std::runtime_error("");
        }

        /**
         * Add key-value pair to map.
         *
         * @param key term. The reference is used.
         * @param value the associated term value. The reference is used.
         */
        virtual void putValue(Context& ctx, K& key, V& value)
        {
            throw new std::runtime_error("");
        }

        /**
         * Get value corresponding to given key
         * @param key. The reference is NOT consumed.
         * @return An new reference to an optional typed term.
         */
        virtual Option<V>& getValue(Context& ctx, K& key)
        {
            throw new std::runtime_error("");
        }

        /**
         * Put all entries in the given map into this map
         * @param map
         */
        virtual void putAll(Context& ctx, MapTerm<K, V>& map)
        {
            throw new std::runtime_error("");
        }

        /**
         * Gets map values
         * @param context
         * @return
         */
        virtual std::vector<V*> values(Context& ctx)
        {
            throw new std::runtime_error("");
        }

        /**
         * Gets map keys
         * @param context
         * @return
         */
        virtual std::vector<K*> keys(Context& ctx)
        {
            throw new std::runtime_error("");
        }

        /**
         * @return true when this map is empty
         */
        virtual bool isEmpty()
        {
            throw new std::runtime_error("");
        }

        /**
         * @return true when this map contains an entry for the given key
         */
        virtual bool containsKey(K key)
        {
            throw new std::runtime_error("");
        }

        /**
         * @return true when this map contains an entry for the given key, including variables.
         */
        virtual bool contains(Term key)
        {
            throw new std::runtime_error("");
        }

        /*
         * Make new map term.
         */
        static Term& MakeTerm(Context& ctx, const std::string& symbol)
        {
            return newMapTerm<K, V>(ctx);
        }

        // Overrides

        const tosca::string& Symbol() const
        {
            // TODO: map have no symbol.
            static const tosca::string emptymap("{}", StaticAlloc());
            return emptymap;
        }

        bool IsMap() const
        {
            return true;
        }
    };


    // MapTerm value
    template<typename K, typename V>
    class CMapTerm: public MapTerm<K, V>
    {
    public:
        virtual ~CMapTerm()
        {
            if (parent)
                parent.value().Release();

            for (auto iter = map.begin(); iter != map.end(); iter ++)
            {
                iter->first->Release();
                iter->second->Release();
            }
        }

        Term& Copy(Context& ctx)
        {
            return newMapTerm<K, V>(ctx);
        }

        MapTerm<K, V>& extend(Context& ctx)
        {
            static const bool mapreuse = getenv("nomapreuse") == 0;
            if (mapreuse && this->refcount == 1)
            	return Compact(ctx, false);

            Compact(ctx, true);

            // temp code for debugging non-immortal extension.
            if (this->IsImmortal())
            {
            	CMapTerm<K, V>& extended = *(new (ctx) CMapTerm<K, V>(ctx, *this));
            	return extended;
            }

        	CMapTerm<K, V>& extended = *(new (ctx) CMapTerm<K, V>(ctx, *this));
        	return extended;
        }

        void putValue(Context& ctx, K& key, V& value)
        {
            auto search = map.find(&key);
            const bool found = search != map.end();
            if (found)
                search->second->Release();

            map[&key] = &value;

            if (found)
                key.Release();
        }

        Option<V>& getValue(Context& ctx, K& key)
        {
            Optional<Term> ovalue = MapGetValue(ctx, key);
            if (ovalue)
                return newSOME<V>(ctx, dynamic_cast<V&>(ovalue.value()));
            return newNONE<V>(ctx);
        }

        Optional<Term> MapGetValue(Context& ctx, Term& key)
        {
        	Compact(ctx, true);
        	CMapTerm<K, V>* cmap = this;
        	while (true)
        	{
        		auto search = cmap->map.find(&dynamic_cast<K&>(key));
        		if (search == cmap->map.end())
        		{
        			if (cmap->parent)
        				cmap = &cmap->parent.value();
        			else
        				break;
        		}
        		else
        			return make_optional<Term>(tosca::NewRef(*search->second));
        	}
        	return Optional<Term>::nullopt;
        }

        Term& MapPutValue(Context& ctx, Term& key, Term& value)
        {
            tosca::MapTerm<K, V>& xmap = this->extend(ctx);
            xmap.putValue(ctx, dynamic_cast<K&>(key), dynamic_cast<V&>(value));
            return xmap;
        }

        void putAll(Context& ctx, MapTerm<K, V>& map)
        {
        	std::vector<K*> keys = map.keys(ctx);
        	for (auto it = keys.rbegin(); it != keys.rend(); it ++)
        	{
        		K& key = **it;
        		Optional<Term> ovalue = map.MapGetValue(ctx, key);
        		V& value = *(dynamic_cast<V*>(&ovalue.value()));

        		key.AddRef();
        		value.AddRef();
        		putValue(ctx, key, value);
        	}
        }

        std::vector<V*> values(Context& ctx)
        {
            CMapTerm<K, V>* cmap = this;
            std::vector<V*> result;
            while (true)
            {
                for (auto it = cmap->map.begin(); it != cmap->map.end(); it ++)
                {
                    it->second->AddRef();
                    result.push_back(it->second);
                }
                if (!cmap->parent)
                    break;

                cmap = &cmap->parent.value();
            }

        	 return result; // RVO
        }

        std::vector<K*> keys(Context& ctx)
        {
            CMapTerm<K, V>* cmap = this;
            std::vector<K*> result;
            while (true)
            {
                for (auto it = cmap->map.begin(); it != cmap->map.end(); it ++)
                {
                    it->first->AddRef();
                    result.push_back(it->first);
                }
                if (!cmap->parent)
                    break;

                cmap = &cmap->parent.value();
            }
            return result; // RVO
        }

        void MapKeys(std::set<Term*>& keys) const
        {
            const CMapTerm<K, V>* cmap = this;
            while (true)
            {
                for (auto it = cmap->map.begin(); it != cmap->map.end(); it ++)
                {
                    it->first->AddRef();
                    keys.insert(it->first);
                }
                if (!cmap->parent)
                    break;

                cmap = &cmap->parent.value();
            }
        }

        bool isEmpty()
        {
            return map.empty() && ((parent) ? parent.value().isEmpty() : true);
        }

        bool containsKey(K key)
        {
            throw new std::runtime_error("");
        }

        bool contains(Term key)
        {
            throw new std::runtime_error("");
        }

        bool DeepEquals(const Term& rhs, std::unordered_map<Variable*, Variable*>& varmap) const
        {
            // implement equality modulo variable and map.
            return true;
        }

        size_t Hash(size_t code, std::unordered_set<tosca::Variable*>& deBruijn) const
        {
        	return code;
        }

        Term& Substitute(tosca::Context& ctx, std::unordered_map<Variable*, Term*>& substitutes)
        {
            return *this;
        }

        void Print(IOWrapper& out, PrintOptions& options)
        {
            CMapTerm<K, V>* cmap = this;

            out.Write('\n');
            if (options.indent)
                out.Indent(options.count);
            out.Write('{');
            options.count += 2;
            bool first = true;
            while (true)
            {
                for (auto it = cmap->map.begin(); it != cmap->map.end(); it ++)
                {
                    if (!first)
                        out.Write(',');
                    else
                        first = false;
                    out.Write('\n');
                    if (options.indent)
                        out.Indent(options.count);
                    it->first->Print(out, options);
                    out.Write(':');
                    options.count += 2;
                    it->second->Print(out, options);
                    options.count -= 2;
                }
                if (!cmap->parent)
                    break;

                cmap = &cmap->parent.value();
            }
            options.count -= 2;
            if (!isEmpty())
            {
                out.Write('\n');
                if (options.indent)
                    out.Indent(options.count);
            }
            out.Write('}');
        }

		static void* operator new(std::size_t sz, Context& ctx)
		{
			return Allocate(sz, ctx);
		}

		static void operator delete(void* ptr)
		{
			Deallocate(ptr, sizeof(CMapTerm<K, V>));
		}

    protected:
		typedef std::unordered_map<K*, V*, std::hash<K*>, std::equal_to<K*>, Allocator<std::pair<K* const, V*>>> tsmap;

		tsmap map;

        // Extended map
        Optional<CMapTerm> parent;

        CMapTerm() : Ref(true), parent(Optional<CMapTerm>::nullopt)
        {}

        CMapTerm(Context& ctx, CMapTerm& parent): map(10, std::hash<K*>(), std::equal_to<K*>(), Allocator<std::pair<K* const, V*>>(ctx)), parent(make_optional<CMapTerm>(parent))
        {}

        /* Compact linked maps */
        CMapTerm<K, V>& Compact(Context& ctx, bool onlyParent)
        {
        	if (onlyParent && !parent)
        		return *this;
        	if (!onlyParent && this->refcount > 1)
        		return *this;

        	// Search for the oldest map with only one reference
        	std::vector<CMapTerm<K, V>*> candidates;
        	CMapTerm<K, V>* oldest = onlyParent ? &parent.value() : this;
        	while (true)
        	{
        		if (!oldest->parent || oldest->parent.value().refcount > 1)
        			break;

        		candidates.push_back(oldest);
        		oldest = &oldest->parent.value();
        	}

        	// Add all candidates entries to the oldest map
        	for (auto it = candidates.rbegin(); it != candidates.rend(); it ++)
        	{
        		CMapTerm<K, V>* cmap = *it;
        		for (auto it2 = cmap->map.begin(); it2 != cmap->map.end(); it2 ++)
        		{
        			it2->first->AddRef();
        			it2->second->AddRef();
        			oldest->putValue(ctx, *it2->first, *it2->second);
        		}
        	}

        	oldest->AddRef();

        	if (onlyParent)
        	{
            	// Patch parent.
        		parent.value().Release();
        		parent = make_optional<CMapTerm>(*oldest);
        		return *this;
        	}

        	this->Release();
        	return *oldest;
        }

        friend MapTerm<K, V>& newMapTerm<K, V>(Context& ctx);
        static CMapTerm<K, V>& SINGLETON;
    };

    template<typename K, typename V>
    CMapTerm<K, V>& CMapTerm<K, V>::SINGLETON = *(::new CMapTerm<K, V>());

    // Construction
    template<typename K, typename V>
    MapTerm<K, V>& newMapTerm(Context& ctx)
    {
        return CMapTerm<K, V>::SINGLETON;
    }

}

#endif
