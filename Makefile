run: clean
	python3 vcs.py

clean:
	rm save-directory -rfd
	rm test-directory/.retrospect -rfd
