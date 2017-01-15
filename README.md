# bf\_live

Goal: a [brainfuck](https://en.wikipedia.org/wiki/Brainfuck) language interpreter and programming environment for the [Arduboy]().

## flashing

Open `bf_live/bf_live.ino` in the Arduino IDE. Make sure you have the Arduboy libraries. Press 'Upload' (the &#x27a1; button).

## inspiration

@[fuopy](https://github.com/fuopy)'s incredible [ABasm](https://github.com/fuopy/ABasm) project is a similar project but with a custom assembly-like target language.

## to do

  - [X] build the interpreter
  - [ ] make it interactive by ~~stealing~~ adapting ABasm programming environment
  - [ ] push buffer sizes to Arduboy's maximum stack size

## never asked questions

> Why is the code in an extra subdirectory?

The default Arduino IDE requires any file X.ino to be in a directory X.

> Why does one file have the extension `.c.dummy`?

The default Arduino IDE automatically compiles any \*.c and \*.h files it finds in the directory together with the \*.ino source.

> The default Arduino IDE is awful; why even bother supporting it?

It's the most frictionless, cross-platform way to upload finished code to an Arduino-compatible chip.
