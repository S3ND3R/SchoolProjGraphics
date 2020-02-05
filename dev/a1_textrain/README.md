# Assignment 1: Text Rain
Warren Weber

CSCI 4611

Spring Semester 2020

## Design Decisions

### Pseudo-Random Letter selection

Letter are selected randomly from a string that is delimited by spaces. Whenever
a space is selected as the location for the next character the entire word that
follows that space is output instead.
