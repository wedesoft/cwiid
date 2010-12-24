cwiid
=====

**Author**:       Jan Wedekind
**Copyright**:    2010
**License**:      GPL

Synopsis
--------

This Ruby extension provides access to a Wii Remote via L. Donnie Smith's libcwiid.

Installation
------------

To install this Ruby extension, use the following command:

    $ sudo gem install cwiid

Alternatively you can build and install the Ruby extension from source as follows:

    $ rake
    $ sudo rake install

Usage
-----

Simply run Interactive Ruby:

    $ irb

Here's a small example displaying the state of the accelerometer:

    #!/usr/bin/env ruby
    require 'rubygems'
    require 'cwiid'
    require 'hornetseye_rmagick'
    require 'hornetseye_xorg'
    include Hornetseye
    include Magick
    wiimote = WiiMote.new
    X11Display.show do |display|
      display.status = wiimote.buttons != WiiMote::BTN_1
      wiimote.get_state
      acc = wiimote.acc
      image = Image.new 240, 240, HatchFill.new( 'white', 'lightcyan2' )
      gc = Draw.new
      gc.fill_opacity 0
      gc.stroke_width 2
      gc.stroke 'red'
      gc.line 120, 120, acc[0], acc[2]
      gc.stroke 'green'
      gc.line 120, 120, acc[1], acc[2]
      gc.draw image
      image.to_multiarray
    end

