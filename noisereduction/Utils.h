#pragma once

#include <assert.h>

typedef struct SndInfo {
    int	frames ;		/* Used to be called samples.  Changed to avoid confusion. */
    int	samplerate ;
    int	channels ;
    int	format ;
    int	sections ;
    int	seekable ;
};

typedef struct SndContext {
    SndInfo info;
};

