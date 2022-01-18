`libslapsegiii`
===============

`libslapsegiii` is a shared library containing implementations for methods
defined in [`slapsegiii.h`]. *Core* SlapSeg III libraries must link against
`libslapsegiii`.

NIST will build a clean copy of `libslapsegiii` and link it against the main
evaluation executable along with the SlapSeg III core library. This is to ensure
that the implementations of `libslapsegiii` are equivalent for all participants,
and is additionally the reason why these methods were not implemented directly
in [`slapsegiii.h`].

Building
--------
```
mkdir build && cd build
cmake .. && make
```

Linking
-------
When building a core SlapSeg III library, use these example compiler flags (from
`g++`/`ld`) to properly link against this library.

> `-L/path/to/libslapsegiii -lslapsegiii -Wl,-rpath,/path/to/libslapsegiii -Wl,--enable-new-dtags`

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

[`slapsegiii.h`]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/src/slapsegiii.h
[validation]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation
[NIST SlapSeg team]: mailto:slapseg@nist.gov
[open an issue]: https://github.com/usnistgov/slapseg/issues
[mailing list site]: https://groups.google.com/a/list.nist.gov/forum/#!forum/slapseg/join
[LICENSE]: https://github.com/usnistgov/slapseg/blob/master/LICENSE.md
