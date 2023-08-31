# HIbernator V 0.5

It just works now.

The goal of this app is to auto hibernate your computer during the night. You have to select the time to hibernate and the time to wake up and that it on the web interface !

`rtcwake` has to be installed.

# Compile it

You have to install TWebserver (See the webserver project on my account)

```
git clone https://github.com/titicplusplus/Hibernator.git
g++ main.cpp webserver.cpp -o main -lTWebServer -lstdc++fs
./main
```

And go to [http://127.0.0.1:9000](http://127.0.0.1:9000)
