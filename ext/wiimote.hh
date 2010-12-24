/* ruby-cwiid - Wii Remote interface.
   Copyright (C) 2010 Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef WIIMOTE_HH
#define WIIMOTE_HH
#include <boost/shared_ptr.hpp>
#include <cwiid.h>
#include <map>
#include "rubyinc.hh"
#include "error.hh"

class WiiMote
{
public:
  WiiMote(void) throw (Error);
  virtual ~WiiMote(void);
  void close(void);
  void requestStatus(void) throw (Error);
  void getState(void) throw (Error);
  unsigned char getRptMode(void);
  void setRptMode( unsigned char mode ) throw (Error);
  unsigned char getBattery(void);
  unsigned char getLED(void);
  void setLED( unsigned char state ) throw (Error);
  bool getRumble(void);
  void setRumble( bool state ) throw(Error);
  unsigned short int getButtons(void);
  unsigned char getAcc( int id );
  bool irValid( int i );
  unsigned short int getIRX( int i );
  unsigned short int getIRY( int i );
  void err( const char *s, va_list ap );
  static VALUE cRubyClass;
  static VALUE registerRubyClass(void);
  static void deleteRubyObject( void *ptr );
  static VALUE wrapNew( VALUE rbClass );
  static VALUE wrapClose( VALUE rbSelf );
  static VALUE wrapRequestStatus( VALUE rbSelf );
  static VALUE wrapGetState( VALUE rbSelf );
  static VALUE wrapGetRptMode( VALUE rbSelf );
  static VALUE wrapSetRptMode( VALUE rbSelf, VALUE rbMode );
  static VALUE wrapGetBattery( VALUE rbSelf );
  static VALUE wrapGetLED( VALUE rbSelf );
  static VALUE wrapSetLED( VALUE rbSelf, VALUE rbState );
  static VALUE wrapGetRumble( VALUE rbSelf );
  static VALUE wrapSetRumble( VALUE rbSelf, VALUE rbState );
  static VALUE wrapGetButtons( VALUE rbSelf );
  static VALUE wrapGetAcc( VALUE rbSelf );
  static VALUE wrapGetIR( VALUE rbSelf );
  static WiiMote *current;
protected:
  cwiid_wiimote_t *m_wiimote;
  struct cwiid_state m_state;
  bool m_error;
  std::string m_errorMsg;
};

typedef boost::shared_ptr< WiiMote > WiiMotePtr;

#endif
