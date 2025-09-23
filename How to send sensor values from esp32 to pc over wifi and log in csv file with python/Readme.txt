Sensors Pinout
1.  DHT11                    =>     4
2.  mq2Pin                   =>    33
3.  mq7Pin                   =>    32
4.  mq9Pin                   =>    35
5.  mq135Pin                 =>  34
    VCC of obove all sensors =>  5V
    GND of obove all sensors =>  GND

6.  SCL pin of SGP30          =>   39
    SDA pin of SGP30          =>  42
    VCC pin of SGP30          =>  3.3V
    GND pin of SGP30          =>  GND

Your PC and ESP32 must connected in same WiFi Network.
In arduino code, insert your PC IP Address (e.g., const char * udpAddress = "192.168.74.98";).
In your pc, open command promt(cmd), type and hit "ipconfig". Look for the IPv4 Address of your WiFi adapter (e.g., 192.168.74.98).
