Lazy-Couch
==========

Your couch controls the video player based on your presence, by a simple Arduino Client for RES(T)mote Control (https://github.com/becuz/RESTmoteControl)


The purpose of this simple client is to play/pause your pc video player when you sit in or stand up from your couch.
The client has a button, a force resistor and a led. When the button is pushed, or the force resistor reads a big delta value, a POST /api/activeapps/smplayer/control/play is made to the server and the led is briefly lighted.
