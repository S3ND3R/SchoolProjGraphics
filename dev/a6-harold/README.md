# CSCI 4611 Assignment 6: Harold
## Description of C-level implementation
### Requirement 1: Mouse-Sky Intersections
Mouse-sky intersection was implemented by completing the function in sky.cc, ScreenPtHitsSky().

ScreenPtHitsSky() uses the view and projection matrix to calculate the position of the eye in camera space, and then find the point in 3d space of the mouse on the 2d screen. This is accomplished by using thi function:

```
GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, normalized_screen_pt);

```
A ray is than calculated that points from the eye to the 3d-point. Finally a check is made to see if the ray intersects with the sky and the pointer to the 3d skypoint is set to that value.

### Requirement 2: Drawing in the Sky
Drawing in the sky was implemented by completing the function in sky.cc, AddSkyStroke().

AddSkyStroke() creates a new SkyStroke object and adds it to the collection of SkyStrokes. To implement this the vertices of the 2dstroke mesh are changed to 3d and then the new mesh and color are used in SkyStroke.

Draw calls for the sky loop through the array of SkyStrokes.

## Description of A-level and B-level implementation
### Requirement 3: Editing the Ground
Editing the ground is very similar to adding strokes to the sky. The intersection with the ground is checked in ScreenPtHitsGround(), but instead of adding a stroke the ground is reshaped by adjusting the y values according to the distance from a plane that is created based on the silhouette of the curve that was drawn on the screen.
