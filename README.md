# cheap-shooter

Old SDL overhead shooter/puzzler/go figure out.

I really worked a bit on this one to get it right but had lots of problems because of all the custom code I had to write to make SDL do my bidding. This was way before I even had a libdansdl for SDL1.2.

It is incomplete, of course, but it had bosses, achievements, different levels, menus... The function loop_principal() in controlador.cpp had a few variables that can be changed. I tried to make it so it behaves as a regular game.

I'd like to revisit this concept in the future: I am sure I can do it better and faster now.

## Controls

- Arrow keys to move.
- Left click to activate power.
- Left control to change power when available.
- Right click to slow down time.
- Space to pause.
- q to remove the stupid log box in the top left.

## Editor controls...

- 9 to show help (with all controls, there are much more than those shown here).
- 1 to edit level.
- 2 to edit properties
- 3 to edit the table of random obstacles
- 4 to edit the table of random bonuses
- 5 to edit the table of random bonus boxes
- 6 to edit the table of random bonus obstacles
- 7 to edit triggers and actions.
- 8 to show a list of objects
- f5 to export the level to a picture (may not work).

## Building

It depends on SDL1.2 and a very primitive SDL engine it comes bundled with.

- ./herramientas.sh
- Choose option c), be patient.
- Choose option 1) to play.
