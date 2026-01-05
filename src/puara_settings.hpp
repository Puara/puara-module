#pragma once
#include <puara_utils.hpp>

#include <reflect>
#include <string>
#include <string_view>

namespace PuaraAPI
{
// Takes an arbitrary struct and returns a corresponding JSON
static inline std::string as_json(const auto& f)
{
  int sz = 2;
  int count = 0;

  // 1. Compute how much space we need for our json
  reflect::for_each([&sz, &count, &f](auto I) {
    using member_type = std::decay_t<decltype(reflect::get<I>(f))>;

    sz += 3; // "":
    sz += reflect::member_name<I>(f).size();
    if constexpr(std::is_same_v<member_type, bool>)
    {
      sz += 5;
    }
    else if constexpr(std::is_integral_v<member_type>)
    {
      sz += 8;
    }
    else if constexpr(requires { std::string_view{reflect::get<I>(f)}; })
    {
      sz += std::string_view{reflect::get<I>(f)}.size();
      sz += 2;
    }
    sz += 1; // ,
    count++;
  }, f);

  // 2. Write our json to a string
  std::string res;
  res.reserve(sz);
  res += '{';
  reflect::for_each([&count, &res, &f](auto I) {
    using member_type = std::decay_t<decltype(reflect::get<I>(f))>;

    res += '"';
    res += reflect::member_name<I>(f);
    res += "\":";

    // 2. Append the value
    if constexpr(std::is_same_v<member_type, bool>)
    {
      res += (reflect::get<I>(f) ? "true" : "false");
    }
    else if constexpr(std::is_integral_v<member_type>) 
    {
      res += std::to_string(reflect::get<I>(f));
    } 
    else if constexpr(requires { std::string_view{reflect::get<I>(f)}; }) 
    {
      res += '"';
      res += reflect::get<I>(f);
      res += '"';
    }
    else
    {
      static_assert(I.is_not_a_supported_type);
    }

    if(--count > 0)
    {
      res += ',';
    }
  }, f);
  res += '}';
  return res;
}

// Matches a HTTP request list of arguments to a structure and update it
static inline void settings_from_http_request(auto& settings, std::string_view request)
{
  static constexpr std::string_view delimiter = "&";
  static constexpr std::string_view field_delimiter = "=";

  size_t pos = 0;

  // Iterate over the request string view
  while(!request.empty())
  {
    // 1. Extract the current "key=value" token
    pos = request.find(delimiter);
    std::string_view token
        = (pos == std::string_view::npos) ? request : request.substr(0, pos);

    // Advance the view for the next iteration
    if(pos != std::string_view::npos)
    {
      request.remove_prefix(pos + delimiter.size());
    }
    else
    {
      request = {}; // Reached the end
    }

    // 2. Parse Key and Value
    size_t field_pos = token.find(field_delimiter);
    if(field_pos != std::string_view::npos)
    {
      std::string_view key = token.substr(0, field_pos);
      std::string_view raw_value = token.substr(field_pos + field_delimiter.size());

      // Decode URL-encoded value (converting view to string for the decoder)
      std::string value = urlDecode(std::string(raw_value));

      // 3. Reflect over struct members to find the match
      bool found = false;
      reflect::for_each([&](auto I) {
        // Optimization: stop checking if we already found the member
        if(found)
          return;

        // Check if struct member name matches the HTTP key
        if(reflect::member_name<I>(settings) == key)
        {
          using member_type = std::decay_t<decltype(reflect::get<I>(settings))>;
          auto& member = reflect::get<I>(settings);

          // 4. Assign value based on type
          if constexpr(std::is_same_v<member_type, bool>)
          {
            // Handle "true", "1", "on" vs everything else
            member = (value == "true" || value == "1" || value == "on");
          }
          else if constexpr(std::is_integral_v<member_type>)
          {
            if(!value.empty())
            {
              try
              {
                if constexpr(std::is_signed_v<member_type>)
                  member = static_cast<member_type>(std::stoll(value));
                else
                  member = static_cast<member_type>(std::stoull(value));
              }
              catch(...)
              { /* Handle parsing error or keep default */
              }
            }
          }
          else if constexpr(std::is_floating_point_v<member_type>)
          {
            if(!value.empty())
            {
              try
              {
                member = static_cast<member_type>(std::stod(value));
              }
              catch(...)
              {
              }
            }
          }
          else if constexpr(requires {
                              member = std::string{};
                            }) // Check if assignable from string
          {
            member = value;
          }

          found = true;
        }
      }, settings);
    }
  }
}
}