# dlib-elixir
dlib for Elixir

## Requirements

 - Dlib
 - Elixir

Compile `dlib.so`

```sh
# ERLANG_PATH should be the path to a directory containing "erl_nif.h"
$ export ERLANG_PATH=/usr/local/lib/erlang/usr/include

# DLIB_PATH should be where you've installed dlib
$ export DLIB_PATH=dlib-19.4

/*  cc -fPIC \
  -I$ERLANG_PATH \
  -I$DLIB_PATH \
  -Wl,-undefined \
  -Wl,dynamic_lookup \
  -shared  \
  -std=c++11 \
  -lstdc++ \
  -ldlib \
  -o dlib.so dlib.cpp
```

## Usage

```
c "dlib.ex"

// The first argument is a matrix of pixels.
// A pixel is a list of integers [r,g,b]
Dlib.frontal_face_detector([
  [
    [255,255,255],
    ...
  ]
])
```

