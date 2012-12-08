# Ore Chasm

## What is it?

Ore Chasm is a game that is greatly inspired by Terraria, but of course aims to
be more.

## Why is it?

It is a project, that is open source from the very beginning, as improving upon
the ideas that Terraria introduced to the world when it released. It isn't
written in XNA and C#, but SFML and C++, so is cross-platform and very fast (I
use the GPU quite a bit, including fragment shaders).

* It will have modding capabilities
* It is open source, everything..including all assets
* It will have loads of features that I missed and I'm sure some others have as
well
* It will *not* be just dropped within a year after release. And in the event
that I get hit by a bus tomorrow, feel free to fork it and pick it up!
* Cross-platform
* Sane GUI/HUD
* Many, many features are planned that people only dreamed of for Terraria to
have.
* Community driven. I'm very open to code reviews, patches, and constructive
criticism, bug and feature requests, etc.


## What license is it under?

All assets are required (by me ;) to be under a Free and Open license.
The code is presently under GPLv2+ as that seems to be the licence
I like the most.

## What platforms will it run on?

The goal initially is to make it run on Linux/Unixen, Mac OS X and Windows.
I'm a developer on Linux, as that's the only real way to program, so this
will likely get the most support. With testers and patches, the other platforms
will have exactly the same support as Linux. I don't want to force anyone onto a
platform just to play my game, even though I think everyone should run Linux
anyways.. ;-)

I would love to see it run on Android for instance. But for this to happen
I'd have to wait until SFML has ports to that/those platforms. Or switch
to a different framework if that becomes the only option.

## How can I help out?

* Merge requests/patches. They will need to match my coding style, which isn't too
difficult (and there's an astyle script at the root directory for ease of use).
And of course, fix the issue. I love features, so chances are I'm going to accept
your patch if it does the job nicely. I'm not too overlordy when it comes to code.
Oh and of course, all other assets are *most* welcome. Of course, they'll also
have to be released under a free license.

* Coding style:  http://techbase.kde.org/Policies/Kdelibs_Coding_Style
use astyle.sh file to automatically format everything to this.

I'll do so ruthlessly every so often. I'd rather have clean code than easy history

* Edit the Wiki on here (github)!
* Submit issues and feature requests on the issue tracker on github!
* Test, test, test!

## How do I install it?
First of all, I use SFML 2.0, which is technically unreleased.

No matter, you can build it easily since I use CMake.

See: http://www.sfml-dev.org/download.php

After this is installed and you have followed their directions, to get my code,
you can either click download the zip, or run `git clone git://github.com/sreich/ore-chasm.git`

After you have my code, go to the top directory of it, `mkdir build; cd build;`
and now to actually build it


## System Requirements
* SFML 2.0 (unreleased, from git)
* Platform of Windows, Linux/Unix, Mac OS X
* Minimum RAM of 250 MiB. Ore Chasm uses so far.. an average of 60 MiB RES.
This will change with world size, since we don't have chunk based loading
(yet?), Currently the size of the Block class is 2 bytes (2 unsigned chars).
Which means for a 8400x2600 map (the default, soft minimum), (8400 \* 2600 \*
2 bytes = ~44 MiB). Presently, to run the game it's about 60MiB of RAM usage on
average. Subject to change, of course.
* GPU capable of OpenGL 3.0+ and/or GLSL 1.30. This is what I *know* it works
on. It may work on lesser GPUs, so please let me know if it does. In theory it
should, but we all know how well theory works..



## Planned Features
Some of these plans are merely thoughts and things to experiment with, some are
must haves and others are things that will hopefully come later. Of course,
nothing is in stone.

* Networking (a must)
* Dungeons
* Ladders! I hated navigation from depths in Terraria. It was terrible and
usually resulted in you dying or being stranded.
* Villages! (randomly generated). Think similar to Minecraft
* No guide! This NPC always lingered around your house even though you wanted to
be left the hell alone. It was really a piss poor replacement for a tutorial
anyways, and he only had a couple tid bits of information. Literally.
* NPCs/merchants you can "place" in your home?
* Teleporters/Portals! Seriously. This is a definite want in my mind.
* Crafting system more simlar to Minecraft? Would people like that? Personally,
I was not much of a fan for Terraria's crafting.
* Menu system (HUD) that isn't impossible to navigate, unlike Terraria's.
* Full modding support (another must). This will have to be a later feature
though of course, until I get a good amount of content into the game.
* Android and in general, a mobile port? This is very much long term though.
* Dynamic 2D lighting and shadow system, versus Terraria's very simplistic
tile-based lighting.
* More than 1 or 2 soundtracks, unlike Terraria's which get very repetitive :-)
* Water..want to experiment with particle based water system, as opposed to the
less realistic tile based water.
* Weather system
* Chunk Loading. Presently Ore Chasm just loads the whole map into memory.
Currently this isn't much of an issue, but with larger worlds and assets,
it could turn into something to think about.


#ToDo
Most of the stuff to do is either in Planned Features, or in my head..but here's
the more technical scratch notes, if I keep them updated:

PARTICLES, neat stuff, may find useful:

http://spark.developpez.com/index.php?page=home&lang=en

