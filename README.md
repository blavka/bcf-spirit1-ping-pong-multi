<a href="https://www.bigclown.com/"><img src="https://bigclown.sirv.com/logo.png" width="200" alt="BigClown Logo" align="right"></a>

# Example firmware PING PONG with Spirit1 for more then two BigClown Core Module

[![Travis](https://img.shields.io/travis/blavka/bcf-spirit1-ping-pong-multi/master.svg)](https://travis-ci.org/blavka/bcf-spirit1-ping-pong-multi)
[![Release](https://img.shields.io/github/release/blavka/bcf-spirit1-ping-pong-multi.svg)](https://github.com/blavka/bcf-spirit1-ping-pong-multi/releases)
[![License](https://img.shields.io/github/license/blavka/bcf-spirit1-ping-pong-multi.svg)](https://github.com/blavka/bcf-spirit1-ping-pong-multi/blob/master/LICENSE)
[![Twitter](https://img.shields.io/twitter/follow/blavka.svg?style=social&label=Follow)](https://twitter.com/blavka)


## Usage

Clone repository

    git clone --recursive https://github.com/blavka/bcf-spirit1-ping-pong-multi

Make firmware.bin

    make -j4

Flash to all Core Modules

    bcf flash

Connet to AT interface over serial port

    picocom -b 115200 --omap crcrlf  --echo /dev/ttyUSB0

Set uniq id from 0 to 255

    AT$ID=1

Read id

    AT$ID?

Help

    AT$HELP


## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT/) - see the [LICENSE](LICENSE) file for details.

---

Made with &#x2764;&nbsp; by [**HARDWARIO s.r.o.**](https://www.hardwario.com/) in the heart of Europe.
