# Monkey

Random action scheduler. It's a command line utility that accepts action files ( **.INI** style formated) describing actions (system commands and event distribution), and it executes those actions at random times. Actions can be defined as reversible, so you can simulate off-on scenarios). For example:

```ini
; Actions for the Chaos Monkey service

[Network cutout simulation]
; How to Perform (and optionally Revert) the action
Perform=netsh interface set interface name="Ethernet 2" admin=DISABLED
Revert=netsh interface set interface name="Ethernet 2" admin=ENABLED

; TBA = Time Between Actions, with Minimum (.Min) and Average (.Avg) times between executions
TBA.Min=30"
TBA.Avg=1'

; TTR = Time To Repair, with Minimum (.Min) and Average (.Avg) times between execution and revert
TTR.Avg=5"

; Action is enabled? (Optional, enabled if ommitted)
Enabled=true
```

Action execution can be logged as a Replay command file (**.BAT**), so it's easy to reproduce the executed scenarios at will.

## Command Syntax
    C:\> Monkey <Actions> [-v] [-o <Output>] [-r <Replay>]"
		<Actions> files : One or more paths to monkey action files.
		-v : Verbose action execution.
		-o : Copy Output to file at <Output> file path.
		-r : Generate Replay command file at <Replay> file path.