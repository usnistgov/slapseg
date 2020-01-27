slapsegiii_log_decoder
--------------------------------------

Decode into plain english the segments and orientation logs outputted from Slapseg III algorithms.

## Requirements

 * Python 2.x

## Arguments

### Required

 * Log filename
    * Example: ` /mnt/slapsegiii/validation/segments-2.log`
    * Example: ` /mnt/slapsegiii/validation/orientation-2.log`

### Optional

 * `--output, -o=[ f | u ]`
    * Format for output results. Defaults to `f` full.
        * `f`: Prints all detected codes in plain language.
        * `u`: Prints unsuccessful segmentation results which are codes that indicate a problem with an image and codes inconsistent with the type of image. 


## Examples

###Print in plain language the two-inch segments log results.

```bash
$ slapsegiii_log_decoder  /mnt/slapsegiii/validation/segments-2.log

00001010_plain_500_14_1584x768.gray
  * Finger 10 failed to segment
00001007_plain_500_13_1564x905.gray
  * Returned successfully and no issues
00001007_plain_500_14_1591x918.gray
  * Finger 10 failed to segment
00001018_plain_1000_13_2910x1892.gray
  * Finger 2 failed to segment
  * Finger 3 failed to segment
  * Finger 4 failed to segment
  * Finger 5 failed to segment
```

###Print in plain language the three-inch orientation log results.   

```bash
$ slapsegiii_log_decoder  /mnt/slapsegiii/validation/orientation-3.log

00002325_R_1000_15_4992x5120.gray
  * Image orientation is thumbs
00002327_R_1000_15_4992x5120.gray
  * Image orientation is right hand - incorrect
00002488_S_500_14_1600x1500.gray
  * Image orientation is right hand - incorrect
00002454_S_500_13_1600x1500.gray
  * Image orientation is right hand
```

###Print in plain language only the images where the orientation value recorded does not agree with the image.        

```bash
$ slapsegiii_log_decoder /mnt/slapsegiii/validation/orientation-3.log -o=u

00002327_R_1000_15_4992x5120.gray
  * Image orientation is right hand - incorrect
00002488_S_500_14_1600x1500.gray
  * Image orientation is right hand - incorrect
00002449_S_500_13_1600x1500.gray
  * Image orientation is left hand - incorrect
```
