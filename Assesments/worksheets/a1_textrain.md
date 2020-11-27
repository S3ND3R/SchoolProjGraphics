# Assignment 1 (Text Rain) Worksheet

For the conceptual worksheets in this course, we'll provide a Markdown
template from the shared-upstream repository. As described in the Canvas
assignment handouts, you'll pull each Markdown template into your repository,
directly edit your local copy with your answers, and commit and push your
answers to GitHub in your `worksheets` folder of your repository. If you're
unfamiliar with Markdown syntax, [check out this lovely guide provided by
GitHub](https://guides.github.com/features/mastering-markdown/) before you get
started.

## Notes

* _Do not make a copy of the provided Markdown template and submit that instead._
Our grading scripts will be looking for these modified files within your
`worksheets` folder of your repository. Do not change the filenames, simply
modify the contents.
* This worksheet is intended to help ease you into the world of graphics
programming and is meant to be a "warmup." Try not to overthink! Just because
an answer is simple does not mean that it is wrong.

## Background

By default, Processing uses the integer-based `0-255` convention to represent
colors. For instance, bright, full-saturation red is represented as
`color(255, 0, 0)`.  Processing also supports grayscale colors; black is
`color(0)` and white is `color(255)`. You may wish to look at the [color class
documentation](https://processing.org/reference/color_.html) and/or the
[tutorial explaining color in
Processing](https://processing.org/tutorials/color/).


## Q1: Indexing

As mentioned in the assignment handout, accessing/setting pixel data via
Processing's `get()` and `set()` routines is a bit easier to code, but it's
much slower than directly accessing/changing a [PImage
object's](https://processing.org/reference/PImage.html) 1D `pixels[]` array.
Processing stores the pixels of a 2D image in this 1D array; this can be
thought of as "flattening" the image. Because of this flattening, getting the
proper pixel value requires a little additional math.

In the code block below, write the equation for calculating the 1D index of a
pixel from its 2D (row, column) index. Keep in mind you can use information
from `inputImg` to help you.

```
PImage inputImg = loadImage("test.jpg");

int index1D = row * inputImg.width + column;
```


## Q2: Thresholding

The image processing technique known as *thresholding* will be useful while
creating your Text Rain. During the thresholding operation, if a pixel's
grayscale value is less than `threshold`, then it becomes black. If the
value is greater than `threshold`, it becomes white. You can use the green
channel of the color as the grayscale value.

In the code block below, write a Java code snippet for thresholding one pixel
(`inputPixel`) to black or white.

```java
final int threshold = 128;

// Returns: thresholded color (black or white)
color thresholdPixel(color inputPixel) {
  color black = color(0);
  color white = color(255);
  float gVal = green(inputPixel);
  if (gVal > threshold) {
    return white;
  }
  return black;
}
```
