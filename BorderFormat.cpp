#include "BorderFormat.h"



Utf8String BOX_DRAWINGS_LIGHT_ARC = "│─╭╮╰╯";
Utf8String BOX_DRAWINGS_LIGHT       = "│─┌┐└┘";
Utf8String BOX_DRAWINGS_HEAVY = "┃━┏┓┗┛";

BorderFormat BorderFormat::GroupBox = BorderFormat(BOX_DRAWINGS_HEAVY);
BorderFormat BorderFormat::Window = BorderFormat(BOX_DRAWINGS_LIGHT_ARC);
BorderFormat BorderFormat::Default = GroupBox;