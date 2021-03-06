### jsoncons::basic_json_encoder

```c++
#include <jsoncons/json_encoder.hpp>

template<
    class CharT,
    class Sink>
> basic_json_encoder : public jsoncons::basic_json_visitor<CharT>

template<
    class CharT,
    class Sink>
> basic_json_compressed_encoder : public jsoncons::basic_json_visitor<CharT>
```

`basic_json_encoder` and `basic_json_compressed_encoder` are noncopyable and nonmoveable.

![basic_json_encoder](./diagrams/basic_json_encoder.png)

Four specializations for common character types and sink types are defined
for both the pretty print and compressed serializers:

Type                       |Definition
---------------------------|------------------------------
json_stream_encoder            |basic_json_encoder<char,jsoncons::stream_sink<char>>
json_string_encoder     |basic_json_encoder<char,jsoncons::string_sink<std::string>>
wjson_stream_encoder           |basic_json_encoder<wchar_t,jsoncons::stream_sink<wchar_t>>
wjson_string_encoder    |basic_json_encoder<wchar_t,jsoncons::string_sink<std::wstring>>
json_compressed_stream_encoder            |basic_json_compressed_encoder<char,jsoncons::stream_sink<char>>
json_compressed_string_encoder     |basic_json_compressed_encoder<char,jsoncons::string_sink<std::string>>
wjson_compressed_stream_encoder           |basic_json_compressed_encoder<wchar_t,jsoncons::stream_sink<wchar_t>>
wjson_compressed_string_encoder    |basic_json_compressed_encoder<wchar_t,jsoncons::string_sink<std::wstring>>

#### Member types

Type                       |Definition
---------------------------|------------------------------
char_type                  |CharT
sink_type           |Sink
string_view_type           |

#### Constructors

    explicit basic_json_encoder(Sink&& sink)
Constructs a new encoder that is associated with the destination `sink`.

    basic_json_encoder(Sink&& sink, 
                       const basic_json_options<CharT>& options)
Constructs a new encoder that is associated with the destination `sink` 
and uses the specified [json options](basic_json_options.md). 

#### Destructor

    virtual ~basic_json_encoder() noexcept

#### Inherited from [basic_json_visitor](../basic_json_visitor.md)

#### Member functions

    bool begin_object(semantic_tag tag=semantic_tag::none,
                      const ser_context& context=ser_context()); 

    bool begin_object(std::size_t length, 
                      semantic_tag tag=semantic_tag::none,
                      const ser_context& context=ser_context()); 

    bool end_object(const ser_context& context = ser_context())

    bool end_object(const ser_context& context, std::errr_code& ec)

    bool begin_array(semantic_tag tag=semantic_tag::none,
                     const ser_context& context=ser_context()); 

    bool begin_array(semantic_tag tag=semantic_tag::none,
                     const ser_context& context=ser_context()); 

    bool end_array(const ser_context& context=ser_context()); 

    bool key(const string_view_type& name, 
              const ser_context& context=ser_context()); 

    bool string_value(const string_view_type& value, 
                      semantic_tag tag = semantic_tag::none, 
                      const ser_context& context=ser_context());

    bool byte_string_value(const byte_string_view& b, 
                           semantic_tag tag=semantic_tag::none, 
                           const ser_context& context=ser_context()); 

    bool byte_string_value(const uint8_t* p, std::size_t size, 
                           semantic_tag tag=semantic_tag::none, 
                           const ser_context& context=ser_context()); 

    bool int64_value(int64_t value, 
                     semantic_tag tag = semantic_tag::none, 
                     const ser_context& context=ser_context());

    bool uint64_value(uint64_t value, 
                      semantic_tag tag = semantic_tag::none, 
                      const ser_context& context=ser_context()); 

    bool half_value(uint16_t value, 
                    semantic_tag tag = semantic_tag::none, 
                    const ser_context& context=ser_context()); 

    bool double_value(double value, 
                      semantic_tag tag = semantic_tag::none, 
                      const ser_context& context=ser_context()); 

    bool bool_value(bool value, 
                    semantic_tag tag = semantic_tag::none,
                    const ser_context& context=ser_context());  

    bool null_value(semantic_tag tag = semantic_tag::none,
                    const ser_context& context=ser_context());  

    void flush()

### Examples

### Feeding json events directly to a `json_stream_encoder`
```c++
#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <jsoncons/json_encoder.hpp>

using namespace jsoncons;
using boost::numeric::ublas::matrix;

int main()
{
    matrix<double> A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;

    json_options options;
    json_stream_encoder os(std::cout, options); 
    os.begin_array();
    for (std::size_t i = 0; i < A.size1(); ++i)
    {
        os.begin_array();
        for (std::size_t j = 0; j < A.size2(); ++j)
        {
            os.double_value(A(i, j));
        }
        os.end_array();
    }
    os.end_array();

    return 0;
}
```

Output:

```json
[
    [1,2],
    [3,4]
]
```
