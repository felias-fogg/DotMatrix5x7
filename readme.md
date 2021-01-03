# DotMatrix5x7
A 5x7 dot matrix display driven directly by Arduino pins (12 needed!)

Version: 1.0

Date:  03-01-2021

## Features

* Supports 5x7 dot matrix column anode displays such as TA07-11SEKWA.
* Assumes only Arduino pins, no external controller
* Interrupt driven (using timer 1)
* Can scroll vertically and horizontally

## Usage

This library has been written for 5x7 dot matrix displays with column anodes. 12 Arduino pins are necessary to control it (5 column pins, 7 row pins). It uses the TimerOne library by Paul Stoffregen in order to display the text in an interrupt driven way. This means you can other things after you have requested to display particular. 

