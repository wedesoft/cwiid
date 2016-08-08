require "cwiid"

puts "Press 1+2 or sync..."

wm = WiiMote.new

wm.rpt_mode = WiiMote::RPT_NUNCHUK | ::WiiMote::RPT_ACC | WiiMote::RPT_BTN

loop do
  wm.get_state
  print "\rSTICK:#{wm.nunchuk_stick} BUTTONS:#{wm.nunchuk_buttons} ACC:#{wm.nunchuk_acc}        "
end
