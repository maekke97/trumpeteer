#include <espeak/speak_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFSIZE 512
static unsigned int uid = 42;

struct word_description {
    int word_number;
    int duration_ms;
};

typedef struct word_description word_description_t;

struct callback_data {
    char *text;
    word_description_t *word_description;
};
typedef struct callback_data callback_data_t;

int callback(short *wav, int numsamples, espeak_EVENT *events) {
    static int word_number = 0;
    static int last_duration = 0;
    char buff[BUFFSIZE];
    espeak_EVENT *ptr = events;
    
    for (; ptr->type != espeakEVENT_LIST_TERMINATED; ptr++) {
	callback_data_t *data = (callback_data_t*) events->user_data;
	
	switch(ptr->type) {
	case espeakEVENT_WORD:
	    data->word_description[word_number].word_number = word_number;
	    data->word_description[word_number].duration_ms = ptr->audio_position;

	    if (word_number > 0) {
		// Now, we can start propagating effective duration
		data->word_description[word_number-1].duration_ms =
		    ptr->audio_position - data->word_description[word_number-1].duration_ms;
	    }
    
	    word_number++;
	    break;
	case espeakEVENT_MSG_TERMINATED:
	    data->word_description[word_number-1].duration_ms =
		last_duration - data->word_description[word_number-1].duration_ms;
	    break;
	case espeakEVENT_END:
	    // We have to keep this to compute the final duration
	    last_duration = ptr->audio_position;
	    break;
	}
    }
    
    return 0;
}

int count_spaces(const char* buffer) {
    int spaces=0;

    for (int i=0; i<strlen(buffer); i++) {
	if (isspace(buffer[i])) {
	    spaces++;
	}
    }

    return spaces;
}

void initialize_espeak(void) {
    int err = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL,
				0,
				NULL,
				0);
    if (err == EE_INTERNAL_ERROR) {
	fprintf(stderr, "Error initializing espeak\n");
	exit(1);
    }

    espeak_SetSynthCallback(callback);
}

void synthetize(char* buff, callback_data_t *callback_data) {
    int err = espeak_Synth(buff,
		       strlen(buff),
		       0,
		       POS_WORD,
		       strlen(buff) - 1,
		       espeakCHARS_UTF8,
		       &uid,
		       (void*) callback_data);
    
    if (err != EE_OK) {
        fprintf(stderr, "Error synth\n");
	exit(1);
    }
}
	    

int main() {
    char buff[BUFFSIZE];
    int err, spaces;
    callback_data_t callback_data;

    fgets(buff, BUFFSIZE-1, stdin);

    spaces = count_spaces(buff);
    if (spaces == 0) {
	fprintf(stderr, "At least two words are required");
	exit(2);
    }

    callback_data.text = buff;
    callback_data.word_description = calloc(spaces+1, sizeof(word_description_t));
   
    initialize_espeak();
    synthetize(buff, &callback_data);

    espeak_Synchronize();

    for (int i=0; i<spaces; i++) {

	fprintf(stdout, "Word #%d: %d\n", callback_data.word_description[i].word_number,
		callback_data.word_description[i].duration_ms);
    }
}
