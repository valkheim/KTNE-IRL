# KTNE-IRL

Keep Talking and Nobody Explodes, the IRL version.

## Getting Started

This project is about creating a real version of the bomb defusal game Keep Talking and Nobody Explodes. By getting a copy of the project, you will be provided hardware schematics, software sources, documentation about inner project protocols and deployment informations.

The game is modular. Slaves, organised as modules, treat with a master module accordingly to a specific protocol.

### Prerequisites

We are using Atmega328* micro controllers as you can found on most Arduino boards. You can use their IDE. You will also need the [Wire](https://www.arduino.cc/en/Reference/Wire) library.

### Installing

In order to run a minimal setup, you'll need to build and run at least one master module and one slave module.
You will find those two under [examples](examples/).

## Protocol

Master and slaves are communication using the following protocol which consists of a command type and a value that fit one byte.
Slaves can only respond to the master questions. Thus, a command number refers to a question and an answer.

This table describes the questions asked by the master :

| Command | Question    |
| :-----: | :---------: |
| 1       | Defused ?   |


This table describes the answers of the slaves :

| Command | Value | Answer |
| :-----: | :---: | :----: |
| 1       | 1     | Yes    |
| 1       | 0     | No     |

## Running the tests

You can debug modules using serial communication. 

## Built With

* [Wire](https://www.arduino.cc/en/Reference/Wire) - I2C / TWI devices communication library

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **Charles Paulet** - *Initial work* - [Github](https://github.com/valkheim)

See also the list of [contributors](https://github.com/valkheim/KTNE-IRL/contributors) who participated in this project.

## License

This project is licensed under the GPL v3 License - see the [LICENSE.md](LICENSE) file for details
