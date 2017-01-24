# MonkeyCrew

Inspired by **Jeff Atwood**'s post [Working with the Chaos Monkey](https://blog.codinghorror.com/working-with-the-chaos-monkey/) and **Netfilx**'s [SimianArmy](https://github.com/Netfilx/SimianArmy) tools, MonkeyCrew helps you test the resiliency of your Windows&reg; systems.

MonkeyCrew is currently based of these tools:

- **Chimp**: It's a random action scheduler. It accepts one or more files describing basic actions (system commands), and it executes those actions at random times. The executed actions can be logged to a **.BAT** file, so it's easy to replay scenarios at will.
- **Wait**: It's a simple count-down timer to make the system wait a given amout of miliseconds. It helps in replaying the actions logged by **Chimp**.
