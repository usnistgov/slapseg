slapsegiii_check_orientation_accuracy
--------------------------------------

Scan participant validation orientation logs to verify finger position and orientation value match.

## Requirements

 * Python 2.x

## Arguments

### Required

 * Full path to validation directory
    * Example: ` /mnt/slapsegiii/validation/`

### Optional

 * `--kind`
   * A number representing the type of image log to validate. Defaults to all (`2 3 5 8`).
   * Example `--kind 2` or `--kind 2 3 5`

 * `--output=[stats | incorrect]`
    * Format for output results. Defaults to `incorrect`.
        * `stats`: Prints number and percent correct, incorrect and skipped in a log.
        * `incorrect`: Prints each incorrect filename, the actual type of image and the incorrect type listed in the log.


## Examples

 * Validate two-inch and three-inch orientation logs. Print number and percent correct, incorrect and skipped in a log.
   ```bash
   $ slapsegiii_check_orientation_accuracy /mnt/slapsegiii/validation/ --kind 2 3 --output=stats
   
   Two Inch /mnt/slapsegiii/validation/validation_Participant_0001/orientation-2.log:
    * Right:
     * Correct: 39 (56.5%)
     * Incorrect: 30 (43.5%)
     * Skip: 0 (0.0%)
    * Left:
     * Correct: 24 (48.0%)
     * Incorrect: 26 (52.0%)
     * Skip: 0 (0.0%)  
   Three Inch /mnt/slapsegiii/validation/validation_Participant_0001/orientation-3.log:
    * Right:
     * Correct: 20 (40.0%)
     * Incorrect: 30 (60.0%)
     * Skip: 0 (0.0%)
    * Left:
     * Correct: 21 (33.9%)
     * Incorrect: 41 (66.1%)
     * Skip: 0 (0.0%)
    * Thumbs:
     * Correct: 15 (30.0%)
     * Incorrect: 35 (70.0%)
     * Skip: 0 (0.0%)  
   ```
 * Validate three-inch orientation log. Defaults to print incorrect items only.
   ```bash
   $ slapsegiii_check_orientation_accuracy /mnt/slapsegiii/validation/ --kind 3
   
   Three Inch (/mnt/slapsegiii/validation/validation_Participant_0001/orientation-3.log):
     * 00009440_R_1000_13_4992x5120.gray (Actual: Right, Paricipant: Thumbs)
     * 00009440_R_1000_14_4992x5120.gray (Actual: Left, Paricipant: Thumbs)
   ```
 * Validate two-inch, three-inch, upperpalm and fullpalm orientation logs. Defaults to print incorrect items in the log.
   ```bash
   $ slapsegiii_check_orientation_accuracy /mnt/slapsegiii/validation/
   
   Two Inch (/mnt/slapsegiii/validation/validation_Participant_0001/orientation-2.log):
     * 0 items incorrect.
   
   Three Inch (/mnt/slapsegiii/validation/validation_Participant_0001/orientation-3.log):
     * 00009440_R_1000_13_4992x5120.gray (Actual: Right, Paricipant: Thumbs)
     * 00009440_R_1000_14_4992x5120.gray (Actual: Left, Paricipant: Thumbs)
     * 00009442_R_1000_13_4992x5120.gray (Actual: Right, Paricipant: Thumbs)
   
   UpperPalm (/mnt/slapsegiii/validation/validation_Participant_0001/orientation-5.log):
     * 0 items incorrect.
   
   FullPalm (/mnt/slapsegiii/validation/validation_Participant_0001/orientation-8.log):
     * 00009440_R_1000_13_4992x5120.gray (Actual: Right, Paricipant: Thumbs)
     * 00009440_R_1000_14_4992x5120.gray (Actual: Left, Paricipant: Thumbs)
     * 00009442_R_1000_13_4992x5120.gray (Actual: Right, Paricipant: Thumbs)
   ```
