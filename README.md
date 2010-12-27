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

    require 'rubygems'
    require 'opengl'
    require 'cwiid'
    wiimote = WiiMote.new
    wiimote.rpt_mode = WiiMote::RPT_BTN | WiiMote::RPT_ACC
    $list = nil
    def init
      GL.ClearColor 0.0, 0.0, 0.0, 1.0
      GL.Light GL_LIGHT0, GL_POSITION, [ 0.5, 0.5, 1.0, 0.0 ]
      GL.Enable GL_LIGHTING
      GL.Enable GL_LIGHT0
      GL.DepthFunc GL_LESS
      GL.Enable GL_DEPTH_TEST
      $list = GL.GenLists 1
      GL.NewList $list, GL_COMPILE
      GL.Material GL_FRONT, GL_SPECULAR, [ 1.0, 1.0, 1.0, 0.15 ]
      GL.Material GL_FRONT, GL_SHININESS, [ 100.0 ]
      GL.Material GL_FRONT, GL_EMISSION, [ 0.0, 0.2, 0.0, 1.0 ]
      GL.Material GL_FRONT, GL_DIFFUSE, [ 0.1, 0.8, 0.1, 1.0 ]
      GLUT.SolidSphere 0.4, 16, 16
      GL.EndList
    end
    display = proc do
      acc = wiimote.acc
      GL.Clear GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
      GL.PushMatrix
      GL.Translate( ( acc[0] - 120 ) * 0.01,
                    ( 120 - acc[2] ) * 0.01,
                    ( 120 - acc[1] ) * 0.01 )
      GL.CallList $list
      GL.PopMatrix
      GLUT.SwapBuffers
    end
    reshape = proc do |w, h|
      GL.Viewport 0, 0, w, h
      GL.MatrixMode GL_PROJECTION
      GL.LoadIdentity
      GLU.Perspective 60.0, w.to_f/h, 1.0, 20.0
      GL.MatrixMode GL_MODELVIEW
      GL.LoadIdentity
      GL.Translate 0.0, 0.0, -3.5
    end
    keyboard = proc do |key, x, y|
      case key
      when ?\e
        exit 0
      end
    end
    animate = proc do
      wiimote.get_state
      exit 0 if wiimote.buttons == WiiMote::BTN_1
      GLUT.PostRedisplay
    end
    GLUT.Init
    GLUT.InitDisplayMode GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH
    GLUT.InitWindowSize 640, 480
    GLUT.CreateWindow 'Wii Remote'
    init
    GLUT.DisplayFunc display
    GLUT.ReshapeFunc reshape
    GLUT.KeyboardFunc keyboard
    GLUT.IdleFunc animate
    GLUT.MainLoop
 
