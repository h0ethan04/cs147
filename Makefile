.PHONY: run, clean, upload


run:
	pio run

clean:
	pio run -t clean

upload:
	pio run -t upload
