# Chimp ![alt text](https://./Res/Chimp_256.png)
Chimp is a random scheduler: It accepts system commands (specified through action files), and it randomly executes those commands. Command execution can be logged as a Replay command file (**.BAT**), so it's easy to reproduce the executed scenarios at will.

## Command Syntax
    C:\> Chimp <Actions> [-v] [-o <Output>] [-r <Replay>]"
		<Actions> files : One or more paths to chimp action files.
		-v : Verbose action execution.
		-o : Copy Output to file at <Output> file path.
		-r : Generate Replay command file at <Replay> file path.

## Action File Format
Action files use a *.INI* syntax, where every *Section* specifies a different action. The next key values are available at every section:
- **Perform**: Defines the system command to be performed.
- **Revert**: Optinally defines the system command to perform if the action must be reverted.
- **TBA.Avg**: Defines the Average Time Between Actions. After the exact minimum time (TBA.Min) has been elapsed, this is the average, random time to wait before Performing the action.
- **TBA.Min**: Optinally defines the Minimum Time Between Actions. The exact minimum time to wait before Performing the action.
- **TTR.Avg**: Defines the Average Time To Revert the action. After the exact minimum time (TTR.Min) has been elapsed, this is the average, random time to wait before Reverting the action.
- **TTR.Min**: Optionally defines the Minimum Time To Revert the action. The exact minimum time to wait before Reverting the action.
- **Alert.Ok**: Optionally defines the parameters to use when Alerting about a successfully performed / reverted action.
- **Alert.Error**: Optionally defines the parameters to use when Alerting about a failure while performing / reverting an action.
- **Enabled**: Defines wheter the action should be performed or ignored.

### Notes
- **Alerts** are performed through the [MSG Windows Command](https://technet.microsoft.com/en-us/library/bb490796.aspx), so  "Alert.Ok" and and "Alert.Error" correspond to parameters available for this command.
- Perform and Revert **Events** are generated using an [Exponential distribution](https://en.wikipedia.org/wiki/Exponential_distribution) with the given Avg average rate.
- **.Min** and **.Avg** time parameters can be specified as:
-- **1234** 	--> 1234 miliseconds
-- **1:12:34** 	--> 1 hour, 12 minutes, 34 seconds
-- **1:12** 	--> 1 hour, 12 minutes
-- **12'34"** 	--> 12 minutes, 34 seconds
-- **12'** 		--> 12 minutes
-- **34"** 		--> 34 seconds


### Example:
```ini
; Chaos Monkey actions

[Network cutout simulation]
; Simulate Network cutouts by simply disabling / enabling the network interface
Perform=netsh interface set interface name="Ethernet 2" admin=DISABLED
Revert=netsh interface set interface name="Ethernet 2" admin=ENABLED

; Network is available at least 30", and it goes down approximately after a 1'
TBA.Min=30"
TBA.Avg=1'

; Once it goes down, Network takes approximately 5" to be available
TTR.Avg=5"

; Action is enabled
Enabled=true
```
