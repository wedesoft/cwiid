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
#include "wiimote.hh"

using namespace std;

VALUE WiiMote::cRubyClass = Qnil;

WiiMote *WiiMote::current = NULL;

static void staticErr( cwiid_wiimote_t *wiimote, const char *s, va_list ap )
{
  WiiMote::current->err( s, ap );
}

WiiMote::WiiMote(void) throw (Error):
  m_wiimote(NULL), m_error(false)
{
  bdaddr_t bdaddrAny = { { 0, 0, 0, 0, 0, 0 } };
  current = this;
  cwiid_set_err( staticErr );
  memset( &m_state, 0, sizeof(m_state) );
  m_wiimote = cwiid_open( &bdaddrAny, 0 );
  ERRORMACRO( !m_error, Error, , "Failed to connect to Wii Remote: "
              << m_errorMsg );
}

WiiMote::~WiiMote(void)
{
  close();
}

void WiiMote::close(void)
{
  if ( m_wiimote != NULL ) {
    cwiid_close( m_wiimote );
    m_wiimote = NULL;
    current = NULL;
    cwiid_set_err( cwiid_err_default );
  };
}

int WiiMote::requestStatus(void) throw (Error)
{
  ERRORMACRO( m_wiimote != NULL, Error, , "Wii Remote is closed. "
              "Did you call \"close\" before?" );
  m_error = false;
  int status = cwiid_request_status( m_wiimote );
  ERRORMACRO( !m_error, Error, , "Status request error: " << m_errorMsg );
  return status;
}

void WiiMote::getState(void) throw (Error)
{
  ERRORMACRO( m_wiimote != NULL, Error, , "Wii Remote is closed. "
              "Did you call \"close\" before?" );
  m_error = false;
  cwiid_get_state( m_wiimote, &m_state );
  ERRORMACRO( !m_error, Error, , "Error getting state: " << m_errorMsg );
}

unsigned char WiiMote::getRptMode(void)
{
  return m_state.rpt_mode;
}

void WiiMote::setRptMode( unsigned char mode ) throw (Error)
{
  ERRORMACRO( m_wiimote != NULL, Error, , "Wii Remote is closed. "
              "Did you call \"close\" before?" );
  m_error = false;
  cwiid_set_rpt_mode( m_wiimote, mode );
  ERRORMACRO( !m_error, Error, , "Error setting RPT mode: " << m_errorMsg );
  m_state.rpt_mode = mode;
}

unsigned char WiiMote::getBattery(void)
{
  return m_state.battery;
}

unsigned char WiiMote::getLED(void)
{
  return m_state.led;
}

void WiiMote::setLED( unsigned char state ) throw (Error)
{
  ERRORMACRO( m_wiimote != NULL, Error, , "Wii Remote is closed. "
              "Did you call \"close\" before?" );
  m_error = false;
  cwiid_set_led( m_wiimote, state );
  ERRORMACRO( !m_error, Error, , "Error setting LED state: " << m_errorMsg );
  m_state.led = state;
}

bool WiiMote::getRumble(void)
{
  return m_state.rumble != 0;
}

void WiiMote::setRumble( bool state ) throw( Error )
{
  ERRORMACRO( m_wiimote != NULL, Error, , "Wii Remote is closed. "
              "Did you call \"close\" before?" );
  m_error = false;
  cwiid_set_rumble( m_wiimote, state ? 1 : 0 );
  ERRORMACRO( !m_error, Error, , "Error setting rumble state: " << m_errorMsg );
  m_state.rumble = state ? 1 : 0;
}

unsigned short int WiiMote::getButtons(void)
{
  return m_state.buttons;
}

unsigned short int WiiMote::getNunchukButtons(void)
{
  return m_state.ext.nunchuk.buttons;
}

unsigned char WiiMote::getAcc( int id )
{
  return m_state.acc[ id ];
}

unsigned char WiiMote::getNunchukAcc( int id )
{
  return m_state.ext.nunchuk.acc[ id ];
}

unsigned char WiiMote::getNunchukJoyStick( int id )
{
  return m_state.ext.nunchuk.stick[ id ];
}


bool WiiMote::irValid( int i )
{
  return m_state.ir_src[ i ].valid != 0;
}

unsigned short int WiiMote::getIRX( int i )
{
  return m_state.ir_src[ i ].pos[ CWIID_X ];
}

unsigned short int WiiMote::getIRY( int i )
{
  return m_state.ir_src[ i ].pos[ CWIID_Y ];
}

char WiiMote::getIRSize( int i )
{
  return m_state.ir_src[ i ].size;
}

unsigned short int WiiMote::getMotionPlus( int id )
{
  return m_state.ext.motionplus.angle_rate[ id ];
}

void WiiMote::err( const char *s, va_list ap )
{
  char buffer[4096];
  vsnprintf( &buffer[0], sizeof(buffer), s, ap );
  m_errorMsg = buffer;
  m_error = true;
}

VALUE WiiMote::registerRubyClass(void)
{
  cRubyClass = rb_define_class( "WiiMote", rb_cObject );
  rb_define_const( cRubyClass, "RPT_STATUS", INT2NUM( CWIID_RPT_STATUS ) );
  rb_define_const( cRubyClass, "RPT_BTN", INT2NUM( CWIID_RPT_BTN ) );
  rb_define_const( cRubyClass, "RPT_ACC", INT2NUM( CWIID_RPT_ACC ) );
  rb_define_const( cRubyClass, "RPT_IR", INT2NUM( CWIID_RPT_IR ) );
  rb_define_const( cRubyClass, "RPT_NUNCHUK", INT2NUM( CWIID_RPT_NUNCHUK ) );
  rb_define_const( cRubyClass, "RPT_CLASSIC", INT2NUM( CWIID_RPT_CLASSIC ) );
  rb_define_const( cRubyClass, "RPT_BALANCE", INT2NUM( CWIID_RPT_BALANCE ) );
  rb_define_const( cRubyClass, "RPT_MOTIONPLUS", INT2NUM( CWIID_RPT_MOTIONPLUS ) );
  rb_define_const( cRubyClass, "BTN_2", INT2NUM( CWIID_BTN_2 ) );
  rb_define_const( cRubyClass, "BTN_1", INT2NUM( CWIID_BTN_1 ) );
  rb_define_const( cRubyClass, "BTN_B", INT2NUM( CWIID_BTN_B ) );
  rb_define_const( cRubyClass, "BTN_A", INT2NUM( CWIID_BTN_A ) );
  rb_define_const( cRubyClass, "NUNCHUK_BTN_C", INT2NUM( CWIID_NUNCHUK_BTN_C ) );
  rb_define_const( cRubyClass, "NUNCHUK_BTN_Z", INT2NUM( CWIID_NUNCHUK_BTN_Z ) );
  rb_define_const( cRubyClass, "BTN_MINUS", INT2NUM( CWIID_BTN_MINUS ) );
  rb_define_const( cRubyClass, "BTN_HOME", INT2NUM( CWIID_BTN_HOME ) );
  rb_define_const( cRubyClass, "BTN_LEFT", INT2NUM( CWIID_BTN_LEFT ) );
  rb_define_const( cRubyClass, "BTN_RIGHT", INT2NUM( CWIID_BTN_RIGHT ) );
  rb_define_const( cRubyClass, "BTN_DOWN", INT2NUM( CWIID_BTN_DOWN ) );
  rb_define_const( cRubyClass, "BTN_UP", INT2NUM( CWIID_BTN_UP ) );
  rb_define_const( cRubyClass, "BTN_PLUS", INT2NUM( CWIID_BTN_PLUS ) );
  rb_define_const( cRubyClass, "LED1_ON", INT2NUM( CWIID_LED1_ON ) );
  rb_define_const( cRubyClass, "LED2_ON", INT2NUM( CWIID_LED2_ON ) );
  rb_define_const( cRubyClass, "LED3_ON", INT2NUM( CWIID_LED3_ON ) );
  rb_define_const( cRubyClass, "LED4_ON", INT2NUM( CWIID_LED4_ON ) );
  rb_define_const( cRubyClass, "BATTERY_MAX", INT2NUM( CWIID_BATTERY_MAX ) );
  rb_define_singleton_method( cRubyClass, "new",
                              RUBY_METHOD_FUNC( wrapNew ), 0 );
  rb_define_method( cRubyClass, "close",
                    RUBY_METHOD_FUNC( wrapClose ), 0 );
  rb_define_method( cRubyClass, "request_status",
                    RUBY_METHOD_FUNC( wrapRequestStatus ), 0 );
  rb_define_method( cRubyClass, "get_state",
                    RUBY_METHOD_FUNC( wrapGetState ), 0 );
  rb_define_method( cRubyClass, "rpt_mode",
                    RUBY_METHOD_FUNC( wrapGetRptMode ), 0 );
  rb_define_method( cRubyClass, "rpt_mode=",
                    RUBY_METHOD_FUNC( wrapSetRptMode ), 1 );
  rb_define_method( cRubyClass, "battery",
                    RUBY_METHOD_FUNC( wrapGetBattery ), 0 );
  rb_define_method( cRubyClass, "led",
                    RUBY_METHOD_FUNC( wrapGetLED ), 0 );
  rb_define_method( cRubyClass, "led=",
                    RUBY_METHOD_FUNC( wrapSetLED ), 1 );
  rb_define_method( cRubyClass, "rumble",
                    RUBY_METHOD_FUNC( wrapGetRumble ), 0 );
  rb_define_method( cRubyClass, "rumble=",
                    RUBY_METHOD_FUNC( wrapSetRumble ), 1 );
  rb_define_method( cRubyClass, "buttons",
                    RUBY_METHOD_FUNC( wrapGetButtons ), 0 );
  rb_define_method( cRubyClass, "nunchuk_buttons",
                    RUBY_METHOD_FUNC( wrapGetNunchukButtons ), 0 );
  rb_define_method( cRubyClass, "acc",
                    RUBY_METHOD_FUNC( wrapGetAcc ), 0 );
  rb_define_method( cRubyClass, "nunchuk_acc",
                    RUBY_METHOD_FUNC( wrapGetNunchukAcc ), 0 );
  rb_define_method( cRubyClass, "nunchuk_stick",
                    RUBY_METHOD_FUNC( wrapGetNunchukJoyStick ), 0 );
  rb_define_method( cRubyClass, "ir",
                    RUBY_METHOD_FUNC( wrapGetIR ), 0 );
  rb_define_method( cRubyClass, "motionplus",
                    RUBY_METHOD_FUNC( wrapGetMotionPlus ), 0 );
  return cRubyClass;
}

void WiiMote::deleteRubyObject( void *ptr )
{
  delete (WiiMotePtr *)ptr;
}

VALUE WiiMote::wrapNew( VALUE rbClass )
{
  VALUE retVal = Qnil;
  try {
    WiiMotePtr ptr( new WiiMote );
    retVal = Data_Wrap_Struct( rbClass, 0, deleteRubyObject,
                               new WiiMotePtr( ptr ) );
  } catch ( exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return retVal;
}

VALUE WiiMote::wrapClose( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  (*self)->close();
  return rbSelf;
}

VALUE WiiMote::wrapRequestStatus( VALUE rbSelf )
{
  VALUE rbRetVal = Qnil;
  try {
    WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
    rbRetVal = INT2NUM( (*self)->requestStatus() );
  } catch ( exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbRetVal;
}

VALUE WiiMote::wrapGetState( VALUE rbSelf )
{
  try {
    WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
    (*self)->getState();
  } catch ( exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbSelf;
}

VALUE WiiMote::wrapGetRptMode( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  (*self)->getRptMode();
  return rbSelf;
}

VALUE WiiMote::wrapSetRptMode( VALUE rbSelf, VALUE rbMode )
{
  try {
    WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
    (*self)->setRptMode( NUM2INT( rbMode ) );
  } catch ( exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbMode;
}

VALUE WiiMote::wrapGetBattery( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return INT2NUM( (*self)->getBattery() );
}

VALUE WiiMote::wrapGetLED( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return INT2NUM( (*self)->getLED() );
}

VALUE WiiMote::wrapSetLED( VALUE rbSelf, VALUE rbState )
{
  try {
    WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
    (*self)->setLED( NUM2INT( rbState ) );
  } catch ( exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbState;
}

VALUE WiiMote::wrapGetRumble( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return (*self)->getRumble() ? Qtrue : Qfalse;
}

VALUE WiiMote::wrapSetRumble( VALUE rbSelf, VALUE rbState )
{
  try {
    WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
    (*self)->setRumble( rbState == Qtrue );
  } catch ( exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbState;
}

VALUE WiiMote::wrapGetButtons( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return INT2NUM( (*self)->getButtons() );
}



VALUE WiiMote::wrapGetAcc( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return rb_ary_new3( 3, INT2NUM( (*self)->getAcc( 0 ) ),
                         INT2NUM( (*self)->getAcc( 1 ) ),
                         INT2NUM( (*self)->getAcc( 2 ) ) );
}

VALUE WiiMote::wrapGetIR( VALUE rbSelf )
{
  VALUE rbRetVal = rb_ary_new();
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  for ( int i=0; i<CWIID_IR_SRC_COUNT; i++ )
    if ( (*self)->irValid( i ) )
      rb_ary_push( rbRetVal, rb_ary_new3( 3,
                                          INT2NUM( (*self)->getIRX( i ) ),
                                          INT2NUM( (*self)->getIRY( i ) ),
                                          INT2NUM( (*self)->getIRSize( i ) ) ) );
  return rbRetVal;
}

VALUE WiiMote::wrapGetMotionPlus( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return rb_ary_new3( 3, INT2NUM( (*self)->getMotionPlus( 0 ) ),
                         INT2NUM( (*self)->getMotionPlus( 1 ) ),
                         INT2NUM( (*self)->getMotionPlus( 2 ) ) );
}

VALUE WiiMote::wrapGetNunchukAcc( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return rb_ary_new3( 3, INT2NUM( (*self)->getNunchukAcc( 0 ) ),
                         INT2NUM( (*self)->getNunchukAcc( 1 ) ),
                         INT2NUM( (*self)->getNunchukAcc( 2 ) ) );
}

VALUE WiiMote::wrapGetNunchukJoyStick( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return rb_ary_new3( 2, INT2NUM( (*self)->getNunchukJoyStick( 0 ) ),
                         INT2NUM( (*self)->getNunchukJoyStick( 1 ) ));
}

VALUE WiiMote::wrapGetNunchukButtons( VALUE rbSelf )
{
  WiiMotePtr *self; Data_Get_Struct( rbSelf, WiiMotePtr, self );
  return INT2NUM( (*self)->getNunchukButtons() );
}

