/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 Paul Guenette

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

// Built with -nostdinc++ and ETL_NO_STL: no C++ standard header exists to include.

#include "etl/memory.h"
#include "etl/optional.h"

namespace
{
  struct Pod
  {
    int value;
  };

  struct NoDefaultConstructor
  {
    constexpr explicit NoDefaultConstructor(int value_)
      : value(value_)
    {
    }

    int value;
  };

  struct NonTrivialDestructor
  {
    constexpr explicit NonTrivialDestructor(int value_)
      : value(value_)
    {
    }

    constexpr ~NonTrivialDestructor() {}

    int value;
  };

  //***************************************************************************
  constexpr etl::optional<Pod> pod_engaged{Pod{3}};
  constexpr etl::optional<Pod> pod_empty{};
  static_assert(pod_engaged.has_value() && (pod_engaged.value().value == 3));
  static_assert(!pod_empty.has_value());

  //***************************************************************************
  constexpr etl::optional<NoDefaultConstructor> no_default_engaged{NoDefaultConstructor{4}};
  constexpr etl::optional<NoDefaultConstructor> no_default_empty{};
  static_assert(no_default_engaged.value().value == 4);
  static_assert(!no_default_empty.has_value());

  //***************************************************************************
  constexpr int reset_then_assign()
  {
    etl::optional<NonTrivialDestructor> opt{NonTrivialDestructor{1}};
    opt.reset();
    const bool was_reset = !opt.has_value();
    opt                  = NonTrivialDestructor{5};

    return was_reset ? opt.value().value : -1;
  }
  static_assert(reset_then_assign() == 5);

  //***************************************************************************
  constexpr int emplace_twice()
  {
    etl::optional<NoDefaultConstructor> opt{};
    opt.emplace(1);
    opt.emplace(6);

    return opt.value().value;
  }
  static_assert(emplace_twice() == 6);
} // namespace

//*****************************************************************************
int main()
{
  etl::optional<NonTrivialDestructor> opt{NonTrivialDestructor{7}};

  if (!opt.has_value() || (opt.value().value != 7))
  {
    return 1;
  }

  opt.reset();

  return opt.has_value() ? 2 : 0;
}
