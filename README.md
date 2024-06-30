# Doubler

A simple LV2 plugin for doubling audio.

## Installing

You'll need a C compiler and `lv2` headers. The headers should be available in
your distro's package manager under something like "lv2" or "lv2-dev".

Then:

```shell
$ make && make install
```

This will put the plugin into `~/.lv2/` - if you want to install it elsewhere,
open up the `Makefile` and change `LV2_DIR`.

## Usage

There is one control: delay. This is the amount of time (in milliseconds) between
the left and right tracks. Adjust to your needs.
