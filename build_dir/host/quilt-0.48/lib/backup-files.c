/*
  File: backup-files.c

  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008
  Andreas Gruenbacher <agruen@suse.de>, SuSE Labs

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/*
 * Create backup files of a list of files similar to GNU patch. A path
 * name prefix and suffix for the backup file can be specified with the
 * -B and -z options.
 */

#define _GNU_SOURCE 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#if !defined(HAVE_MKSTEMP) && defined(HAVE_MKTEMP)
# define mkstemp(x) creat(mktemp(x), 0600)
#endif

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

#ifdef __MINGW32__
#define mkdir(x,y) mkdir(x)
/* Symlinks are not supported */
#define lstat stat
static int link(const char *oldpath, const char *newpath)
{
	errno = ENOSYS;
	return -1;
}
#endif

const char *progname;

enum { what_noop, what_backup, what_restore, what_remove };

const char *opt_prefix="", *opt_suffix="", *opt_file;
int opt_silent, opt_what=what_noop;
int opt_nolinks, opt_touch;
int opt_keep_backup;

#define LINE_LENGTH 1024


static void
usage(void)
{
	printf("Usage: %s [-B prefix] [-z suffix] [-f {file|-}] [-sktL] [-b|-r|-x] {file|-} ...\n"
	       "\n"
	       "\tCreate hard linked backup copies of a list of files\n"
	       "\tread from standard input.\n"
	       "\n"
	       "\t-b\tCreate backup\n"
	       "\t-r\tRestore the backup\n"
	       "\t-x\tRemove backup files and empty parent directories\n"
	       "\t-k\tWhen doing a restore, keep the backup files\n"
	       "\t-B\tPath name prefix for backup files\n"
	       "\t-z\tPath name suffix for backup files\n"
	       "\t-s\tSilent operation; only print error messages\n"
	       "\t-f\tRead the filenames to process from file (- = standard input)\n"
	       "\t-t\tTouch original files after restore (update their mtimes)\n\n"
	       "\t-L\tEnsure that when finished, the source file has a link count of 1\n\n",
	       progname);
}

static void *
malloc_nofail(size_t size)
{
	void *p = malloc(size);
	if (!p) {
		perror(progname);
		exit(1);
	}
	return p;
}

static void
create_parents(const char *filename)
{
	struct stat st;
	int rv = -1;
	char *fn = malloc_nofail(strlen(filename) + 1), *f;

	strcpy(fn, filename);

	f = strrchr(fn, '/');
	if (f == NULL)
		goto out;
	*f = '\0';
	if (stat(fn, &st) == 0)
		goto out;
	*f = '/';

	f = strchr(fn, '/');
	while (f != NULL) {
		*f = '\0';
		if (!rv || (rv = stat(fn, &st)) != 0) {
			mkdir(fn, 0777);
		}
		*f = '/';
		f = strchr(f+1, '/');
	}
out:
	free(fn);
}

static void
remove_parents(const char *filename)
{
	char *fn = malloc_nofail(strlen(filename) + 1), *f;

	strcpy(fn, filename);

	f = strrchr(fn, '/');
	if (f == NULL)
		goto out;
	do {
		*f = '\0';
		if (rmdir(fn) == -1)
			goto out;
	} while ((f = strrchr(fn, '/')) != NULL);
	rmdir(fn);
out:
	free(fn);
}

static int
copy_fd(int from_fd, int to_fd)
{
	char buffer[16384];
	char *wbuf;
	ssize_t len, l;

	for ( ;; ) {
		len = read(from_fd, buffer, sizeof(buffer));
		if (len == 0)
			return 0;
		if (len < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			return 1;
		}
		for (wbuf = buffer; len != 0; ) {
			l = write(to_fd, wbuf, len);
			if (l < 0) {
				if (errno == EINTR || errno == EAGAIN)
					continue;
				return 1;
			}
			wbuf += l;
			len -= l;
		}
	}
}

static int
copy_file(const char *from, const struct stat *st, const char *to)
{
	int from_fd, to_fd, error = 1;

	if ((from_fd = open(from, O_RDONLY)) == -1) {
		perror(from);
		return 1;
	}
	unlink(to);  /* make sure we don't inherit this file's mode. */
	if ((to_fd = creat(to, st->st_mode)) < 0) {
		perror(to);
		close(from_fd);
		return 1;
	}
#if defined(HAVE_FCHMOD)
	(void) fchmod(to_fd, st->st_mode);
#elif defined(HAVE_CHMOD)
	(void) chmod(to, st->st_mode);
#endif
	if (copy_fd(from_fd, to_fd)) {
		fprintf(stderr, "%s -> %s: %s\n", from, to, strerror(errno));
		unlink(to);
		goto out;
	}

	error = 0;
out:
	close(from_fd);
	close(to_fd);

	return error;
}

static int
link_or_copy_file(const char *from, const struct stat *st, const char *to)
{
	if (link(from, to) == 0)
		return 0;
	if (errno != EXDEV && errno != EPERM &&
	    errno != EMLINK && errno != ENOSYS) {
		fprintf(stderr, "Could not link file `%s' to `%s': %s\n",
		       from, to, strerror(errno));
		return 1;
	}
	return copy_file(from, st, to);
}

static int
ensure_nolinks(const char *filename)
{
	struct stat st;

	if (stat(filename, &st) != 0) {
		perror(filename);
		return 1;
	}
	if (st.st_nlink > 1) {
		char *tmpname = malloc(1 + strlen(filename) + 7 + 1), *c;
		int from_fd = -1, to_fd = -1;
		int error = 1;

		if (!tmpname)
			goto fail;
		from_fd = open(filename, O_RDONLY);
		if (from_fd == -1)
			goto fail;

		/* Temp file name is "path/to/.file.XXXXXX" */
		strcpy(tmpname, filename);
		strcat(tmpname, ".XXXXXX");
		c = strrchr(tmpname, '/');
		if (c == NULL)
			c = tmpname;
		else
			c++;
		memmove(c + 1, c, strlen(c) + 1);
		*c = '.';

		to_fd = mkstemp(tmpname);
		if (to_fd == -1)
			goto fail;
		if (copy_fd(from_fd, to_fd))
			goto fail;
#if defined(HAVE_FCHMOD)
		(void) fchmod(to_fd, st.st_mode);
#elif defined(HAVE_CHMOD)
		(void) chmod(tmpname, st.st_mode);
#endif
		close(from_fd);
		from_fd = -1;
		close(to_fd);
		to_fd = -1;
		if (rename(tmpname, filename))
			goto fail;

		error = 0;
	fail:
		if (error)
			perror(filename);
		if (from_fd != -1)
			close(from_fd);
		if (to_fd != -1)
			close(to_fd);
		free(tmpname);
		return error;
	} else
		return 0;
}

static int
process_file(const char *file)
{
	char *backup = malloc_nofail(
		strlen(opt_prefix) + strlen(file) + strlen(opt_suffix) + 1);

	sprintf(backup, "%s%s%s", opt_prefix, file, opt_suffix);

	if (opt_what == what_backup) {
		struct stat st;
		int missing_file = (stat(file, &st) == -1 && errno == ENOENT);

		unlink(backup);
		create_parents(backup);
		if (missing_file) {
			int fd;

			if (!opt_silent)
				printf("New file %s\n", file);
			/* GNU patch creates new files with mode==0. */
			if ((fd = creat(backup, 0)) == -1) {
				perror(backup);
				goto fail;
			}
			close(fd);
		} else {
			if (!opt_silent)
				printf("Copying %s\n", file);
			if (opt_nolinks && st.st_nlink == 1) {
				if (copy_file(file, &st, backup))
					goto fail;
			} else {
				if (link_or_copy_file(file, &st, backup))
					goto fail;
				if (opt_nolinks && ensure_nolinks(file))
					goto fail;
			}
			if (opt_touch)
				(void) utime(backup, NULL);
			else {
				struct utimbuf ut;
				ut.actime = ut.modtime = st.st_mtime;
				(void) utime(backup, &ut);
			}
		}
	} else if (opt_what == what_restore) {
		struct stat st;

		create_parents(file);
		if (stat(backup, &st) != 0) {
			perror(backup);
			goto fail;
		}
		if (st.st_size == 0) {
			if (unlink(file) == 0 || errno == ENOENT) {
				if (!opt_silent)
					printf("Removing %s\n", file);
			} else {
				perror(file);
				goto fail;
			}
			if (!opt_keep_backup) {
				unlink(backup);
				remove_parents(backup);
			}
		} else {
			if (!opt_silent)
				printf("Restoring %s\n", file);
			unlink(file);
			if (opt_nolinks && st.st_nlink != 1) {
				if (copy_file(backup, &st, file))
					goto fail;
			} else {
				if (link_or_copy_file(backup, &st, file))
					goto fail;
				if (opt_nolinks && ensure_nolinks(file))
					goto fail;
			}
			if (!opt_keep_backup) {
				unlink(backup);
				remove_parents(backup);
			}
			if (opt_touch)
				(void) utime(file, NULL);
			else {
				struct utimbuf ut;
				ut.actime = ut.modtime = st.st_mtime;
				(void) utime(file, &ut);
			}
		}
	} else if (opt_what == what_remove) {
		unlink(backup);
		remove_parents(backup);
	} else if (opt_what == what_noop) {
		struct stat st;
		int missing_file = (stat(file, &st) == -1 && errno == ENOENT);

		if (!missing_file && opt_nolinks) {
			if (ensure_nolinks(file))
				goto fail;
		}
	} else
		goto fail;

	free(backup);
	return 0;

fail:
	free(backup);
	return 1;
}

static int
foreachdir_rec(const char *path, struct stat *st,
	       int (*walk)(const char *, const struct stat *))
{
	DIR *dir;
	struct dirent *dp;
	int failed = 0;
	char *p = malloc_nofail(PATH_MAX);

	if (access(path, R_OK|X_OK) || !(dir = opendir(path)))
		return walk(path, NULL);
	while ((dp = readdir(dir))) {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
			continue;
		if (strlen(path) + 1 + strlen(dp->d_name) + 1 > PATH_MAX) {
			fprintf(stderr, "%s/%s: name too long\n", path,
				dp->d_name);
			failed = -1;
			goto out;
		}
		sprintf(p, "%s/%s", path, dp->d_name);
		
		if (lstat(p, st))
			continue;  /* file has disappeared meanwhile */
		if (S_ISDIR(st->st_mode)) {
			failed = foreachdir_rec(p, st, walk);
			if (failed)
				goto out;
		} else {
			failed = walk(p, st);
			if (failed)
				goto out;
		}
	}
	if (closedir(dir) != 0)
		failed = -1;

out:
	free(p);
	return failed;
}

static int
foreachdir(const char *path,
	   int (*walk)(const char *, const struct stat *))
{
	struct stat st;

	if (lstat(path, &st))
		return walk(path, NULL);
	return foreachdir_rec(path, &st, walk);
}

static int
walk(const char *path, const struct stat *st)
{
	size_t prefix_len=strlen(opt_prefix), suffix_len=strlen(opt_suffix);
	size_t len = strlen(path);
	char *p;
	int ret;

	if (!st) {
		perror(path);
		return 1;
	}
	if (!S_ISREG(st->st_mode))
		return 0;
	if (strncmp(opt_prefix, path, prefix_len))
		return 0;  /* prefix does not match */
	if (len < suffix_len || strcmp(opt_suffix, path + len - suffix_len))
		return 0;  /* suffix does not match */

	p = malloc_nofail(len - prefix_len - suffix_len + 1);
	memcpy(p, path + prefix_len, len - prefix_len - suffix_len);
	p[len - prefix_len - suffix_len] = '\0';
	ret = process_file(p);
	free(p);
	return ret;
}

int
main(int argc, char *argv[])
{
	int opt, status = 0;

	progname = argv[0];

	while ((opt = getopt(argc, argv, "brkxB:z:f:shLt")) != -1) {
		switch(opt) {
			case 'b':
				opt_what = what_backup;
				break;

			case 'r':
				opt_what = what_restore;
				break;

			case 'k':
				opt_keep_backup = 1;
				break;

			case 'x':
				opt_what = what_remove;
				break;

			case 'B':
				opt_prefix = optarg;
				break;

			case 'f':
				opt_file = optarg;
				break;

			case 'z':
				opt_suffix = optarg;
				break;

			case 's':
				opt_silent = 1;
				break;

			case 'L':
				opt_nolinks = 1;
				break;

			case 't':
				opt_touch = 1;
				break;

			case 'h':
			default:
				usage();
				return 0;
		}
	}

	if ((*opt_prefix == '\0' && *opt_suffix == '\0') ||
	    (opt_file == NULL && optind == argc)) {
		usage();
		return 1;
	}

	if (opt_file != NULL) {
		FILE *file;
		char line[LINE_LENGTH];

		if (!strcmp(opt_file, "-")) {
			file = stdin;
		} else {
			if ((file = fopen(opt_file, "r")) == NULL) {
				perror(opt_file);
				return 1;
			}
		}

		while (fgets(line, sizeof(line), file)) {
			char *l = strchr(line, '\0');

			if (l > line && *(l-1) == '\n')
				*(l-1) = '\0';
			if (*line == '\0')
				continue;

			if ((status = process_file(line)) != 0)
				return status;
		}

		if (file != stdin) {
			fclose(file);
		}
	}
	for (; optind < argc; optind++) {
		if (strcmp(argv[optind], "-") == 0) {
			struct stat st;
			char *dir = strdup(opt_prefix), *d = strrchr(dir, '/');
			if (d)
				*d = '\0';
			else
				d = ".";
			if (stat(dir, &st) == 0) {
				status = foreachdir(dir, walk);
				if (status == -1)
					perror(dir);
			}
			free(dir);
		} else
			status = process_file(argv[optind]);
		if (status)
			return status;
	}

	return status;
}
