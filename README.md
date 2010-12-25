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
    display = proc do
      acc = wiimote.acc
      glClear GL_COLOR_BUFFER_BIT
      glLineWidth 3.0
      glColor 1.0, 0.0, 0.0
      glBegin GL_LINES
      glVertex 120, 120
      glVertex acc[0], acc[2]
      glEnd
      glColor 0.0, 1.0, 0.0
      glBegin GL_LINES
      glVertex 120, 120
      glVertex acc[1], acc[2]
      glEnd
      glutSwapBuffers
    end
    reshape = proc do |w, h|
      glViewport 0, 0, w, h
      glMatrixMode GL_PROJECTION
      glLoadIdentity
      GLU.Ortho2D 0.0, 240, 0.0, 240
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
      glutPostRedisplay
    end
    glutInit
    glutInitDisplayMode GLUT_DOUBLE | GLUT_RGB
    glutInitWindowSize 240, 240
    glutCreateWindow 'Wii Remote'
    glClearColor 1.0, 1.0, 1.0, 0.0
    glShadeModel GL_FLAT
    
    glutDisplayFunc display
    glutReshapeFunc reshape
    glutKeyboardFunc keyboard
    glutIdleFunc animate
    glutMainLoop

