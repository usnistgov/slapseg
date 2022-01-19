slapsegiii_validation_show_boxes
--------------------------------

Visualize participant and/or ground truth segmentation positions using
ImageMagick.

## Requirements

 * Perl 5
 * perl-modules-5.30
 * ImageMagick

Under Ubuntu 20.04, install all required packages with:

```bash
apt install perl perl-base perl-modules-5.30 imagemagick
```

## Arguments

### Required

 *  `--img_name`
    * Image file whose segmentation positions are to be visualized.
    * Example: *`--img_name 00001000_plain_1000_13_3196x1890.gray`*
 * `--dir_path`
    * Path to the root of the validation directory. This is the directory
      containing the directory `output`.

### Optional

 * `--size ##%`
    * Resize percentage to pass to ImageMagick. Defaults to **`40%`**.
    * Example: *`--size 25%`*
 * `--view [b | n | p]`
    * Customize which segmentation positions to draw. Defaults to **`b`**.
        * b: Show both NIST ground truth and participant segmentation
          positions.
        * n: Show only NIST segmentation positions.
        * p: Show only participant segmentation positions
 * `--save <path/to/image.png>`
    * Save the image as a PNG to the specified location. Image will not be
      displayed if this option is provided.

## Examples

### View segmentation positions for `00001000_plain_1000_13_3196x1890.gray`

```bash
slapsegiii_validation_show_boxes --img_name \
    00001000_plain_1000_13_3196x1890.gray   \
    --dir_path ~/slapsegiii/validation --view b
```

### Save participant segmentation positions for all TwoInch captures

```bash
validation_dir=~/slapsegiii/validation
output_dir=${validation_dir}/preview
mkdir -p ${output_dir}

filelist=$(mktemp)
for line in $(tail -n +2 ${validation_dir}/output/segments-2.log); do
    cut -f 1 -d ',' <<< ${line} >> ${filelist}
done

for name in $(sort ${filelist} | uniq); do
    echo "Making image for ${name}..."
    slapsegiii_validation_show_boxes --img_name ${name} \
        --dir_path ${validation_dir} -view p \
        --save ${output_dir}/$(basename ${name} gray)png
done
rm ${filelist}
```

