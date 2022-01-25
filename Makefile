
all:	email_filter calendar_filter location_updater

email_filter: email_filter.c
	gcc -o email_filter email_filter.c
calendar_filter: calendar_filter.c
	gcc -o calendar_filter calendar_filter.c
location_updater: location_updater.c
	gcc -o location_updater location_updater.c
clean:
	rm -rf email_filter calendar_filter location_updater
