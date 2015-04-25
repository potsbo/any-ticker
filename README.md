any-ticker
==========

Extension of "golly-ticker" in Conway's "Game of Life", including useful tools for "Game of Life"

## Introduction
This program enables us to make and convert files for [Conway's Game of Life](http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

### Any Ticker
The biggest feature of this program is "any-ticker", which makes a ticker message,
like [golly-ticker](https://code.google.com/p/vals-game-of-life/source/browse/trunk/src/main/resources/gameoflife/patterns/golly/Guns/golly-ticker.rle),
which works under the rule of [Conway's Game of Life](http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).
The output file will be saved as [Life 1.06](http://conwaylife.com/wiki/Life_1.06). It is recommended to open it with [Golly](http://golly.sourceforge.net/)
### File Conversion
When I was writing "any-ticker", the file conversion from rle to Life 1.06 was necessary
I have written a program to make and included it.

## Motivation
To show **any** string or image as a **ticker** just like golly-ticker.

## Install
You can simply use any-ticker by this command below in a terminal.
`git clone https://github.com/potsbo/any-ticker.git && cd any-ticker && make`
Certain programs are required.

##Usage
I have implemented subcommand system.
### Any Ticker
Use the command below to use any-ticker in very basic way.
Note that you have to change <foo> into suitable values.
`life ticker -m "<message>" -s <font size> -f <font name>`
If you want just a ticker message, the command below is the easiest way.
`life ticker -m "<message>"`

## TFont
TODO

## Future Works
* To add a function to make a ticker message from a black/white image.
* To make this compatible with font files written in Life 1.06 format.

## Note
I'm trying to convert this into a cpp program gradually, since I had written in C firstly.

## Licence
Released under MIT licence.
