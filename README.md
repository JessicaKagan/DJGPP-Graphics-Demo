#DOS Graphics Programming with DJGPP

At the moment, this program runs through some very basic VGA graphics effects that are enabled by DJGPP's stock graphics and low level hardware libraries. In time, it should do more of interest.

What's the point of this repository? I've got a few goals in mind:
- Getting better at programming in C
- Practicing the use of data structures
- Maybe making some sort of big retrocomputing project if I get good enough
Hopefully, DOS programming skills aren't too important in the business programming world (lest we be overwhelmed with decades old technology), but this whole idea of 'retrocomputing' is not only something I find interesting, but something that naturally synergizes with my past and present fields of study and career choices.

##Technical Details
Since the code in this repository is written for an obsolete platform that is no longer in widespread use, getting this (currently relatively simple) program to run takes a bit of doing. You'll need a DOS environment, of course; the latest version of DOSBox should do just fine, although I personally extend beyond this a bit by using yhkwong's SVN-Daum fork. You'll also need to download and set up the DOS version of [DJGPP](http://www.delorie.com/djgpp/). I personally use RHIDE within DOSBox, but the IDE probably isn't too important. DJGPP gives you access to recent versions of GCC/GPP within DOS, so if you set up the paths and have access to the proper libraries this shouldn't be too hard to compile, especially since for the foreseeable future it'll be fairly trivial.
