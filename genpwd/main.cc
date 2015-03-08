#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void usage();

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		usage();
		exit(0);
	}

	const char* salt = "$1$EndOfTheWorld";
	crypt_set_format("md5");

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp("-f", argv[i]) == 0  &&  argc > (i + 1))
		{
			crypt_set_format(argv[i + 1]);
			++i;
			continue;
		}

		if (strcmp("-s", argv[i]) == 0  &&  argc > (i + 1))
		{
			salt = argv[i + 1];
			++i;
			continue;
		}

		const char* cipher = crypt(argv[i], salt);

		// drop 3rd $, line starts with $, so we've already found the first one
		int n = 1;
		const char* display = cipher;
		for (; display && *display == '$' && n < 3; ++n)
		{
			if (display[1] == '\0')
			{
				break;
			}

			display = strchr(display + 1, '$');
		}
		if (n != 3 || *display != '$')
			display = NULL;

		if (display)
		{
			++display;	// step past $
		}
		else
		{
			display = cipher;
		}
		fprintf(stdout, "%s\n", display);
	}

	return 0;
}

void usage()
{
	const char* msg[] =
	{
		"genpwd: Generate a sequence of encypted passwords using crypt(3) for input to pw(8).",
		"        The seed can set with -s <seed> on the input sequence.",
		"",
		"example: genpwd hello world -s '$1$MacLaren' hello world",
		NULL
	};

	for (size_t i = 0; msg[i]; ++i)
	{
		fprintf(stdout, "%s\n", msg[i]);
	}
}
