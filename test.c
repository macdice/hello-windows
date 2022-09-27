#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
doit(const char *name)
{
	char *full;

	errno = 0;
	full = _fullpath(NULL, name, 0);
	if (full) {
		printf("%s -> %s\n", name, full);
		free(full);
	}
	else
		printf("%s -> NULL, errno = %d, %s\n", name, errno, strerror(errno));
}

int
main(int argc, char *argv[])
{
	doit("c:\\windows");
	doit("c:\\does not exist.txt");
	doit("x:\\bad drive.txt");
	doit("x:bad drive.txt");
	doit("1:\\very bad drive.txt");
	doit("1:very bad drive.txt");
	doit("cc:\\impossible drive.txt");
	doit("cc:impossible drive.txt");
	doit("a:remember floppy drives?.txt");
	doit("test.c");
	doit("test.c/foo");
	doit(".");
	doit("..");
	doit("../..");
	doit("../../..");
	doit("../../../..");
	doit("../../../../..");
	doit("../../../../../..");
	doit("../../../../../../..");
	doit("../../../../../../../..");
	doit("..\\..");
	doit("c:\\.\\does not exists\\..\\windows");
	doit("good_junction");
	doit("good_junction/.");
	doit("good_junction/..");
	doit("good_junction/../my_junction");
	doit("good_junction/system32");
	doit("good_symlink");
	doit("good_symlink/.");
	doit("good_symlink/..");
	doit("good_symlink/../my_symlink");
	doit("good_symlink/system32");
	doit("bad_junction");
	doit("bad_junction/.");
	doit("bad_junction/..");
	doit("bad_junction/../my_junction");
	doit("bad_junction/system32");
	doit("bad_symlink");
	doit("bad_symlink/.");
	doit("bad_symlink/..");
	doit("bad_symlink/../my_symlink");
	doit("bad_symlink/system32");
	return 0;
}

