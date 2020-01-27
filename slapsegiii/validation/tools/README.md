Validation Tools
================

*These tools are provided for your reference only. Running them is not a requirement for submission to SlapSeg III.*

## Tools

 * [Show Boxes]
   * Visualize the SegmentationPositions returned by your library by 
     superimposing them overtop of the source slap image. It can also overlay
     the groundtruth positions provided by NIST.
 * [Orientation Accuracy]
   * Parse the orientation determination validation logs and print the image
     names that were incorrectly identified for further investigation. 
 * [Log Decoder]
   * Decode into plain language the segments and orientation logs outputted from Slapseg III alogorithms.

## Communication
If you found a bug and can provide steps to reliably reproduce it, or if you
have a feature request, please [open an issue]. Other questions may be addressed
to the [NIST SlapSeg team].

## License
The items in this repository are released in the public domain. See the
[LICENSE] for details.

[Show Boxes]: https://github.com/usnistgov/slapseg/tree/master/slapsegiii/validation/tools/slapsegiii_validation_show_boxes
[Orientation Accuracy]: https://github.com/usnistgov/slapseg/tree/master/slapsegiii/validation/tools/slapsegiii_check_orientation_accuracy
[Log Decoder]: https://github.com/usnistgov/slapseg/tree/master/slapsegiii/validation/tools/slapsegiii_log_decoder
[open an issue]: https://github.com/usnistgov/slapseg/issues
[LICENSE]: https://github.com/usnistgov/slapseg/blob/master/LICENSE.md
[NIST SlapSeg team]: mailto:slapseg@nist.gov
