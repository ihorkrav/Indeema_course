| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-H21 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | --------- | -------- | -------- | -------- |

# FreeRTOS Task Monitoring & CPU Load Example

(See the README.md file in the upper level examples directory for more information about ESP-IDF examples.)

This example demonstrates how to:
Create multiple FreeRTOS tasks
Pin tasks to different CPU cores on ESP32
Simulate CPU load inside tasks
Monitor task state, priority, stack usage, and core affinity
Use FreeRTOS diagnostic functions such as vTaskList()


# Example Description
# Tasks Overview
This application creates three tasks:

Task Name	Core	Function
Task1	Core 0	Simulates CPU load every 1 second
Task2	Core 1	Simulates heavier CPU load
Receiver	Core 1	Monitors and prints system task info
CPU Load Simulation

The function simulate_cpu_load() artificially occupies the CPU for a specified time using a busy loop based on esp_timer_get_time().

simulate_cpu_load(500);   // 500 ms CPU load
simulate_cpu_load(1000);  // 1000 ms CPU load

This allows you to observe how:
Tasks block lower-priority tasks on the same core
Tasks on the other core continue running independently
Task Monitoring
The vTaskMonitor task periodically prints task information using:
vTaskList(buffer);

Displayed information includes:
Task name
Task state (Running, Blocked, Ready, Suspended)
Priority
Remaining stack (words)
Task number
Core ID

## How to Use Example
Before configuring and building the project, set the correct chip target:
"idf.py set-target <chip_name>"

Open the project configuration menu:
"idf.py menuconfig"

Make sure the following options are enabled:
Component config → FreeRTOS → kernel
☑ Enable configUSE_TRACE_FACILITY 
☑ Enable configUSE_STATS_FORMATTING_FUNCTIONS
☑ Enable dispaly pf xCoreId in vTaskList

###Build and Flash
Build, flash, and monitor the application:
"idf.py -p PORT flash monitor"

To exit the monitor, press:
"Ctrl + ]"

#### Example Output

Task Name    Status  Prio  Stack  Num  CoreId
------------------------------------------------------------
Task1        R       1     1700    5
Task2        B       1     1600    6
Receiver     R       1     1800    7
IDLE0        R       0     800     0
IDLE1        R       0     780     1

# Output Explanation

# Status

R – Running

B – Blocked

S – Suspended

# Stack shows remaining stack space (words)

# CoreId

0 → PRO_CPU

1 → APP_CPU

.gitignore contains:
.vscode
.build
.devcontainer
