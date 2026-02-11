# Decompression-Srl
RLE and LZSS decompression functions

These functions are required for the Ponesound-SRL module and an updated TMSF module.

## Use
Copy modules_extra to your SRL folder

To include a module in your project, add this line to your project’s Makefile:
```
MODULES_EXTRA = decompression
```
Include it in your code:
```
#include <decompression.hpp>
using namespace SRL::Decompression;
```
