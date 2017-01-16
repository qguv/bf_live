# bf\_live

A [brainfuck](https://en.wikipedia.org/wiki/Brainfuck) language interpreter and programming environment for the [Arduboy](http://community.arduboy.com/).

<img src="https://kuuv.io/i/Zh4oCHB.png" alt="bf_live running on a white Arduboy" />

## flashing

Open `bf_live/bf_live.ino` in the Arduino IDE. Make sure you have the Arduboy libraries. Press 'Upload' (the &#x27a1; button). You can start with a blank slate by holding _down_ on boot; otherwise, a simple template will be loaded.

## use

The program is in the top part of the screen and its output is at the bottom. As the program source is modified, the program will be re-run and its output updated.

Move around the program source with the arrow keys. Hold a button and tap an arrow key to change a cell of program source. Tap either button to clear a cell.

If you're getting 'err' as your output, make sure you're not violating any syntax or semantics rules:

  - all loop starts/ends must be matched
  - left of the zeroth tape cell, there are no other cells
  - practical limits on memory (cell count and loop recursion depth)

## inspiration

@[fuopy](https://github.com/fuopy)'s incredible [ABasm](https://github.com/fuopy/ABasm) project is a similar project but with a custom assembly-like target language.

## to do

  - [X] build the interpreter
  - [X] make it interactive
  - [ ] ~~steal~~ adapt ABasm programming environment UX
  - [ ] push buffer sizes to Arduboy's maximum stack size

## never asked questions

> Why is the code in an extra subdirectory?

The default Arduino IDE requires any file X.ino to be in a directory X.

> Why does one file have the extension `.c.dummy`?

The default Arduino IDE automatically compiles any \*.c and \*.h files it finds in the directory together with the \*.ino source.

> The default Arduino IDE is awful; why even bother supporting it?

It's the most frictionless, cross-platform way to upload finished code to an Arduino-compatible chip.
