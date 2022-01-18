SlapSeg III Null Implementation
===============================

This directory contains example code that could be used as a starting point for
a SlapSeg III [API] implementation. The code never returns any segmentation
failures, and always returns the same hard-coded segmentation positions.

Building
--------
```
mkdir build && cd build
cmake .. && make
```

The CMake configuration extracts the name and version number for the library to
be built from within the C++ source.

Communication
-------------
If you found a bug and can provide steps to reliably reproduce it, or if you
have a feature request, please [open an issue]. Other questions may be addressed
to the [NIST SlapSeg team].

The SlapSeg team sends updates about the SlapSeg tests to their mailing list.
Enter your e-mail address on the [mailing list site], or send a blank e-mail to
SlapSeg+subscribe@list.nist.gov to be automatically subscribed.

License
-------
The items in this repository are released in the public domain. See the
[LICENSE] for details.

[API]: https://pages.nist.gov/slapseg/doc/slapsegiii/api/
[`libslapsegiii`]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/src/libslapsegiii
[NIST SlapSeg team]: mailto:slapseg@nist.gov
[open an issue]: https://github.com/usnistgov/slapseg/issues
[mailing list site]: https://groups.google.com/a/list.nist.gov/forum/#!forum/slapseg/join
[LICENSE]: https://github.com/usnistgov/slapseg/blob/master/LICENSE.md
