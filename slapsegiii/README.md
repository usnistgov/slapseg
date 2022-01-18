SlapSeg III
===========

Slap Fingerprint Segmentation Evaluation (SlapSeg) III is a public test of
automated slap fingerprint segmentation algorithms. In SlapSeg III, output from
automated algorithms is compared to output from a human examiner and analyzed
for similarity. Algorithms are additionally encouraged to note deficiencies in
images tenprint card scans, identification flats, upper palm, and full palm
captures.

Contents
--------
 * [Validation]
   - Package used to verify and submit SlapSeg III libraries for testing at
     NIST.
- [`libslapsegiii`]
   - Library containing required implementations of methods from the
     [SlapSeg III API]. Submitted libraries must link against **but not
     include** this library
- [nullimpl]
   - A stub library compliant with the [SlapSeg III API] that can be used as a
     starting point for development.

Communication
-------------
If you found a bug and can provide steps to reliably reproduce it, or if you
have a feature request, please [open an issue]. Other questions may be addressed
to the [NIST SlapSeg team](mailto:slapseg@nist.gov).

The SlapSeg team sends updates about the SlapSeg tests to their mailing list.
Enter your e-mail address on the [mailing list site], or send a blank e-mail to
SlapSeg+subscribe@list.nist.gov to be automatically subscribed.

License
-------
The items in this repository are released in the public domain. See the
[LICENSE] for details.

[Validation]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/
[`libslapsegiii`]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/src/libslapsegiii
[nullimpl]: https://github.com/usnistgov/slapseg/tree/master/slapsegiii/nullimpl
[SlapSeg III API]: https://pages.nist.gov/slapseg/doc/slapsegiii/api
[open an issue]: https://github.com/usnistgov/slapseg/issues
[mailing list site]: https://groups.google.com/a/list.nist.gov/forum/#!forum/slapseg/join
[LICENSE]: https://github.com/usnistgov/slapseg/blob/master/LICENSE.md
