
# Process Prioritization and Load Monitoring

## Problem:

Check how many cores do you have using top command.

Create number of cores + 2 processes dd if=/dev/zero of=/dev/null run in background.

Change priority for them:
- -20, -10, 0, .. , 19

Monitor them using top command, did you notice any change ?

Kill them all using killall command.

## Commands used

$ `nproc`

$ `for i in $(seq 1 $(( $(nproc) + 2 ))); do
    dd if=/dev/zero of=/dev/null &
done`

$ `sudo renice {priority} -p pid  # for each of the dd processes`

$ `top`

$ `killall dd`

## Screenshots

![dd priorities.png](./images/dd%20priorities.png)

![top view.png](./images/top%20view.png)

## Observations

📌 The nice values, ranging from -20 to 19, are mapped to real priority values within the range of 0 to 39 by the scheduler.

📌 The CPU usage correlates directly with the priority level: as the priority increases, the CPU usage decreases.

📌 For example, a renice value of -20 (if not ignored by scheduler) may result in high CPU usage
