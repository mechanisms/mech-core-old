
[mech-home-link]: https://github.com/mechanisms/mech "Home repository for mechanisms"

# mech-core

This library contains the core mechanisms and C code for describing mechanisms.

It is very early in development and the first C code I can remember ever writing.

See [Mechanisms Home][mech-home-link] for more information and other libraries.

Supported Mechanisms:

* *[num](#num-mechanism)* - a long primitive as a mechanism

# Supported Mechanisms

## <a name="num-mechanism"></a> num mechanism

Although you can use literals when programming, we provide numeric mechanism. Numeric mechanisms are most likely used for testing and teaching.

```c
Mechanism* num23 = num(23);
goLong(num23); // returns 23
goFloat(num23); // returns 23.0

```

# Setup

## Using In Your Projects

Change directory to your node project.

    $ npm install --save mech-core

## Development

### Setup

    $ npm install

### Continuous Rebuild and Testing

See ./dist for files we build.

    $ gulp
