#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <argp.h>
#include <stdbool.h>
//#include <iconv.h>

#include "conv_tbl.h"
#include "qrcode.h"

static char doc[] = "Write some funny message on Discord";
static char args_doc[] = "MESSAGE";
static char DEF_VAL[] = "Hello world";

struct arguments
{
	char *msg;
	bool aesthetic, fatten, mock, qrcode;
};


/**
 * Transliterate the msg from UTF-8 to ASCII
 * ⚠️ the string returned can be longer than the one given.
 */
char* clean_str(char *msg, int msg_len) {
	//iconv_t iconv_desc = iconv_open("ASCII", "UTF−8");
	///iconvctl(iconv_desc,ICONV_SET_TRANSLITERATE, 1);
	return msg;
}


/**
 * Transliterate the msg from UTF-8 to ASCII
 * ⚠️ the string returned can be longer than the one given.
 */
char* convert(char c) {
	c = tolower(c);
	if (c >= 'a' && c <= 'z') {
		char* chr_emote = malloc(strlen(ALPHA_CHR));
		sprintf(chr_emote, ALPHA_CHR, c);
		return chr_emote;
	} else {
		const char* tmp;
		switch(c) {
			case ' ':
				tmp = SPACE;
				break;
			case '0':
				tmp = ZERO;
				break;
			case '1':
				tmp = ONE;
				break;
			case '2':
				tmp = TWO;
				break;
			case '3':
				tmp = THREE;
				break;
			case '4':
				tmp = FOUR;
				break;
			case '5':
				tmp = FIVE;
				break;
			case '6':
				tmp = SIX;
				break;
			case '7':
				tmp = SEVEN;
				break;
			case '8':
				tmp = EIGHT;
				break;
			case '9':
				tmp = NINE;
				break;
			default:
				tmp = INTERRO_PT;
				break;
		}
		int tmp_len = strlen(tmp);
		char* str_ptr = calloc(1, tmp_len);
		strncpy(str_ptr, tmp, tmp_len);
		return str_ptr;
	}
}

void convert_str(char* msg) {
	int msg_len = strlen(msg);
	msg = clean_str(msg, msg_len);
	char *new_msg;

	for (int i=0; i<msg_len; i++) {
		new_msg = convert(msg[i]);
		printf("%s", new_msg);
		free(new_msg);
	}
	puts("");
}

void aesthetic(char* msg) {
	int msg_len = strlen(msg);
	printf("*");
	for (int i=0; i<msg_len; i++) {
		if (i == msg_len - 1) {
			printf("%c*\n", msg[i]);
		} else {
			printf("%c ", msg[i]);
		}
	}
}


/**
 * Print the message in the sponge bob mocking style
 * 
 */
void mock(char* msg) {
	int msg_len = strlen(msg);

	for (int i=0; i<msg_len; i++) {
		if (i%2==0) {
			printf("%c", tolower(msg[i]));
		} else {
			printf("%c", toupper(msg[i]));
		}
	}
	puts("");
}

void qrcode(char* msg) {
	QRCode qrcode;
	uint8_t qrcodeBytes[qrcode_getBufferSize(4)];

	qrcode_initText(&qrcode, qrcodeBytes, 3, ECC_LOW, msg);

	for (uint8_t i = 0; i < (qrcode.size + 2); i++) {
		printf(WHITE);
	}
	puts("");

	for (uint8_t y = 0; y < qrcode.size; y++) {
		printf(WHITE);
		for (uint8_t x = 0; x < qrcode.size; x++) {
			if (qrcode_getModule(&qrcode, x, y)) {
				printf(BLACK);
			} else {
				printf(WHITE);
			}
		}
		printf("%s\n", WHITE);
	}

	for (uint8_t i = 0; i < (qrcode.size + 2); i++) {
		printf(WHITE);
	}
	puts("");
}

int parse_options(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;

	switch (key) {
		case 'a':
			arguments->aesthetic = true;
			break;

		case 'f':
			arguments->fatten = true;
			break;

		case 'm':
			arguments->mock = true;
			break;

		case 'q':
			arguments->qrcode = true;
			break;

		case ARGP_KEY_ARG:
			/* Too many arguments.  */
			if (state->arg_num > 0) {
				argp_usage(state);
			}
			arguments->msg = arg;
			break;


		case ARGP_KEY_END:
			/* Not enough arguments.  */
			if (state->arg_num < 1) {
				argp_usage (state);
			}
			break;


		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

int main(int argc, char **argv) {
	struct arguments arguments;

	/* Default values.  */
	arguments.msg = DEF_VAL;
	arguments.fatten = false;
	arguments.mock = false;
	arguments.qrcode = false;
	arguments.aesthetic = false;

	struct argp_option options[] =
	{
		{ "aesthetic", 'a', 0, 0, "Make the MESSAGE a e s t h e t i c", 0 },
		{ "fat", 'f', 0, 0, "Fatten the MESSAGE", 0 },
		{ "mock", 'm', 0, 0, "Mock someone with the MESSAGE", 0 },
		{ "qrcode", 'q', 0, 0, "Create the QRcode containing the MESSAGE", 0 },
		{ 0 }
	};

	struct argp argp = { options, parse_options, args_doc, doc };
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.fatten) {
		convert_str(arguments.msg);
	} else if (arguments.mock) {
		mock(arguments.msg);
	} else if (arguments.qrcode) {
		qrcode(arguments.msg);
	} else if (arguments.aesthetic) {
		aesthetic(arguments.msg);
	}

	return 0;
}
