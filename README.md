# Shiscord

Shitpost in discord, this repository is supposed to be a good project template. Actually it was based on [Pantuza's C project template](https://github.com/pantuza/c-project-template).

# Install

```
git clone git@github.com:MaitreRenard/Shiscord.git
cd Shiscord/
make

sudo cp ./bin/shiscord /usr/bin/ #or anywhere else on your path to call it easily
```

# Dependencies

* [ricmoo's QRcode generator](https://github.com/ricmoo/QRCode)
* [GNU's libiconv](https://savannah.gnu.org/projects/libiconv/)

# Usage

`./shiscord [OPTIONS] "my message"`

## Talk a e s t h e t i c a l l y

`./shiscord -i message`
`./shiscord --aesthetic "my message"`

## Mock

`./shiscord -m message`
`./shiscord --mock "my message"`

## Fatten your message

`./shiscord -f message`
`./shiscord --fatten "my message"`

## Create unicode QRcode

`./shiscord -q hello`
`./shiscord --qrcode "https://song.entropy.land"`

# TODO

* Improve the project architecture (library linking particularly)
