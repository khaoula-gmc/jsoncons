// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://sourceforge.net/projects/jsoncons/files/ for latest version
// See https://sourceforge.net/p/jsoncons/wiki/Home/ for documentation.

#ifndef JSONCONS_JSON_DESERIALIZER_HPP
#define JSONCONS_JSON_DESERIALIZER_HPP

#include <string>
#include <sstream>
#include <vector>
#include <istream>
#include <cstdlib>
#include <memory>
#include "jsoncons/json_char_traits.hpp"
#include "jsoncons/json1.hpp"
#include "jsoncons/json_input_handler.hpp"
#include "jsoncons/json_structures.hpp"

namespace jsoncons {

template <class C>
class basic_json_deserializer : public basic_json_input_handler<C>
{
    typedef std::pair<std::basic_string<C>,basic_json<C>> member_type;

    struct stack_item
    {
        stack_item(bool is_object, size_t minimum_structure_capacity)
            : is_object_(is_object)
        {
            minimum_structure_capacity_ = minimum_structure_capacity;
            if (is_object_)
            {
                object_ = new json_object<C>();
                object_->reserve(minimum_structure_capacity);
            }
            else
            {
                array_ = new json_array<C>();
                array_->reserve(minimum_structure_capacity);
            }
        }

        void destroy()
        {
            try
            {
                if (is_object_)
                {
                    delete object_;
                }
                else
                {
                    delete array_;
                }
            }
            catch (...)
            {
                // no throw
            }
        }

        bool is_object() const
        {
            return is_object_;
        }
        bool is_array() const
        {
            return !is_object_;
        }

        json_object<C>* release_object() {json_object<C>* p(0); std::swap(p,object_); return p;}

        json_array<C>* release_array() {json_array<C>* p(0); std::swap(p,array_); return p;}

        std::basic_string<C> name_;
        bool is_object_;
        json_object<C>* object_;
        json_array<C>* array_;
        size_t minimum_structure_capacity_;
    };

public:
    ~basic_json_deserializer()
    {
        for (size_t i = 0; i < stack_.size(); ++i)
        {
            stack_[i].destroy();
        }
    }

    virtual void begin_json()
    {
    }

    virtual void end_json()
    {
    }

    virtual void begin_object(const basic_parsing_context<C>& context)
    {
        stack_.push_back(stack_item(true,context.minimum_structure_capacity()));
    }

    virtual void end_object(const basic_parsing_context<C>& context)
    {
        stack_.back().object_->sort_members();
        basic_json<C> val(stack_.back().release_object());	    
        stack_.pop_back();
        if (stack_.size() > 0)
        {
            if (stack_.back().is_object())
            {
                stack_.back().object_->push_back(JSONCONS_MOVE(stack_.back().name_),JSONCONS_MOVE(val));
            }
            else
            {
                stack_.back().array_->push_back(JSONCONS_MOVE(val));
            }
        }
        else
        {
            root_.swap(val);
        }
    }

    virtual void begin_array(const basic_parsing_context<C>& context)
    {
        stack_.push_back(stack_item(false,context.minimum_structure_capacity()));
    }

    virtual void end_array(const basic_parsing_context<C>& context)
    {
        basic_json<C> val(stack_.back().release_array());	    
        stack_.pop_back();
        if (stack_.size() > 0)
        {
            if (stack_.back().is_object())
            {
                stack_.back().object_->push_back(JSONCONS_MOVE(stack_.back().name_),JSONCONS_MOVE(val));
            }
            else
            {
                stack_.back().array_->push_back(JSONCONS_MOVE(val));
            }
        }
        else
        {
            root_ = JSONCONS_MOVE(val);
        }
    }

    virtual void name(const std::basic_string<C>& name, const basic_parsing_context<C>& context)
    {
        stack_.back().name_ = name;
    }

    virtual void value(const std::basic_string<C>& value, const basic_parsing_context<C>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().object_->push_back(JSONCONS_MOVE(stack_.back().name_),basic_json<C>(value));
        } 
        else 
        {
            stack_.back().array_->push_back(JSONCONS_MOVE(basic_json<C>(value)));
        }
    }

    virtual void value(double value, const basic_parsing_context<C>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().object_->push_back(JSONCONS_MOVE(stack_.back().name_),basic_json<C>(value));
        } 
        else
        {
            stack_.back().array_->push_back(basic_json<C>(value));
        }
    }

    virtual void value(long long value, const basic_parsing_context<C>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().object_->push_back(JSONCONS_MOVE(stack_.back().name_),basic_json<C>(value));
        } 
        else
        {
            stack_.back().array_->push_back(basic_json<C>(value));
        }
    }

    virtual void value(unsigned long long value, const basic_parsing_context<C>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().object_->push_back(JSONCONS_MOVE(stack_.back().name_),basic_json<C>(value));
        } 
        else
        {
            stack_.back().array_->push_back(basic_json<C>(value));
        }
    }

    virtual void value(bool value, const basic_parsing_context<C>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().object_->push_back(JSONCONS_MOVE(stack_.back().name_),basic_json<C>(value));
        } 
        else
        {
            stack_.back().array_->push_back(basic_json<C>(value));
        }
    }

    virtual void null_value(const basic_parsing_context<C>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().object_->push_back(JSONCONS_MOVE(stack_.back().name_),basic_json<C>(basic_json<C>::null));
        } 
        else
        {
            stack_.back().array_->push_back(basic_json<C>::null);
        }
    }

    basic_json<C>& root()
    {
        return root_;
    }

private:
	basic_json<C> root_;
    std::vector<stack_item> stack_;
};

typedef basic_json_deserializer<char> json_deserializer;

}

#endif
