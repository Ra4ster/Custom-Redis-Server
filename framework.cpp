#include "framework.h"
#include <string>

const char* red = "\x1b[31m";  // Red
const char* orange = "\x1b[38;5;208m"; // Bright orange (extended 256-color mode)
const char* yellow = "\x1b[33m";  // Yellow
const char* green = "\x1b[32m";  // Green
const char* blue = "\x1b[34m";  // Blue
const char* indigo = "\x1b[38;5;54m";  // Indigo (approximate)
const char* violet = "\x1b[35m";  // Magenta (used for violet)
const char* reset = "\x1b[0m";   // Reset to default color

std::string asciiArt = std::string(red) +
"\n/**////** /**///// /**////** /** **////// \n" + std::string(orange) +
"/**   /** /**      /**    /**/**/**\n" + std::string(yellow) +
"/*******  /******* /**    /**/**/*********\n" + std::string(green) +
"/**///**  /**////  /**    /**/**////////**\n" + std::string(blue) + 
"/**  //** /**      /**    ** /**       /**\n" + std::string(indigo) + 
"/**   //**/********/*******  /** ********\n" + std::string(violet) +
"//     // //////// ///////   // ////////\n\n" + std::string(reset);


const char* REDIS_LOGO = asciiArt.c_str();

std::string helpstr =
"\n\nWelcome to the Redis help section.\n"
"<---------------------------------------------------->\n" + std::string(blue) +
"About:\n" + std::string(reset) +
"<---------------------------------------------------->\n" +
"Redis is a key-value database stored on the heap;\n" +
"what makes this different from any other database?\n" +
"Most databases are stored on the disk, a slow\n" +
"moving memory location equivalent to a hard drive.\n" +
"The heap, conversely, is part of RAM, meaning\n" +
"It can send/receive data as fast as you can\n" +
"print \"Hello World!\" Want to try it out?\n" +
"This next section is for how to use:\n" +
"<---------------------------------------------------->\n" + std::string(blue) +
"How to use:\n" + std::string(reset) +
"<---------------------------------------------------->\n" +
"To start the Redis server, simply create a Redis\n" +
"object with the port as the parameter, then\n" +
"set it to servable, and finally, in a while loop\n" +
"do .serve(); it's as simple as pie!\n" +
"To connect, use \" telnet localhost (port)\"\n" +
"(default) or whichever other address you\n" +
"specified. Then write some commands!\n" +
"<---------------------------------------------------->\n" + std::string(blue) +
"Commands:\n" + std::string(reset) +
"<---------------------------------------------------->\n" +
"1. SET (key) (value)\n" + std::string(green) +
"@return +OK\\r\\n" + std::string(reset) + " if accepted\n" +
"2. GET (key)\n" + std::string(green) +
"@return $(length)\\r\\n(value)\\r\\n" + std::string(reset) + " if accepted, \n" +
"else " + std::string(green) + "$ - 1\\r\\n" + std::string(reset) + ".\n" +
"3. DEL (any keys) \n" + std::string(green) +
"@return :(amount deleted)\\r\\n\n" + std::string(reset) +
"4. ECHO (text)\n" + std::string(green) +
"@return $(length)\\r\\n(text)\\r\\n\n" + std::string(reset) +
"5. PING\n" + std::string(green) +
"@return +PONG\\r\\n\n" + std::string(reset) +
"6. MODE (CLI / RAW)\n" + std::string(green) +
"@return +OK\\r\\n" + std::string(reset) + " and switches to either\n" +
"RAWMessenger (returns as you see here)\n" +
"or CLIMessenger (returns prettier responses)\n" +
"7. quit\n" + std::string(green) +
"@return +OK\\r\\n\\r\\n" + std::string(reset) + " and closes connection\n" +
"\nAll other requests may return some version of\n" + std::string(red) +
"\"-ERR ...\"" + std::string(reset) + ".\n"  +
"<---------------------------------------------------->\n"+ std::string(indigo) +
"For other questions, visit http://github.com/Ra4ster.\n"+ std::string(reset) +
"<---------------------------------------------------->\n\n";

const char* help = helpstr.c_str();