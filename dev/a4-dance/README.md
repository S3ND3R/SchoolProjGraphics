CSci-4611 Assignment 4: So You Think Ants Can Dance

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

## Description of A level implementation
### Goal: Return to the DrawBonesRecursive() function and extend it to draw an ant.
