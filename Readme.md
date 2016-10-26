# loadmin

Simple wrapper to start a process, which will be paused / resumed when the loadavg of the systems
goes above / under a given value.

Useful to launch a long running command on a loaded system.

Exemple :

```sh
loadmin 10 rsync file/from to::here
```

The rsync command will paused itself when the loadavg goes above 10, and resume
once it goes under 10.

# Installation


```sh
git clone https://github.com/drasill/loadmin.git
cd loadmin
make
# Install into /usr/local/bin/loadmin
sudo make install
```

----

Totally inspired/copied from here :
http://www.linuxonly.nl/docs/9/77_Suspending_a_process_on_high_load.html

(Thank you Sjoerd Langkemper).

With only minors modifications :
* LoadAvg max can be specified on command line
* Displays when Stopping and Resuming
