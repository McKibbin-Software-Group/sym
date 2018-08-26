# Sym Usage

Translates models from algebraic form into one of several programming
languages.

## Usage:
    sym [options] <language> <symfile> <codefile>

## Options:
    -version -d -dd -doc -first -last -syntax -merge_only

## Languages:
    -debug -html -msgproc -oxgs -oxgst -oxnewton -tablo -troll 

## Details:

### Argument language
Indicates the target output language and must be one of the alternatives
listed under the Languages heading. Required unless the -merge_only option
is used.

### Argument symfile
Indicates the input model to be translated. Required.

### Argument codefile
The name that should be used for the resulting target-language file. Some
target languages will involve multiple files that will be based on this
name. Required.

### Option -d
Turn on the most commonly used debugging messages.

### Option -dd
Turn on all debugging messages.

### Option -doc
Print this message.

### Option -first
Build a single-year model using only the first year.

### Option -last
Build a single-year model using only the last year.

### Option -merge_only
Combine all included modules and return the resulting file
without generating any target-language code.

### Option -syntax
Print a short summary of the input syntax, including some
notes about rules appling to specific target languages.

### Option -version
Print detailed information about the versions of the main
program and the individual language support modules.

Sym build 705 (2018/08/16)