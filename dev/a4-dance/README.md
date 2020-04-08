#CSci-4611 Assignment 4: So You Think Ants Can Dance

- Warren Weber
- weber767
- Spring Semester 2020

## Description of C level implementation
### Goal: Correctly set the “child_root_transform” before the recursive call at the end of the function in order to draw all of the bones in the skeleton.

   - Step 1: The ctm matrix was set to a composition of several matrix transformations to correctly move the matrix to the location of the end of the current bone in the
  world space.

   - Step 2: Before recursive calls were made to DrawBoneRecursive(), the ctm was translated to the bone space of the child of the current bone by multiplying ctm by the matrix returned from the BoneSpaceToChildrenSpace() function.

## Description of B level implementation
### Goal: Create good motion clips for the ballet-dancing ant inside dance_app.cc

- Step 1: Following the implementation already provided in the code for ballet_special1_ the  dance_app.cc was extended to add the additional 4 movements.  The moves used were from the .acm files that were in the data folder. They were trimmed in either the front or back to cutout unnecessary poses from the motion. This was done using the two functions TrimFront() and TrimBack(). Finally a call was made to CalcRelativeTranslations() since the ballet dancer could as a result of the movement end in a new location, making it necessary for the position of the base movement to be recalculated.

- Step 2: In the functions that handled the button press event a call was made to OverlayClip() to interpolate between the base motion and the motion that was passed into the OverlayClip() function call.

## Description of A level implementation
### Goal: Return to the DrawBonesRecursive() function and extend it to draw an ant.

- Drawing the bones was handled in the large if block that drew each bone based on the name of the bone that had been passed. The head, hips, eyes, and antenna were rotated, scaled, and translated so that they looked visually appealing. Composition of shapes were used for some of the body parts.
