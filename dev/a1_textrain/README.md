# Assignment 1: Text Rain
Warren Weber

CSCI 4611

Spring Semester 2020

## Design Decisions

### Pseudo-Random Letter selection

Letter are selected randomly from a string that is delimited by spaces. Whenever
a space is selected as the location for the next character the entire word that
follows that space is output instead.

### Speed connected to pixels per-second

Letters increase their y position based on a randomly set speed value. This is
implemented by checking that the change in time that elapsed since a letter
last updated is greater than the speed value. If that case is true then the
letter updates its y position and resets the time it last updated. This frees
letter updates from being connected to the amount of times the draw loop is
called.
