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

Master and slaves are communicating using the following protocol which consists of a command type and a value that fit one byte.
Slaves can only respond to the master questions. Thus, a command number refers to a question and an answer.

This table describes the questions asked by the master :

| Command | Parameter    | Meaning              |
| :-----: | :----------: | :------------------: |
| 0       | timeleft (s) | Update timeleft      |
| 1       |              | Defused ?            |
| 2       | level [1;3]  | Update difficulty    |
| 3       |              | User made misktake ? |


This table describes the answers of the slaves :

| Command | Value        | Answer                        |
| :-----: | :----------: | :---------------------------: |
| 0       | 1            | Ok                            |
| 1       | 1            | Yes                           |
| 1       | 0            | No                            |
| 2       | 1            | Ok                            |
| 3       | penality (s) | User potentially made mistake |

## Running the tests

You can debug modules using serial communication.

## Built With

* [Wire](https://www.arduino.cc/en/Reference/Wire) - I2C / TWI devices communication library

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **Charles Paulet** - *Initial work* - [Github](https://github.com/valkheim)
* **Théophile Champion** - *Helper* - [Github](https://github.com/ChampiB)

See also the list of [contributors](https://github.com/valkheim/KTNE-IRL/contributors) who participated in this project.

## License

Copyright (C) 2017-2018 Charles Paulet

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

See the [LICENSE.md](LICENSE) file for details
