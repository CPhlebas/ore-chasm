# Ore Chasm

## What is it?

Ore Chasm is a game that is greatly inspired by Terraria, but of course aims to
be more.

## What license is it under?

All assets are required (by me ;) to be under a Free and Open license.
The code is presently under GPLv2+ as that seems to be the licence
I like the most.

## What platforms will it run on?

The goal initially is to make it run on Linux/Unixen, Mac OS X and Windows.
I'm a developer on Linux, as that's the only real way to program, so this
will likely get the most support.

I would love to see it run on Android for instance. But for this to happen
I'd have to wait until SFML has ports to that/those platforms. Or switch
to a different framework if that becomes the only option.

## How can I help out?

Merge requests/patches. They will need to match my coding style, which isn't too
difficult (and there's an astyle script at the root directory for ease of use).
And of course, fix the issue. I love features, so chances are I'm going to accept
your patch if it does the job nicely. I'm not too overlordy when it comes to code.
Oh and of course, all other assets are *most* welcome. Of course, they'll also
have to be released under a free license.

Coding style:  http://techbase.kde.org/Policies/Kdelibs_Coding_Style
use astyle.sh file to automatically format everything to this.

I'll do so ruthlessly every so often. I'd rather have clean code than easy history

## How do I install it?
First of all, I use SFML 2.0, which is technically unreleased.

No matter, you can build it easily since I use CMake.

See: http://www.sfml-dev.org/download.php

After this is installed and you have followed their directions, to get my code,
you can either click download the zip, or run `git clone git://github.com/sreich/ore-chasm.git`


## System Requirements
* SFML 2.0 (unreleased, from git)
* Platform of Windows, Linux/Unix, Mac OS X



## Planned Features
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



#ToDo
PARTICLES, neat stuff, may find useful:

http://spark.developpez.com/index.php?page=home&lang=en

