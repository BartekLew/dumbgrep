dumbgrep: dumbgrep.c
	gcc -Wall -pedantic dumbgrep.c -o dumbgrep

test: dumbgrep
	./dumbgrep "*foo*" > /tmp/dumbgrep.out
	grep -R "foo" . | sed 's/^[^:]*://g' > /tmp/grep.out
	diff /tmp/dumbgrep.out /tmp/grep.out
	./dumbgrep "maa*" > /tmp/dumbgrep.out
	grep -R "^maa" . | sed 's/^[^:]*://g' > /tmp/grep.out
	diff /tmp/dumbgrep.out /tmp/grep.out
	./dumbgrep "*z*d*" > /tmp/dumbgrep.out
	grep -R "z.*d" . | sed 's/^[^:]*://g' > /tmp/grep.out
	diff /tmp/dumbgrep.out /tmp/grep.out

.PHONY: test
