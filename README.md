## Requirements

Requires GCC 14 or newer


## Makefile

**Main targets:**
* `make pi PRECISION=100` — run sample program calculating 100 places of pi
* `make test` — test the library

## Usage

### General info

The library introduces a `LongNum` class that implements fixed-point arithmetic (fractional part has fixed size,
total number size is unlimited) with selectable binary precision.

### Creating an instance

**It is possible to create a `LongNum` instance in different ways:**

* From `long long` or `unsigned long long` (then precision will be set to `0`):
```c++
LongNum x(25LL);
LongNum y = 25;
```

* From integer or double literal using `_longnum` suffix:
```c++
-125_longnum  // precision = 0 (integer)
3.1415_longnum  // precision is set automatically
```


### Controlling the precision
Since numbers are stored in binary, precision is specified as the number of **binary** places after the point.

There are two methods to control the precision of numbers:
* `LongNum::setPrecision(precision)` sets the specified precision
* `LongNum::withPrecision(precision)` returns a copy of `LongNum` instance with the specified precision

When decreasing the precision, some of the places that no longer fit are simply cut off.

Example:
```c++
// 0.125 = 0.001 (binary)

std::cout << (0.125_longnum).withPrecision(3) << '\n';
// >>> 0.125

std::cout << (0.125_longnum).withPrecision(2) << '\n';  // 0.001 -> 0.00
// >>> 0
```

### Relational operators
Relational operators `==`, `!=`, `>`, `<`  are overloaded for the `LongNum` class.

### Mathematical operators
Operators `+`, `-`, `*`, `/`, `<<`, `>>` as well as operators `+=`, `-=`, `*=`, `/=` are overloaded for the `LongNum`
class just like you would expect them to.  
This makes operations with its instances the same as with built-in numeric types.

> [!IMPORTANT]  
> The precision of the result of binary arithmetic operation is set to the **maximum** of operands' precision.

> [!NOTE]  
> To improve performance due to implementation details it is recommended to choose precision to be a multiple of **32**
> and, if possible, perform `+` and `-` operations on numbers of the same precision.

Some other mathematical functions:
* `LongNum::pow(x)` returns a number raised to the power `x`
* `LongNum::sqrt()` calculates square root of a number
* `LongNum::abs()` returns the absolute value of a number


