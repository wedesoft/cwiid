# ruby-cwiid - Wii Remote interface for Ruby.
# Copyright (C) 2010 Jan Wedekind
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
class WiiMote

  class << self

    alias_method :orig_new, :new

    def new
      retval = orig_new
      retval.instance_eval do
        @led = 0
        @rumble = false
      end
      retval
    end

  end

  attr_reader :led
  attr_reader :rumble

  alias_method :orig_set_led, :led=

  def led=( state )
    @led = state
    orig_set_led state
  end

  alias_method :orig_set_rumble, :rumble=

  def rumble=( state )
    @rumble = state
    orig_set_rumble state
  end

end

