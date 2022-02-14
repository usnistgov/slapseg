SlapSeg III Validation
======================

We require exercising SlapSeg III [API] implementations with the SlapSeg III
validation package. Validation is mutually-beneficial to NIST and SlapSeg III
participants. The hope is that successful execution of validation ensures your
algorithm:

 * runs as expected at NIST;
 * does not crash;
 * syntactically and semantically implements the [API] correctly.

This helps cut the runtime of the evaluation and gives a higher
level of confidence that the results presented by NIST are a true measure of
the submitted software.

Contents
--------
 * Interaction Required:
   - [lib/]: Directory in which all required libraries reside. There must be at
     least one *core* library, and that library **must** follow the SlapSeg III
     naming convention.
   - [validate]: Script that automates running the validation and performing
     checks on the output.
 * Supporting Files
   - [README.md]: This file.
   - [src/]: Source code to build the validation executable.
   - [VERSION]: Version number of the validation package.

Requirements
------------

 * Fingerprint Imagery
   - Because organizations must agree to NIST Special Database terms and conditions, the required fingerprint imagery is not included in this GitHub repository. Request and download the data from our requests website. Download the data by requesting it from the SlapSeg III test staff and [agreeing to the terms].
 * Ubuntu Server 20.04.03 LTS
   - Even if this is not the latest version of Ubuntu Server, it will be the
     version used to run the evaluation. Direct downloads are available from the
     [Ubuntu Mirrors] ([🇺🇸 USA], [🇪🇺 Europe]) and directly from the [NIST
     Image Group].
   - We **highly suggest** matching the exact versions of packages installed in
     our environment. A link to the names and versions of these pacakages is
     available.
   - The [validate] script  requires these base Ubuntu Server packages:
      - `base-files`, `binutils`, `cmake`, `coreutils`, `curl`, `dpkg`, `file`,
        `findutils`, `g++`, `gawk`, `grep`, `libc-bin`, `make`, `sed`, `tar`,
        `xz-utils`

It is **highly suggested** that you make sure your submission will build and run
as expected on environments as close as possible to the NIST evaluation
machines, in order to avoid validation delays. The CPUs used for SlapSeg III are
**Intel Xeon E5-2680** and **Intel Xeon E5-4650**.

How to Run
----------
 1. Put your compiled core library and any other required libraries in [lib/].
 2. Put SlapSeg III fingerprint imagery received from NIST in this directory
    (i.e., the directory containing this file, [README.md]).
 3. Execute [validate].
 4. **If successful**, sign *and* encrypt the resulting output archive in a
    single step, and e-mail it, along with the encrypting identity's public key,
    to slapseg@nist.gov. If unsuccessful, correct any errors described and try
    again.

<details>
  <summary><em>Expand to view the output from an example run.</em></summary>

```
$ cp /path/to/libslapsegiii_nfseg_5001.so lib/
$ cp /path/to/slapsegiii_validation_images_*.tar.gz .
$ ./validate
================================================================================
|  SlapSeg III Validation | Version 202201261256 | 26 Jan 2022 | 17:58:03 UTC  |
================================================================================
Checking for required packages... [OKAY]
Checking for previous validation attempts... [OKAY]
Checking validation version... (202201261256) [OKAY]
Checking OS and version... (Ubuntu Server 20.04.3 LTS (Focal Fossa)) [OKAY]
Checking for unexpanded validation image tarballs... [DEFER]
Expanding slapsegiii_validation_images_upperpalm-202006111448.tar.gz... [OKAY]
Expanding slapsegiii_validation_images_fullpalm-201902271132.tar.gz... [OKAY]
Expanding slapsegiii_validation_images_twoinch-201902271132.tar.gz... [OKAY]
Expanding slapsegiii_validation_images_threeinch-201902271132.tar.gz... [OKAY]
Checking for unexpanded validation image tarballs... [OKAY]
Checking validation image versions... [OKAY]
Looking for core library... (libslapsegiii_nfseg_5001.so) [OKAY]
Checking for known environment variables... [OKAY]
Building... [OKAY]
Checking API version... [OKAY]
Checking that you have all necessary kinds of images... [OKAY]
Running segmentation... [OKAY]
Checking segmentation logs... [WARN]

================================================================================
|                        failure return values present                         |
================================================================================
Running orientation determination... (not implemented) [SKIP]
Checking orientation logs... (not implemented) [SKIP]
Creating validation submission... (slapsegiii_validation_nfseg_0001.tar.gz) [OKAY]
================================================================================
| You have successfully completed your part of SlapSeg III validation. Please  |
| sign and encrypt the file listed below (run './validate encrypt' for an      |
| example).                                                                    |
|                                                                              |
|                  slapsegiii_validation_nfseg_0001.tar.gz                  |
|                                                                              |
| Please attach both slapsegiii_validation_nfseg_0001.tar.gz.asc and your   |
| public key to an email addressed to slapseg@nist.gov.                        |
================================================================================

Wed 26 Jan 2022 06:04:32 PM UTC
```
</details>

Submission Contents
-------------------
 * canary.log: MD5 checksums of randomly-generated images we provide as part of
   the validation fingerprint imagery. This helps us make sure that you are
   using the most recent version of SlapSeg III validation fingerprint imagery.
 * compile.log: Output from compiling the validation executable and other
   information like MD5 checksums, versions, and library dependencies that may
   help us debug your submission if an error occurs.
    - This file contains absolute paths to files on your system. If this
      information is sensitive, you may redact the paths only.
 * id.log: Information about the linked core library, including supported slap
   sizes. This is information derived from values compiled into the core library
   or passed to [validate].
 * lib: A copy of [lib/].
 * run-orientation.log: The command used to launch the validation executable
   for orientation determination.
 * run-segments.log: The command used to launch the validation executable for
   segmentation
 * orientation-#.log: The output from the validation executable containing
   hypothesized orientations for each slap image provided.
 * segments-#.log: The output from the validation executable containing
   segmentation positions for each slap image provided.

Checks Performed
----------------

 * Implementation can handle being `fork`ed.
 * Validation package and imagery is at most recent revision level.
 * Appropriate operating system version installed.
 * Appropriately named SlapSeg III core software library is present.
 * Software library links properly against the validation driver.
 * Crashes do not occur when handling challenging images, such as:
   - missing fingers;
   - touching fingers;
   - blank or gradient patterns.
 * Segmentation positions are within the image for non-rotated slaps.
 * Segmentation positions are rectangular.
 * Segmentation positions are not rotated when not permitted.

While the validation package tries to eliminate errors from the SlapSeg III
submission, it is not perfect, and there are still several ways in which the
package might approve you for submission that NIST may later reject.

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
[Ubuntu Mirrors]: https://launchpad.net/ubuntu/+cdmirrors
[🇺🇸 USA]: https://mirror.math.princeton.edu/pub/ubuntu-iso/focal/ubuntu-20.04.3-live-server-amd64.iso
[🇪🇺 Europe]: http://mirror.init7.net/ubuntu-releases/focal/ubuntu-20.04.3-live-server-amd64.iso
[NIST Image Group]: https://nigos.nist.gov/evaluations/ubuntu-20.04.3-live-server-amd64.iso
[lib/]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/lib
[bin/]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/bin
[README.md]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/README.md
[src/]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/src
[VERSION]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/VERSION
[validate]: https://github.com/usnistgov/slapseg/blob/master/slapsegiii/validation/validate
[NIST SlapSeg team]: mailto:slapseg@nist.gov
[open an issue]: https://github.com/usnistgov/slapseg/issues
[mailing list site]: https://groups.google.com/a/list.nist.gov/forum/#!forum/slapseg/join
[LICENSE]: https://github.com/usnistgov/slapseg/blob/master/LICENSE.md
[test plan]: https://pages.nist.gov/slapseg/doc/slapsegiii/testplan.pdf
[agreeing to the terms]: https://nigos.nist.gov/datasets/slapsegiii_validation/request
